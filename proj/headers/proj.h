#ifndef _H_PROJ_
#define _H_PROJ_

/** @defgroup proj proj
 * @{
 *
 * In this module we only have one funtion: proj_main_loop.
 * Here we have the various texts to use in the game, 
 * subscribed and unsubscribed to all the devices,
 * detected the interrupts received in driver_receive loop
 * and implemented the game state machine.
 */

/**
 * @brief Main function of the project
 * 
 */
int(proj_main_loop)(int argc, char *argv[]);

/**
 * @brief Initializes and configure the video card, initializes Chars module and call the function that read the file of best results
 * 
 * @return 0 if success 
 */
int proj_init();
/**
 * @brief Exits from video card game mode and free memory allocated, end Chars module and writes the best results in their file
 * 
 * @return int 
 */
int proj_end();
/**
 * @brief Subscribe all interrupts and enable mouse data reporting
 * 
 * @param timer Bit where notify timer interrupt
 * @param kbd Bit where notify keyboard interrupt
 * @param mouse Bit where notify mouse interrupt
 * @param rtc Bit where notify RTC interrupt
 * @return 0 if success, 1 otherwise
 */
int subscribe_all_int(uint8_t *timer, uint8_t *kbd, uint8_t *mouse, uint8_t *rtc);
/**
 * @brief Unsubscribe all interrupts and disable mouse data reporting
 * 
 * @return 0 if success, 1 otherwhise
 */
int unsubscribe_all_int();


/** @} end of proj */


#endif
