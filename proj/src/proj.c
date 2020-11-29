#include <lcom/lcf.h>


#include "../headers/proj.h"
#include "../headers/graphic.h"
#include "../headers/xpixmap.h"
#include "../headers/keyboard.h"
#include <machine/int86.h>

extern xpm_map_t letters[];
extern uint32_t scancode;

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

  char *text = "Yeah, they got you where they want you. There's a better life and you think about it, don't you? It's a rich man's game no matter what they call it and you spend your life putting money in his wallet.";
  
  uint8_t bit_no = 1;
  int ipc_status;
  message msg;
  uint32_t irq_set = BIT(bit_no);
  int r = 0;
  kbc_subscribe_int(&bit_no);


  vbe_mode_info_t info;
  uint16_t mode = 0x105;
  //vbe_get_mode_info(mode, &info);
  graphic_get_mode_info(mode, &info);
  graphic_def(&info);
  graphic_init(mode, SET_VBE_MODE);
  
  vg_draw_rectangle(0,0,1024, 768, 63);
  sleep(2);

  int32_t i = 0;
  int32_t j = 0;
  for (size_t n = 0; n < strlen(text); n++) {
    graphic_xpm(letters[get_char_xpm(text[n])], 20 + i*(10+2), 20+j*(14+3), false);
    i++;
    if (text[n] == ' ' && 20 + i > 90) {
      j++; i = 0;
    }
  }

  char * inst = "ESC to exit";
  for (size_t n = 0; n < strlen(inst); n++) {
    graphic_xpm(letters[get_char_xpm(inst[n])], 20 + n*(10+2), 100, false);
  }
  
  //sair através da ESC key
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
          }
          break;
        default:
          printf("Receive no interrupt\n");
          break; /* no other notifications expected: do nothing */	
      }
    } 

    tickdelay(micros_to_ticks(DELAY_US));
  }
  //unsubscribe KBC interrupts
  kbc_unsubscribe_int();

  vg_exit();
  return 0;
}
