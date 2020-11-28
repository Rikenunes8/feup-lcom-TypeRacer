#include <lcom/lcf.h>


#include "../headers/proj.h"
#include "../headers/graphic.h"
#include "../headers/xpixmap.h"
#include <machine/int86.h>

extern xpm_map_t letters[];
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
  vbe_mode_info_t info;
  uint16_t mode = 0x115;
  //vbe_get_mode_info(mode, &info);
  graphic_get_mode_info(mode, &info);
  graphic_def(&info);
  graphic_init(mode, SET_VBE_MODE);
  vg_draw_rectangle(0,0,500, 500, 0xFFFFFF);
  sleep(5);
  graphic_xpm(letters[0], 20, 20, false);
  sleep(5);

  vg_exit();
  return 0;
}
