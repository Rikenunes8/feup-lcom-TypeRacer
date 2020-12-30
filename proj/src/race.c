#include "../headers/race.h"
#include "../headers/best_results.h"
#include "../headers/rtc.h"



extern uint32_t timer_counter;

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
static Sprite* key_bar;
static AnimSprite* traffic_light;
static bool begin_race;

static Sprite* results_menu;
static size_t no_bubbles;
static AnimSprite** bubbles;
static char name[20] = "noName";

static char* aux_buffer;


// Race page
void race_init(const char *text, size_t l)
{
  len = l;
  traffic_light = create_asprite(600, 70, 0, 0, 10, 4, traffic_light_red_xpm, traffic_light_red_xpm, traffic_light_yellow_xpm, traffic_light_green_xpm);
  car = create_sprite(yellow_car_xpm, 50, 80, 0, 0);
  back = create_sprite(background, 0, 0, 0, 0);
  key_bar = create_sprite(key_bar_xpm, X_TYPE-1, y_pos_typed, 0, 0);
  text_Char = malloc(len*sizeof(Char));
  MAX_LEN = len+5; // Margin of 10 more Chars to write in typed_text
  typed_text = malloc((MAX_LEN)*sizeof(Char));

  // Set alarm according to text lenght
  rtc_turn_on_alarm();
  uint8_t time[3];
  rtc_read_time(time);
  uint16_t time_to_alarm = len*6/20 + 4;
  uint8_t new_sec = time_to_alarm%60 + time[0];
  uint8_t new_min = time_to_alarm/60 + time[1];
  rtc_set_alarm(binary_to_bcd(new_sec%60), binary_to_bcd((new_sec/60+new_min)%60), DONT_CARE);

  // Reset variables
  timer_counter = 0;
  no_seconds = 0;
  n_keys = 0;
  current_key = 0;
  correct_keys = 0;
  count_backspaces = 0;
  CPM = 0;
  accuracy = 0;
  begin_race = false;
  if (strcmp(name, "Disqualified") == 0) strcpy(name, "noName");

  no_lines = convert_text_to_text_char(text, text_Char, len, X_TEXT, Y_TEXT);
  y_pos_typed = Y_BOX + (CHAR_H+3)*no_lines+3*Y_BOX_MARGIN+Y_BTW_BOXES;
  
  display_race_background(no_lines);
  for (size_t i = 0; i < len; i++) {
    display_Char(&text_Char[i]);
  }
  display_results(true);
  draw_sprite(car);
  draw_asprite(traffic_light);

}

void race_end() 
{
  rtc_turn_off_alarm();
  destroy_sprite(car);
  destroy_sprite(back);
  destroy_sprite(key_bar);
  free(text_Char);
  free(typed_text);
  return;
}

void race_process_timer_int(Menu_state *state, uint32_t counter, Sprite* mouse) {

  if(counter % 60 == 0) {
    no_seconds++;
    if (begin_race) set_results();
  }
 
  if (!begin_race && no_seconds == 3) {
    begin_race = true;
    destroy_asprite(traffic_light);
    no_seconds = 0;
  }
  
  if (counter%6 == 0) {
    display_race_background();
    if (begin_race) display_results(true);
    display_text_Char(text_Char, len);
    display_text_Char(typed_text, n_keys);
    draw_sprite(key_bar);
    set_sprite(car, 50+correct_keys*520/len, car->y, car->xspeed, car->yspeed);    
    draw_sprite(car);
    if (!begin_race) {
      animate_asprite(traffic_light);
      draw_asprite(traffic_light);
    }    
    draw_sprite(mouse);
    
    if (correct_keys == len) {
      *state = RESULTS;
      fr_buffer_to_video_mem();
    }
  }
  if (get_alarm()) {
    use_alarm();
    *state = RESULTS;
    CPM = 0;
    accuracy = 0;
    strcpy(name, "Disqualified");
  }
}

void race_process_kbd_int(Menu_state *state, uint8_t aux_key) {
  if (!begin_race) return;
  if (aux_key == NOTHING || aux_key == D_ARROW || aux_key == T_ARROW) return; // If not a char to draw, break
  if (aux_key == ESC) {
    *state = MENU;
    return;
  }
  if (aux_key == BACKSPACE) count_backspaces++; //counts number of backspaces typed

  update_typed_text(aux_key);
  // Count matched keys (correct_keys) and paint text_Char depending on wheter the Chars match or not
  update_correct_keys();
  return;
}

void race_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse)
{
  read_mouse_event(&mouse_event, &mouse->x, &mouse->y);
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
  draw_sprite(back);
  graphic_draw_bordered_rectangle(X_BOX,16,BOX_WIDTH,120);

  //draws the text box with variable dimensions (incomplete)
  //passar numero de linhas como argumento, definir heigth consoante esse numero e não exceder max_height
  //graphic_draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color)
  draw_text_box(X_BOX, Y_BOX, BOX_WIDTH);
  draw_text_box(X_BOX, Y_BOX + (CHAR_H+3)*no_lines+2*Y_BOX_MARGIN+Y_BTW_BOXES, BOX_WIDTH);

  graphic_draw_rectangle(714, 70, 22, 52, BLACK);
  for (int i = 0; i < 5; i++)
    graphic_draw_rectangle(715+(i%2)*10, 71+i*10, 10, 10, WHITE);

}

