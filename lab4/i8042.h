#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define OK 0

#define DELAY_US    20000


#define KEYBOARD1_IRQ   1  /* Keyboard 1 IRQ line */
#define MOUSE12_IRQ     12 /* Mouse 12 IRQ line */

/* Commands for KBC */
#define READ_CB    0x20 /* Read command byte register */
#define WRITE_CB   0x60 /* Write command byte register */
#define DIS_MOUSE  0xA7 /* Disable mouse */
#define ENB_MOUSE  0xA8 /* Enable mouse */
#define WRT_MOUSE  0xD4 /* Write byte to mouse*/

/* Mouse commands */
#define ENB_DR     0xF4 /* Enable ddata reporting */
#define DIS_DR     0xF5 /* Disable data reporting */
#define SET_RM     0xF0 /* Set remote mode */ 
#define SET_SM     0xEA /* Set stream mode */
#define ST_REQUEST 0xE9 /* Get mouse configuration */

/* Mouse Byte 1 */
#define LB      BIT(0)
#define RB      BIT(1)
#define MB      BIT(2)
#define CTRL_B  BIT(3)
#define MSB_X   BIT(4)
#define MSB_Y   BIT(5)
#define XOV     BIT(6)
#define YOV     BIT(7)

/* Flags send by mouse */
#define ACK        0xFA /* Everything ok */
#define NACK       0xFE /* Invalid byte */
#define ERROR      0xFC /* Second consecutive invalid byte */

/* I/O port addresses */
#define OUT_BUF    0x60 /* Output buffer register */
#define IN_BUF     0x64 /* Input buffer register */

#define STAT_REG   0x64 /* Status register */


/* Status register */
#define OBF      BIT(0) /* Output buffer full */
#define IBF      BIT(1) /* Input buffer is full */
#define AUX      BIT(5) /* Mouse data */
#define TIMEOUT  BIT(6) /* Receive time out error */
#define PARITY   BIT(7) /* Parity error */

#define ESC_KEY   0x81

#endif /* _LCOM_I8042_H */
