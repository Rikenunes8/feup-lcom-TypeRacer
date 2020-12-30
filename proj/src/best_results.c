#include "../headers/best_results.h"
#include "../headers/graphic.h"
#include "../xpm/best_results_page.h"
#include "../headers/rtc.h"

static uint8_t MAX_SCORES = 3;
static Score* best_scores;
static char* aux_buffer;
static Sprite* best_results_page;

int br_read_file() {
  best_scores = (Score*)malloc(MAX_SCORES*sizeof(Score));

  FILE *fp;
  fp = fopen("/home/lcom/labs/proj/best_results.txt", "r");
  for (int i = 0; i < MAX_SCORES; i++) {
    best_scores[i].date = (uint8_t*)malloc(6*sizeof(uint8_t));
    fscanf(fp, "%d %f\n", &best_scores[i].cpm, &best_scores[i].accuracy);
    fscanf(fp, "%hhu %hhu %hhu %hhu %hhu %hhu\n", &best_scores[i].date[0], &best_scores[i].date[1], &best_scores[i].date[2], &best_scores[i].date[3], &best_scores[i].date[4], &best_scores[i].date[5]);
    fscanf(fp, "%s\n", best_scores[i].name);
  }
  fclose(fp);
  return 0;
}

int br_write_file() {
  FILE *fp;
  fp = fopen("/home/lcom/labs/proj/best_results.txt", "w");
  for (int i = 0; i < MAX_SCORES; i++) {
    fprintf(fp, "%d %d\n", best_scores[i].cpm, (int)best_scores[i].accuracy);
    fprintf(fp, "%u %u %u %u %u %u\n", best_scores[i].date[0], best_scores[i].date[1], best_scores[i].date[2], best_scores[i].date[3], best_scores[i].date[4], best_scores[i].date[5]);
    fprintf(fp, "%s\n", best_scores[i].name);
  }  
  fclose(fp);
  free(best_scores);
  return 0;
}

int br_draw_best_results() {
  Char * txt_Char = NULL;
  char txt[20];

  sprintf(txt, "Name");
  txt_Char = malloc(strlen(txt)*sizeof(Char));
  display_text(txt, txt_Char, strlen(txt), 215, 224);
  sprintf(txt, "CPM");
  txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
  display_text(txt, txt_Char, strlen(txt), 355, 224);
  sprintf(txt, "Accuracy");
  txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
  display_text(txt, txt_Char, strlen(txt), 425, 224);
  sprintf(txt, "Date");
  txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
  display_text(txt, txt_Char, strlen(txt), 610, 224);
  sprintf(txt, "Back");
  txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
  display_text(txt, txt_Char, strlen(txt), 712, 536);
  free(txt_Char);

  
  
  for (int i = 0; i < MAX_SCORES; i++) {
    display_integer(i+1, 95, 275+65*i);
    sprintf(txt, "%s", best_scores[i].name);
    txt_Char = malloc(strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), 165, 275+65*i);
    sprintf(txt, "%d", best_scores[i].cpm);
    txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), 355, 275+65*i);
    sprintf(txt, "%.1f", best_scores[i].accuracy);
    txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), 445, 275+65*i);
    sprintf(txt, "%u:%u  %u:%u", best_scores[i].date[2], best_scores[i].date[1], best_scores[i].date[3], best_scores[i].date[4]);
    txt_Char = realloc(txt_Char, strlen(txt)*sizeof(Char));
    display_text(txt, txt_Char, strlen(txt), 570, 275+65*i);
    free(txt_Char);
  }
  return 0;
}


void br_init() {
  aux_buffer = (char*)malloc(get_h_res()*get_v_res()*get_BPP());
  best_results_page = create_sprite((xpm_map_t)best_results_xpm, 0, 0, 0, 0);

  draw_sprite(best_results_page, 0, 0);
  br_draw_best_results();
  fr_buffer_to_aux(aux_buffer);
}

void br_end() {
  free(aux_buffer);
  destroy_sprite(best_results_page);
}

void br_process_timer_int(uint32_t counter, Sprite* mouse) {
  if (counter%2 == 0) {
    aux_to_fr_buffer(aux_buffer);
    draw_sprite(mouse, mouse->x, -mouse->y);
  }
}

void br_process_kbd_int(Menu_state *state, uint8_t aux_key) {
  Menu_event event;
  event = read_kbd_event(aux_key);
  switch (event) {
    case type_ESC:  
      *state = MENU;
      break;
    default:
      break;
  }
}

void br_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse) {
  Menu_event event;
  event = read_mouse_event(&mouse_event, &mouse->x, &mouse->y);
  switch (event) {
    case click_on_best_results_back:  
      *state = MENU;
      break;
    default:
      break;
  }
}

int add_score(size_t CPM, float accuracy, char* name) {
  bool better = false;
  Score new; 
  new.cpm = CPM; 
  new.accuracy = accuracy;
  strncpy(new.name, name, sizeof(new.name)-1); new.name[11] = '\0';
  new.date = (uint8_t*)malloc(6*sizeof(uint8_t));
  rtc_read_time_date(new.date);
   
  for (int i = MAX_SCORES-1; i >= 0; i--) {
    if (new.cpm > best_scores[i].cpm || (new.cpm == best_scores[i].cpm && new.accuracy > best_scores[i].accuracy)) {
      better = true;
      Score tmp = best_scores[i];
      best_scores[i] = new;
      if (i < MAX_SCORES-1)
        best_scores[i+1] = tmp;
      else
        free(tmp.date);
    }
    else {
      break;
    }
  }
  if (!better)
    free(new.date);
  return 0;
}


