/*  BEGIN_FILE_HDR
************************************************************************************************
* Copyright 2015 by Automotive Engineering Research Institute
* All rights exclusively reserved for Automotive Engineering Research Institute,
* unless expressly agreed to otherwise.
************************************************************************************************
*   File Name       : EB_Prj.h
************************************************************************************************
*   Project/Product :
*   Title           :
*   Author          : CATARC
************************************************************************************************
*   Description     : This is common module for body netword
*
************************************************************************************************
*   Limitations     : NONE
*
************************************************************************************************
*   END_FILE_HDR*/

#ifndef EB_PRJ_H
#define EB_PRJ_H

#include "ncs.h"

/* TYPES */
/* - Header Files: CAN                                                           */
#include "CAN_Types.h"
/* - Header Files: COM                                                           */
#include "COM_Types.h"
/* - Header Files: IL                                                            */
#include "IL_Types.h"
/*-------------------------------------------------------------------------------*/
/* - Header Files: TP                                                            */
#include "TP_Types.h"
/*-------------------------------------------------------------------------------*/
/* - Header Files: DIAG                                                          */
#include "UDS_Types.h"
/* - Header Files: EB                                                            */
#include "EB_Types.h"
/* - Header Files: APP                                                           */
#include "APP_Types.h"


/* PUB */
/* - Header Files: CAN                                                           */
#include "CAN_Pub.h"
/* - Header Files: COM                                                           */
#include "COM_Pub.h"
/* - Header Files: IL                                                            */
#include "IL_Pub.h"
/*-------------------------------------------------------------------------------*/
/* - Header Files: TP                                                            */
#include "TP_Pub.h"
/*-------------------------------------------------------------------------------*/
/* - Header Files: DIAG                                                           */
#include "UDS_Pub.h"
/*-------------------------------------------------------------------------------*/
/* - Header Files: EB                                                            */
#include "EB_Pub.h"
/*-------------------------------------------------------------------------------*/
/* - Header Files: APP                                                           */
#include "APP_Pub.h"

/* CFG */
/* - Header Files: CAN                                                           */
#include "CAN_Cfg.h"
/* - Header Files: COM                                                           */
#include "COM_Cfg.h"
/* - Header Files: IL                                                            */
#include "IL_Cfg.h"
/*-------------------------------------------------------------------------------*/
/* - Header Files: TP                                                            */
#include "TP_Cfg.h"
/*-------------------------------------------------------------------------------*/
/* - Header Files: DIAG                                                          */
#include "UDS_Cfg.h"
/*-------------------------------------------------------------------------------*/
/* - Header Files: EB                                                            */
#include "EB_Cfg.h"


/* HW */
/* - Header Files: CAN                                                           */
#include "CAN_Hw.h"

/* CBK */
/* - Header Files: CAN                                                            */
#include "CAN_Cbk.h"
/* - Header Files: COM                                                            */
#include "COM_Cbk.h"
/* - Header Files: IL                                                            */
#include "IL_Cbk.h"
/*-------------------------------------------------------------------------------*/
/* - Header Files: TP                                                            */
#include "TP_Cbk.h"
/*-------------------------------------------------------------------------------*/
/* - Header Files: DIAG                                                          */
#include "UDS_Cbk.h"
/* - Header Files: EB                                                            */
#include "EB_Cbk.h"
/* - Header Files: APP                                                           */
#include "APP_Cbk.h"

/*Random Generate*/
#include "stdlib.h"

#include "main.h"
#endif      /* EB_PRJ_H */
