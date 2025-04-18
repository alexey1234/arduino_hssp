// filename: ProgrammingSteps.c
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
//--------------------------------------------------------------------------

#include <m8c.h>        // part specific constants and macros
#include "PSoCAPI.h"    // PSoC API definitions for all User Modules
#include "ISSP_Extern.h"
#include "ISSP_Errors.h"
#include "ISSP_Revision.h"

#ifdef PROJECT_REV_300

unsigned char bBankCounter;
unsigned int  iBlockCounter;
unsigned int  iChecksumData;
unsigned int  iChecksumTarget;

/* ========================================================================= */
// ProgramDevice()
// This function programs the target device. It runs through each stage and 
// returns "PASS" if all stages succeed. 
/* ========================================================================= */
signed char ProgramDevice(void)
{
	/* >>>> ISSP Programming Starts Here <<<< */
	
	/* Step 1. Initialization Procedure */
	if(InitializeTargetProcedure()!=PASS)
		return (INIT_ERROR);
	
	/* Step 2. Verify Silicon ID Procedure */
	if(VerifySiliconIDProcedure()!=PASS)
		return (SID_ERROR);

	/* Step 3. Erase All Flash Procedure */
	if (EraseAllFlashProcedure()!=PASS)
		return (ERASE_ERROR);
	
	/* Step 4. Program Flash Procedure */
	if (ProgramFlashProcedure()!=PASS)
		return(BLOCK_ERROR);
		
	/* Step 5. Verify Flash Procedure */
	if(VerifyFlashProcedure()!=PASS)
		return (VERIFY_ERROR);
	
	/* Step 6. Program Security Bits Procedure */
	if(ProgramSecurityBitsProcedure()!=PASS)
		return (SECURITY_ERROR);
	
	/* Step 7. Verify Security Bits Procedure */
	if (VerifySecurityBitsProcedure()!=PASS)
		return (SECURITY_ERROR);
	
	/* Step 8. Verify Checksum Procedure */
	if (VerifyChecksumProcedure()!=PASS)
		return (ERROR);
	
	/* All stages of programming passed */
	return(PASS);
}

/* ========================================================================= */
// InitializeTargetProcedure()
// This function acquires the target device. Returns "PASS" if succeeds. 
/* ========================================================================= */
signed char InitializeTargetProcedure(void)
{
	/* Acquire the device through reset or power cycle */
	
    #ifdef RESET_MODE
	
		/* Reset Mode */
	
		/* Initialize device */
		fIsError = fXRESInitializeTargetForISSP();
		
		/* If failed, return error flag */
	    if (fIsError != PASS) 
		{		
			#ifdef USE_TX
				SendDebugMessage("> Target Reset Initialization Failed\n");	
			#endif
			
	        return (fIsError);
	    }

		/* Initialization succeded */
		#ifdef USE_TX
			SendDebugMessage("> Reset Initialization Completed\n");
		#endif	
	
    #else
	
	    /* Power Cycle Mode */
		
		#ifdef USE_TX
			SendDebugMessage("> Power Cycling...\n");
		#endif 
		
		/* Initialize device */ 
		fIsError = fPowerCycleInitializeTargetForISSP();
		
		/* If failed, return error flag */
		if (fIsError != PASS) 
		{		
			/* Remove VDD from the target device */
			RemoveTargetVDD();
			
			#ifdef USE_TX
				SendDebugMessage("> Target Power Cycle Initialization Failed\n");
			#endif
			
			return(fIsError);		
		}

		/* Initialization succeded */
		#ifdef USE_TX
			SendDebugMessage("> Power Cycle Initialization Completed\n");
		#endif
	
    #endif
	
	return (PASS);
}

/* ========================================================================= */
// VerifySiliconIDProcedure()
// This function verifies the silicon ID of the target device. Returns 
// "PASS" if succeeds. 
/* ========================================================================= */
signed char VerifySiliconIDProcedure(void)
{	
	fIsError = fVerifySiliconID();
	
	if (fIsError != PASS)
	{ 		
		#ifdef USE_TX
			SendDebugMessage("> Silicon ID Verification Failed\n");
			PrintReceivedSiliconID();
		#endif 		
		
        return(fIsError);
    }
		
	#ifdef USE_TX
		SendDebugMessage("> Silicon ID Verified\n");
		PrintReceivedSiliconID();		
	#endif	
	
	return (PASS);
}
	
