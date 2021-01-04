#ifndef _LCOM_VBE_H_
#define _LCOM_VBE_H_

#include <lcom/lcf.h>

/** @defgroup vbe vbe
 * @{
 *
 * VBE macros
 */


/* To set AX register */
#define SET_DIS_START       0x07        //!< Set VBE display start
#define SET_VBE_MODE        0x02        //!< Set VBE mode
#define RET_VBE_MODE        0x01        //!< Return VBE mode information
#define RET_VBE_CONTROLLER  0x00        //!< Return VBE controller information

/* To choose 0x07h function BL */
#define SET_DS      0x00    //!< Set display start
#define GET_DS      0x01    //!< Get display start
#define SET_DS_VR   0x80    //!< Set display start during Vertical Retrace

/* To set AH register */
#define VBE_FUNCTION 0x4F00 //!< Invoking a VBE function

/** @} end of vbe */

#endif /* _LCOM_VBE_H */
