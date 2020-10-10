#include <lcom/lcf.h>

#include <stdint.h>

int(util_get_LSB)(uint16_t val, uint8_t *lsb) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
  lsb = val & 0x0000FFFF 
  return 1;
}

int(util_get_MSB)(uint16_t val, uint8_t *msb) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
  msb = val & 0xFFFF0000
  return 1;
}

int (util_sys_inb)(int port, uint8_t *value) 
{  
  uint32_t a32;
  
  if (!sys_inb(port, &a32)){
    *value = a32;
    return 0;
  }
  return 1;
}
