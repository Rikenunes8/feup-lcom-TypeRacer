#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#include <lcom/lcf.h>

#define OK 0

/* To set AX register */
#define SET_VBE_MODE 0x02         //Set VBE mode
#define RET_VBE_MODE 0x01         //Return VBE mode information
#define RET_VBE_CONTROLLER 0x02   //Return VBE controller information

/* To set AH register */
#define VBE_FUNCTION 0x4F00 //Invoking a VBE function

#endif /* _LCOM_VBE_H */
