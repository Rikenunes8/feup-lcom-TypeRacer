#include <mouse.h>

static int hook_id;
uint32_t scancode;
uint32_t packet_byte;
uint32_t packet_byte_counter = 0;


int (kbc_subscribe_int)(uint8_t *bit_no, uint8_t irq) 
{
  hook_id = *bit_no;
  if (sys_irqsetpolicy(irq, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
  // if (sys_irqsetpolicy(MOUSE12_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
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

void (mouse_ih)()
{

  //Read the scancode byte from the output buffer. 
  uint32_t st;

  
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
      if (st & AUX)
      {
        if(sys_inb(OUT_BUF, &packet_byte) != OK)
        {
          printf("Error in sys_inb()");
          return;
        } 
      }  
      else
      {
        if(sys_inb(OUT_BUF, &scancode) != OK)
        {
          printf("Error in sys_inb()");
          return;
        }      
      }
    }
  }
  return;
}

int (kbc_write_byte)(uint8_t cmd, uint8_t arg) {
  uint32_t st;
  int i=0;
  while (i<5) {
    sys_inb(STAT_REG, &st);
    if ((st & IBF) == 0){
      sys_outb(IN_BUF, cmd);
      sys_inb(IN_BUF, &st);
      if ((st & IBF) == 0){
        sys_outb(OUT_BUF, arg);
        sys_inb(OUT_BUF, &st);
        if (st == ACK) {
          return 0;
        }   
      }
    }
    i++;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  if (i==5) {
    printf("Error while\n");
    return 1;
  }
  return 0;
}

void assemble_packet(struct packet *pp, bool *fail_packet) {
  if (packet_byte_counter%3 == 0) {
    if (!(packet_byte & 0x08)) {
      *fail_packet = true;
      return;
    }
    else {
      pp->bytes[0] = packet_byte;
      pp->lb = packet_byte & 0x01;
      pp->rb = packet_byte & 0x02;
      pp->mb = packet_byte & 0x04;
      pp->x_ov = packet_byte & 0x40;
      pp->y_ov = packet_byte & 0x80;
      packet_byte_counter++;
      *fail_packet = false;
    }
  }
  else if(packet_byte_counter%3 == 1) {
    pp->bytes[1] = packet_byte;
    if (pp->bytes[0] & 0x10)
      pp->delta_x = -packet_byte;
    else
      pp->delta_x = packet_byte;
    packet_byte_counter++;
  }
  else {
    pp->bytes[2] = packet_byte;
    if (pp->bytes[0] & 0x20)
      pp->delta_y = -packet_byte;
    else
      pp->delta_y = packet_byte;
    packet_byte_counter++;
  }
  return;
}
