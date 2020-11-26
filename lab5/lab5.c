// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <lcom/lab5.h>
#include <machine/int86.h>
#include <vbe.h>
#include <graphic.h>
#include <keyboard.h>

#include <stdint.h>
#include <stdio.h>

extern uint32_t scancode;

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
  vbe_mode_info_t info;
  vbe_get_mode_info(mode, &info);

  graphic_init(mode, &info, SET_VBE_MODE);

  sleep(delay);

  if(vg_exit() != OK)
  {
    printf("Error in vg_exit() \n");
    return 1;
  };

  return 0;
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  /* To be completed */
  uint8_t bit_no = 1;
  int ipc_status;
  message msg;
  uint32_t irq_set = BIT(bit_no);
  int r = 0;

  // usar o keyboard para ler a tecla ESC
  //subscribe KBC interrupts
  kbc_subscribe_int(&bit_no);

  //mudar para modo gráfico
  //map the video memory to the process' adress space
  vbe_mode_info_t info;
  vbe_get_mode_info(mode, &info);
  graphic_init(mode, &info, SET_VBE_MODE);

  //draw a rectangle
  vg_draw_rectangle(x, y, width, height, color);

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
    //draw a rectangle
    //vg_draw_rectangle(x, y, width, height, color);

    tickdelay(micros_to_ticks(DELAY_US));
  }
  //unsubscribe KBC interrupts
  kbc_unsubscribe_int();

  //return to text mode
  if(vg_exit() != OK)
  {
    printf("Error in vg_exit() \n");
    return 1;
  };

  return 0;
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  /* To be completed */
  printf("%s(0x%03x, %u, 0x%08x, %d): under construction\n", __func__,
         mode, no_rectangles, first, step);

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
