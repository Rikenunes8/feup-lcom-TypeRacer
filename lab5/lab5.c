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
extern int timer_counter;
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
  //vbe_get_mode_info(mode, &info);
  graphic_get_mode_info(mode, &info);
  graphic_def(&info);
  graphic_init(mode, SET_VBE_MODE);

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
  //vbe_get_mode_info(mode, &info);
  graphic_get_mode_info(mode, &info);
  graphic_def(&info);
  graphic_init(mode, SET_VBE_MODE);

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
  //vbe_get_mode_info(mode, &info);
  graphic_get_mode_info(mode, &info);
  graphic_def(&info);
  graphic_init(mode, SET_VBE_MODE);

  //draw a rectangle
  //vg_draw_rectangle(x, y, width, height, color);
  vg_draw_pattern(mode, no_rectangles, first, step);

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

  //return to text mode
  if(vg_exit() != OK)
  {
    printf("Error in vg_exit() \n");
    return 1;
  };

  return 0;

  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  uint8_t bit_no = 1;
  int ipc_status;
  message msg;
  uint32_t irq_set = BIT(bit_no);
  int r = 0;
  uint16_t mode = 0x115;
  
  enum xpm_image_type type;
  switch (mode) {
    case 0x105: type = XPM_INDEXED; break;
    case 0x110: type = XPM_1_5_5_5; break;
    case 0x115: type = XPM_8_8_8; break;
    case 0x11A: type = XPM_5_6_5; break;
    case 0x14C: type = XPM_8_8_8_8; break;
  }


  // usar o keyboard para ler a tecla ESC
  //subscribe KBC interrupts
  kbc_subscribe_int(&bit_no);

  //mudar para modo gráfico
  //map the video memory to the process' adress space
  vbe_mode_info_t info;
  //vbe_get_mode_info(mode, &info);
  graphic_get_mode_info(mode, &info);
  graphic_def(&info);
  graphic_init(mode, SET_VBE_MODE);

  // Para testar 0x105 comentar proxima linha, alterar argumento new para xpm em graphic_general_xpm e alterar mode
  xpm_map_t new = (const xpm_row_t*)&ubuntu_xpm;
  
  graphic_general_xpm(new, x, y, type);


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

  //return to text mode
  if(vg_exit() != OK)
  {
    printf("Error in vg_exit() \n");
    return 1;
  };

  return 0;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  uint8_t timer_bit_no = 0;
  uint8_t kbd_bit_no = 1;
  int ipc_status;
  message msg;
  uint32_t timer_irq_set = BIT(timer_bit_no);
  uint32_t kbd_irq_set = BIT(kbd_bit_no);
  int r = 0;
  uint16_t mode = 0x105;
  bool axis; // Positive to move in yy, negative to move in xx
  int16_t way;
  uint8_t frame_counter = 0;

  // usar o keyboard para ler a tecla ESC
  //subscribe KBC interrupts
  kbc_subscribe_int(&kbd_bit_no);
  timer_subscribe_int(&timer_bit_no);

  //mudar para modo gráfico
  //map the video memory to the process' adress space
  vbe_mode_info_t info;
  //vbe_get_mode_info(mode, &info);
  graphic_get_mode_info(mode, &info);
  graphic_def(&info);
  graphic_init(mode, SET_VBE_MODE);


  if (xi == xf) {
    axis = true;
    if (yi < yf)
      way = 1;
    else 
      way = -1;
  }
  else {
    axis = false;
    if (xi < xf)
      way = 1;
    else 
      way = -1;
  }
  // First image
  graphic_xpm(xpm, xi, yi, false);

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
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            if (timer_counter == 60) {
              timer_counter = 0;
            }
          }				
          if (msg.m_notify.interrupts & kbd_irq_set) 
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

    if (yi != yf || xi != xf) {
      if (timer_counter%(sys_hz()/fr_rate) == 0) {
        if (speed > 0) { 
          graphic_xpm(xpm, xi, yi, true);
          if (axis) {
            yi += way*speed;
            if (speed > abs(yf-yi))
              speed = abs(yf - yi);
          }
          else {
            xi += way*speed;
            if (speed > abs(xf-xi))
              speed = abs(xf - xi);
          } 
          graphic_xpm(xpm, xi, yi, false);
        }
        else {
          frame_counter++;
          if (frame_counter%(-speed) == 0) {
            frame_counter = 0;
            graphic_xpm(xpm, xi, yi, true);
            if (axis)
              yi += way;
            else
              xi += way;
            graphic_xpm(xpm, xi, yi, false);
          }
        }
      }
    }
    tickdelay(micros_to_ticks(DELAY_US));
  }

  timer_unsubscribe_int();
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

int(video_test_controller)() {
  vg_vbe_contr_info_t info;
  
  graphic_cntrl_info(&info);
  
  vg_display_vbe_contr_info(&info);
  return 0;
}
