/*
  MspRadio.h - A low-level interface library for the CC1101 radio core in the CC430 series of devices.

  Adapted from the CC430 RF Examples from TI: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
  
  by Zac Manchester

*/

#include "cc430x513x.h"
#include "MspRadio.h"

static void __inline__ delayClockCycles(register unsigned int n);

// Send a command to the radio - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
unsigned char MspRadio::strobe(unsigned char command)
{
	unsigned char status_byte = 0;
	unsigned int  gdo_state;
	
	// Check for valid strobe command 
	if((command == 0xBD) || ((command >= RF_SRES) && (command <= RF_SNOP)))
	{
    	// Clear the Status read flag 
    	RF1AIFCTL1 &= ~(RFSTATIFG);    
    
    	// Wait for radio to be ready for next instruction
    	while( !(RF1AIFCTL1 & RFINSTRIFG));
    
    	// Write the strobe instruction
    	if ((command > RF_SRES) && (command < RF_SNOP))
    	{
      		gdo_state = readRegister(IOCFG2);    // buffer IOCFG2 state
      		writeRegister(IOCFG2, 0x29);         // chip-ready to GDO2
      
      		RF1AINSTRB = command; 
      		if ( (RF1AIN&0x04)== 0x04 )           // chip at sleep mode
      		{
        		if ( (command == RF_SXOFF) || (command == RF_SPWD) || (command == RF_SWOR) ) { }
        		else  	
        		{
          			while ((RF1AIN&0x04)== 0x04);     // chip-ready ?
          			delayClockCycles(6480); // Delay for ~810usec at 8MHz CPU clock, see erratum RF1A7
        		}
      		}
      		writeRegister(IOCFG2, gdo_state);    // restore IOCFG2 setting
    
      		while( !(RF1AIFCTL1 & RFSTATIFG) );
    	}
		else		                    // chip active mode (SRES)
    	{	
      		RF1AINSTRB = command; 	   
    	}
		status_byte = RF1ASTATB;
	}
	return status_byte;
}

// Reset the radio core - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
void MspRadio::reset(void) {
	strobe(RF_SRES);  // Reset the radio core
	strobe(RF_SNOP);  // Reset the radio pointer
}

// Read a single byte from the radio register - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
unsigned char MspRadio::readRegister(unsigned char address) {
	
	unsigned char data_out;

	// Check for valid configuration register address, 0x3E refers to PATABLE 
	if ((address <= 0x2E) || (address == 0x3E))
	{
		// Send address + Instruction + 1 dummy byte (auto-read)
	    RF1AINSTR1B = (address | RF_SNGLREGRD);
	} 
	else
	{
		// Send address + Instruction + 1 dummy byte (auto-read)
		RF1AINSTR1B = (address | RF_STATREGRD);
	}

	while (!(RF1AIFCTL1 & RFDOUTIFG) );
	data_out = RF1ADOUTB;  // Read data and clear the RFDOUTIFG

	return data_out;
}

// Write a single byte to the radio register - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
void MspRadio::writeRegister(unsigned char address, unsigned char value) {
	
	while (!(RF1AIFCTL1 & RFINSTRIFG));  // Wait for the Radio to be ready for next instruction
	RF1AINSTRB = (address | RF_SNGLREGWR);	// Send address + Instruction

	RF1ADINB = value;  // Write data

	nop();
}

// Write the RF configuration settings to the radio - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
void MspRadio::writeConfiguration(CC1101Settings *settings) {
	writeRegister(FSCTRL1,  settings->fsctrl1);
    writeRegister(FSCTRL0,  settings->fsctrl0);
    writeRegister(FREQ2,    settings->freq2);
    writeRegister(FREQ1,    settings->freq1);
    writeRegister(FREQ0,    settings->freq0);
    writeRegister(MDMCFG4,  settings->mdmcfg4);
    writeRegister(MDMCFG3,  settings->mdmcfg3);
    writeRegister(MDMCFG2,  settings->mdmcfg2);
    writeRegister(MDMCFG1,  settings->mdmcfg1);
    writeRegister(MDMCFG0,  settings->mdmcfg0);
    writeRegister(CHANNR,   settings->channr);
    writeRegister(DEVIATN,  settings->deviatn);
    writeRegister(FREND1,   settings->frend1);
    writeRegister(FREND0,   settings->frend0);
    writeRegister(MCSM0 ,   settings->mcsm0);
    writeRegister(FOCCFG,   settings->foccfg);
    writeRegister(BSCFG,    settings->bscfg);
    writeRegister(AGCCTRL2, settings->agcctrl2);
    writeRegister(AGCCTRL1, settings->agcctrl1);
    writeRegister(AGCCTRL0, settings->agcctrl0);
    writeRegister(FSCAL3,   settings->fscal3);
    writeRegister(FSCAL2,   settings->fscal2);
    writeRegister(FSCAL1,   settings->fscal1);
    writeRegister(FSCAL0,   settings->fscal0);
    writeRegister(FSTEST,   settings->fstest);
    writeRegister(TEST2,    settings->test2);
    writeRegister(TEST1,    settings->test1);
    writeRegister(TEST0,    settings->test0);
    writeRegister(FIFOTHR,  settings->fifothr);
    writeRegister(IOCFG2,   settings->iocfg2);
    writeRegister(IOCFG0,   settings->iocfg0);
    writeRegister(PKTCTRL1, settings->pktctrl1);
    writeRegister(PKTCTRL0, settings->pktctrl0);
    writeRegister(ADDR,     settings->addr);
    writeRegister(PKTLEN,   settings->pktlen);
}

// Set radio output power registers - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
void MspRadio::writePATable(unsigned char value) {
	
	unsigned char valueRead = 0;
	while(valueRead != value)
  	{
    	/* Write the power output to the PA_TABLE and verify the write operation.  */
    	unsigned char i = 0; 

    	/* wait for radio to be ready for next instruction */
    	while( !(RF1AIFCTL1 & RFINSTRIFG));
    	RF1AINSTRW = (0x7E00 | value); // PA Table write (burst)
    
    	/* wait for radio to be ready for next instruction */
    	while( !(RF1AIFCTL1 & RFINSTRIFG));
      	RF1AINSTR1B = RF_PATABRD;
    
    	// Traverse PATABLE pointers to read 
    	for (i = 0; i < 7; i++)
    	{
      		while( !(RF1AIFCTL1 & RFDOUTIFG));
      		valueRead  = RF1ADOUT1B;     
    	}
    	while( !(RF1AIFCTL1 & RFDOUTIFG));
    	valueRead  = RF1ADOUTB;
	}
}

static void __inline__ delayClockCycles(register unsigned int n)
{
    __asm__ __volatile__ (
                "1: \n"
                " dec        %[n] \n"
                " jne        1b \n"
        : [n] "+r"(n));
}

MspRadio Radio;
