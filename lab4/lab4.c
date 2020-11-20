// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab4.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include <mouse.h>

extern uint32_t packet_byte_counter;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need/ it]
  lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab4/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int (mouse_test_packet)(uint32_t cnt) {

  //mouse_enable_data_reporting();
  kbc_write_byte(WRT_MOUSE, ENB_DR);

  uint8_t bit_no = 12;

  kbc_subscribe_int(&bit_no, MOUSE12_IRQ);

  bool fail_packet = false;
  int ipc_status;
  message msg;
  uint32_t irq_set = BIT(bit_no);
  int r = 0;
  
  struct packet pp;

  while(packet_byte_counter/3 < cnt) { 
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) {   /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & irq_set) { 
            /* mouse interrupt */ 
            mouse_ih();

            assemble_packet(&pp, &fail_packet);

            if (packet_byte_counter%3 == 0) {
              mouse_print_packet(&pp);
            }
          }
          break;
        default:
          printf("Receive no interrupt\n");
          break; /* no other notifications expected: do nothing */	
      }
    } 
    else {  
      /* received a standard message, not a notification */
       /* no standard messages expected: do nothing */
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }

  kbc_unsubscribe_int();
  kbc_write_byte(WRT_MOUSE, DIS_DR);
  return 1;
}

extern int timer_counter;

int (mouse_test_async)(uint8_t idle_time) {
  //mouse_enable_data_reporting();
  kbc_write_byte(WRT_MOUSE, ENB_DR);

  uint8_t bit_no_mouse = 12;
  uint8_t bit_no_timer = 0;

  kbc_subscribe_int(&bit_no_mouse, MOUSE12_IRQ);
  timer_subscribe_int(&bit_no_timer);

  bool fail_packet = false;
  int ipc_status;
  message msg;
  uint32_t irq_set_mouse = BIT(bit_no_mouse);
  uint32_t irq_set_timer = BIT(bit_no_timer);
  int r = 0;
  
  struct packet pp;
  printf("Time to end: %d\n", idle_time * sys_hz());

  while((uint32_t)timer_counter < ( idle_time * sys_hz())) { 
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) {   /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & irq_set_mouse) { 
            /* mouse interrupt */ 
            mouse_ih();

            assemble_packet(&pp, &fail_packet);
            timer_counter = 0;
          }
          if (msg.m_notify.interrupts & irq_set_timer) { 
            timer_int_handler();
          }
          break;
        default:
          printf("Receive no interrupt\n");
          break; /* no other notifications expected: do nothing */	
      }
    } 
    else {  
      /* received a standard message, not a notification */
       /* no standard messages expected: do nothing */
    }
    if (packet_byte_counter%3 == 0) {
      mouse_print_packet(&pp);
    }
    tickdelay(micros_to_ticks(DELAY_US));
    printf("Timer counter: %d\n", timer_counter);
  }

  timer_unsubscribe_int();
  kbc_unsubscribe_int();
  kbc_write_byte(WRT_MOUSE, DIS_DR);
  return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {
    /* To be completed */
    printf("%s: under construction\n", __func__);
    return 1;
}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
  bool fail_packet = false;
  struct packet pp;

  while (packet_byte_counter/3 < cnt) {
    kbc_write_byte(WRT_MOUSE, ST_REQUEST);

    for (int i = 0;i<3;i++) {
      mouse_ih();

      assemble_packet(&pp, &fail_packet);
    }

    if (packet_byte_counter%3 == 0) {
        mouse_print_packet(&pp);
    }

    tickdelay(period);
  }

  kbc_write_byte(WRT_MOUSE, SET_SM);
  kbc_write_byte(WRT_MOUSE, DIS_DR);
  minix_get_dflt_kbc_cmd_byte();
  return 0;
}
