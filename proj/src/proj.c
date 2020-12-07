#include <lcom/lcf.h>


#include "../headers/proj.h"
#include "../headers/graphic.h"
#include "../headers/race.h"
#include <machine/int86.h>


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
  //char text[] = "Yeah, they got you where they want you. There's a better life and you think about it, don't you? It's a rich man's game no matter what they call it and you spend your life putting money in his wallet.";
  char text[] = "Yeah, ya."; 
  
  vbe_mode_info_t info;
  uint16_t mode = 0x115;
  graphic_get_mode_info(mode, &info);
  graphic_def(&info);
  graphic_init(mode, SET_VBE_MODE);
  
  //graphic_set_background(0xFFFFFF);
  //fr_buffer_to_video_mem();
  graphic_draw_rectangle(0,0,get_h_res(), get_v_res(), WHITE);
  sleep(2);

  race_init(text, strlen(text));
  
  //destroy_fr_bufffer();
  vg_exit();
  return 0;
}
