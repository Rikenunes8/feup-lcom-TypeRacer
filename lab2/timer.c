#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
    /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

int (timer_unsubscribe_int)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);

  return 1;
}

void (timer_int_handler)() {
  /* To be implemented by the students */
  printf("%s is not yet implemented!\n", __func__);
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) 
{

  uint8_t read_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); 
  sys_outb(TIMER_CTRL, read_command); // Prepare timer to be changed

  if (!util_sys_inb(TIMER_0+timer, st)) // Read the changes form the timer
  { 
    return 0;
  }
  return 1;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) 
{
  union timer_status_field_val b;
  if (field == tsf_all){
    b.byte = st;
  }
  else if (field == tsf_initial)
  {
    if ((st & TIMER_LSB_MSB) == TIMER_LSB)
      b.in_mode = 1;
    else if ((st & TIMER_LSB_MSB) == TIMER_MSB)
      b.in_mode = 2;
    else if ((st & TIMER_LSB_MSB) == TIMER_LSB_MSB)
      b.in_mode = 3;
    else
      b.in_mode = 0;
  }
  else if (field == tsf_mode){
    b.count_mode = st/2 & 0x07;
  }
  else if ( field == tsf_base) {
    b.bcd = st & TIMER_BCD;
  }

  if (!timer_print_config(timer, field, b)){
    return 0;
  }
  return 1;
}
