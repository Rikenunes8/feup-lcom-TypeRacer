#include "../headers/race.h"



extern uint32_t timer_counter;
static uint8_t **letters_maps;

static size_t MAX_LEN;
static size_t len; // text lenght
static size_t no_lines;


static uint16_t no_seconds; // counts the number of seconds
static size_t n_keys; // Number of elements in typed_text
static size_t current_key; // Index of the element corresponding to where the cursor is
static size_t correct_keys; // Matched elements between typed_text and text_Char
static uint16_t count_backspaces; //counts the number of backspaces

static size_t CPM;
static float accuracy;

static Char * text_Char;
static Char * typed_text;

static Sprite* back;
static Sprite* car;
static Sprite* results_menu;
static Sprite* key_bar;

static size_t no_bubbles;
static AnimSprite** bubbles;


// Race page
void race_init(const char *text, size_t l, uint8_t ** maps)
{
  letters_maps = maps;
  len = l;
  car = create_sprite(yellow_car_xpm, 50, 80, 0, 0);
  back = create_sprite(background, 0, 0, 0, 0);
  key_bar = create_sprite(key_bar_xpm, X_TYPE-1, y_pos_typed, 0, 0);
  text_Char = malloc(len*sizeof(Char));
  MAX_LEN = len+5; // Margin of 10 more Chars to write in typed_text
  typed_text = malloc((MAX_LEN)*sizeof(Char));

  timer_counter = 0;
  no_seconds = 0;
  n_keys = 0;
  current_key = 0;
  correct_keys = 0;
  count_backspaces = 0;

  no_lines = display_text(text, text_Char, len, X_TEXT, Y_TEXT);
  display_race_background(no_lines);
  y_pos_typed = Y_BOX + (CHAR_H+3)*no_lines+3*Y_BOX_MARGIN+Y_BTW_BOXES;


}

void race_end() 
{
  destroy_sprite(car);
  destroy_sprite(back);
  destroy_sprite(key_bar);
  free(text_Char);
  free(typed_text);
  return;
}

void race_process_timer_int(uint32_t counter) {
  if(counter % 60 == 0) {
    no_seconds++;
    set_results();
  }
  if (counter%6 == 0) {
    display_race_background();
    display_results(true);
    for (size_t i = 0; i < len; i++) {
      display_Char(&text_Char[i]);
    }
    for (size_t i = 0; i < n_keys; i++) {
      display_Char(&typed_text[i]);
    }
    draw_sprite(key_bar, key_bar->x, key_bar->y);
    set_sprite(car, 50+correct_keys*520/len, car->y, car->xspeed, car->yspeed);    
    draw_sprite(car, car->x, car->y);
  }
}

void race_process_kbd_int(Menu_state *state, uint8_t aux_key) {
  if (aux_key == NOTHING || aux_key == D_ARROW || aux_key == T_ARROW) return; // If not a char to draw, break
  if (aux_key == ESC) {
    *state = MENU;
    return;
  }
  if (aux_key == BACKSPACE) count_backspaces++; //counts number of backspaces typed

  update_typed_text(aux_key);
  // Count matched keys (correct_keys) and paint text_Char depending on wheter the Chars match or not
  update_correct_keys();
  if (correct_keys == len) {
    *state = RESULTS;
  }
  return;
}

void race_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse)
{
  Menu_event event;
  event = read_mouse_event(&mouse_event, &mouse->x, &mouse->y);
  switch (event) {
    case click_on_race:  
      *state = RACE;
      break;
    case click_on_exit:
      *state = EXIT;
      break;
    case click_on_best_results:
      *state = BEST_RESULTS;
      break;
    case click_on_race_with_friend:
      *state = RACE_WITH_FRIEND;
      break;
    default:
      break;
  }

}


