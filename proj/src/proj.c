#include "../headers/proj.h"


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(proj_main_loop)(int argc, char *argv[])
{ 
  vbe_mode_info_t info;
  uint16_t mode = 0x115;
  graphic_get_mode_info(mode, &info);
  graphic_def(&info);
  graphic_init(mode);
  
  //size_t no_lines = 8;
  //char text[] = "The Brothers Karamazov is a passionate philosophical novel that enters deeply into questions of God, free will, and morality. It is a theological drama dealing with problems of faith, doubt and reason in the context of a modernizing Russia, with a plot that revolves around the subject of patricide. Dostoevsky composed much of the novel in Staraya Russa, which inspired the main setting. It is one of the supreme achievements in world literature.";

  //size_t no_lines = 4;
  //char text[] = "Yeah, they got you where they want you. There's a better life and you think about it, don't you? It's a rich man's game no matter what they call it and you spend your life putting money in his wallet."; 
  
  //size_t no_lines = 2;
  //char text[] = "Yeah, they got you where they want you. There's a better life and you think about it, don't you?";
  size_t no_lines = 1;
  char text[] = "aa.";

  y_pos_typed = Y_BOX + (CHAR_H+3)*no_lines+3*Y_BOX_MARGIN+Y_BTW_BOXES;

  
  
  /* Timer stuff */
  extern uint32_t timer_counter;
  timer_counter = 0;
  uint8_t timer_bit_no = 0;
  timer_subscribe_int(&timer_bit_no);
  uint32_t timer_irq_set = BIT(timer_bit_no);

  /*Keyboard stuff*/
  uint8_t scancode_bytes[2];
  uint8_t kbd_bit_no = 1;
  kbd_subscribe_int(&kbd_bit_no);
  uint32_t kbd_irq_set = BIT(kbd_bit_no);

  /*Mouse stuff*/
  kbc_write_byte(WRT_MOUSE, ENB_DR);
  uint8_t mouse_bit_no = 12;
  mouse_subscribe_int(&mouse_bit_no);
  uint32_t mouse_irq_set = BIT(mouse_bit_no);
  struct packet pp;
  extern uint8_t packet_byte_counter;
  Mouse_event mouse_event; mouse_event.ev = MOVE;
  Sprite* mouse = create_sprite((xpm_map_t)mouse_xpm, 0, 0, 0, 0);
  //extern int32_t mouse_x; extern int32_t mouse_y;
  //uint8_t *map_mouse; xpm_image_t img_mouse;
  //graphic_xpm_load(&map_mouse, &img_mouse, XPM_8_8_8, (xpm_map_t)mouse_xpm);


  Sprite* main_menu = create_sprite((xpm_map_t)menu, 0, 0, 0, 0);

  Menu_state state = MENU;
  uint8_t aux_key = NOTHING;

  bool timer_int = false;
  bool kbd_int = false;
  bool mouse_int = false;



  int ipc_status;
  message msg;
  int r = 0;

  while(state != EXIT) { 
  //Get a request message. 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) {  // received notification 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // hardware interrupt notification 				
          if (msg.m_notify.interrupts & timer_irq_set) {
            if (timer_counter%2 == 0) {
              fr_buffer_to_video_mem();
            }
            timer_int_handler();
            timer_int = true;

          }
          if (msg.m_notify.interrupts & kbd_irq_set) { 
            // keyboard interrupt 
            kbc_ih();
            assemble_scancode(scancode_bytes);
            // If scancode is all set
            if (!(scancode_bytes[0] == 0xE0 && scancode_bytes[1] == 0x00)) {
              aux_key = get_scancode_char(scancode_bytes);
              kbd_int = true;
            }
          }
          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            if (packet_byte_counter == 3) {
              packet_byte_counter = 0;
              assemble_packet(&pp);
              mouse_events(&mouse_event, &pp);
              mouse_int = true; 
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
    switch (state) {
      case MENU:
        if (timer_int) {
          menus_proccess_timer_int(timer_counter, main_menu, mouse);
        }
        if (kbd_int) {
          menus_proccess_kbd_int(&state, aux_key);
          if (state == RACE)
            race_init(text, strlen(text), no_lines);
        }
        if (mouse_int) {
          if (state != MENU) break;
          menus_proccess_mouse_int(&state, mouse_event, mouse);
          if (state == RACE)
            race_init(text, strlen(text), no_lines);
        }
        break;
      case RACE:
        if (kbd_int) {
          race_process_kbd_int(&state, aux_key);
          if (state == RESULTS) {
            results_init();
          }
        }
        if (timer_int) {
          race_process_timer_int(timer_counter);
        }
        if (state != RACE)
          race_end();
        break;
      case RESULTS:
        if (timer_int) {
          results_proccess_timer_int(timer_counter, mouse);
        }
        if (kbd_int) {
          results_proccess_kbd_int(&state, aux_key);
          if (state == RACE)
            race_init(text, strlen(text), no_lines);
        }
        if (mouse_int) {
          results_proccess_mouse_int(&state, mouse_event, mouse);
          if (state == RACE)
            race_init(text, strlen(text), no_lines);
        }
        if (state != RESULTS)
          results_end();
        break;
      case RACE_WITH_FRIEND:
        state = MENU;
        break;
      case BEST_RESULTS:
        state = MENU;
        break;
      case EXIT:
        break;
      default:
        break;
    }
    timer_int = false;
    kbd_int = false;
    mouse_int = false;
  }

  
  timer_unsubscribe_int();
  kbd_unsubscribe_int();
  mouse_unsubscribe_int(); 
  kbc_write_byte(WRT_MOUSE, DIS_DR); // Disable data report

  destroy_fr_bufffer();
  vg_exit();
  return 0;
}


