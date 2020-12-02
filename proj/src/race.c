#include "../headers/race.h"

extern xpm_map_t letters[];
extern uint8_t scancode;

void race_init(char *text, size_t len) {
  uint8_t scancode_bytes[2];
  // Prepare space to allocate text
  Char * text_Char = malloc(len*sizeof(Char));
  
  int16_t x = 0;
  int16_t y = 0;  
  for (size_t i = 0; i < len; i++) {
    // Set index of char draw in letters
    text_Char[i].index = get_char_xpm(text[i]);
    // Set position where to be drawn
    text_Char[i].posx = 20 + x*(10+2);
    text_Char[i].posy = 20 + y*(14+3);
    x++; // Next horizontal position
    // If the char is ' ' and x passed the limit set next vertical position
    if (20+x>90 && text_Char[i].index == 62) {
        y++; 
        x = 0;
    }
  }

  // Draw text
  for (size_t n = 0; n < len; n++) {
    graphic_xpm(letters[text_Char[n].index], text_Char[n].posx, text_Char[n].posy, false, 0);
  }

  // Prepare array to write
  Char * typed_text = malloc((len+10)*sizeof(Char));
  size_t n_keys = 0; // Number of elements in typed_text
  size_t correct_keys = 0; // Matched elements
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
            assemble_scancode(scancode_bytes);
            // If scancode is all set
            if (!(scancode_bytes[0] == 0xE0 && scancode_bytes[1] == 0x00)) 
            {
              // Get typed key
              aux_key = get_scancode_char(scancode_bytes);
              if (aux_key == 100) break; // If not a char to draw
              update_typed_text(aux_key, typed_text, &n_keys);
              update_correct_keys(typed_text, &n_keys, text_Char, &correct_keys);
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
  free(text_Char);
  free(typed_text);
  //unsubscribe KBC interrupts
  kbc_unsubscribe_int();
}

void update_typed_text(uint8_t aux_key, Char * typed_text, size_t *n_keys) {
  Char key;
  Char previous_key;
  // If not the first key typed, find the previous one
  if (*n_keys != 0)
    previous_key = typed_text[*n_keys-1];
  
  key.index = aux_key;
  // If typed key is able to be drawn
  if (key.index != 100 && key.index != 200) {
    // Set position to the first typed key
    if (*n_keys == 0) {
      key.posx = 20;
      key.posy = 400;
    }
    else { // Set position to non first typed keys
      // If previous key is a space and and is after a limit set position to next vertical pos
      if (previous_key.posx > 900 && previous_key.index == 62) {
        key.posx = 20;
        key.posy = previous_key.posy + (14+3);
      }
      else { // Set next horizontal position and keep previous vertical position
        key.posx = previous_key.posx + (10+2);
        key.posy = previous_key.posy;
      }
    }
    // Draw new typed key
    graphic_xpm(letters[key.index], key.posx, key.posy, false, 0);
    typed_text[*n_keys] = key; // Add new typed key to array typed text
    (*n_keys)++; // Increment number of elements in array
  }
  else if(key.index == 200) { // If key typed is backspace
    if (*n_keys != 0) { // If there are keys in typed_text to be erased
      (*n_keys)--; // Decrement number of elements in typed_text array
      // Draw white rectangle in last element position
      graphic_xpm(letters[typed_text[*n_keys].index], typed_text[*n_keys].posx, typed_text[*n_keys].posy, true, 63);
    }
  }
}

void update_correct_keys(Char* typed_text, size_t *n_keys, Char* text_Char, size_t *correct_keys) {
  // If comparing key typed with corresponding char in the same position of text
  if (*correct_keys == *n_keys-1){
    // If they are same chars increment number of correct keys, otherwise ignore
    if (text_Char[*correct_keys].index == typed_text[*n_keys-1].index)
      (*correct_keys)++;
  }
  else if (*correct_keys > *n_keys) {
    *correct_keys = *n_keys; // No more correct keys than typed
  }
}

