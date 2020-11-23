#include <mouse.h>

static int hook_id;
static uint8_t PACKET[3];
bool fail_packet = false;
uint32_t packet_byte_counter = 0;



int (kbc_subscribe_int)(uint8_t *bit_no, uint8_t irq) 
{
  hook_id = *bit_no;
  if (sys_irqsetpolicy(irq, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
  //if (sys_irqsetpolicy(MOUSE12_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
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
  uint8_t st;
  uint8_t packet_byte;

    //lê a informação de STAT_REG
  if(util_sys_inb(STAT_REG, &st) != OK)
    return;

  /* loop while 8042 output buffer is empty */
  if ((st & (PARITY | TIMEOUT)) == OK ) {  
    if ((st & OBF) != 0) {
      if(util_sys_inb(OUT_BUF, &packet_byte) != OK)
        return;
      if ((packet_byte_counter == 0) && ((packet_byte & CTRL_B) == 0)) 
        return;
      else {
        PACKET[packet_byte_counter%3] = packet_byte;
        packet_byte_counter++;
      }
    }
    else
      printf("Erro OBF\n");
  }
  else 
    printf("Error in parity and timeout\n");
  return;
}

int (kbc_write_byte)(uint8_t cmd, uint8_t arg) {
  uint8_t st;
  int i=0;
  while (i<5) {
    util_sys_inb(STAT_REG, &st);
    if ((st & IBF) == 0){
      sys_outb(IN_BUF, cmd);
      util_sys_inb(STAT_REG, &st);
      if ((st & IBF) == 0){
        sys_outb(OUT_BUF, arg);
        util_sys_inb(OUT_BUF, &st);
        if (st == ACK) {
          //printf("ACK\n");
          return 0;
        }   
        else {
          printf("%d ERROR\n", i);
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


void assemble_packet(struct packet *pp) {
  //printf("count %d\n", packet_byte_counter);
  if (packet_byte_counter%3 == 0 && !fail_packet) {
    //printf("%x, %x, %x\n", PACKET[0], PACKET[1], PACKET[2]);
    pp->bytes[0] = PACKET[0];
    pp->lb = PACKET[0] & LB;
    pp->rb = PACKET[0] & RB;
    pp->mb = PACKET[0] & MB;
    pp->x_ov = PACKET[0] & XOV;
    pp->y_ov = PACKET[0] & YOV;

    pp->bytes[1] = PACKET[1];
    if (pp->bytes[0] & MSB_X)
      pp->delta_x = PACKET[1] | 0xFF00 ;
    else
      pp->delta_x = PACKET[1];

    pp->bytes[2] = PACKET[2];
    if (pp->bytes[0] & MSB_Y)
      pp->delta_y = PACKET[2] | 0xFF00;
    else
      pp->delta_y = PACKET[2];
  }
  
  return;
}
