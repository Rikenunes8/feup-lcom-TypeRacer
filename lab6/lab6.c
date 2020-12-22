// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

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

//ports to access the RTC's registers 
#define RTC_ADDR_REG 0x70 //must be loaded with the address of the RTC register to be accessed
#define RTC_DATA_REG 0x71 //is used to transfer data to/from the RTC's register accessed

//RTC's internal address space
#define MINUTES 0x02
#define HOURS 0x04

int(video_test_init)(uint16_t mode, uint8_t delay) 
{
  uint32_t minutes = 0, hours = 0;
	sys_outb(RTC_ADDR_REG, MINUTES);
  sys_inb(RTC_DATA_REG, &minutes);
  sys_outb(RTC_ADDR_REG, HOURS);
  sys_inb(RTC_DATA_REG, &hours);

  printf("minutes: %d\n", minutes);
  printf("hours: %d\n", hours);

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


