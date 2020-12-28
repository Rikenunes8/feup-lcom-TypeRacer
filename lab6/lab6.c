// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include "rtc.h"

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}



int(video_test_init)(uint16_t mode, uint8_t delay) 
{
  uint8_t time_date[6];
  
  rtc_read_time_date(time_date);
  
  printf("seconds: %d\n", time_date[0]);
  printf("minutes: %d\n", time_date[1]);
  printf("hours: %d\n", time_date[2]);
  
  printf("day: %d\n", time_date[3]);
  printf("month: %d\n", time_date[4]);
  printf("year: %d\n", time_date[5]);

	return 0;
}

int rtc_test_date(void) 
{
	/* To be completed */
	return 0;
}

int rtc_test_int(/* to be defined in class */) 
{ 
	/* To be completed */
	return 0;
}


