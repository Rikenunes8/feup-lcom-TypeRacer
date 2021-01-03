#include <lcom/lcf.h>

#include <../headers/keyboard.h>

static int hook_id;
uint32_t scancode;

int (kbd_subscribe_int)(uint8_t *bit_no) 
{
  hook_id = *bit_no;
  if (sys_irqsetpolicy(KEYBOARD1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) 
  {
    printf("Error in sys_irqsetpolicy()\n");
    return 1;
  }
  return 0;
  
}

int (kbd_unsubscribe_int)() 
{
  if (sys_irqrmpolicy(&hook_id) != OK) 
  {
    printf("Error in sys_irqrmpolicy()\n");
    return 1;
  }
  return 0;
  
}

void (kbd_ih)()
{

  //Read the scancode byte from the output buffer. 
  uint32_t st;

  
    //lê a informação de STAT_REG e coloca-a em st
  if(sys_inb(STAT_REG, &st) != OK)
  {
    printf("Error in sys_inb()");
    return;
  }

  if ((st & AUX) != 0) return;

  if ( (st & (PARITY | TIMEOUT)) == 0 ) //check if there was some communications error
  {
    if(st & OBF) // entra no if se output buffer está cheio e se tem algo para ler (o scancode)
    {
      if(sys_inb(OUT_BUF, &scancode) != OK) //lê a informação de OUT_BUF e coloca-a em scancode
      {
        printf("Error in sys_inb()");
        return;
      }
    }
  }
  
  return;
}

/*
Escrevemos em IN_BUF um comando (0x20) para pedir para ler um command byte. 
Depois, lemos esse command byte em OUT_BUF e colocamo-lo em cmd. 
O command byte é o byte que me permitirá reativar as interrupções.
*/
int read_cmd_byte(uint8_t *cmd){
  uint8_t st;
  int i=0, j=0;

  while (i<5) {
    util_sys_inb(STAT_REG, &st); //lê STAT_REG e põe em st a informação lida
    if ((st & IBF) == 0){ //entra no if se input buffer estiver vazio
      sys_outb(IN_BUF, READ_CB); //estou a passar ao IN_BUF um comando
      j = 0;
      while (j<5){
        util_sys_inb(STAT_REG, &st);
        if ((st & IBF) == 0){ //entra no if se input buffer estiver vazio
          util_sys_inb(OUT_BUF, cmd); //lê a informação de OUT_BUF  e coloca-a em cmd
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

int write_cmd_byte(uint8_t *cmd){
  uint8_t st;
  int i=0;
  while (i<5) {
    util_sys_inb(STAT_REG, &st);
    if ((st & IBF) == 0){
      sys_outb(IN_BUF, WRITE_CB);
      util_sys_inb(STAT_REG, &st);
      if ((st & IBF) == 0){
        sys_outb(OUT_BUF, *cmd);
        break;
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

void assemble_scancode(uint8_t *bytes) {
  if (scancode == 0xE0) {
    bytes[0] = scancode;
    bytes[1] = 0x00;
  }
  else {
    if ((bytes[0] == 0xE0) && (bytes[1] == 0x00)) {
      bytes[1] = scancode;
    }
    else {
      bytes[0] = scancode;
      bytes[1] = 0x00;
    }
  }
}

void print_scancode(uint8_t *bytes) {
  bool make;
  if (bytes[0] == 0xE0) {
    if (bytes[1]>>7 == 1) //0x92e0 example
      make = false; // breakcode
    else //0x12e0 example
      make = true; // makecode
    kbd_print_scancode(make, 2, bytes);
  }
  else {
    if (bytes[0]>>7 == 1) //0x92 example
      make = false; // breakcode
    else //0x12 example
      make = true; // makecode
    kbd_print_scancode(make, 1, bytes);
  }
}



