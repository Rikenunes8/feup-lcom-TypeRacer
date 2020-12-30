#include "../headers/menus.h"


void menus_process_timer_int(uint32_t counter, Sprite* main_menu, Sprite* mouse) {
  if (counter%2 == 0) 
  {
    draw_sprite(main_menu);
    draw_sprite(mouse);
  }
}

void menus_process_kbd_int(Menu_state *state, uint8_t aux_key) {
  Menu_event event;
  event = read_kbd_event(aux_key);
  switch (event) {
    case type_left_arrow:  
      *state = RACE;
      break;
    case type_right_arrow:
      *state = EXIT;
      break;
    case type_top_arrow:
      *state = BEST_RESULTS;
      break;
    case type_down_arrow:
      *state = RACE_WITH_FRIEND;
      break;
    default:
      break;
  }
}

void menus_process_mouse_int(Menu_state *state, Mouse_event mouse_event, Sprite* mouse) {
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

Menu_event read_kbd_event(uint8_t aux_key)
{
  if(aux_key == L_ARROW)
    return type_left_arrow;
  else if(aux_key == R_ARROW)
    return type_right_arrow;
  else if(aux_key == T_ARROW)
    return type_top_arrow;
  else if(aux_key == D_ARROW)
    return type_down_arrow;
  else if(aux_key == ESC)
    return type_ESC;
  else 
    return none;
}

Menu_event read_mouse_event(Mouse_event *ev, int32_t *mouse_x, int32_t *mouse_y)
{  
  *mouse_x += ev->dx;
  *mouse_y -= ev->dy;
  if (*mouse_x > (int32_t)get_h_res()-20)
    *mouse_x = (int32_t)get_h_res()-20;
  if (*mouse_x < 0)
    *mouse_x = 0;
  if (*mouse_y > (int32_t)get_v_res()-28)
    *mouse_y = (int32_t)get_v_res()-28;
  if (*mouse_y < 0)
    *mouse_y = 0;

  if (ev->ev == LB_DOWN) 
  {
    if (*mouse_x > race_x_left && *mouse_x < race_x_right && *mouse_y > race_y_top && *mouse_y < race_y_down)
      return click_on_race;
    else if (*mouse_x > friend_race_x_left && *mouse_x < friend_race_x_right && *mouse_y > friend_race_y_top &&  *mouse_y < friend_race_y_down)
      return click_on_race_with_friend;
    else if (*mouse_x > best_results_x_left && *mouse_x < best_results_x_right && *mouse_y > best_results_y_top &&  *mouse_y < best_results_y_down)
      return click_on_best_results;
    else if (*mouse_x > exit_x_left && *mouse_x < exit_x_right && *mouse_y > exit_y_top &&  *mouse_y < exit_y_down)
      return click_on_exit;
    else if (*mouse_x > try_again_x_left && *mouse_x < try_again_x_right && *mouse_y > try_again_y_top &&  *mouse_y < try_again_y_down)
      return click_on_try_again_race;
    else if (*mouse_x > results_exit_x_left && *mouse_x < results_exit_x_right && *mouse_y > results_exit_y_top &&  *mouse_y < results_exit_y_down)
      return click_on_results_exit;
    else if (*mouse_x > best_results_back_x_left && *mouse_x < best_results_back_x_right && *mouse_y > best_results_back_y_top &&  *mouse_y < best_results_back_y_down)
      return click_on_best_results_back;
  }
  return none;
}
