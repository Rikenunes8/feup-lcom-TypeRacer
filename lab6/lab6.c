// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "rtc.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}



int(video_test_init)(uint16_t mode, uint8_t delay) 
{
  uint8_t time[3];
  uint8_t bit_no = 8;

  if (rtc_subscribe_int(&bit_no) != 0) {
    printf("Error kbc_subscribe\n");
    return 0;
  }
  uint8_t timer_bit_no = 0;
  timer_subscribe_int(&timer_bit_no);
  extern int timer_counter;
  int no_seconds = 0;

  rtc_turn_on_alarm();
  rtc_set_alarm(0x00, DONT_CARE, DONT_CARE);

  int ipc_status;
  message msg;
  uint32_t irq_set = BIT(bit_no);
  uint32_t timer_irq_set = BIT(timer_bit_no);
  int r = 0;
  
  while( no_seconds < 60) { 
    
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
            rtc_ih();            
          }
          if (msg.m_notify.interrupts & timer_irq_set) 
          { 
            timer_int_handler();
            if (timer_counter%60 == 0) {
              rtc_read_time(time);
              printf("\nTime\n");
              printf("seconds: %d\n", time[0]);
              printf("minutes: %d\n", time[1]);
              printf("hours: %d\n", time[2]);

              if (get_alarm()) {
                printf("ALARMEEEEEEEEE\n");
                use_alarm();
              }
              
              no_seconds++;


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
    tickdelay(micros_to_ticks(20000));
  }

  timer_unsubscribe_int();
  rtc_unsubscribe_int();

	return 0;
}

int rtc_test_date(void) 
{
	/* To be completed */
	return 0;
}

int rtc_test_int(/* to be defined in class */) 
{ 
	/* To be completed */
	return 0;
}


