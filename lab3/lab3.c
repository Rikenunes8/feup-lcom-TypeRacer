#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include <keyboard.h>

#include <stdbool.h>
#include <stdint.h>

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
  /* incompleto */
  uint8_t bit_no = 1;
  int ipc_status;
  message msg;
  uint32_t irq_set = BIT(bit_no);
  int r = 0;
  uint8_t bytes[2];
  bool make; //true is makecode; false otherwise
  
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
                  if(scancode == 0xE0)
                  {     
                    bytes[0] = 0xE0;  
                    bytes[1] = scancode & 0x0000FFFF;
                    if((bytes[1]>>7) == 1) //breakcode
                      make = false;
                    else //makecode
                      make = true;
                    kbd_print_scancode(make, 2, bytes);           
                  }
                  else
                  {
                    bytes[0] = scancode;
                    if((scancode>>7) == 1) //breakcode
                      make = false;
                    else //makecode
                      make = true;
                      kbd_print_scancode(make, 1, bytes);
                  }
                  
                  
                }
                break;
            default:
              // printf("Receive no interrupt\n");
              break; /* no other notifications expected: do nothing */	
        }
    } 
    else 
    {  /* received a standard message, not a notification */
       /* no standard messages expected: do nothing */
    }
  }  

  //print the number of sys_inb kernell calls
  kbd_print_no_sysinb(counter);

  //unsubscribe KBC interrupts
  kbc_unsubscribe_int();
  return 1;
}

int(kbd_test_poll)() {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int(kbd_test_timed_scan)(uint8_t n) {
  /* To be completed by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}
