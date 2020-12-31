#include <lcom/lcf.h>

#include "../headers/mouse.h"  

static int hook_id;
static uint8_t PACKET[3];
uint8_t packet_byte_counter = 0;
int32_t mouse_x = 0;
int32_t mouse_y = 0;



int (mouse_subscribe_int)(uint8_t *bit_no) 
{
  hook_id = *bit_no;
  //if (sys_irqsetpolicy(irq, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
  if (sys_irqsetpolicy(MOUSE12_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
  {
    printf("Error in sys_irqsetpolicy()\n");
    return 1;
  }
  return 0;
}

int (mouse_unsubscribe_int)() 
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
  if(util_sys_inb(STAT_REG, &st) != OK) return;
  /* loop while 8042 output buffer is empty */
  if ((st & AUX) == 0) return;
  //printf("mouse_byte\n");
  if ((st & (PARITY | TIMEOUT)) != OK ) return;  
  if ((st & OBF) == 0) return;

  if(util_sys_inb(OUT_BUF, &packet_byte) != OK)
    return;
  if ((packet_byte_counter == 0) && ((packet_byte & CTRL_B) == 0)) 
    return;
  else {
    PACKET[packet_byte_counter%3] = packet_byte;
    packet_byte_counter++;
  }
  return;
}

int kbc_write_byte(uint8_t cmd, uint8_t arg) {
  uint8_t st;
  int i=0;
  while (i<5) {
    util_sys_inb(STAT_REG, &st);
    if ((st & IBF) == 0){
      sys_outb(IN_BUF, cmd);
      if (arg == 0)
        return 1;
      util_sys_inb(STAT_REG, &st);
      if ((st & IBF) == 0){
        sys_outb(OUT_BUF, arg);
        util_sys_inb(OUT_BUF, &st);
        if (st == ACK) {
          //printf("ACK\n");
          return 0;
        }   
        else if (st == NACK) {
          printf("NACK\n");
        }
        else if (st == ERROR) {
          printf("ERROR\n");
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
  return;
}

void mouse_events(Mouse_event *event, struct packet *pp) {
  if ((pp->lb && pp->mb) || (pp->lb && pp->rb) || (pp->mb && pp->rb) || (pp->lb && pp->mb && pp->rb))
    event->ev = MANY_DOWN;
  else if (pp->lb)
    event->ev = LB_DOWN;
  else if (event->ev == LB_DOWN && !pp->lb)
    event->ev = LB_UP;
  else if (pp->mb)
    event->ev = MB_DOWN;
  else if (event->ev == MB_DOWN && !pp->mb)
    event->ev = MB_UP;
  else if(pp->rb)
    event->ev = RB_DOWN;
  else if(event->ev == RB_DOWN && !pp->rb)
    event->ev = RB_UP;
  else
    event->ev = MOVE;
  event->dx = pp->delta_x;
  event->dy = pp->delta_y;
  return;
}
