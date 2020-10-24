#include <keyboard.h>

static int hook_id;
uint32_t scancode;
int counter;

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
    printf("Error in sys_irqrmpolicy()\n");
    return 1;
  }
  return 0;
  
}

void (kbc_ih)()
{

  //Read the scancode byte from the output buffer. 
  uint32_t st;

  for(int i = 0; i <= 3; i++)
  {
    //lê a informação de STAT_REG
    if(sys_inb(STAT_REG, &st) != OK)
    {
      printf("Error in sys_inb()");
      return;
    }
    counter++;
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
        counter++;
      }
      else
        return;
    }
  }

    return;
}
