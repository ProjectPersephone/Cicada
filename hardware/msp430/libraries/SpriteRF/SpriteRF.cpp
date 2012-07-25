/*
  SpriteRF.cpp - A basic Energia library for interfacing with the CC1101 radio core on the CC430 series of devices
  
  by Zac Manchester

*/

#include "cc430x513x.h"
#include "SpriteRF.h"
#include "utility/hal_pmm.h"

void SpriteRF::init(CC1101Settings *settings, int tx_power_dbm) {
	
	// Increase PMMCOREV level to 2 in order to avoid low voltage error when the RF core is enabled
    SetVCore(2);

	strobe(RF_SRES);  // Reset the radio core
	strobe(RF_SNOP);  // Reset radio pointer
	
	// Write CC1101 configuration registers
	writeConfiguration(settings);
	
	// Set the output power of the transmitter.
	// These values are from TI Design Note DN013 and are calibrated for operation at 434 MHz.
	switch (tx_power_dbm) {
		case 10:
			writePATable(0xC0);
			break;
		case 9:
			writePATable(0xC3);
			break;
		case 8:
			writePATable(0xC6);
			break;
		case 7:
			writePATable(0xC9);
			break;
		case 6:
			writePATable(0x82);
			break;
		case 5:
			writePATable(0x84);
			break;
		case 4:
			writePATable(0x87);
			break;
		case 3:
			writePATable(0x8A);
			break;
		case 2:
			writePATable(0x8C);
			break;
		case 1:
			writePATable(0x50);
			break;
		case 0:
			writePATable(0x60);
			break;
		case -1:
			writePATable(0x52);
			break;
		case -2:
			writePATable(0x63);
			break;
		case -3:
			writePATable(0x65);
			break;
		case -4:
			writePATable(0x57);
			break;
		case -5:
			writePATable(0x69);
			break;
		case -6:
			writePATable(0x6A);
			break;
		case -7:
			writePATable(0x6C);
			break;
		case -8:
			writePATable(0x6D);
			break;
		case -9:
			writePATable(0x6E);
			break;
		case -10:
			writePATable(0x34);
			break;
		case -11:
			writePATable(0x25);
			break;
		case -12:
			writePATable(0x26);
			break;
		case -13:
		case -14:
		case -15:
			writePATable(0x1D);
			break;
		case -16:
		case -17:
		case -18:
			writePATable(0x1A);
			break;
		case -19:
		case -20:
			writePATable(0x0E);
			break;
		case -21:
		case -22:
		case -23:
			writePATable(0x0A);
			break;
		case -24:
		case -25:
		case -26:
			writePATable(0x07);
			break;
		case -27:
		case -28:
		case -29:	
		case -30:
			writePATable(0x03);
			break;
		default:
			writePATable(0x03); // -30 dBm
	}
}

void SpriteRF::transmit(char bytes[], unsigned int length) {
	
}

void SpriteRF::sleep() {
	
}

// Send a command to the radio - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
unsigned char SpriteRF::strobe(unsigned char command)
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
      		gdo_state = readSingleReg(IOCFG2);    // buffer IOCFG2 state
      		writeSingleReg(IOCFG2, 0x29);         // chip-ready to GDO2
      
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
      		writeSingleReg(IOCFG2, gdo_state);    // restore IOCFG2 setting
    
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

// Read a single byte from the radio register - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
unsigned char SpriteRF::readSingleReg(unsigned char address) {
	
	unsigned char data_out;

	// Check for valid configuration register address, 0x3E refers to PATABLE 
	if ((addr <= 0x2E) || (address == 0x3E))
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
void SpriteRF::writeSingleReg(unsigned char address, unsigned char value) {
	
	while (!(RF1AIFCTL1 & RFINSTRIFG));  // Wait for the Radio to be ready for next instruction
	RF1AINSTRB = (address | RF_SNGLREGWR);	// Send address + Instruction

	RF1ADINB = value;  // Write data

	nop();
}

void SpriteRF::writeConfiguration(CC1101Settings *settings) {
	writeSingleReg(FSCTRL1,  settings->fsctrl1);
    writeSingleReg(FSCTRL0,  settings->fsctrl0);
    writeSingleReg(FREQ2,    settings->freq2);
    writeSingleReg(FREQ1,    settings->freq1);
    writeSingleReg(FREQ0,    settings->freq0);
    writeSingleReg(MDMCFG4,  settings->mdmcfg4);
    writeSingleReg(MDMCFG3,  settings->mdmcfg3);
    writeSingleReg(MDMCFG2,  settings->mdmcfg2);
    writeSingleReg(MDMCFG1,  settings->mdmcfg1);
    writeSingleReg(MDMCFG0,  settings->mdmcfg0);
    writeSingleReg(CHANNR,   settings->channr);
    writeSingleReg(DEVIATN,  settings->deviatn);
    writeSingleReg(FREND1,   settings->frend1);
    writeSingleReg(FREND0,   settings->frend0);
    writeSingleReg(MCSM0 ,   settings->mcsm0);
    writeSingleReg(FOCCFG,   settings->foccfg);
    writeSingleReg(BSCFG,    settings->bscfg);
    writeSingleReg(AGCCTRL2, settings->agcctrl2);
    writeSingleReg(AGCCTRL1, settings->agcctrl1);
    writeSingleReg(AGCCTRL0, settings->agcctrl0);
    writeSingleReg(FSCAL3,   settings->fscal3);
    writeSingleReg(FSCAL2,   settings->fscal2);
    writeSingleReg(FSCAL1,   settings->fscal1);
    writeSingleReg(FSCAL0,   settings->fscal0);
    writeSingleReg(FSTEST,   settings->fstest);
    writeSingleReg(TEST2,    settings->test2);
    writeSingleReg(TEST1,    settings->test1);
    writeSingleReg(TEST0,    settings->test0);
    writeSingleReg(FIFOTHR,  settings->fifothr);
    writeSingleReg(IOCFG2,   settings->iocfg2);
    writeSingleReg(IOCFG0,   settings->iocfg0);
    writeSingleReg(PKTCTRL1, settings->pktctrl1);
    writeSingleReg(PKTCTRL0, settings->pktctrl0);
    writeSingleReg(ADDR,     settings->addr);
    writeSingleReg(PKTLEN,   settings->pktlen);
}

// Set radio output power registers - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
void SpriteRF::writePATable(unsigned char value) {
	
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

SpriteRF Radio;
