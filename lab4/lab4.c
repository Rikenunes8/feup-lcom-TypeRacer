// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab4.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include <mouse.h>

extern uint32_t packet_byte_counter;
extern bool fail_packet;

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
  /*if (mouse_enable_data_reporting() != OK) {
    printf("Error enable reporting\n");
    return 1;
  }*/
  kbc_write_byte(WRT_MOUSE, ENB_DR);

  uint8_t bit_no = 12;

  if (kbc_subscribe_int(&bit_no, MOUSE12_IRQ) != 0) {
    printf("Error kbc_subscribe\n");
    return 0;
  }


  int ipc_status;
  message msg;
  uint32_t irq_set = BIT(bit_no);
  int r = 0;
  
  struct packet pp;

  while( cnt>0) { 
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
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
              cnt--;
              packet_byte_counter = 0;
              assemble_packet(&pp);
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
  kbc_write_byte(WRT_MOUSE, ENB_DR);

  uint8_t bit_no_mouse = 12;
  uint8_t bit_no_timer = 0;

  kbc_subscribe_int(&bit_no_mouse, MOUSE12_IRQ);
  timer_subscribe_int(&bit_no_timer);

  int ipc_status;
  message msg;
  uint32_t irq_set_mouse = BIT(bit_no_mouse);
  uint32_t irq_set_timer = BIT(bit_no_timer);
  int r = 0;
  
  struct packet pp;

  while(timer_counter < (int)(idle_time * sys_hz())) { 
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) {   /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & irq_set_timer) { 
            timer_int_handler();
          }
          if (msg.m_notify.interrupts & irq_set_mouse) { 
            /* mouse interrupt */ 
            mouse_ih();
            if (packet_byte_counter == 3) {
              packet_byte_counter = 0;
              assemble_packet(&pp);
              mouse_print_packet(&pp);
            }
            timer_counter = 0;
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

  timer_unsubscribe_int();
  kbc_unsubscribe_int();
  kbc_write_byte(WRT_MOUSE, DIS_DR);
  return 1;
}

int (mouse_test_gesture)(uint8_t x_len, uint8_t tolerance) {

  if (kbc_write_byte(WRT_MOUSE, ENB_DR) != 0) // Enable data report
    return 1;

  uint8_t bit_no = 12;

  if (kbc_subscribe_int(&bit_no, MOUSE12_IRQ) != 0) {
    printf("Error kbc_subscribe\n");
    return 1;
  }


  State state = INIT;
  Mouse_event event;
  int32_t move_x = 0, move_y = 0, errorx = 0, errory = 0;
  int32_t dx, dy;


  struct packet pp;
  int ipc_status;
  message msg;
  uint32_t irq_set = BIT(bit_no);
  int r = 0;

  while(state != FINAL) { 
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
            if (packet_byte_counter == 3) {
              packet_byte_counter = 0;
              assemble_packet(&pp);
              mouse_print_packet(&pp);
              mouse_events(&event, &pp);
              dx = pp.delta_x;
              dy = pp.delta_y;
              printf("Move_x: %d\n", move_x);
              printf("Move_y: %d\n", move_y);
              switch (event) {
                case LB_DOWN:
                  printf("LB_DOWN\n");
                  break;
                case LB_UP:
                  printf("LB_UP\n");
                  break;
                case MB_DOWN:
                  printf("MB_DOWN\n");
                  break;
                case MB_UP:
                  printf("MB_UP\n");
                  break;
                case RB_DOWN:
                  printf("RB_DOWN\n");
                  break;
                case RB_UP:
                  printf("RB_UP\n");
                  break;
                case MOVE:
                  printf("MOVE\n");
                  break;
                case MANY_DOWN:
                  printf("MANY_DOWN\n");
                  break;
              }
              switch (state) {
                case INIT:
                  printf("INIT\n");
                  if (event == LB_DOWN) {
                    state = DRAW1;
                    move_x = 0; move_y = 0;
                    errorx = 0; errory = 0;
                  }
                  break;
                case DRAW1:
                  printf("DRAW1\n");
                  if (dx < 0)
                    errorx += abs(dx);
                  if (dy < 0)
                    errory += abs(dy);
                  if (errorx > tolerance || errory > tolerance) 
                    state = INIT;
                  else if (event == LB_DOWN) {
                    move_x += dx;
                    move_y += dy;
                  }
                  else if (event == LB_UP) {
                    if (move_y > move_x && move_x >= x_len) {
                      state = VERTEX;
                      move_x = 0; move_y = 0;
                      errorx = 0; errory = 0;
                    }
                    else 
                      state = INIT;
                  }
                  else
                    state = INIT;
                  break;
                case VERTEX:
                  printf("VERTEX\n");
                  if (move_x > tolerance && move_y > tolerance)
                    state = INIT;
                  else if (event == MOVE) {
                    move_x += abs(dx);
                    move_y += abs(dy);
                  }
                  else if (event == RB_DOWN) {
                    state = DRAW2;
                    move_x = 0; move_y = 0;
                    errorx = 0; errory = 0;
                  }
                  else
                    state = INIT;
                  break;
                case DRAW2:
                  printf("DRAW2\n");
                  if (dx < 0)
                    errorx += abs(dx);
                  if (dy > 0)
                    errory += abs(dy);
                  if (errorx > tolerance || errory > tolerance) 
                    state = INIT;
                  else if (event == RB_DOWN) {
                    move_x += dx;
                    move_y += dy;
                  }
                  else if (event == RB_UP) {
                    if (abs(move_y) > move_x && move_x >= x_len) {
                      state = FINAL;
                      move_x = 0; move_y = 0;
                      errorx = 0; errory = 0;
                    }
                    else 
                      state = INIT;
                  }
                  else
                    state = INIT;
                  break;
                case FINAL:
                  printf("FINAL\n");
                  break;
              }
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
  printf("END\n");

  kbc_unsubscribe_int();
  kbc_write_byte(WRT_MOUSE, DIS_DR); // Disable data report
  return 1;

}

int (mouse_test_remote)(uint16_t period, uint8_t cnt) {
  struct packet pp;

  while (cnt>0) {
    kbc_write_byte(WRT_MOUSE, READ_DATA);
    mouse_ih();
    if (packet_byte_counter == 3) {
      cnt--;
      packet_byte_counter = 0;
      assemble_packet(&pp);
      mouse_print_packet(&pp);
      tickdelay(micros_to_ticks(period*1000));
    }
  }

  kbc_write_byte(WRT_MOUSE, SET_SM);
  kbc_write_byte(WRT_MOUSE, DIS_DR);
  kbc_write_byte(WRITE_CB, minix_get_dflt_kbc_cmd_byte());
  return 0;
}

