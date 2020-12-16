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

  //char text[] = "Yeah, they got you where they want you. There's a better life and you think about it, don't you? It's a rich man's game no matter what they call it and you spend your life putting money in his wallet.";
  char text[] = "aa."; 
  
  
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
            timer_int_handler();
            switch (state) {
              case MENU:
                menus_proccess_timer_int(timer_counter, main_menu, mouse);
                break;
              case RACE:
                race_process_timer_int(timer_counter);
                break;
              case RESULTS:
                //timer_counter = 0;
                //results_process_timer_int(timer_counter);
                //printf("timer_counter: %d\n", timer_counter);
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

            if (timer_counter%1 == 0) {
              fr_buffer_to_video_mem();
            }
          }
          if (msg.m_notify.interrupts & kbd_irq_set) { 
            // keyboard interrupt 
            kbc_ih();
            assemble_scancode(scancode_bytes);
            // If scancode is all set
            if (!(scancode_bytes[0] == 0xE0 && scancode_bytes[1] == 0x00)) {
              aux_key = get_scancode_char(scancode_bytes);
              switch (state) {
                case MENU:
                  menus_proccess_kbd_int(&state, aux_key);
                  if (state == RACE)
                    race_init(text, strlen(text));
                  break;
                case RACE:
                  race_process_kbd_int(&state, aux_key);
                  if (state == RESULTS)
                  {
                    race_end();
                  }
                  break;
                case RESULTS:
                  printf("aqui kbd results\n");
                  race_end();
                  sleep(2);
                  state = EXIT;
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
            }
          }
          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            if (packet_byte_counter == 3) {
              packet_byte_counter = 0;
              assemble_packet(&pp);
              mouse_events(&mouse_event, &pp);
              switch (state) {
                case MENU:
                  menus_proccess_mouse_int(&state, mouse_event, mouse);
                  if (state == RACE)
                    race_init(text, strlen(text));
                  break;
                case RACE:
                  //race_process_mouse_int(aux_key);
                  break;
                case RESULTS:
                  state = MENU;
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
  }

  
  timer_unsubscribe_int();
  kbd_unsubscribe_int();
  mouse_unsubscribe_int(); 
  kbc_write_byte(WRT_MOUSE, DIS_DR); // Disable data report

  destroy_fr_bufffer();
  vg_exit();
  return 0;
}


