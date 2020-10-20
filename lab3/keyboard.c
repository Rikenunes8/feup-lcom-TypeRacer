#include <lcom/keyboard.h>


static int hook_id;

int keyboard_subscribe_int(uint8_t *bit_no) 
{
  hook_id = *bit_no;
  if (sys_irqsetpolicy(KEYBOARD1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
  {
    printf("Error in sys_irqsetpolicy()\n");
    return 1;
  }
  return 0;
  
}

int keyboard_unsubscribe_int() 
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
    uint8_t 
}