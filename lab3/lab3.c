#include <lcom/lcf.h>

#include <lcom/lab3.h>
#include <keyboard.h>

#include <stdbool.h>
#include <stdint.h>

extern uint8_t scancode;
extern int counter_sys_inb; //counter for number os sys_inb
extern int timer_counter; //counter for timer handler

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
  bool make;

  
  //subscribe KBC interrupts
  subscribe_int(&bit_no, KEYBOARD1_IRQ, true);

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
            //assemble_scancode(bytes, &sc_two_byte);
            parse_scancode(bytes);
            if (!(bytes[0] == 0xE0 && bytes[1] == 0x00)) {
              if (bytes[0] == 0xE0) {
                if (bytes[1]>>7 == 1) //0x92 example
                  make = false; // breakcode
                else //0x12 example
                  make = true; // makecode
                kbd_print_scancode(make, 2, bytes);
              }
              else {
                if (bytes[0]>>7 == 1) //0x92 example
                  make = false; // breakcode
                else //0x12 example
                  make = true; // makecode
                kbd_print_scancode(make, 1, bytes);
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
    {  /* received a standard message, not a notification */
       /* no standard messages expected: do nothing */
    }    
    tickdelay(micros_to_ticks(DELAY_US));
  }

  //print the number of sys_inb kernell calls
  kbd_print_no_sysinb(counter_sys_inb);

  //unsubscribe KBC interrupts
  subscribe_int(&bit_no, KEYBOARD1_IRQ, false);
  return 1;
}

/*
Nas interrupções, só se verifica se OBF está cheio quando já se sabe que ele está cheio (ou seja, quando se gera a interrupção)
Com polling, verifica-se se OBF está cheio a cada iteração (ou a cada 20 ms por causa do delay).
Se estiver cheio, faz assemble_scancode(), senão volta a verificar na próxima iteração.
*/

int(kbd_test_poll)() {
  uint32_t st;
  uint32_t cmb_before, cmb_after;
  uint8_t bytes[2];
  bool make;

  while (scancode != ESC_KEY){
    if (sys_inb(STAT_REG, &st)!= OK){
      printf("Error in sys_inb()\n");
      return 1;
    }
    #ifdef LAB3
    counter_sys_inb++;
    #endif
    
    if ((st & OBF) != OK) {
      if ((st & (PARITY | TIMEOUT)) == OK){
        if (util_sys_inb(OUT_BUF, &scancode) != OK){
          printf("Error in sys_inb()\n");
          return 1;
        }
        #ifdef LAB3
        counter_sys_inb++;
        #endif
        
        parse_scancode(bytes);
        if (!(bytes[0] == 0xE0 && bytes[1] == 0x00)) {
          if (bytes[0] == 0xE0) {
            if (bytes[1]>>7 == 1) //0x92 example
              make = false; // breakcode
            else //0x12 example
              make = true; // makecode
            kbd_print_scancode(make, 2, bytes);
          }
          else {
            if (bytes[0]>>7 == 1) //0x92 example
              make = false; // breakcode
            else //0x12 example
              make = true; // makecode
            kbd_print_scancode(make, 1, bytes);
          }
        }
      }
    } 
    tickdelay(micros_to_ticks(DELAY_US));
  }

  //Antes de acabar o programa deve-se ler o command byte, 
  //alterá-lo de modo a ativar de novo as interrupções do teclado, 
  //escrever no input buffer e sair
  
  read_cmd_byte(&cmb_before);

  cmb_after = cmb_before | 0x01;

  write_cmd_byte(&cmb_after);

  kbd_print_no_sysinb(counter_sys_inb);
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
  bool make;

  //subscribe KBC interrupts
  kbc_subscribe_int(&bit_no_kbd);

  //subscribe timer interrupts
  timer_subscribe_int(&bit_no_timer);

 //number of ticks = frequency (60 Hz, base frequency) * number of seconds (n)
  while((scancode != ESC_KEY) && (timer_counter < n*60))
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
              parse_scancode(bytes);
              if (!(bytes[0] == 0xE0 && bytes[1] == 0x00)) {
                if (bytes[0] == 0xE0) {
                  if (bytes[1]>>7 == 1) //0x92 example
                    make = false; // breakcode
                  else //0x12 example
                    make = true; // makecode
                  kbd_print_scancode(make, 2, bytes);
                }
                else {
                  if (bytes[0]>>7 == 1) //0x92 example
                    make = false; // breakcode
                  else //0x12 example
                    make = true; // makecode
                  kbd_print_scancode(make, 1, bytes);
                }
              }

              timer_counter = 0; //starts counting again
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
  kbd_print_no_sysinb(counter_sys_inb);

  //unsubscribe KBC interrupts
  kbc_unsubscribe_int();

  //unsubscribe timer interrupts
  timer_unsubscribe_int();
  return 1;
}
