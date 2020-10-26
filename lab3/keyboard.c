#include <keyboard.h>

static int hook_id;
uint32_t scancode;
int counter;

int (kbc_subscribe_int)(uint8_t *bit_no) 
{
  hook_id = *bit_no;
  if (sys_irqsetpolicy(KEYBOARD1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
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

void (kbc_ih)()
{

  //Read the scancode byte from the output buffer. 
  uint32_t st;

  
    //lê a informação de STAT_REG
  if(sys_inb(STAT_REG, &st) != OK)
  {
    printf("Error in sys_inb()");
    return;
  }
  counter++;
  /* loop while 8042 output buffer is empty */
  if(st & OBF) 
  {
    if ( (st & (PARITY | TIMEOUT)) == 0 )
    {
      if(sys_inb(OUT_BUF, &scancode) != OK)
      {
        printf("Error in sys_inb()");
        return;
      }
      counter++;
    }
    else
      return;
  }
  
  return;
}

int (read_cmd_byte(uint32_t *cmd)){
  uint32_t st;
  int i=0, j=0;

  while (i<5) {
    sys_inb(STAT_REG, &st);
    if ((st & IBF) == 0){
      sys_outb(IN_BUF, READ_CB);
      j = 0;
      while (j<5){
        sys_inb(STAT_REG, &st);
        if ((st & IBF) == 0){
          sys_inb(OUT_BUF, cmd);
          break;
        }
        j++;
        tickdelay(micros_to_ticks(DELAY_US));
      }      
      break;
    }
    i++;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  if ((i==5) | (j == 5)) {
    printf("Error while\n");
    return 1;
  }
  return 0;
}

int (write_cmd_byte(uint32_t *cmd)){
  uint32_t st;
  int i=0, j=0;
  while (i<5) {
    sys_inb(STAT_REG, &st);
    if ((st & IBF) == 0){
      sys_outb(IN_BUF, WRITE_CB);
      j = 0;
      while (j<5){
        sys_inb(STAT_REG, &st);
        if ((st & IBF) == 0){
          sys_outb(OUT_BUF, *cmd);
          break;
        }
        j++;
        tickdelay(micros_to_ticks(DELAY_US));
      }      
      break;
    }
    i++;
    tickdelay(micros_to_ticks(DELAY_US));
  }
  if (i==5 | j == 5) {
    printf("Error while\n");
    return 1;
  }
  return 0;
}

void (assembleScancode(uint8_t *bytes, size_t size)) {
  bool make = true; //true is makecode; false otherwise
  if ((scancode & 0x000000FF) == 0x000000E0){
    bytes[0] = 0xE0;
    bytes[1] = scancode>>8;
    if (bytes[1]>>7 == 1)
      make = false; // breakcode
    else
      make = true; // makecode
    kbd_print_scancode(make, 2, bytes);
  }
  else {
    bytes[0] = scancode;
    if (bytes[0]>>7 == 1)
      make = false; // breakcode
    else
      make = true; // makecode
    kbd_print_scancode(make, 1, bytes);
  }
}