void display_results(bool real_time)
{

  Char * txt_Char = NULL;
  char txt[20];

  if(real_time == true)
  {
    //displays time in format minutes:seconds
    display_time(no_seconds, 110, 32);

    //displays caracters per minute (CPM)
    sprintf(txt, "%d cpm  ", CPM);
    txt_Char = malloc(strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), 370, 32);
    free(txt_Char);


    //displays accuracy
    sprintf(txt, "%.1f %%  ", accuracy);
    txt_Char = malloc(strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), 620, 32);
    free(txt_Char);
  }
  else //results page
  {
    draw_sprite(results_menu);

    //Speed text
    sprintf(txt, "Your speed: %d cpm   ", CPM);
    txt_Char = malloc(strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), 250, 210);

    //Time text
    sprintf(txt, "Time: %d : %d", no_seconds/60, no_seconds%60);
    txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), 250, 290);

    //Accuracy text
    sprintf(txt, "Accuracy: %.1f %%  ", accuracy);
    txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), 250, 370);

    //Try again text
    sprintf(txt, "TRY AGAIN");
    txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), try_again_x_left + 20, try_again_y_top + 20);

    //Exit text
    sprintf(txt, "EXIT");
    txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), results_exit_x_left + 25, results_exit_y_top + 20);

    //Name text
    sprintf(txt, "Name:");
    txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), 250, 490);
    free(txt_Char);
  }  
}

void set_results() {
  CPM = (correct_keys * 60) / (float)(no_seconds);
  accuracy = (((float)correct_keys-(float)count_backspaces)/(float)n_keys)*100;
  if (accuracy < 0 || n_keys == 0) accuracy = 0;
}


// Results page
void results_init() {
  typed_text = (Char*)malloc(25*sizeof(Char));
  n_keys = strlen(name);
  convert_text_to_text_char((const char*)name, typed_text, n_keys, 350, 490);

  aux_buffer = (char*)malloc(get_h_res()*get_v_res()*get_BPP());
  int32_t pos[] = {400,300, 100,100, 600,50, 100,450, 300,0, 500,300, 450,100};
  int8_t speeds[] = {1,-2, 2,1, 2,3, 1,-5, -4,2, 5,5, -2,9};

  no_bubbles = 7;
  bubbles = (AnimSprite**)malloc(no_bubbles*sizeof(AnimSprite*));
  for (size_t i = 0; i < no_bubbles; i++) {
    bubbles[i] = create_asprite(pos[2*i], pos[2*i+1], speeds[2*i], speeds[2*i+1], 2, 5, bubble_1, bubble_2, bubble_3, bubble_4, bubble_5);
    set_asprite(bubbles[i], bubbles[i]->aspeed, bubbles[i]->cur_aspeed, 1);
  }
  results_menu = create_sprite(results_page, 0, 0, 0, 0);

  display_results(false);
  fr_buffer_to_aux(aux_buffer);
}

void results_end() {
  if (n_keys != 0)
    convert_text_Char_to_text(name, typed_text, n_keys);
  else
    strcpy(name, "noName");
  add_score(CPM, accuracy, name);
  free(typed_text);
  free(aux_buffer);
  for (size_t i = 0; i < no_bubbles; i++)
    destroy_asprite(bubbles[i]);
  destroy_sprite(results_menu);
}

void results_process_timer_int(uint32_t counter, Sprite* mouse) 
{
  if (counter%2 == 0) {
    aux_to_fr_buffer(aux_buffer);
    display_text_Char(typed_text, n_keys);
    for (size_t i = 0; i < no_bubbles; i++)
      move_bubbles(i); 
    draw_sprite(mouse);
  }
}

void results_process_kbd_int(Menu_state *state, uint8_t aux_key) {
  if (aux_key == L_ARROW || aux_key == R_ARROW || aux_key == T_ARROW || aux_key == D_ARROW || aux_key == ESC) {
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
  else if (aux_key == BACKSPACE) {
    if (n_keys != 0)
      n_keys--;
  }
  else if (aux_key != NOTHING && aux_key != SPACE && n_keys+1 < 19) {
    Char key;
    key.index = aux_key;
    key.state = NORMAL;
    if (n_keys == 0) {
      key.posx = 350; 
      key.posy = 490;
    }
    else {
      key.posx = typed_text[n_keys-1].posx + (CHAR_W+2);
      key.posy = typed_text[n_keys-1].posy;
    }
    typed_text[n_keys] = key;
    n_keys++;
  }

}

void results_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse) {
  Menu_event event;
  if (mouse_event.ev == LB_DOWN || mouse_event.ev == RB_DOWN)
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

void collison_mouse(Sprite* mouse) {
  for (size_t i = 0; i < no_bubbles; i++) {
    if (check_asp_collison(bubbles[i], mouse->x, mouse->y)) {
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
