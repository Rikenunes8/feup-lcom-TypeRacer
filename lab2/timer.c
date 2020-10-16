#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

static int hook_id;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st, lsb, msb;
  uint16_t auxFreq = TIMER_FREQ/freq;
  timer_get_conf(timer, &st);
  st &= 0x0F;
  st |= TIMER_LSB_MSB;
  if (timer == 0)
    st |= TIMER_SEL0;
  else if (timer == 1)
    st |= TIMER_SEL1;
  else 
    st |= TIMER_SEL2;
  if (sys_outb(TIMER_CTRL, st) != OK)
    return 1;

  util_get_LSB(auxFreq, &lsb);
  if (sys_outb(TIMER_0+timer, lsb) != OK)
    return 1;
  util_get_MSB(auxFreq, &msb);
  if (sys_outb(TIMER_0+timer, msb) != OK)
    return 1;
  
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  hook_id = *bit_no;
  if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id) != OK) {
    printf("Error in sys_irqsetpolicy()\n");
    return 1;
  }
  return 0;
  
}

int (timer_unsubscribe_int)() {
  if (sys_irqrmpolicy(&hook_id) != OK) {
    printf("Error in sys_irqsetpolicy()\n");
    return 1;
  }
  return 0;
  
}
int counter = 0;

void (timer_int_handler)() {
  counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) 
{

  uint8_t read_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); 
  if (sys_outb(TIMER_CTRL, read_command) != OK){ // Prepare timer to be changed
    printf("Error in sys_outb()");
    return 1;
  }

  if (util_sys_inb(TIMER_0+timer, st) != OK) { // Read the changes form the timer
    printf("Error in util_sys_inb()");
    return 1;
  }
  return 0;
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

  if (timer_print_config(timer, field, b) != OK){
    printf("Error in timer_print_config\n");
    return 1;
  }
  return 0;
}
