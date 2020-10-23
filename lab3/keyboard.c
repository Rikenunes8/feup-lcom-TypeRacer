#include <keyboard.h>



int (kbc_subscribe_int)(uint8_t *bit_no) 
{
  hook_id = *bit_no;
  if (sys_irqsetpolicy(KEYBOARD1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
  {
    printf("Error in sys_irqsetpolicy()\n");
    return 1;
  }
  return 0;
  
}

int (kbc_unsubscribe_int)() 
{
  if (sys_irqrmpolicy(&hook_id) != OK) 
  {
    printf("Error in sys_irqsetpolicy()\n");
    return 1;
  }
  return 0;
  
}

void (kbc_ih)()
{
    //Read the scancode byte from the output buffer. 
    uint32_t st;

    while(1) 
    {
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
            if(sys_inb(OUT_BUF, &scancode) != OK)
            {
              printf("Error in sys_inb()");
              return;
            }
          }
          else
            return;
        }
        tickdelay(WAIT_KBC); // e.g. tickdelay()
    }

    return;
}
