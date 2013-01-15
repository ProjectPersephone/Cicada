/*
  SpriteRadio.cpp - An Energia library for transmitting data using the CC430 series of devices
  
  by Zac Manchester

*/

#include "SpriteRadio.h"
#include "utility/CC430Radio.h"
#include "cc430f5137.h"
#include "Energia.h"

SpriteRadio::SpriteRadio(unsigned char prn0[], unsigned char prn1[]) {
	
	m_power = 0xC3;
	
	m_settings = (CC1101Settings){
	    0x0E,   // FSCTRL1
		0x00,   // FSCTRL0
		0x10,   // FREQ2
		0xD1,   // FREQ1
		0x21,   // FREQ0
		0x0B,   // MDMCFG4
		0x43,   // MDMCFG3
		0x70,   // MDMCFG2
		0x02,   // MDMCFG1
		0xF8,   // MDMCFG0
		0x00,   // CHANNR
		0x07,   // DEVIATN
		0xB6,   // FREND1
		0x10,   // FREND0
		0x18,   // MCSM0
		0x1D,   // FOCCFG
		0x1C,   // BSCFG
		0xC7,   // AGCCTRL2
		0x00,   // AGCCTRL1
		0xB0,   // AGCCTRL0
		0xEA,   // FSCAL3
		0x2A,   // FSCAL2
		0x00,   // FSCAL1
		0x1F,   // FSCAL0
		0x59,   // FSTEST
		0x88,   // TEST2
		0x31,   // TEST1
		0x09,   // TEST0
		0x07,   // FIFOTHR
		0x29,   // IOCFG2
		0x06,   // IOCFG0
		0x00,   // PKTCTRL1  Packet Automation (0x04 = append status bytes)
		0x02,   // PKTCTRL0  0x02 = infinite packet length, 0x00 = Fixed Packet Size, 0x40 = whitening, 0x20 = PN9
		0x00,   // ADDR      Device address.
		0xFF    // PKTLEN    Packet Length (Bytes)
	};

	m_prn0 = prn0;
	m_prn1 = prn1;
}

SpriteRadio::SpriteRadio(unsigned char prn0[], unsigned char prn1[], CC1101Settings settings) {
	
	m_power = 0xC3;
	m_settings = settings;
	m_prn0 = prn0;
	m_prn1 = prn1;
}

// Set the output power of the transmitter.
void SpriteRadio::setPower(int tx_power_dbm) {
	
	// These values are from TI Design Note DN013 and are calibrated for operation at 434 MHz.
	switch (tx_power_dbm) {
		case 10:
			m_power = 0xC0;
			break;
		case 9:
			m_power = 0xC3;
			break;
		case 8:
			m_power = 0xC6;
			break;
		case 7:
			m_power = 0xC9;
			break;
		case 6:
			m_power = 0x82;
			break;
		case 5:
			m_power = 0x84;
			break;
		case 4:
			m_power = 0x87;
			break;
		case 3:
			m_power = 0x8A;
			break;
		case 2:
			m_power = 0x8C;
			break;
		case 1:
			m_power = 0x50;
			break;
		case 0:
			m_power = 0x60;
			break;
		case -1:
			m_power = 0x52;
			break;
		case -2:
			m_power = 0x63;
			break;
		case -3:
			m_power = 0x65;
			break;
		case -4:
			m_power = 0x57;
			break;
		case -5:
			m_power = 0x69;
			break;
		case -6:
			m_power = 0x6A;
			break;
		case -7:
			m_power = 0x6C;
			break;
		case -8:
			m_power = 0x6D;
			break;
		case -9:
			m_power = 0x6E;
			break;
		case -10:
			m_power = 0x34;
			break;
		case -11:
			m_power = 0x25;
			break;
		case -12:
			m_power = 0x26;
			break;
		case -13:
		case -14:
		case -15:
			m_power = 0x1D;
			break;
		case -16:
		case -17:
		case -18:
			m_power = 0x1A;
			break;
		case -19:
		case -20:
			m_power = 0x0E;
			break;
		case -21:
		case -22:
		case -23:
			m_power = 0x0A;
			break;
		case -24:
		case -25:
		case -26:
			m_power = 0x07;
			break;
		case -27:
		case -28:
		case -29:	
		case -30:
			m_power = 0x03;
			break;
		default:
			m_power = 0xC3; // 10 dBm
		}
}