/* ========================================================================= */
// EraseAllFlashProcedure()
// This function erases the entire flash. Returns "PASS" if succeeds. 
/* ========================================================================= */
signed char EraseAllFlashProcedure(void)
{	
    #ifdef USE_TP
    	SetTPHigh();    
    #endif
	
	fIsError = fEraseTarget();
	
	#ifdef USE_TP
    	SetTPLow();    
    #endif
	
    if (fIsError != PASS) 
	{		
		#ifdef USE_TX
			SendDebugMessage("> Bulk Erase Failed\n");
		#endif
		
        return(fIsError);
    }
	else 
		#ifdef USE_TX
			SendDebugMessage("> Bulk Erase Completed\n");
		#endif
	
    return (PASS);
}
	
/* ========================================================================= */
// ProgramFlashProcedure()
// This function programs the entire flash. Returns "PASS" if succeeds. 
/* ========================================================================= */
signed char ProgramFlashProcedure(void)
{
	/* Program Flash blocks with predetermined data. In the final application
    this data should come from the HEX output of PSoC Designer.*/
	
	#ifdef USE_TX
    	SendDebugMessage("> Programming in progress...");
	#endif
	
	iChecksumData = 0;     
    for (bBankCounter=0; bBankCounter<NUM_BANKS; bBankCounter++)
    {
        #ifdef MULTI_BANK		
        	/* Set the bank number */
        	SetBankNumber(bBankCounter);
        #endif        
		
        for (iBlockCounter=0; iBlockCounter<BLOCKS_PER_BANK; iBlockCounter++) 
		{
            LoadProgramData((unsigned char)iBlockCounter, bBankCounter);
        
			iChecksumData += iLoadTarget();
            
			#ifdef USE_TP
            	SetTPHigh();    
            #endif
            
			fIsError = fProgramTargetBlock(bBankCounter,(unsigned char)iBlockCounter);
			
			#ifdef USE_TP
	        	SetTPLow();      
	        #endif
			
			if (fIsError != PASS) 
			{
				#ifdef USE_TX
					SendDebugMessage("  Programming Failed\n");
				#endif 
				
                return(fIsError);				
            }	       
        }
    }
	
	#ifdef USE_TX
		SendDebugMessage("  Programming Completed\n");
	#endif
	
	return (PASS);
}
	
/* ========================================================================= */
// VerifyFlashProcedure()
// This function verifies the entire flash content. Returns "PASS" if succeeds. 
/* ========================================================================= */
signed char VerifyFlashProcedure(void)
{
	/* Verify the data block-by-block after programming all of the blocks and
     before setting security. */
	
	#ifdef USE_TX
		SendDebugMessage("> Program Verification in Progress...");
	#endif
	
    for (bBankCounter=0; bBankCounter<NUM_BANKS; bBankCounter++)
    {
        #ifdef MULTI_BANK
        	// Set the bank number
        	SetBankNumber(bBankCounter);
        #endif        
		
        for (iBlockCounter=0; iBlockCounter<BLOCKS_PER_BANK; iBlockCounter++) 
		{
            LoadProgramData((unsigned char)iBlockCounter, bBankCounter);
        
			fIsError = fVerifyTargetBlock(bBankCounter,(unsigned char)iBlockCounter);
			
			if (fIsError != PASS) 
			{
				#ifdef USE_TX
					SendDebugMessage("  Verification Failed\n");
				#endif 
				
                return(fIsError);
            }
        }
    }
	
	#ifdef USE_TX
		SendDebugMessage("  Program Verified\n");
	#endif
	
	return (PASS);
}
	
