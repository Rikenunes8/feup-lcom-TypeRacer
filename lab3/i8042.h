#ifndef _LCOM_I8042_H_
#define _LCOM_I8042_H_

#include <lcom/lcf.h>

#define OK 0

#define KEYBOARD1_IRQ 1 /* Keyboard 1 IRQ line */

/* I/O port addresses */
#define OUT_BUF    0x60 /* Output buffer register */
#define IN_BUF     0x64 /* Input buffer register */

#define STAT_REG   0x64 /* Status register */


/* Status register */
#define OBF      BIT(0) /* Output buffer full */
#define IBF      BIT(1) /* Input buffer is full */
#define TIMEOUT  BIT(6) /* Receive time out error */
#define PARITY   BIT(7) /* Parity error */

#define ESC_KEY   0x81

#endif /* _LCOM_I8042_H */