void update_typed_text(uint8_t aux_key) 
{
  Char key;
  Char previous_key;
  
  key.index = aux_key;

  if(key.index == BACKSPACE) { // If key typed is backspace
    if (current_key == 0) return;
    
    // Eliminate Char of typed_text
    for (uint16_t i = current_key-1; i < n_keys-1; i++) {
      typed_text[i] = typed_text[i+1];
    }
    current_key--; // Decrement index of the cursor in typed_text array
    n_keys--; // Decrement number of elements in typed_text array

    // New coordenates to Chars after changing
    rearrange_coors_text(typed_text, current_key, n_keys); 
        
    if (current_key == 0) set_sprite(key_bar, X_TYPE-1, y_pos_typed, 0, 0);
    else set_sprite(key_bar, typed_text[current_key-1].posx+CHAR_W, typed_text[current_key-1].posy, 0, 0);
  }
  else if (key.index == L_ARROW) {
    if (current_key == 0) return; // Don't allow cursor to go back when it's at the begin
    
    current_key--; // Decrement index of the cursor in typed_text array
    // Set key bar position
    if (current_key == 0) set_sprite(key_bar, X_TYPE-1, y_pos_typed, 0, 0);
    else set_sprite(key_bar, typed_text[current_key-1].posx+CHAR_W, typed_text[current_key-1].posy, 0, 0);
  }
  else if (key.index == R_ARROW) {
    if (current_key == n_keys) return; // Don't allow cursor to advance when it's at the end
    
    current_key++; // Increment index of the cursor in typed_text array
    // Set key bar position
    set_sprite(key_bar, typed_text[current_key-1].posx+CHAR_W, typed_text[current_key-1].posy, 0, 0);
  }
  // If typed key is able to be drawn and does not exceed maximum length allocated
  else if (key.index != NOTHING && n_keys+1<MAX_LEN) {
    key.state = NORMAL;

    // Set position to the first typed key
    if (current_key == 0) {
      key.posx = X_TYPE;
      key.posy = y_pos_typed;
    }
    else { // Set position to non first typed keys
      // Find the previous key
      previous_key = typed_text[current_key-1];

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
    
    // Add Char to typed_text
    for (size_t i = current_key; i < n_keys+1; i++) {
      Char temp = typed_text[i];
      typed_text[i] = key;
      key = temp;
    }

    current_key++; // Increment index of the cursor in typed_text array
    n_keys++; // Increment number of elements in array
    
    // New coordenates to Chars after changing
    rearrange_coors_text(typed_text, current_key, n_keys); 

    // Set key bar position
    set_sprite(key_bar, typed_text[current_key-1].posx+CHAR_W, typed_text[current_key-1].posy, 0, 0);

  }
}

void update_correct_keys() {
  size_t n;
  // If no need to review all letters
  if (correct_keys == n_keys-1) {
    n = correct_keys;
  }
  else {
    correct_keys = 0; // Reset counting of matched keys
    n = 0;
  }
  // while keys match paint text_Char's Chars green
  while (typed_text[n].index == text_Char[n].index && n<n_keys && n_keys<len+1) {
    (correct_keys)++;
    text_Char[n].state = RIGHT;
    display_Char(&text_Char[n]);
    n++;
  }
  // then paint other keys util the same length of typed_text red
  while (n<n_keys && n_keys<len+1) {
    text_Char[n].state = WRONG;
    display_Char(&text_Char[n]);
    n++;
  }
  if (n_keys < len || n_keys == 0) {
    text_Char[n].state = NORMAL;
    display_Char(&text_Char[n]);
  }
    
  return;
}

void rearrange_coors_text(Char* typed_text, size_t begin, size_t end) {

  uint16_t x;
  uint16_t y;
  // Recognize coors of the begin
  if (begin == 0) {
     x = X_TYPE;
     y = y_pos_typed;
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


void graphic_draw_bordered_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height) 
{
  graphic_draw_rectangle(x, y, width, height, BLACK); 
  graphic_draw_rectangle(x+2, y+2, width-4, height-4, WHITE); 
}

void draw_text_box(uint16_t x, uint16_t y, uint16_t width)
{
  graphic_draw_bordered_rectangle(x, y, width, (CHAR_H+3)*no_lines+2*Y_BOX_MARGIN);

}

void display_race_background()
{
  draw_sprite(back, 0, 0);
  graphic_draw_bordered_rectangle(X_BOX,16,BOX_WIDTH,120);

  //draws the text box with variable dimensions (incomplete)
  //passar numero de linhas como argumento, definir heigth consoante esse numero e não exceder max_height
  //graphic_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
  draw_text_box(X_BOX, Y_BOX, BOX_WIDTH);
  draw_text_box(X_BOX, Y_BOX + (CHAR_H+3)*no_lines+2*Y_BOX_MARGIN+Y_BTW_BOXES, BOX_WIDTH);

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
  return y+1;
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
  sprintf(time, "%d : %d ", seconds/60, seconds%60); 
  
  Char* time_Char = malloc(strlen(time)*sizeof(Char)); // Convert string of chars to string of Chars

  display_text(time, time_Char, strlen(time), x, y);

  free(time_Char);
}

void display_Char(Char *c) {
  /*uint8_t * map;
  xpm_image_t img;
  graphic_xpm_load(&map, &img, XPM_8_8_8, letters[c->index]);*/
  graphic_Char_xpm(letters_maps[c->index], c->posx, c->posy, c->state);
}

void display_results(bool real_time)
{

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
    free(text_Char);


    //displays accuracy
    sprintf(text, "%.1f %%  ", accuracy);
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 620, 32);
    free(text_Char);
  }
  else //results page
  {
    draw_sprite(results_menu, 0, 0);

    //Speed text
    sprintf(text, "Your speed: %d cpm   ", CPM);
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 250, 210);
    free(text_Char);

    //Time text
    sprintf(text, "Time: %d : %d", no_seconds/60, no_seconds%60);
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 250, 290);
    free(text_Char);

    //Accuracy text
    sprintf(text, "Accuracy: %.1f %%  ", accuracy);
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), 250, 370);
    free(text_Char);

    //Try again text
    sprintf(text, "TRY AGAIN");
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), try_again_x_left + 20, try_again_y_top + 20);
    free(text_Char);

    //Exit text
    sprintf(text, "EXIT");
    text_Char = malloc(strlen(text)*sizeof(Char));
    display_text(text, text_Char, strlen(text), results_exit_x_left + 25, results_exit_y_top + 20);
    free(text_Char);
  }  
}

