#include "../headers/race.h"
#include "../headers/xpixmap.h"
#include "../headers/graphic.h"
#include "../headers/keyboard.h"

extern xpm_map_t letters[];
extern uint8_t scancode;

void race_init(char *text, size_t len) {
  uint8_t bytes[2];
  Char * text_int = malloc(len*sizeof(Char));
  
  int16_t x = 0;
  int16_t y = 0;  
  for (size_t i = 0; i < len; i++) {
    text_int[i].index = get_char_xpm(text[i]);
    text_int[i].posx = 20 + x*(10+2);
    text_int[i].posy = 20 + y*(14+3);
    x++;
    if (20+x>90 && text_int[i].index == 62) {// 62 => ' '
        y++; 
        x = 0;
    }
  }

  // Draw text
  for (size_t n = 0; n < len; n++) {
    graphic_xpm(letters[text_int[n].index], text_int[n].posx, text_int[n].posy, false, 0);
  }

  // Prepare array to write
  Char * digited_text = malloc((len+10)*sizeof(Char));
  size_t n_keys = 0;
  size_t correct_keys = 0;
  uint8_t aux_key;

  // Prepare keyboard interruptions
  uint8_t kbd_bit_no = 1;
  uint32_t kbd_irq_set = BIT(kbd_bit_no);
  kbc_subscribe_int(&kbd_bit_no);
    
  int ipc_status;
  message msg;
  int r = 0;
  //sair através da ESC key
  while(scancode != ESC_KEY && correct_keys != len) 
  { 
    /* Get a request message. */
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
      printf("driver_receive failed with: %d", r);
      continue;
    }
    if (is_ipc_notify(ipc_status)) {   /* received notification */
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: /* hardware interrupt notification */				
          if (msg.m_notify.interrupts & kbd_irq_set) { 
            /* subscribed interrupt */
            kbc_ih();
            assemble_scancode(bytes);
            
            if (!(bytes[0] == 0xE0 && bytes[1] == 0x00)) {
              aux_key = get_scancode_char(bytes);
              if (aux_key == 100) break;
              update_digited_text(aux_key, digited_text, &n_keys);
              update_correct_keys(digited_text, &n_keys, text_int, &correct_keys);
            }
            
          }
          break;
        default:
          printf("Receive no interrupt\n");
          break; /* no other notifications expected: do nothing */	
      }
    } 
    tickdelay(micros_to_ticks(DELAY_US));
  }
  free(text_int);
  free(digited_text);
  //unsubscribe KBC interrupts
  kbc_unsubscribe_int();
}

void update_digited_text(uint8_t aux_key, Char * digited_text, size_t *n_keys) {
  Char key;
  Char previous_key;

  if (*n_keys != 0)
    previous_key = digited_text[*n_keys-1];
  key.index = aux_key;
  if (key.index != 100 && key.index != 200) {
    if (*n_keys == 0) {
      key.posx = 20;
      key.posy = 400;
    }
    else {
      if (previous_key.posx > 900 && previous_key.index == 62) {
        key.posx = 20;
        key.posy = previous_key.posy + (14+3);
      }
      else {
        key.posx = previous_key.posx + (10+2);
        key.posy = previous_key.posy;
      }
    }
      
    graphic_xpm(letters[key.index], key.posx, key.posy, false, 0);
    digited_text[*n_keys] = key;
    (*n_keys)++;
  }
  else if(key.index == 200) {
    if (*n_keys != 0) {
      (*n_keys)--;
      graphic_xpm(letters[digited_text[*n_keys].index], digited_text[*n_keys].posx, digited_text[*n_keys].posy, true, 63);
    }
  }
}

void update_correct_keys(Char* digited_text, size_t *n_keys, Char* text_int, size_t *correct_keys) {
  if (*correct_keys == *n_keys-1){
    if (text_int[*correct_keys].index == digited_text[*n_keys-1].index)
      (*correct_keys)++;
  }
  else if (*correct_keys > *n_keys) {
    *correct_keys = *n_keys;
  }
}

