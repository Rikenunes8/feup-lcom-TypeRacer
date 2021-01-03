#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <machine/int86.h>

#include "../headers/proj.h"
#include "../headers/timer.h"
#include "../headers/keyboard.h"
#include "../headers/mouse.h"
#include "../headers/graphic.h"
#include "../headers/rtc.h"
#include "../headers/sprite.h"
#include "../headers/menus.h"
#include "../headers/race.h"
#include "../headers/best_results.h"

#include "../xpm/menu.h"
#include "../xpm/others.h"


int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}


int(proj_main_loop)(int argc, char *argv[])
{ 
  proj_init();
  

  char text1[] = "The Brothers Karamazov is a passionate philosophical novel that enters deeply into questions of God, free will, and morality. It is a theological drama dealing with problems of faith, doubt and reason in the context of a modernizing Russia, with a plot that revolves around the subject of patricide. Dostoevsky composed much of the novel in Staray Russa, which inspired the main setting. It is one of the supreme achievements in world literature.";
  char text2[] = "Yeah, they got you where they want you. There's a better life and you think about it, don't you? It's a rich man's game no matter what they call it and you spend your life putting money in his wallet."; 
  char text3[] = "Yeah, they got you where they want you.";
  char text4[] = "Getting information off the internet is like taking a drink from a fire hydrant.";
  char text5[] = "Look into my eyes and it's easy to see, one and one make two, two and one make three, it was destiny. Once every hundred thousand years or so, when the sun doth shine and the moon doth glow and the grass doth grow.";
  char text6[] = "Kid, I've flown from one side of this galaxy to the other, and I've seen a lot of strange stuff. But I've never seen anything to make me believe that there's one all-powerful Force controlling everything. There's no mystical energy field that controls MY destiny.";
  char* texts[] = {text1, text2, text3, text4, text5, text6};
  uint8_t text_choice = 0;
  uint8_t no_texts = 6;
  

  /* Timer stuff */
  extern uint32_t timer_counter;
  timer_counter = 0;
  uint8_t timer_bit_no = 0;
  uint32_t timer_irq_set = BIT(timer_bit_no);

  /*Keyboard stuff*/
  uint8_t scancode_bytes[2];
  uint8_t kbd_bit_no = 1;
  uint32_t kbd_irq_set = BIT(kbd_bit_no);

  /*Mouse stuff*/
  uint8_t mouse_bit_no = 12;
  uint32_t mouse_irq_set = BIT(mouse_bit_no);
  struct packet pp;
  extern uint8_t packet_byte_counter;
  Mouse_event mouse_event; mouse_event.ev = MOVE;

  /*RTC stuff*/
  uint8_t rtc_bit_no = 8;
  uint32_t rtc_irq_set = BIT(rtc_bit_no);

  Sprite* mouse = create_sprite((xpm_map_t)mouse_xpm, 0, 0, 0, 0);
  Sprite* main_menu = create_sprite((xpm_map_t)menu, 0, 0, 0, 0);

  Menu_state state = MENU;
  uint8_t aux_key = NOTHING;

  bool timer_int = false;
  bool kbd_int = false;
  bool mouse_int = false;


  if (subscribe_all_int(&timer_bit_no, &kbd_bit_no, &mouse_bit_no, &rtc_bit_no) != 0) return 1;

  int ipc_status;
  message msg;
  int r = 0;

  while(state != EXIT) { 
  //Get a request message. 
    if ( (r = driver_receive(ANY, &msg, &ipc_status)) != 0 ) { 
        printf("driver_receive failed with: %d", r);
        continue;
    }
    if (is_ipc_notify(ipc_status)) {  // received notification 
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE: // hardware interrupt notification 				
          if (msg.m_notify.interrupts & timer_irq_set) {
            if (timer_counter%2 == 0) {
              fr_buffer_to_video_mem();
            }
            timer_ih();
            timer_int = true;

          }
          if (msg.m_notify.interrupts & kbd_irq_set) {
            // keyboard interrupt 
            kbd_ih();
            assemble_scancode(scancode_bytes);
            // If scancode is all set
            if (!(scancode_bytes[0] == 0xE0 && scancode_bytes[1] == 0x00)) {
              aux_key = get_scancode_char(scancode_bytes);
              kbd_int = true;
            }
          }
          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            if (packet_byte_counter == 3) {
              packet_byte_counter = 0;
              assemble_packet(&pp);
              mouse_events(&mouse_event, &pp);
              mouse_int = true; 
            }    
          }
          if (msg.m_notify.interrupts & rtc_irq_set) {
            rtc_ih(); 
          }
          break;
        default:
          printf("Receive no interrupt\n");
          break; // no other notifications expected: do nothing 	
      }
    } 
    else 
    {  
      //received a standard message, not a notification 
      //no standard messages expected: do nothing 
    }
    switch (state) {
      case MENU:
        if (timer_int)
          menus_process_timer_int(timer_counter, main_menu, mouse);
        if (kbd_int)
          menus_process_kbd_int(&state, aux_key);
        if (mouse_int)
          menus_process_mouse_int(&state, mouse_event, mouse);
        
        if (state == RACE) {
          rtc_read_second(&text_choice);
          race_init(texts[text_choice%no_texts], strlen(texts[text_choice%no_texts]));
        }
        if (state == BEST_RESULTS)
          br_init();  
        break;

      case RACE:
        if (kbd_int)
          race_process_kbd_int(&state, aux_key);
        if (mouse_int)
          race_process_mouse_int(&state, mouse_event, mouse);
        if (timer_int)
          race_process_timer_int(&state, timer_counter, mouse);
        
        if (state != RACE)
          race_end();
        if (state == RESULTS)
          results_init(mouse);
        break;

      case RESULTS:
        if (timer_int)
          results_process_timer_int(timer_counter, mouse);
        if (mouse_int)
          results_process_mouse_int(&state, mouse_event, mouse);
        if (kbd_int)
          results_process_kbd_int(&state, aux_key);

        if (state != RESULTS)
          results_end();
        if (state == RACE)
          race_init(texts[text_choice%no_texts], strlen(texts[text_choice%no_texts]));
        break;

      case RACE_WITH_FRIEND:
        state = MENU;
        break;

      case BEST_RESULTS:
        if (timer_int)
          br_process_timer_int(timer_counter, mouse);
        if (kbd_int)
          br_process_kbd_int(&state, aux_key);
        if (mouse_int)
          br_process_mouse_int(&state, mouse_event, mouse);
        
        if (state != BEST_RESULTS)
          br_end();
        break;

      case EXIT:
        break;

      default:
        break;
    }
    timer_int = false;
    kbd_int = false;
    mouse_int = false;
  }

  
  
  if (unsubscribe_all_int() != 0) return 1;

  destroy_sprite(mouse);
  destroy_sprite(main_menu);
  proj_end();
  
  return 0;
}


