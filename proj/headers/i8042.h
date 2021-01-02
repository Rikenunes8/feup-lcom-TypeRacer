#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

/** @defgroup i8042 i8042
 * @{
 *
 * Macros for i8042
 */

#define DELAY_US    20000 //!< Delay


#define KEYBOARD1_IRQ   1  //!< Keyboard 1 IRQ line 
#define MOUSE12_IRQ     12 //!< Mouse 12 IRQ line 

/* Commands for KBC */
#define READ_CB    0x20 //!< KBC command: Read command byte register 
#define WRITE_CB   0x60 //!< KBC command: Write command byte register 
#define DIS_MOUSE  0xA7 //!< KBC command: Disable mouse 
#define ENB_MOUSE  0xA8 //!< KBC command: Enable mouse 
#define WRT_MOUSE  0xD4 //!< KBC command: Write byte to mouse

/* Mouse commands */
#define ENB_DR     0xF4 //!< Mouse command: Enable data reporting 
#define DIS_DR     0xF5 //!< Mouse command: Disable data reporting 
#define SET_RM     0xF0 //!< Mouse command: Set remote mode 
#define SET_SM     0xEA //!< Mouse command: Set stream mode 
#define READ_DATA  0xEB //!< Mouse command: Get mouse configuration 

/* Mouse Byte 1 */
#define LB      BIT(0) //!< Mouse byte: left button 
#define RB      BIT(1) //!< Mouse byte: right button
#define MB      BIT(2) //!< Mouse byte: middle button
#define CTRL_B  BIT(3) //!< Mouse byte: control
#define MSB_X   BIT(4) //!< Mouse byte: MSB_X
#define MSB_Y   BIT(5) //!< Mouse byte: MSB_Y
#define XOV     BIT(6) //!< Mouse byte: X overflow
#define YOV     BIT(7) //!< Mouse byte: Y overflow

/* Flags send by mouse */
#define ACK        0xFA //!< Flag send by mouse: Everything ok 
#define NACK       0xFE //!< Flag send by mouse: Invalid byte 
#define ERROR      0xFC //!< Flag send by mouse: Second consecutive invalid byte 

/* I/O port addresses */
#define OUT_BUF    0x60 //!< I/O port addresses: Output buffer register
#define IN_BUF     0x64 //!< I/O port addresses: Input buffer register

#define STAT_REG   0x64 //!< I/O port addresses: Status register 


/* Status register */
#define OBF      BIT(0) //!< Status register: Output buffer full 
#define IBF      BIT(1) //!< Status register: Input buffer is full 
#define AUX      BIT(5) //!< Status register: Mouse data 
#define TIMEOUT  BIT(6) //!< Status register: Receive time out error 
#define PARITY   BIT(7) //!< Status register: Parity error 


/* Keys */
#define ENTER_KEY   0x1c  //!< Scancode of enter key 
#define ESC_KEY     0x81  //!< Scancode of esc key 

/** @} end of i8042 */

#endif /* _LCOM_I8042_H */
