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

/** @} end of proj */


#endif
