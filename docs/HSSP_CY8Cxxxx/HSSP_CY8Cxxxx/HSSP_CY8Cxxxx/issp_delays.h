// filename: ISSP_Delays.h
// Copyright 2015, Cypress Semiconductor Corporation.
//
// This software is owned by Cypress Semiconductor Corporation (Cypress)
// and is protected by and subject to worldwide patent protection (United
// States and foreign), United States copyright laws and international 
// treaty provisions. Cypress hereby grants to licensee a personal, 
// non-exclusive, non-transferable license to copy, use, modify, create 
// derivative works of, and compile the Cypress Source Code and derivative 
// works for the sole purpose of creating custom software in support of 
// licensee product to be used only in conjunction with a Cypress integrated 
// circuit as specified in the applicable agreement. Any reproduction, 
// modification, translation, compilation, or representation of this 
// software except as specified above is prohibited without the express 
// written permission of Cypress.
//
// Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND,EXPRESS OR IMPLIED, 
// WITH REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
// Cypress reserves the right to make changes without further notice to the
// materials described herein. Cypress does not assume any liability arising
// out of the application or use of any product or circuit described herein.
// Cypress does not authorize its products for use as critical components in
// life-support systems where a malfunction or failure may reasonably be
// expected to result in significant injury to the user. The inclusion of
// Cypress� product in a life-support systems application implies that the
// manufacturer assumes all risk of such use and in doing so indemnifies
// Cypress against all charges.
//
// Use may be limited by and subject to the applicable Cypress software
// license agreement.
//
//-----------------------------------------------------------------------------
#ifndef INC_ISSP_DELAYS
#define INC_ISSP_DELAYS

#include "ISSP_Revision.h"
#ifdef PROJECT_REV_300

// The Delay() routine, in ISSP_PhysicalLayer.c, has a delay of 2n+7 usec, 
// where n is the value passed to the routine.  This is true for the m8c micro-
// processor in the PSoC when it is running at a CPU clock of 12MHz.
//
// PROCESSOR_SPECIFIC
// If another processor is used, or if the m8c is running at a slower clock
// speed, then the delay parameters will be different. This file makes changing
// the delays simpler when porting the program to other processors.

// DELAY_M is the slope of the Delay = Mx + B equation
#define DELAY_M    2
// DELAY_B is the offset of the delay in Delay = Mx + B.
#define DELAY_B    7

///////////////////////////////////////////////////////////////////////////////
// CAUTION:
// For the above parameters the minimum delay value is 7 (this would result in 
// 0 being passed for a minimum delay. A value less than 7 would actually 
// create a negative number, causing a very long delay
///////////////////////////////////////////////////////////////////////////////

// TRANSITION_TIMEOUT is a loop counter for a 100msec timeout when waiting for 
// a high-to-low transition. This is used in the polling loop of 
// fDetectHiLoTransition(). 5700 loops approximately provides 100ms timeout 
#define TRANSITION_TIMEOUT     5700

// XRES_DELAY is the time duration for which XRES is asserted. This defines
// a 63 usec delay.
// The minimum Xres time (from the device datasheet) is 10 usec.
#define XRES_CLK_DELAY    ((63 - DELAY_B) / DELAY_M)

// POWER_CYCLE_DELAY is the time required when power is cycled to the target
// device to create a power reset after programming has been completed. The
// actual time of this delay will vary from system to system depending on the
// bypass capacitor size.  A delay of 150 usec is used here.
#define POWER_CYCLE_DELAY ((150 - DELAY_B) / DELAY_M)

// DELAY_100us delays 100 usec. This is used in fXRESInitializeTargetForISSP to
// time the wait for Vdd to become stable after a power up.  A loop runs 10 of
// these for a total delay of 1 msec.
#define DELAY100us        ((100 - DELAY_B) / DELAY_M)

#endif //(PROJECT_REV_)
#endif //(INC_ISSP_DELAYS)
//end of file ISSP_Delays.h