#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {

  *lsb = val & 0x00FF; 
  return 0;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {

  *msb = (val & 0xFF00)/256;
  return 0;
}

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
