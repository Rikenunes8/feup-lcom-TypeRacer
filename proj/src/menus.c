#include <../headers/menus.h>
#include <../headers/xpixmap.h>
#include <../headers/graphic.h>
#include <../headers/race.h>


int main_menu()
{
    /* Mouse stuff
    if (kbc_write_byte(WRT_MOUSE, ENB_DR) != 0) // Enable data report
    return 1;

    uint8_t bit_no = 12;
    if (kbc_subscribe_int(&bit_no, MOUSE12_IRQ) != 0) 
    {
        printf("Error kbc_subscribe\n");
        return 1;
    }

    int32_t move_x = 0, move_y = 0, errorx = 0, errory = 0;
    int32_t dx, dy;
    struct packet pp;*/

    /*Keyboard stuff*/
    uint8_t scancode_bytes[2];
    uint8_t kbd_bit_no = 1;
    uint32_t kbd_irq_set = BIT(kbd_bit_no);

    uint8_t aux_key;

    Menu_state state = MAIN;
    Menu_event event;
    int ipc_status;
    message msg;
    int r = 0;

    while(state != EXIT) 
    { 
    //Get a request message. 
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) 
        { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) 
        {   // received notification 
            switch (_ENDPOINT_P(msg.m_source)) 
            {
                case HARDWARE: // hardware interrupt notification 				
                    if (msg.m_notify.interrupts & kbd_irq_set) 
                    { 
                        // keyboard interrupt 
                        kbc_ih();
                        assemble_scancode(scancode_bytes);
                        // If scancode is all set
                        if (!(scancode_bytes[0] == 0xE0 && scancode_bytes[1] == 0x00))  
                        {
                            // Get typed key
                            aux_key = get_scancode_char(scancode_bytes);
                            /*
                            read_event não implementado
                            ideia de função:
                            Menu_event* read_event(struct packet *pp)
                            recebe o pacote e retorna o evento correspondente
                            ver mouse.h
                            */
                            event = read_event(/*&pp,*/ aux_key);
                            
                            switch (state) 
                            {
                              case MAIN:
                                printf("MAIN\n");
                                if (event == click_on_race || event == type_left_arrow) 
                                {
                                    state = RACE;
                                }
                                /*
                                else if (event == click_on_race_with_friend || event == type_down_arrow) 
                                {
                                    state = RACE_WITH_FRIEND;
                                }
                                else if (event == click_on_best_results || event == type_top_arrow) 
                                {
                                    state = BEST_RESULTS;
                                }*/
                                else if (event == click_on_exit || event == type_right_arrow) 
                                {
                                    state = EXIT;
                                }
                                else 
                                {
                                  printf("Event not available here\n");
                                  state = EXIT;
                                }
                                break;
                              case RACE:
                                printf("RACE\n");
                                graphic_draw_rectangle(16,16,800-32, 600-32, WHITE);
                                char text[] = "Yeah, ya.";
                                race_init(text, strlen(text));
                                //state = RESULTS;
                                display_main_menu();
                                state = MAIN;
                                if (event == type_ESC) 
                                {
                                    display_main_menu();
                                    state = MAIN;
                                }
                                break;
                              /*case RACE_WITH_FRIEND:
                                printf("RACE_WITH_FRIEND\n");
                                friend_race_init();
                                state = RESULTS;
                                if (event == type_ESC) 
                                {
                                    state = MAIN;
                                }
                                break;
                              case BEST_RESULTS:
                                printf("BEST_RESULTS\n");
                                display_best_results();
                                if (event == type_ESC) 
                                {
                                    state = MAIN;
                                }
                                break;
                              case RESULTS:
                                printf("RESULTS\n");
                                display_results();
                                if (event == click_on_try_again_race) 
                                {
                                    state = RACE;
                                }
                                else if (event == click_on_try_again_friends_race) 
                                {
                                    state = RACE_WITH_FRIEND;
                                }
                                else if (event == click_on_save_results) 
                                {
                                    save_results();
                                    state = RESULTS;
                                }
                                else if (event == type_ESC) 
                                {
                                    state = MAIN;
                                }
                                break;
                              case EXIT:
                                printf("EXIT\n");
                                return 0;*/
                              default:
                                printf("Other state (like released key\n");
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
    printf("END\n");

  //kbc_write_byte(WRT_MOUSE, DIS_DR); // Disable data report
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

Menu_event read_event(/*struct packet *pp,*/ uint8_t aux_key)
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
    return 0;
}