/* ========================================================================= */
// ProgramSecurityBitsProcedure()
// This function programs the security bits. Returns "PASS" if succeeds. 
/* ========================================================================= */
signed char ProgramSecurityBitsProcedure(void)
{
    /* Program security data into target PSoC. In the final application this 
    data should come from the HEX output of PSoC Designer. */	
    for (bBankCounter=0; bBankCounter<NUM_BANKS; bBankCounter++)
    {
        #ifdef MULTI_BANK
        	// Set the bank number
        	SetBankNumber(bBankCounter);
        #endif        

        /* Load one bank of security data from hex file into buffer */
		fIsError = LoadSecurityData(bBankCounter);
		
        if (fIsError != PASS) 
		{
			#ifdef USE_TX
				SendDebugMessage("> Security Data Loading Failed\n");
			#endif
			
            return(fIsError);
        }
		
        /* Secure one bank of the target flash */
		fIsError = fSecureTargetFlash();
		
        if (fIsError != PASS) 
		{
			#ifdef USE_TX
				SendDebugMessage("> Programming Security Data Failed\n");
			#endif
			
            return(fIsError);
        }
    }
	
	#ifdef USE_TX
		SendDebugMessage("> Program Security Settings Stored\n");
	#endif
	
	return (PASS);
}
	
/* ========================================================================= */
// VerifySecurityBitsProcedure()
// This function verifies the security bits. Returns "PASS" if succeeds. 
/* ========================================================================= */
signed char VerifySecurityBitsProcedure(void)
{
	#ifdef USE_TX
		SendDebugMessage("> Reading Program Security Settings\n");
	#endif
	
	for (bBankCounter=0; bBankCounter<NUM_BANKS; bBankCounter++)
    {
        #ifdef MULTI_BANK
	        // Set the bank number
	        SetBankNumber(bBankCounter);
        #endif  
		
		/* Read and verify security bits of one bank of the target flash */
		fIsError = fVerifySecurity();
		
        if (fIsError != PASS) 
		{
			#ifdef USE_TX
				SendDebugMessage("  Program Security Data Incorrect\n");
			#endif
			
            return(fIsError);
		}
		
		#ifdef USE_TX
			/* Print the security values */
			DisplaySecurityValues(abTargetDataOUT,SECURITY_BYTES_PER_BANK); 
		#endif
	}
	
	#ifdef USE_TX
		SendDebugMessage("\n  Program Security Settings Verified\n");
	#endif
	
	return (PASS);
}

/* ========================================================================= */
// VerifyChecksumProcedure()
// This function verifies the checksum. Returns "PASS" if succeeds. 
/* ========================================================================= */
signed char VerifyChecksumProcedure(void)
{
	/* Run the Target-Checksum Verification, and proceed according to result.
    Checksum only valid if every Flash block is programed */
	
    #ifdef USE_TP
   		SetTPHigh();    // Only used of Test Points are enabled
    #endif
    
	iChecksumTarget = 0;
    
	for (bBankCounter=0; bBankCounter<NUM_BANKS; bBankCounter++)
    {
        #ifdef MULTI_BANK
	        /* Set the bank number */
	        SetBankNumber(bBankCounter);
        #endif        
		
		fIsError = fAccTargetBankChecksum(&iChecksumTarget);
		
        if (fIsError) 
		{	
			#ifdef USE_TX
				SendDebugMessage("> Checksum Read Failed\n");
			#endif
			
            return(fIsError);
        }
    }  
	
    if (iChecksumTarget != iChecksumData) 
	{	
		#ifdef USE_TX
			SendDebugMessage("> Checksum Failed\n");
		#endif
		
        return(VERIFY_ERROR);
	}
	
	#ifdef USE_TX
	    SendDebugMessage("> Checksum Verified\n");
	#endif
	
	return (PASS);
}
	
/* ========================================================================= */
// ErrorTrap()
// This code should be replaced with appropriate function. This function is being
// called if there is a error in any of the stages of programming.
/* ========================================================================= */
void ErrorTrap(void)
{
	#ifndef RESET_MODE
	    // Set all pins to highZ to avoid back powering the PSoC through the GPIO
	    // protection diodes.
	    SetSCLKHiZ();   
	    SetSDATAHiZ();
	
	    // Turn off the target
	    RemoveTargetVDD();
	#endif
	
    while (1)
	{
	}
}

#endif  //(PROJECT_REV_)
// end of file ProgrammingSteps.c

