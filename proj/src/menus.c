#include "../headers/menus.h"

int main_menu(Menu_state *state)
{
  /* Timer stuff */
  extern uint32_t timer_counter;
  uint8_t timer_bit_no = 0;
  timer_subscribe_int(&timer_bit_no);
  uint32_t timer_irq_set = BIT(timer_bit_no);

  /*Mouse stuff*/
  kbc_write_byte(WRT_MOUSE, ENB_DR);
  uint8_t mouse_bit_no = 12;
  mouse_subscribe_int(&mouse_bit_no);
  uint32_t mouse_irq_set = BIT(mouse_bit_no);
  extern int32_t mouse_x; extern int32_t mouse_y;
  struct packet pp;
  extern uint8_t packet_byte_counter;
  Mouse_event mouse_event; mouse_event.ev = MOVE;
  uint8_t *map_mouse; xpm_image_t img_mouse;
  graphic_xpm_load(&map_mouse, &img_mouse, XPM_8_8_8, (xpm_map_t)mouse_xpm);


  /*Keyboard stuff*/
  uint8_t scancode_bytes[2];
  uint8_t kbd_bit_no = 1;
  kbd_subscribe_int(&kbd_bit_no);
  uint32_t kbd_irq_set = BIT(kbd_bit_no);


  uint8_t aux_key;
  display_main_menu();

  Menu_event event;
  int ipc_status;
  message msg;
  int r = 0;

  while(*state == MENU) { 
  //Get a request message. 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) {  // received notification 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // hardware interrupt notification 				
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            if (timer_counter%2 == 0) {
              graphic_xpm(map_mouse, &img_mouse, mouse_x, -mouse_y);
              fr_buffer_to_video_mem();
            }
          }
          if (msg.m_notify.interrupts & kbd_irq_set) { 
            // keyboard interrupt 
            kbc_ih();
            assemble_scancode(scancode_bytes);
            // If scancode is all set
            if (!(scancode_bytes[0] == 0xE0 && scancode_bytes[1] == 0x00)) {
              // Get typed key
              aux_key = get_scancode_char(scancode_bytes);
              event = read_kbd_event(aux_key);
              switch (event) {
                case type_left_arrow:  
                  *state = RACE;
                  break;
                case type_right_arrow:
                  *state = EXIT;
                  break;
                case type_top_arrow:
                  *state = BEST_RESULTS;
                  break;
                case type_down_arrow:
                  *state = RACE_WITH_FRIEND;
                  break;
                default:
                  break;
              }
            }
          }
          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            if (packet_byte_counter == 3) {
              if (*state != MENU) continue;
              packet_byte_counter = 0;
              assemble_packet(&pp);
              mouse_events(&mouse_event, &pp);
              event = read_mouse_event(&mouse_event, &mouse_x, &mouse_y);
              switch (event) {
                case click_on_race:  
                  *state = RACE;
                  break;
                case click_on_exit:
                  *state = EXIT;
                  break;
                case click_on_best_results:
                  *state = BEST_RESULTS;
                  break;
                case click_on_race_with_friend:
                  *state = RACE_WITH_FRIEND;
                  break;
                default:
                  break;
              }
            }    
          }
          break;
        default:
          printf("Receive no interrupt\n");
          break; // no other notifications expected: do nothing 	
      }
    } 
    else 
    {  
      //received a standard message, not a notification 
      //no standard messages expected: do nothing 
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }

  timer_unsubscribe_int();
  kbd_unsubscribe_int();
  mouse_unsubscribe_int();  
  kbc_write_byte(WRT_MOUSE, DIS_DR); // Disable data report
  return 0;
}


void display_main_menu()
{
  uint8_t * map;
  xpm_image_t img;
  xpm_map_t xpm = menu;
  map = xpm_load(xpm, XPM_8_8_8, &img);
  graphic_Char_xpm(map, &img, 0, 0, NORMAL);

  return;
}

void display_race_background()
{
  uint8_t * map;
  xpm_image_t img;
  xpm_map_t xpm = background;
  map = xpm_load(xpm, XPM_8_8_8, &img);
  graphic_Char_xpm(map, &img, 0, 0, NORMAL);
  graphic_draw_rectangle(16,16,get_h_res()-32, get_v_res()-32, WHITE);

  //draws the text box with variable dimensions (incomplete)
  //passar numero de linhas como argumento, definir heigth consoante esse numero e não exceder max_height
  //graphic_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
  int max_height = 150;
  int width = 736;
  graphic_draw_rectangle(32, 418, width, max_height, BLACK); 
  graphic_draw_rectangle(32, 252, width, max_height, BLACK); 

  graphic_draw_rectangle(34, 420, width-4, max_height-4, WHITE); 
  graphic_draw_rectangle(34, 254, width-4, max_height-4, WHITE);

  xpm_map_t xpm_ = dashed_line;
  map = xpm_load(xpm_, XPM_8_8_8, &img);
  graphic_Char_xpm(map, &img, 16, 110, NORMAL);

}

Menu_event read_kbd_event(uint8_t aux_key)
{
  if(aux_key == L_ARROW)
    return type_left_arrow;
  else if(aux_key == R_ARROW)
    return type_right_arrow;
  else if(aux_key == T_ARROW)
    return type_top_arrow;
  else if(aux_key == D_ARROW)
    return type_down_arrow;
  else if(aux_key == ESC)
    return type_ESC;
  else 
    return none;
}

Menu_event read_mouse_event(Mouse_event *ev, int32_t *mouse_x, int32_t *mouse_y)
{  
  *mouse_x += ev->dx;
  *mouse_y += ev->dy;
  if (*mouse_x > (int32_t)get_h_res()-20)
    *mouse_x = (int32_t)get_h_res()-20;
  if (*mouse_x < 0)
    *mouse_x = 0;
  if (*mouse_y < -(int32_t)get_v_res()+28)
    *mouse_y = -(int32_t)get_v_res()+28;
  if (*mouse_y > 0)
    *mouse_y = 0;

  if (ev->ev == LB_DOWN) {
    if (*mouse_x > race_x_left && *mouse_x < race_x_right && -(*mouse_y) > race_y_top &&  -(*mouse_y) < race_y_down)
      return click_on_race;
    else if (*mouse_x > friend_race_x_left && *mouse_x < friend_race_x_right && -(*mouse_y) > friend_race_y_top &&  -(*mouse_y) < friend_race_y_down)
      return click_on_race_with_friend;
    else if (*mouse_x > best_results_x_left && *mouse_x < best_results_x_right && -(*mouse_y) > best_results_y_top &&  -(*mouse_y) < best_results_y_down)
      return click_on_best_results;
    else if (*mouse_x > exit_x_left && *mouse_x < exit_x_right && -(*mouse_y) > exit_y_top &&  -(*mouse_y) < exit_y_down)
      return click_on_exit;
  }
  
  return none;
}
