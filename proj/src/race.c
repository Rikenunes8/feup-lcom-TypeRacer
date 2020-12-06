#include "../headers/race.h"
#include "../headers/xpixmap.h"


extern xpm_map_t letters[];
extern uint8_t scancode;
extern int timer_counter;
int no_seconds = 0;

static size_t MAX_LEN;

void race_init(const char *text, size_t len) {
  uint8_t scancode_bytes[2];
  // Prepare space to allocate text
  Char * text_Char = malloc(len*sizeof(Char));

  display_time(no_seconds, 20, 20);
  display_text(text, text_Char, len, X_STRT_TEXT, Y_STRT_TEXT);

  // Prepare array to write
  MAX_LEN = len+10; // Margin of 10 more Chars to write in typed_text
  Char * typed_text = malloc((MAX_LEN)*sizeof(Char));
  size_t n_keys = 0; // Number of elements in typed_text
  size_t current_key = 0; // Index of the element corresponding to where the cursor is
  size_t correct_keys = 0; // Matched elements between typed_text and text_Char
  uint8_t aux_key;

  // Prepare keyboard interruptions
  uint8_t kbd_bit_no = 1;
  uint32_t kbd_irq_set = BIT(kbd_bit_no);
  kbc_subscribe_int(&kbd_bit_no);

  // Prepare timer interruptions
  uint8_t timer_bit_no = 0;
  uint32_t timer_irq_set = BIT(timer_bit_no);
  timer_subscribe_int(&timer_bit_no);
    
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
              if (aux_key == 100) break; // If not a char to draw, break

              update_typed_text(aux_key, typed_text, &n_keys, &current_key);
              // Count matched keys (correct_keys) and paint text_Char depending on wheter the Chars match or not
              update_correct_keys(typed_text, &n_keys, text_Char, &correct_keys, &len);
              
              //display_time(correct_keys, 700, 20);
              //display_time(n_keys, 500, 20);
              //display_time(current_key, 300, 20);
            }

            
          }
          if (msg.m_notify.interrupts & timer_irq_set) 
          { 
            /* subscribed interrupt */
            timer_int_handler();
            if(timer_counter % 60 == 0)
            {
              no_seconds = no_seconds + 1;
              display_time(no_seconds, 20, 20);              
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

  //displays total number of seconds (incomplete)
  display_time(no_seconds, 20, 20);
  //printf("\nstr: %s", str);
  sleep(2);


  free(text_Char);
  free(typed_text);
  //unsubscribe interrupts
  kbc_unsubscribe_int();
  timer_unsubscribe_int();
}

void update_typed_text(uint8_t aux_key, Char * typed_text, size_t *n_keys, size_t *current_key) {
  Char key;
  Char previous_key;
  
  key.index = aux_key;

  if(key.index == BACKSPACE) { // If key typed is backspace
    if (*current_key == 0) return;

    // Erase previous key bar
    graphic_draw_rectangle(typed_text[*current_key-1].posx+CHAR_W, typed_text[*current_key-1].posy, 1, CHAR_H, WHITE);
    // Draw white rectangle in last element position since removing a letter decreases the length
    graphic_draw_rectangle(typed_text[*n_keys-1].posx, typed_text[*n_keys-1].posy, CHAR_W, CHAR_H, WHITE);
    
    // Eliminate Char of typed_text
    for (uint16_t i = *current_key-1; i < *n_keys-1; i++) {
      typed_text[i] = typed_text[i+1];
    }
    (*current_key)--; // Decrement index of the cursor in typed_text array
    (*n_keys)--; // Decrement number of elements in typed_text array

    // New coordenates to Chars after changing
    rearrange_coors_text(typed_text, *current_key, *n_keys); 
    
    // Display Chars after change until the end
    for (size_t n = *current_key; n < *n_keys; n++)
      display_Char(&typed_text[n]);
    
    // Draw current key bar
    if (*current_key == 0) graphic_draw_rectangle(X_STRT_TYPE-1, Y_STRT_TYPE, 1, CHAR_H, BLACK);
    else graphic_draw_rectangle(typed_text[*current_key-1].posx+CHAR_W, typed_text[*current_key-1].posy, 1, CHAR_H, BLACK);
  }
  else if (key.index == L_ARROW) {
    if (*current_key == 0) return; // Don't allow cursor to go back when it's at the begin

    // Erase previous key bar
    graphic_draw_rectangle(typed_text[*current_key-1].posx+CHAR_W, typed_text[*current_key-1].posy, 1, CHAR_H, WHITE);
    
    (*current_key)--; // Decrement index of the cursor in typed_text array
    
    // Draw current key bar
    if (*current_key == 0) graphic_draw_rectangle(X_STRT_TYPE-1, Y_STRT_TYPE, 1, CHAR_H, BLACK);
    else graphic_draw_rectangle(typed_text[*current_key-1].posx+CHAR_W, typed_text[*current_key-1].posy, 1, CHAR_H, BLACK);
  }
  else if (key.index == R_ARROW) {
    if (*current_key == *n_keys) return; // Don't allow cursor to advance when it's at the end

    // Erase previous key bar
    if (*current_key == 0) graphic_draw_rectangle(X_STRT_TYPE-1, Y_STRT_TYPE, 1, CHAR_H, WHITE);
    else graphic_draw_rectangle(typed_text[*current_key-1].posx+CHAR_W, typed_text[*current_key-1].posy, 1, CHAR_H, WHITE);
    
    (*current_key)++; // Increment index of the cursor in typed_text array
    
    // Draw current key bar
    graphic_draw_rectangle(typed_text[*current_key-1].posx+CHAR_W, typed_text[*current_key-1].posy, 1, CHAR_H, BLACK);
  }
  // If typed key is able to be drawn and does not exceed maximum length allocated
  else if (key.index != NOTHING && *n_keys+1<MAX_LEN) {
    key.state = NORMAL;

    // Set position to the first typed key
    if (*current_key == 0) {
      key.posx = X_STRT_TYPE;
      key.posy = Y_STRT_TYPE;
    }
    else { // Set position to non first typed keys
      // Find the previous key
      previous_key = typed_text[*current_key-1];

      // If previous key is a space and and is after a limit set position to next vertical pos
      if (previous_key.posx > 900 && previous_key.index == SPACE) {
        key.posx = X_STRT_TYPE;
        key.posy = previous_key.posy + (CHAR_H+3);
      }
      else { // Set next horizontal position and keep previous vertical position
        key.posx = previous_key.posx + (CHAR_W+2);
        key.posy = previous_key.posy;
      }
    }

    // Erase current key bar
    if (*current_key == 0) graphic_draw_rectangle(X_STRT_TYPE-1, Y_STRT_TYPE, 1, CHAR_H, WHITE);
    else graphic_draw_rectangle(typed_text[*current_key-1].posx+CHAR_W, typed_text[*current_key-1].posy, 1, CHAR_H, WHITE);
    
    // Add Char to typed_text
    for (size_t i = *current_key; i < *n_keys+1; i++) {
      Char temp = typed_text[i];
      typed_text[i] = key;
      key = temp;
    }

    (*current_key)++; // Increment index of the cursor in typed_text array
    (*n_keys)++; // Increment number of elements in array
    
    // New coordenates to Chars after changing
    rearrange_coors_text(typed_text, *current_key, *n_keys); 

    // Draw new typed key
    for (size_t n = *current_key-1; n < *n_keys; n++)
      display_Char(&typed_text[n]);

    // Draw current key bar
    graphic_draw_rectangle(typed_text[*current_key-1].posx+CHAR_W, typed_text[*current_key-1].posy, 1, CHAR_H, BLACK);    
  }
}

void update_correct_keys(Char* typed_text, size_t *n_keys, Char* text_Char, size_t *correct_keys, size_t *len) {
  *correct_keys = 0; // Reset counting of matched keys
  size_t n = 0;
  // while keys match paint text_Char's Chars green
  while (typed_text[n].index == text_Char[n].index && n<*n_keys && *n_keys<*len+1) {
    (*correct_keys)++;
    text_Char[n].state = RIGHT;
    display_Char(&text_Char[n]);
    n++;
  }
  // then paint other keys util the same length of typed_text red
  while (n<*n_keys && *n_keys<*len+1) {
    text_Char[n].state = WRONG;
    display_Char(&text_Char[n]);
    n++;
  }
  if (*n_keys < *len || *n_keys == 0) {
    text_Char[n].state = NORMAL;
    display_Char(&text_Char[n]);
  }
    
  return;
  
  /*
  // If comparing key typed with corresponding char in the same position of text
  if (*correct_keys == *n_keys-1){
    // If they are same chars increment number of correct keys, otherwise ignore
    if (text_Char[*correct_keys].index == typed_text[*n_keys-1].index) {
      (*correct_keys)++;
      //text_Char[*n_keys-1].state = RIGHT;
      //display_Char(&typed_text[*n_keys-1]);
    }
      
  }
  else if (*correct_keys > *n_keys) {
    *correct_keys = *n_keys; // No more correct keys than typed
  }*/


}

int display_text(const char* text, Char* text_Char, size_t len, uint16_t x_position, uint16_t y_position) {
  // Set chars to be drawn
  uint16_t x = 0;
  uint16_t y = 0;  
  for (size_t i = 0; i < len; i++) {
    // Set index of char draw in letters
    text_Char[i].index = get_char_xpm(text[i]);
    text_Char[i].state = NORMAL;
    
    // Set position where to be drawn
    text_Char[i].posx = x_position + x*(CHAR_W+2);
    text_Char[i].posy = y_position + y*(CHAR_H+3);
    x++; // Next horizontal position
    // If the char is ' ' and x passed the limit set next vertical position
    if (x_position+x*(CHAR_W+2)>get_h_res()-100 && text_Char[i].index == SPACE) {
        y++; 
        x = 0;
    }
  }
  // Draw text
  for (size_t n = 0; n < len; n++) {
    display_Char(&text_Char[n]);
  }
  return 0;
}

void display_time(int time, uint16_t x, uint16_t y) {
  char str[20];    //empty string to store no_seconds
  sprintf(str, "%d", time); //casts the number no_seconds to the string str
  
  Char* str_Char = malloc(strlen(str)*sizeof(Char)); // Convert string of chars to string of Chars

  display_text(str, str_Char, strlen(str), x, y);

  free(str_Char);
}

void display_Char(Char *c) {
  uint8_t * map;
  xpm_image_t img;
  graphic_xpm_load(&map, &img, XPM_8_8_8, letters[c->index]);
  graphic_Char_xpm(map, &img, c->posx, c->posy, c->state);
}

void rearrange_coors_text(Char* typed_text, size_t begin, size_t end) {
  uint16_t x;
  uint16_t y;
  // Recognize coors of the begin
  if (begin == 0) {
     x = X_STRT_TYPE;
     y = Y_STRT_TYPE;
  }
  // Recognize coors of the key indexed before begin
  else {
    begin--; 
    x = typed_text[begin].posx;
    y = typed_text[begin].posy;
  }

  for (size_t i = begin; i < end; i++) {    
    // Set position where to be drawn
    typed_text[i].posx = x;
    typed_text[i].posy = y;

    x += CHAR_W+2; // Next horizontal position
    // If the char is ' ' and x passed the limit set next vertical position
    if (x>get_h_res()-100 && typed_text[i].index == SPACE) {
      y += CHAR_H+3; 
      x = X_STRT_TYPE;
    }
  }
}
