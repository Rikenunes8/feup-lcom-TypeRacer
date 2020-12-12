#include "../headers/race.h"
#include "../xpm/letters.h"
#include "../xpm/others.h"
#include "../headers/menus.h"


extern xpm_map_t letters[];
extern uint8_t scancode;
extern int timer_counter;

static size_t MAX_LEN;

void race_init(const char *text, size_t len) 
{
  display_race_background();

  uint8_t scancode_bytes[2];
  uint16_t no_seconds = 0; // counts the number of seconds
  uint16_t count_backspaces = 0; //counts the number of backspaces

  // Prepare space to allocate text
  Char * text_Char = malloc(len*sizeof(Char));

  //display_time(no_seconds, 32, 32);
  display_text(text, text_Char, len, X_TEXT, Y_TEXT);

  // Prepare array to write
  MAX_LEN = len+10; // Margin of 10 more Chars to write in typed_text
  Char * typed_text = malloc((MAX_LEN)*sizeof(Char));
  size_t n_keys = 0; // Number of elements in typed_text
  size_t current_key = 0; // Index of the element corresponding to where the cursor is
  size_t correct_keys = 0; // Matched elements between typed_text and text_Char
  uint8_t aux_key = NOTHING;
  timer_counter = 0;


  // Prepare keyboard interruptions
  uint8_t kbd_bit_no = 1;
  uint32_t kbd_irq_set = BIT(kbd_bit_no);
  kbd_subscribe_int(&kbd_bit_no);

  // Prepare timer interruptions
  uint8_t timer_bit_no = 0;
  uint32_t timer_irq_set = BIT(timer_bit_no);
  timer_subscribe_int(&timer_bit_no);

  display_results(no_seconds, correct_keys, count_backspaces, n_keys, len, true);



  int ipc_status;
  message msg;
  int r = 0;
  //sair através da ESC key
  while(aux_key != ESC && correct_keys != len) 
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
            /* subscribed KBC interrupt */
            kbc_ih();
            assemble_scancode(scancode_bytes);
            // If scancode is all set
            if (!(scancode_bytes[0] == 0xE0 && scancode_bytes[1] == 0x00)) 
            {
              // Get typed key
              aux_key = get_scancode_char(scancode_bytes);
              if (aux_key == NOTHING) break; // If not a char to draw, break
              if (aux_key == ESC) break;
              if (aux_key == BACKSPACE) count_backspaces++; //counts number of backspaces typed


              update_typed_text(aux_key, typed_text, &n_keys, &current_key);
              // Count matched keys (correct_keys) and paint text_Char depending on wheter the Chars match or not
              update_correct_keys(typed_text, &n_keys, text_Char, &correct_keys, &len);
              
              //display_integer(correct_keys, 700, 20);
              //display_integer(n_keys, 500, 20);
              //display_integer(current_key, 300, 20);
            }
          }
          if (msg.m_notify.interrupts & timer_irq_set) 
          { 
            /* subscribed TIMER 0 interrupts */
            timer_int_handler();
            if(timer_counter % 60 == 0) {
              no_seconds++;

              display_results(no_seconds, correct_keys, count_backspaces, n_keys, len, true);
            }

            if (timer_counter%2 == 0)
              fr_buffer_to_video_mem();
          }
          break;
        default:
          printf("Receive no interrupt\n");
          break; /* no other notifications expected: do nothing */	
      }
    } 

    tickdelay(micros_to_ticks(DELAY_US));
  }

  //displays the results
  display_results(no_seconds, correct_keys, count_backspaces, n_keys, len, false);
  fr_buffer_to_video_mem();
  sleep(5);


  free(text_Char);
  free(typed_text);

  //unsubscribe interrupts
  kbd_unsubscribe_int();
  timer_unsubscribe_int();

  return;
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
    if (*current_key == 0) graphic_draw_rectangle(X_TYPE-1, Y_TYPE, 1, CHAR_H, BLACK);
    else graphic_draw_rectangle(typed_text[*current_key-1].posx+CHAR_W, typed_text[*current_key-1].posy, 1, CHAR_H, BLACK);
  }
  else if (key.index == L_ARROW) {
    if (*current_key == 0) return; // Don't allow cursor to go back when it's at the begin

    // Erase previous key bar
    graphic_draw_rectangle(typed_text[*current_key-1].posx+CHAR_W, typed_text[*current_key-1].posy, 1, CHAR_H, WHITE);
    
    (*current_key)--; // Decrement index of the cursor in typed_text array
    
    // Draw current key bar
    if (*current_key == 0) graphic_draw_rectangle(X_TYPE-1, Y_TYPE, 1, CHAR_H, BLACK);
    else graphic_draw_rectangle(typed_text[*current_key-1].posx+CHAR_W, typed_text[*current_key-1].posy, 1, CHAR_H, BLACK);
  }
  else if (key.index == R_ARROW) {
    if (*current_key == *n_keys) return; // Don't allow cursor to advance when it's at the end

    // Erase previous key bar
    if (*current_key == 0) graphic_draw_rectangle(X_TYPE-1, Y_TYPE, 1, CHAR_H, WHITE);
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
      key.posx = X_TYPE;
      key.posy = Y_TYPE;
    }
    else { // Set position to non first typed keys
      // Find the previous key
      previous_key = typed_text[*current_key-1];

      // If previous key is a space and and is after a limit set position to next vertical pos
      if (previous_key.posx > get_h_res()-100 && previous_key.index == SPACE) {
        key.posx = X_TYPE;
        key.posy = previous_key.posy + (CHAR_H+3);
      }
      else { // Set next horizontal position and keep previous vertical position
        key.posx = previous_key.posx + (CHAR_W+2);
        key.posy = previous_key.posy;
      }
    }

    // Erase current key bar
    if (*current_key == 0) graphic_draw_rectangle(X_TYPE-1, Y_TYPE, 1, CHAR_H, WHITE);
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
  size_t n;
  // If no need to review all letters
  if (*correct_keys == *n_keys-1) {
    n = *correct_keys;
  }
  else {
    *correct_keys = 0; // Reset counting of matched keys
    n = 0;
  }
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
}

