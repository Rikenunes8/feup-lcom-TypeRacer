#include <../headers/keyboard.h>

static int hook_id;
uint32_t scancode;

static bool shift_right = false;
static bool shift_left = false;

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

  
    //lê a informação de STAT_REG e coloca-a em st
  if(sys_inb(STAT_REG, &st) != OK)
  {
    printf("Error in sys_inb()");
    return;
  }
  /* loop while 8042 output buffer is empty */
  if(st & OBF) // entra no if se output buffer está cheio e se tem algo para ler (o scancode)
  {
    if ( (st & (PARITY | TIMEOUT)) == 0 ) //check if there was some communications error
    {
      if(sys_inb(OUT_BUF, &scancode) != OK) //lê a informação de OUT_BUF e coloca-a em scancode
      {
        printf("Error in sys_inb()");
        return;
      }
    }
    else
      return;
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


uint8_t get_scancode_char(uint8_t *bytes) {
  //indicates that is a two_byte scancode
  if (bytes[0] == 0xE0) {
    switch (bytes[1]) {
      case 0x4b: return L_ARROW;
      case 0x4d: return R_ARROW;
    }
  }
  else {
    if (!shift_right && !shift_left) {
      switch (bytes[0]) {
        case 0x0b: return 0; // '0'
        case 0x02: return 1; // '1'
        case 0x03: return 2;
        case 0x04: return 3;
        case 0x05: return 4;
        case 0x06: return 5;
        case 0x07: return 6;
        case 0x08: return 7;
        case 0x09: return 8;
        case 0x0a: return 9;
        case 0x1e: return 36; // 'a'
        case 0x30: return 37;
        case 0x2e: return 38;
        case 0x20: return 39;
        case 0x12: return 40;
        case 0x21: return 41;
        case 0x22: return 42;
        case 0x23: return 43;
        case 0x17: return 44;
        case 0x24: return 45;
        case 0x25: return 46;
        case 0x26: return 47;
        case 0x32: return 48;
        case 0x31: return 49;
        case 0x18: return 50;
        case 0x19: return 51;
        case 0x10: return 52;
        case 0x13: return 53;
        case 0x1f: return 54;
        case 0x14: return 55;
        case 0x16: return 56;
        case 0x2f: return 57;
        case 0x11: return 58;
        case 0x2d: return 59;
        case 0x15: return 60;
        case 0x2c: return 61;
        case 0x33: return 65; // ','
        case 0x34: return 66; // '.'
        case 0x0c: return 69; // '''
      }
    }
    else {
      switch (bytes[0]) {
        case 0x1e: return 10; // 'A'
        case 0x30: return 11;
        case 0x2e: return 12;
        case 0x20: return 13;
        case 0x12: return 14;
        case 0x21: return 15;
        case 0x22: return 16;
        case 0x23: return 17;
        case 0x17: return 18;
        case 0x24: return 19;
        case 0x25: return 20;
        case 0x26: return 21;
        case 0x32: return 22;
        case 0x31: return 23;
        case 0x18: return 24;
        case 0x19: return 25;
        case 0x10: return 26;
        case 0x13: return 27;
        case 0x1f: return 28;
        case 0x14: return 29;
        case 0x16: return 30;
        case 0x2f: return 31;
        case 0x11: return 32;
        case 0x2d: return 33;
        case 0x15: return 34;
        case 0x2c: return 35; // 'Z'
        case 0x34: return 63; // ':'
        case 0x33: return 64; // ';'
        case 0x02: return 67; // '!'
        case 0x0c: return 68; // '?'
        case 0x03: return 70; // '"'
      }
    }
    switch (bytes[0]) {
      case 0x39: return SPACE; // ' '
      case 0x36: shift_left = true; return NOTHING;
      case 0x2a: shift_right = true; return NOTHING;
      case 0xb6: shift_left = false; return NOTHING;
      case 0xaa: shift_right = false; return NOTHING;
      case 0x0e: return BACKSPACE; // backspace
      default: return NOTHING;
    }
  }
  return 100;
}
