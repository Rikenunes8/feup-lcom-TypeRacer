#ifndef _LCOM_RTC_H
#define _LCOM_RTC_H

#include <lcom/lcf.h>
#include "../headers/utils.h"

#define RTC8_IRQ    8

// ports to access the RTC's registers 
#define RTC_ADDR_REG 0x70 //must be loaded with the address of the RTC register to be accessed
#define RTC_DATA_REG 0x71 //is used to transfer data to/from the RTC's register accessed

// RTC's internal address space
#define SECONDS     0x00
#define MINUTES     0x02
#define HOURS       0x04
#define DAY_WEEK    0x06
#define DAY         0x07
#define MONTH       0x08
#define YEAR        0x09
#define REG_A       0x0A
#define REG_B       0x0B
#define REG_C       0x0C
#define REG_D       0x0D

// RTC control/status bits
#define UIP     BIT(7)  /** Update in progress */
#define SET     BIT(7)  /** Inhibit updates of time/date registers */
#define IRQF    BIT(7)  /** IRQ line active */
#define PI      BIT(6)  /** Periodic interrupt */
#define AI      BIT(5)  /** Alarm interrupt */
#define UI      BIT(4)  /** Update interrupt */



int (rtc_subscribe_int)(uint8_t *bit_no);
int (rtc_unsubscribe_int)();

int rtc_read_register(int port, uint8_t* byte);

int rtc_write_register(int port, uint8_t* byte);

int rtc_check_update(bool* update);

int rtc_read_second(uint8_t *second);
int rtc_read_minute(uint8_t *minute);
int rtc_read_hour(uint8_t *hour);
int rtc_read_day_week(uint8_t *day_week);
int rtc_read_day(uint8_t *day);
int rtc_read_month(uint8_t *month);
int rtc_read_year(uint8_t *year);

int rtc_read_time(uint8_t* time);
int rtc_read_date(uint8_t* date);
int rtc_read_time_date(uint8_t* time_date);

void rtc_ih();

void handle_alarm_int();


#endif