int display_text(const char* text, Char* text_Char, size_t len, uint16_t x_position, uint16_t y_position) 
{
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

void display_integer(int integer, uint16_t x, uint16_t y) {
  char str[20];    //empty string to store no_seconds

  sprintf(str, "%d", integer); //casts the number no_seconds to the string str
  
  Char* str_Char = malloc(strlen(str)*sizeof(Char)); // Convert string of chars to string of Chars

  display_text(str, str_Char, strlen(str), x, y);

  free(str_Char);
}

void display_float(float decimal, uint16_t x, uint16_t y) 
{
  char str[20];    //empty string to store no_seconds

  sprintf(str, "%.2f", decimal); //casts the number no_seconds to the string str
  
  Char* str_Char = malloc(strlen(str)*sizeof(Char)); // Convert string of chars to string of Chars

  display_text(str, str_Char, strlen(str), x, y);

  free(str_Char);
}

void display_time(uint16_t seconds, uint16_t x, uint16_t y) {
  char time[20]; 
  sprintf(time, "%d : %d", seconds/60, seconds%60); 
  
  Char* time_Char = malloc(strlen(time)*sizeof(Char)); // Convert string of chars to string of Chars

  display_text(time, time_Char, strlen(time), x, y);

  free(time_Char);
}

void display_Char(Char *c) {
  uint8_t * map;
  xpm_image_t img;
  graphic_xpm_load(&map, &img, XPM_8_8_8, letters[c->index]);
  graphic_Char_xpm(map, &img, c->posx, c->posy, c->state);
}

void display_results(size_t no_seconds, size_t correct_keys, size_t count_backspaces, size_t n_keys, size_t len, bool real_time)
{
  size_t CPM = (correct_keys * 60) / (float)(no_seconds);
  float accuracy = (((float)correct_keys-(float)count_backspaces)/(float)n_keys)*100;
  if (accuracy < 0 || n_keys == 0) accuracy = 0;

  Char * text_Char = NULL;
  char text[20];

  if(real_time == true)
  {
    //displays time in format minutes:seconds
    display_time(no_seconds, 110, 32);

    //displays caracters per minute (CPM)
    sprintf(text, "%d cpm  ", CPM);
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 370, 32);

    //displays accuracy
    sprintf(text, "%.1f %%  ", accuracy);
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 620, 32);

  }
  else
  {
    uint8_t * map;
    xpm_image_t img;

    //background image
    xpm_map_t xpm = background;
    map = xpm_load(xpm, XPM_8_8_8, &img);
    graphic_Char_xpm(map, &img, 0, 0, NORMAL);

    //int graphic_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) 
    //white rectangle between (150, 100) and (650, 500)
    graphic_draw_rectangle(150,100, 500, 400, WHITE); 

    //"RESULTS" word
    xpm_map_t xpm_results = results_word;
    map = xpm_load(xpm_results, XPM_8_8_8, &img);
    graphic_Char_xpm(map, &img, 160, 110, NORMAL); 

    //speed symbol
    xpm_map_t xpm_speed = speed_symbol;
    map = xpm_load(xpm_speed, XPM_8_8_8, &img);
    graphic_Char_xpm(map, &img, 170, 190, NORMAL);
    sprintf(text, "Your speed: %d cpm   ", CPM);
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 250, 210);

    //time symbol
    xpm_map_t xpm_time = time_symbol;
    map = xpm_load(xpm_time, XPM_8_8_8, &img);
    graphic_Char_xpm(map, &img, 170, 270, NORMAL);
    sprintf(text, "Time: %d : %d", no_seconds/60, no_seconds%60);
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 250, 290);

    //accuracy symbol
    xpm_map_t xpm_accuracy = accuracy_symbol;
    map = xpm_load(xpm_accuracy, XPM_8_8_8, &img);
    graphic_Char_xpm(map, &img, 170, 350, NORMAL);
    sprintf(text, "Accuracy: %.1f %%  ", accuracy);
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 250, 370);

    //"Try again?" button
    graphic_draw_bordered_rectangle(180, 420, 150, 50);
    sprintf(text, "Try again?");
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 190, 440);

    //"Save results" button
    graphic_draw_bordered_rectangle(340, 420, 170, 50);
    sprintf(text, "Save results");
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 350, 440);

    //"Exit" button
    graphic_draw_bordered_rectangle(520, 420, 100, 50);
    sprintf(text, "Exit");
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 530, 440);

    
  }

  free(text_Char);
  
}

void rearrange_coors_text(Char* typed_text, size_t begin, size_t end) {
  uint16_t x;
  uint16_t y;
  // Recognize coors of the begin
  if (begin == 0) {
     x = X_TYPE;
     y = Y_TYPE;
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
      x = X_TYPE;
    }
  }
}
