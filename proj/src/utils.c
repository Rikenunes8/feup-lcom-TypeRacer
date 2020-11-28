#include <../headers/utils.h>

static int hook_id;

int subscribe_int(uint8_t *bit_no, uint8_t irq, int policy) 
{
  hook_id = *bit_no;
  if (sys_irqsetpolicy(irq, policy, &hook_id) != OK)  
  {
    printf("Error in sys_irqsetpolicy()\n");
    return 1;
  }
  return 0;
}

int unsubscribe_int() 
{
  if (sys_irqrmpolicy(&hook_id) != OK) 
  {
    printf("Error in sys_irqrmpolicy()\n");
    return 1;
  }
  return 0;
}

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  *lsb = val & 0x00FF; 
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  *msb = (val & 0xFF00)/256; //move 8 bits para a direita 
  return 0;
}

//Nós só precisamos de ler 1 byte (8 bits), por isso usamos esta função para ler apenas 1 byte
int (util_sys_inb)(int port, uint8_t *value) 
{  
  uint32_t a32;
  
  if (sys_inb(port, &a32) != 0) {
    printf("Error in sys_inb()");
    return 1;
  }
  *value = a32;
  return 0;
}
