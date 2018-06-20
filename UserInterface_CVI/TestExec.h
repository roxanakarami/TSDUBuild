/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2014. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  MAINPANEL                        1       /* callback function: MainPanelCallback */
#define  MAINPANEL_OPENFILEBTN            2       /* control type: activeX, callback function: (none) */
#define  MAINPANEL_RUNSELECTEDBTN         3       /* control type: activeX, callback function: (none) */
#define  MAINPANEL_SEQUENCEVIEW           4       /* control type: activeX, callback function: (none) */
#define  MAINPANEL_APPLICATIONMGR         5       /* control type: activeX, callback function: (none) */
#define  MAINPANEL_SEQUENCEFILEVIEWMGR    6       /* control type: activeX, callback function: (none) */
#define  MAINPANEL_EXECUTIONVIEWMGR       7       /* control type: activeX, callback function: (none) */
#define  MAINPANEL_LABEL                  8       /* control type: activeX, callback function: (none) */
#define  MAINPANEL_LISTBOX                9       /* control type: activeX, callback function: (none) */
#define  MAINPANEL_CHECKBOX               10      /* control type: activeX, callback function: (none) */
#define  MAINPANEL_TEXTMSG                11      /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK MainPanelCallback(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
