/**************************************************************************
 *
 *  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
 *  PURPOSE.
 *
 *  Copyright (C) 1993, 1994  Microsoft Corporation.  All Rights Reserved.
 * 
 **************************************************************************/

/*
 *   filter.h
 */

/* Structure to specify a filter for MIDI events.
 */
typedef struct filter_tag
{
    BOOL channel[16];
    struct {
        BOOL noteOn;
        BOOL noteOff;
        BOOL keyAftertouch;
        BOOL controller;
        BOOL progChange;
        BOOL chanAftertouch;
        BOOL pitchBend;
        BOOL channelMode;
    
        BOOL sysEx;
        BOOL sysCommon;
        BOOL sysRealTime;
        BOOL activeSense;
        BOOL pad0;
        BOOL pad1;
        BOOL pad2;
        BOOL pad3;
    } event;
} FILTER;

typedef FILTER FAR *LPFILTER;


/* Function prototypes
 */
BOOL CheckEventFilter(LPEVENT lpEvent, LPFILTER lpFilter);