void SpriteRadio::transmit(char bytes[], unsigned int length)
{
	//Transmit Preamble
	beginRawTransmit(m_prn1,PRN_LENGTH);
	continueRawTransmit(0,PRN_LENGTH);
	continueRawTransmit(m_prn1,PRN_LENGTH);
	continueRawTransmit(0,PRN_LENGTH);
	continueRawTransmit(m_prn1,PRN_LENGTH);
	continueRawTransmit(0,PRN_LENGTH);
	continueRawTransmit(m_prn1,PRN_LENGTH);
	continueRawTransmit(0,PRN_LENGTH);
	continueRawTransmit(m_prn1,PRN_LENGTH);
	continueRawTransmit(m_prn0,PRN_LENGTH);

	for(unsigned int k = 0; k < length; ++k)
	{
		bytes[k] & BIT0 ? continueRawTransmit(m_prn1,PRN_LENGTH) : continueRawTransmit(m_prn0,PRN_LENGTH);
		bytes[k] & BIT1 ? continueRawTransmit(m_prn1,PRN_LENGTH) : continueRawTransmit(m_prn0,PRN_LENGTH);
		bytes[k] & BIT2 ? continueRawTransmit(m_prn1,PRN_LENGTH) : continueRawTransmit(m_prn0,PRN_LENGTH);
		bytes[k] & BIT3 ? continueRawTransmit(m_prn1,PRN_LENGTH) : continueRawTransmit(m_prn0,PRN_LENGTH);
		bytes[k] & BIT4 ? continueRawTransmit(m_prn1,PRN_LENGTH) : continueRawTransmit(m_prn0,PRN_LENGTH);
		bytes[k] & BIT5 ? continueRawTransmit(m_prn1,PRN_LENGTH) : continueRawTransmit(m_prn0,PRN_LENGTH);
		bytes[k] & BIT6 ? continueRawTransmit(m_prn1,PRN_LENGTH) : continueRawTransmit(m_prn0,PRN_LENGTH);
		bytes[k] & BIT7 ? continueRawTransmit(m_prn1,PRN_LENGTH) : continueRawTransmit(m_prn0,PRN_LENGTH);
	}

	//Transmit Postamble
	continueRawTransmit(0,PRN_LENGTH);
	continueRawTransmit(m_prn0,PRN_LENGTH);
	continueRawTransmit(0,PRN_LENGTH);
	continueRawTransmit(m_prn0,PRN_LENGTH);
	continueRawTransmit(0,PRN_LENGTH);
	continueRawTransmit(m_prn0,PRN_LENGTH);
	continueRawTransmit(0,PRN_LENGTH);
	continueRawTransmit(m_prn0,PRN_LENGTH);

	endRawTransmit();
}

void SpriteRadio::rawTransmit(unsigned char bytes[], unsigned int length) {
	
	beginRawTransmit(bytes, length);
	endRawTransmit();
}

void SpriteRadio::beginRawTransmit(unsigned char bytes[], unsigned int length) {
	char status;

	//Wait for radio to be in idle state
	status = Radio.strobe(RF_SIDLE);
	while (status & 0xF0)
	{
		status = Radio.strobe(RF_SNOP);
	}
	
	//Clear TX FIFO
	status = Radio.strobe(RF_SFTX);

	if(length <= 64)
	{
		Radio.writeTXBuffer(bytes, length); //Write bytes to transmit buffer
		status = Radio.strobe(RF_STX);  //Turn on transmitter
	}
	else
	{
		unsigned char bytes_free, bytes_to_write;
	  	unsigned int bytes_to_go, counter;
		
		Radio.writeTXBuffer(bytes, 64); //Write first 64 bytes to transmit buffer
		bytes_to_go = length - 64;
		counter = 64;

		status = Radio.strobe(RF_STX);  //Turn on transmitter

		//Wait for oscillator to stabilize
		while (status & 0xC0)
		{
			status = Radio.strobe(RF_SNOP);
		}

		while(bytes_to_go)
		{
			delay(1); //Wait for some bytes to be transmitted

			bytes_free = Radio.strobe(RF_SNOP) & 0x0F;
			bytes_to_write = bytes_free < bytes_to_go ? bytes_free : bytes_to_go;

			Radio.writeTXBuffer(bytes+counter, bytes_to_write);
			bytes_to_go -= bytes_to_write;
			counter += bytes_to_write;
		}
	}
}

void SpriteRadio::continueRawTransmit(unsigned char bytes[], unsigned int length) {

	unsigned char bytes_free, bytes_to_write;
	unsigned int bytes_to_go, counter;
		
	bytes_to_go = length;
	counter = 0;

	if(bytes)
	{
		while(bytes_to_go)
		{
			delay(1); //Wait for some bytes to be transmitted

			bytes_free = Radio.strobe(RF_SNOP) & 0x0F;
			bytes_to_write = bytes_free < bytes_to_go ? bytes_free : bytes_to_go;

			Radio.writeTXBuffer(bytes+counter, bytes_to_write);
			bytes_to_go -= bytes_to_write;
			counter += bytes_to_write;
		}
	}
	else
	{
		while(bytes_to_go)
		{
			delay(1); //Wait for some bytes to be transmitted

			bytes_free = Radio.strobe(RF_SNOP) & 0x0F;
			bytes_to_write = bytes_free < bytes_to_go ? bytes_free : bytes_to_go;

			Radio.writeTXBufferZeros(bytes_to_write);
			bytes_to_go -= bytes_to_write;
			counter += bytes_to_write;
		}
	}

	

	return;
}

void SpriteRadio::endRawTransmit() {

	char status = Radio.strobe(RF_SNOP);

	//Wait for transmission to finish
	while(status != 0x7F)
	{
		status = Radio.strobe(RF_SNOP);
	}
	Radio.strobe(RF_SIDLE); //Put radio back in idle mode
	return;
}

void SpriteRadio::txInit() {
	
	char status;

	Radio.reset();
	Radio.writeConfiguration(&m_settings);  // Write settings to configuration registers
	Radio.writePATable(m_power);

	//Put radio into idle state
	status = Radio.strobe(RF_SIDLE);
	while (status & 0xF0)
	{
	  status = Radio.strobe(RF_SNOP);
	}
}

void SpriteRadio::sleep() {
	
	Radio.strobe(RF_SIDLE); //Put radio back in idle mode
}