int proj_init() {
  vbe_mode_info_t info;
  uint16_t mode = 0x115;
  graphic_get_mode_info(mode, &info);
  graphic_def(&info);
  graphic_init(mode);

  Chars_init();
  
  br_read_file();
  return 0;
}

int proj_end() {
  br_write_file();

  Chars_end();
  
  destroy_fr_bufffer();
  vg_exit();
  return 0;
}

int subscribe_all_int(uint8_t *timer, uint8_t *kbd, uint8_t *mouse, uint8_t *rtc) {
  if (timer_subscribe_int(timer) != 0) return 1;
  if (kbd_subscribe_int(kbd) != 0) return 1;
  if (kbc_write_byte(WRT_MOUSE, ENB_DR) != 0) return 1; // Enable data report
  if (mouse_subscribe_int(mouse) != 0) return 1;
  if (rtc_subscribe_int(rtc) != 0) return 1;
  return 0;
}

int unsubscribe_all_int() {
  if (timer_unsubscribe_int() != 0) return 1;
  if (kbd_unsubscribe_int() != 0) return 1;
  if (mouse_unsubscribe_int() != 0) return 1; 
  if (kbc_write_byte(WRT_MOUSE, DIS_DR) != 0) return 1; // Disable data report
  if (rtc_unsubscribe_int() != 0) return 1;
  return 0;
}



