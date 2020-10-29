#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include <keyboard.h>

#include <stdbool.h>
#include <stdint.h>

extern uint32_t scancode;
extern int counter; //counter for number os sys_inb
extern int count; //counter for timer handler

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab3/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab3/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(kbd_test_scan)() 
{
  uint8_t bit_no = 1;

  int ipc_status;
  message msg;
  uint32_t irq_set = BIT(bit_no);
  int r = 0;
  uint8_t bytes[2];
  
  //subscribe KBC interrupts
  kbc_subscribe_int(&bit_no);

  while(scancode != ESC_KEY) 
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
            /* subscribed interrupt */
            kbc_ih();
  
            //assemble the scancode
            assemble_scancode(bytes, 2);
          }
          break;
        default:
          printf("Receive no interrupt\n");
          break; /* no other notifications expected: do nothing */	
      }
    } 
    else 
    {  /* received a standard message, not a notification */
       /* no standard messages expected: do nothing */
    }
    
    tickdelay(micros_to_ticks(DELAY_US));
  }

  //print the number of sys_inb kernell calls
  kbd_print_no_sysinb(counter);

  //unsubscribe KBC interrupts
  kbc_unsubscribe_int();
  return 1;
}



int(kbd_test_poll)() {
  uint32_t st;
  uint32_t cmb_before, cmb_after;
  uint8_t bytes[2];

  while (scancode != ESC_KEY){
    if (sys_inb(STAT_REG, &st)!= OK){
      printf("Error in sys_inb()\n");
      return 1;
    }
    counter++;
    if ((st & OBF) != OK) {
      if ((st & (PARITY | TIMEOUT)) == OK){
        if (sys_inb(OUT_BUF, &scancode) != OK){
          printf("Error in sys_inb()\n");
          return 1;
        }
        counter++;
        
        assemble_scancode(bytes, 2);
      }
    } 
    tickdelay(micros_to_ticks(DELAY_US));
  }
  
  read_cmd_byte(&cmb_before);

  cmb_after = cmb_before | 0x01;

  write_cmd_byte(&cmb_after);

  kbd_print_no_sysinb(counter);
  return 0;
}

int(kbd_test_timed_scan)(uint8_t n) 
{

  uint8_t bit_no_kbd = 1;
  uint8_t bit_no_timer = 0;

  int ipc_status;
  message msg;
  uint32_t kbd_int_bit = BIT(bit_no_kbd);
  uint32_t timer0_int_bit = BIT(bit_no_timer);
  int r = 0;
  uint8_t bytes[2];

  //subscribe KBC interrupts
  kbc_subscribe_int(&bit_no_kbd);

  //subscribe timer interrupts
  timer_subscribe_int(&bit_no_timer);

 //number of ticks = frequency (60 Hz, base frequency) * number of seconds (n)
  while((scancode != ESC_KEY) && (count < n*60))
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
        if (msg.m_notify.interrupts &  kbd_int_bit) 
          { 
            /* process KBD interrupt request */
            kbc_ih();
  
            //assemble the scancode
            assemble_scancode(bytes, 2);

            count = 0; //starts counting again
          }
        if (msg.m_notify.interrupts & timer0_int_bit)
          {
            /* process Timer0 interrupt request */
            timer_int_handler();
          }				
          break;
        default:
          printf("Receive no interrupt\n");
          break; /* no other notifications expected: do nothing */	
      }
    } 
    else 
    {  /* received a standard message, not a notification */
        /* no standard messages expected: do nothing */
    }

  tickdelay(micros_to_ticks(DELAY_US));
  }

  //print the number of sys_inb kernell calls
  kbd_print_no_sysinb(counter);

  //unsubscribe KBC interrupts
  kbc_unsubscribe_int();

  //unsubscribe timer interrupts
  timer_unsubscribe_int();
  return 1;
}
