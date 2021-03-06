#include <lcom/lcf.h>

#include "../headers/rtc.h"
#include "../headers/utils.h"

static int hook_id;

static bool alarmed = false;



int (rtc_subscribe_int)(uint8_t *bit_no) {
    hook_id = *bit_no;
    if (sys_irqsetpolicy(RTC8_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id) != OK) return 1;
    return 0;
}
int (rtc_unsubscribe_int)() {
    if (sys_irqrmpolicy(&hook_id) != OK) return 1;
    return 0;
}


int rtc_read_register(int port, uint8_t* byte) {
  if (sys_outb(RTC_ADDR_REG, port)) return 1;
  if (util_sys_inb(RTC_DATA_REG, byte)) return 1;
  return 0;  
}

int rtc_write_register(int port, uint8_t* byte) {
  if (sys_outb(RTC_ADDR_REG, port)) return 1;
  if (sys_outb(RTC_DATA_REG, *byte)) return 1;
  return 0;
}



int rtc_check_update(bool* update) {
    uint8_t reg_A;
    if (rtc_read_register(REG_A, &reg_A)) return 1;
    *update = reg_A & UIP;
    return 0;
}

int rtc_read_second(uint8_t *second) {
  rtc_read_register(SECONDS, second);
  bcd_to_binary(second);
  return 0;
}
int rtc_read_minute(uint8_t *minute) {
  rtc_read_register(MINUTES, minute);
  bcd_to_binary(minute);
  return 0;
}
int rtc_read_hour(uint8_t *hour) {
  rtc_read_register(HOURS, hour);
  bcd_to_binary(hour);
  return 0;
}
int rtc_read_day_week(uint8_t *day_week) {
  rtc_read_register(DAY_WEEK, day_week);
  bcd_to_binary(day_week);
  return 0;
}
int rtc_read_day(uint8_t *day) {
  rtc_read_register(DAY, day);
  bcd_to_binary(day);
  return 0;
}
int rtc_read_month(uint8_t *month) {
  rtc_read_register(MONTH, month);
  bcd_to_binary(month);
  return 0;
}
int rtc_read_year(uint8_t *year) {
  rtc_read_register(YEAR, year);
  bcd_to_binary(year);
  return 0;
}

int rtc_read_time(uint8_t* time) {
    rtc_read_second(&time[0]);
    rtc_read_minute(&time[1]);
    rtc_read_hour(&time[2]);
    return 0;
}
int rtc_read_date(uint8_t* date) {
    rtc_read_day(&date[0]);
    rtc_read_month(&date[1]);
    rtc_read_year(&date[2]);
    return 0;
}
int rtc_read_time_date(uint8_t* time_date) {
    rtc_read_time(&time_date[0]);
    rtc_read_date(&time_date[3]);
    return 0;
}


void rtc_turn_on_alarm() {
  alarmed = false;
  uint8_t byte;
  rtc_read_register(REG_B, &byte);
  byte = byte | AI;
  rtc_write_register(REG_B, &byte);
}

void rtc_turn_off_alarm() {
  uint8_t byte;
  rtc_read_register(REG_B, &byte);
  byte = byte & (~AI);
  rtc_write_register(REG_B, &byte);
}

/* sec, min and h in hexadecimal*/
void rtc_set_alarm(uint8_t sec, uint8_t min, uint8_t h) {
  uint8_t byte;
  rtc_read_register(REG_B, &byte);
  byte = byte | SET;
  rtc_write_register(REG_B, &byte);
  rtc_write_register(SECONDS_A, &sec);
  rtc_write_register(MINUTES_A, &min);  
  rtc_write_register(HOURS_A, &h);
  byte = byte & (~SET);
  rtc_write_register(REG_B, &byte);
}

void rtc_ih() {
    uint8_t reg;
    rtc_read_register(REG_C, &reg);
    if (reg & AI)
      handle_alarm_int();
}

void handle_alarm_int() {
  alarmed = true;
  return;
}

bool get_alarm() {return alarmed;}

void use_alarm() {alarmed = false;}




