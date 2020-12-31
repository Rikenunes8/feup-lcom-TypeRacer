#ifndef _LCOM_RTC_H
#define _LCOM_RTC_H


#define RTC8_IRQ    8   /** RTC 8 IRQ line */

#define DONT_CARE   0xC0 //! Don't care byte to alarm

// ports to access the RTC's registers 
#define RTC_ADDR_REG 0x70 //! Must be loaded with the address of the RTC register to be accessed
#define RTC_DATA_REG 0x71 //! Used to transfer data to/from the RTC's register accessed

// RTC's internal address space
#define SECONDS     0x00  //! Resister of seconds
#define SECONDS_A   0x01  //! Resister of seconds alarm
#define MINUTES     0x02  //! Resister of minutes
#define MINUTES_A   0x03  //! Resister of minutes alarm
#define HOURS       0x04  //! Resister of hours
#define HOURS_A     0x05  //! Resister of hours alarm
#define DAY_WEEK    0x06  //! Resister of week day
#define DAY         0x07  //! Resister of day
#define MONTH       0x08  //! Resister of month
#define YEAR        0x09  //! Resister of year
#define REG_A       0x0A  //! Resister A
#define REG_B       0x0B  //! Resister B
#define REG_C       0x0C  //! Resister C
#define REG_D       0x0D  //! Resister D

// RTC control/status bits
#define UIP     BIT(7)  /** Update in progress */
#define SET     BIT(7)  /** Inhibit updates of time/date registers */
#define IRQF    BIT(7)  /** IRQ line active */
#define PI      BIT(6)  /** Periodic interrupt */
#define AI      BIT(5)  /** Alarm interrupt */
#define UI      BIT(4)  /** Update interrupt */


/**
 * @brief Subscribes and enables RTC interrupts
 *
 * @param bit_no address of memory to be initialized with the
 *         bit number to be set in the mask returned upon an interrupt
 * @return Return 0 upon success and non-zero otherwise
 */
int (rtc_subscribe_int)(uint8_t *bit_no);
/**
 * @brief Unsubscribes RTC interrupts
 *
 * @return Return 0 upon success and non-zero otherwise
 */
int (rtc_unsubscribe_int)();
/**
 * @brief Read a register
 * 
 * @param port Register to read
 * @param byte Place to put the content read
 * @return 0 if sucess, 1 otherwise
 */
int rtc_read_register(int port, uint8_t* byte);
/**
 * @brief Write a register
 * 
 * @param port Register to be wrote
 * @param byte Byte to write in register
 * @return 0 if sucess, 1 otherwise
 */
int rtc_write_register(int port, uint8_t* byte);
/**
 * @brief Check if rtc is updating
 * 
 * @param update Return true if it's updating, false otherwise
 * @return int 
 */
int rtc_check_update(bool* update);

/**
 * @brief Read seconds
 * 
 * @param second Seconds in binary
 * @return 0 if success
 */
int rtc_read_second(uint8_t *second);
/**
 * @brief Read minutes
 * 
 * @param minute Minutes in binary
 * @return 0 if success
 */
int rtc_read_minute(uint8_t *minute);
/**
 * @brief Read hours
 * 
 * @param hour Hours in binary
 * @return 0 if success
 */
int rtc_read_hour(uint8_t *hour);
/**
 * @brief Read day of week
 * 
 * @param day_week Day of week in binary
 * @return 0 if success
 */
int rtc_read_day_week(uint8_t *day_week);
/**
 * @brief Read day
 * 
 * @param day Day in binary
 * @return 0 if success
 */
int rtc_read_day(uint8_t *day);
/**
 * @brief Read month
 * 
 * @param month Month in binary
 * @return 0 if success
 */
int rtc_read_month(uint8_t *month);
/**
 * @brief Read year
 * 
 * @param year Year in binary
 * @return 0 if success
 */
int rtc_read_year(uint8_t *year);

/**
 * @brief Read current second, minute and hour
 * 
 * @param time Array with seconds, minutes and hours
 * @return 0 if success
 */
int rtc_read_time(uint8_t* time);
/**
 * @brief Read current day, month and year
 * 
 * @param date Array with day, month and year
 * @return 0 if success
 */
int rtc_read_date(uint8_t* date);
/**
 * @brief Read time and date
 * 
 * @param time_date Array with time and date
 * @return 0 if success
 */
int rtc_read_time_date(uint8_t* time_date);
/**
 * @brief Set alarm interrupts on
 * 
 */
void rtc_turn_on_alarm();
/**
 * @brief Set alarm interrupts off
 * 
 */
void rtc_turn_off_alarm();

/**
 * @brief Set alarm in BCD
 * 
 * @param sec Seconds
 * @param min Minutes
 * @param h Hours
 */
void rtc_set_alarm(uint8_t sec, uint8_t min, uint8_t h);


/**
 * @brief Handles RTC interrupts (C implementation)
 * @details Reads Register C and see why is the interrupt.
 * Call the handler of the respective interrupt
 * All communication with other code must be done via global variables, static if possible.
 */
void rtc_ih();
/**
 * @brief Notify a alarm interrupt
 * 
 */
void handle_alarm_int();
/**
 * @brief Check if there was an alarm interrupt
 * 
 * @return True if alarm interrupt is notified, false otherwise
 */
bool get_alarm();
/**
 * @brief Clean notify of alarm interrupt
 * 
 */
void use_alarm();


#endif
