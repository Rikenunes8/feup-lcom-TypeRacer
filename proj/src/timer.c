#include <lcom/lcf.h>
#include <stdint.h>


#include <../headers/timer.h>


static int hook_id;
uint32_t timer_counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  uint8_t st, lsb, msb;
  uint16_t auxFreq = TIMER_FREQ/freq;
  timer_get_conf(timer, &st); //coloca em st a configuração do timer 
  st &= 0x0F; //faz com que 4 LSBs não sejam alterados
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

void (timer_ih)() {
  timer_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) 
{

  uint8_t read_command = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); //assembling the control word 
  if (sys_outb(TIMER_CTRL, read_command) != OK){ // Estou a passar ao controlador informação para aceder ao timer dado
    printf("Error in sys_outb()");
    return 1;
  }

  if (util_sys_inb(TIMER_0+timer, st) != OK) { // Lê a informação do timer dado
    printf("Error in util_sys_inb()");
    return 1;
  }
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) 
{
  union timer_status_field_val b;

  //parseing the status word
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
    b.count_mode = st/2 & 0x07; //Para aceder ao counting mode de status, devo descobrir que bits estão nas posições 3,2,1
                                //st/2 anda 1 bit para a direita e comparamos st com 111 (0x07)
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
