#ifndef _H_PROJ_
#define _H_PROJ_

/** @defgroup proj proj
 * @{
 *
 * Proj main functions.
 */

/**
 * @brief Main function of the project
 * 
 */
int(proj_main_loop)(int argc, char *argv[]);

/**
 * @brief Initializes and configures the video card, initializes Chars module and call the function that read the file of best results
 * 
 * @return 0 if success 
 */
int proj_init();
/**
 * @brief Exits from video card game mode and frees memory allocated, ends Chars module and writes the best results in their file
 * 
 * @return int 
 */
int proj_end();
/**
 * @brief Subscribes all interrupts and enables mouse data reporting
 * 
 * @param timer Bit where notify timer interrupt
 * @param kbd Bit where notify keyboard interrupt
 * @param mouse Bit where notify mouse interrupt
 * @param rtc Bit where notify RTC interrupt
 * @return 0 if success, 1 otherwise
 */
int subscribe_all_int(uint8_t *timer, uint8_t *kbd, uint8_t *mouse, uint8_t *rtc);
/**
 * @brief Unsubscribes all interrupts and disables mouse data reporting
 * 
 * @return 0 if success, 1 otherwhise
 */
int unsubscribe_all_int();


/** @} end of proj */


#endif