void set_results() {
  CPM = (correct_keys * 60) / (float)(no_seconds);
  accuracy = (((float)correct_keys-(float)count_backspaces)/(float)n_keys)*100;
  if (accuracy < 0 || n_keys == 0) accuracy = 0;
}


// Results page
void results_init() {
  int32_t pos[] = {400,300, 100,100, 600,50, 100,450, 300,0, 500,300, 450,100};
  int8_t speeds[] = {1,-2, 2,1, 2,3, 1,-5, -4,2, 5,5, -2,9};

  no_bubbles = 7;
  bubbles = (AnimSprite**)malloc(no_bubbles*sizeof(AnimSprite*));
  for (size_t i = 0; i < no_bubbles; i++) {
    bubbles[i] = create_asprite(pos[2*i], pos[2*i+1], speeds[2*i], speeds[2*i+1], 2, 5, bubble_1, bubble_2, bubble_3, bubble_4, bubble_5);
    set_asprite(bubbles[i], bubbles[i]->aspeed, bubbles[i]->cur_aspeed, 1);
  }
  results_menu = create_sprite(results_page, 0, 0, 0, 0);
}

void results_end() {
  for (size_t i = 0; i < no_bubbles; i++)
    destroy_asprite(bubbles[i]);
  destroy_sprite(results_menu);
}

void results_proccess_timer_int(uint32_t counter, Sprite* mouse) 
{
  if (counter%2 == 0) 
  {
    display_results(false);
    for (size_t i = 0; i < no_bubbles; i++)
      move_bubbles(i);   
    draw_sprite(mouse, mouse->x, -mouse->y);
  }
}

void results_proccess_kbd_int(Menu_state *state, uint8_t aux_key) {
  Menu_event event;
  event = read_kbd_event(aux_key);
  switch (event) {
    case type_left_arrow:  
      *state = RACE;
      break;
    case type_right_arrow:
      *state = MENU;
      break;
    default:
      break;
  }
}

void results_proccess_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse) {
  Menu_event event;
  if (mouse_event.ev == LB_DOWN)
    collison_mouse(mouse);
  event = read_mouse_event(&mouse_event, &mouse->x, &mouse->y);
  switch (event) 
  {
    case click_on_results_exit:  
      *state = MENU;
      break;
    case click_on_try_again_race:
      *state = RACE;
      break;
    default:
      break;
  }
}

/*void collisons() {
  for (int i = 0; i < bubbles.size(); i++) {
    
  }
}*/

void collison_mouse(Sprite* mouse) {
  for (size_t i = 0; i < no_bubbles; i++) {
    if (check_asp_collison(bubbles[i], mouse->x, -mouse->y)) {
      set_asprite(bubbles[i], bubbles[i]->aspeed, bubbles[i]->cur_aspeed, 5);
      break;
    }
  }
}

void bubbles_erase(size_t n) {
  AnimSprite* tmp = bubbles[n];
  for (size_t i = n; i+1 < no_bubbles; i++) {
    bubbles[i] = bubbles[i+1];
  }
  no_bubbles--;
  destroy_asprite(tmp);
}

void move_bubbles(size_t n) {
  if (bubbles[n]->sp->x < 0)
    set_asprite_sprite(bubbles[n], 0, bubbles[n]->sp->y, -bubbles[n]->sp->xspeed, bubbles[n]->sp->yspeed);
  if (bubbles[n]->sp->x+bubbles[n]->sp->width > (int32_t)get_h_res())
    set_asprite_sprite(bubbles[n], get_h_res()-bubbles[n]->sp->width, bubbles[n]->sp->y, -bubbles[n]->sp->xspeed, bubbles[n]->sp->yspeed);
  if (bubbles[n]->sp->y < 0)
    set_asprite_sprite(bubbles[n], bubbles[n]->sp->x, 0, bubbles[n]->sp->xspeed, -bubbles[n]->sp->yspeed);
  if (bubbles[n]->sp->y+bubbles[n]->sp->height > (int32_t)get_v_res())
    set_asprite_sprite(bubbles[n], bubbles[n]->sp->x, get_v_res()-bubbles[n]->sp->height, bubbles[n]->sp->xspeed, -bubbles[n]->sp->yspeed);
    
  draw_asprite(bubbles[n]);

  if (bubbles[n]->cur_fig == 4)
    bubbles_erase(n);
  else
    animate_asprite(bubbles[n]);
}
