#include <../headers/menus.h>

int main_menu()
{
    if (kbc_write_byte(WRT_MOUSE, ENB_DR) != 0) // Enable data report
    return 1;

    uint8_t bit_no = 12;

    if (kbc_subscribe_int(&bit_no, MOUSE12_IRQ) != 0) 
    {
        printf("Error kbc_subscribe\n");
        return 1;
    }

    
    Menu_state state = MAIN;
    Menu_event event;
    int32_t move_x = 0, move_y = 0, errorx = 0, errory = 0;
    int32_t dx, dy;


    struct packet pp;
    int ipc_status;
    message msg;
    uint32_t irq_set = BIT(bit_no);
    int r = 0;

    while(state != EXIT) 
    { 
    /* Get a request message. */
        if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) 
        { 
            printf("driver_receive failed with: %d", r);
            continue;
        }
        if (is_ipc_notify(ipc_status)) 
        {   /* received notification */
            switch (_ENDPOINT_P(msg.m_source)) 
            {
                case HARDWARE: /* hardware interrupt notification */				
                    if (msg.m_notify.interrupts & irq_set) 
                    { 
                        /* mouse interrupt */ 
                        mouse_ih();
                        if (packet_byte_counter == 3) 
                        {
                            packet_byte_counter = 0;
                            assemble_packet(&pp);
                            mouse_print_packet(&pp);

                            /*
                            read_event não implementado
                            ideia de função:
                            Menu_event* read_event(struct packet *pp)
                            recebe o pacote e retorna o evento correspondente
                            ver mouse.h
                            */
                            //event = read_event(&pp);
                            dx = pp.delta_x;
                            dy = pp.delta_y;
                            
                            switch (state) 
                            {
                              case MAIN:
                                printf("MAIN\n");
                                if (event == click_on_race) 
                                {
                                    state = RACE;
                                }
                                else if (event == click_on_race_with_friend) 
                                {
                                    state = RACE_WITH_FRIEND;
                                }
                                else if (event == click_on_best_results) 
                                {
                                    state = BEST_RESULTS;
                                }
                                else if (event == click_on_exit) 
                                {
                                    state = EXIT;
                                }
                                break;
                              case RACE:
                                printf("RACE\n");
                                race_init();
                                state = RESULTS;
                                if (event == type_ESC) 
                                {
                                    state = MAIN;
                                }
                                break;
                              case RACE_WITH_FRIEND:
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
                                return 0;
                              default:
                                printf("Erro\n");
                            }
                        }
                    }
                    break;
                default:
                  printf("Receive no interrupt\n");
                  break; /* no other notifications expected: do nothing */	
            }
        } 
        else 
        {  
          /* received a standard message, not a notification */
          /* no standard messages expected: do nothing */
        }
    tickdelay(micros_to_ticks(DELAY_US));
    }
    printf("END\n");

  kbc_unsubscribe_int();
  kbc_write_byte(WRT_MOUSE, DIS_DR); // Disable data report
  return 1;
}