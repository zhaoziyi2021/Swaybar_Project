#ifndef _CAN_H_
#define _CAN_H_

//****************************************************************************
// @Project Includes
//****************************************************************************

// USER CODE BEGIN (CAN_Header,2)
#include "gd32a50x_can.h"
// USER CODE END


//****************************************************************************
// @Macros
//****************************************************************************

#define SetListCommand(ValueH, ValueL) CAN_PANCTRH = ValueH; CAN_PANCTRL = ValueL; while(CAN_PANCTRL & CAN_PANCTR_BUSY);

// USER CODE BEGIN (CAN_Header,3)
// USER CODE END


//****************************************************************************
// @Defines
//****************************************************************************

// Panel Busy Flag
#define CAN_PANCTR_BUSY       0x0100

// Message Object Status Register
#define MOSTAT_RXPND          0x0001
#define MOSTAT_TXPND          0x0002
#define MOSTAT_NEWDAT         0x0008
#define MOSTAT_MSGLST         0x0010
#define MOSTAT_RST_MNR        0x0019
#define MOSTAT_RST_NT         0x000A

struct stCanObj
{
    uword  uwMOFCRL;    // Function Control Register Low
    uword  uwMOFCRH;    // Function Control Register High
    uword  uwMOFGPRL;   // FIFO/Gateway Pointer Register Low
    uword  uwMOFGPRH;   // FIFO/Gateway Pointer Register High
    uword  uwMOIPRL;    // Interrupt Pointer Register Low
    uword  uwMOIPRH;    // Interrupt Pointer Register High
    uword  uwMOAMRL;    // Acceptance Mask Register Low
    uword  uwMOAMRH;    // Acceptance Mask Register High
    ubyte  ubData[8];   // Message Data 0..7
    uword  uwMOARL;     // Arbitration Register Low
    uword  uwMOARH;     // Arbitration Register High
    uword  uwMOCTRL;    // Control Register Low
    uword  uwMOCTRH;    // Control Register High
};

typedef struct
{
    ubyte ubMOCfg;    // message object configuration
    ulong ulID;       // standard (11-bit)/extended (29-bit) identifier
    ulong ulMask;     // standard (11-bit)/extended (29-bit) mask
    ubyte ubData[8];  // 8-bit data bytes
    uword uwCounter;  // frame counter(MOIPRnH[15-0])
}stCAN_SWObj;

#define CAN_HWOBJ ((struct stCanObj volatile *) 0x201000)
#define CAN_ubRequestMsgObj(ObjNr) (((CAN_HWOBJ[ObjNr].uwMOCTRL & 0x0100) == 0x0000) \
   ? CAN_HWOBJ[ObjNr].uwMOCTRL = 0x0020, 1 : 0)

#define CAN_SRN0INT    0x40

extern void CAN_vInit(void);
extern void CAN_CAN0Init(void);
extern void CanDebugSend(ulong ulID,ubyte byte0,ubyte byte1,ubyte byte2,ubyte byte3,ubyte byte4,ubyte byte5,ubyte byte6,ubyte byte7);

#endif  // ifndef _CAN_H_
