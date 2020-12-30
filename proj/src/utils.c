#include <lcom/lcf.h>
#include <../headers/utils.h>


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

int (bcd_to_binary)(uint8_t *value) {
  uint8_t b=0;
  uint8_t mask = 0x0F;
  b = *value & mask;
  b += (*value>>4)*10;
  *value = b;
  return 0;
}

uint8_t (binary_to_bcd)(uint8_t value) {
  uint8_t first = value%10;
  uint8_t second = value/10;
  value = first | (second<<4);
  return value;
}

