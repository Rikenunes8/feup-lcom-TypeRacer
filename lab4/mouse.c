#include <mouse.h>

static int hook_id;
uint32_t scancode;
uint32_t packet_byte;


int (mouse_subscribe_int)(uint8_t *bit_no) 
{
  hook_id = *bit_no;
  // if (sys_irqsetpolicy(irq, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
  if (sys_irqsetpolicy(MOUSE12_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
  {
    printf("Error in sys_irqsetpolicy()\n");
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)() 
{
  if (sys_irqrmpolicy(&hook_id) != OK) 
  {
    printf("Error in sys_irqrmpolicy()\n");
    return 1;
  }
  return 0;
}

void (mouse_ih)()
{

  //Read the scancode byte from the output buffer. 
  uint32_t st;

  
    //lê a informação de STAT_REG
  if(sys_inb(STAT_REG, &st) != OK)
  {
    printf("Error in sys_inb()");
    return;
  }
  /* loop while 8042 output buffer is empty */
  if(st & OBF) 
  {
    if ( (st & (PARITY | TIMEOUT)) == 0 )
    {
      if (st & AUX)
      {
        if(sys_inb(OUT_BUF, &packet_byte) != OK)
        {
          printf("Error in sys_inb()");
          return;
        } 
      }  
    }
    else
      return;
  }
  return;
}