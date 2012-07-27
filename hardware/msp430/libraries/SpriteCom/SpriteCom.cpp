/*
  SpriteCom.cpp - An Energia library for transmitting data using the CC430 series of devices
  
  by Zac Manchester

*/

#include "utility/MspRadio.h"

SpriteCom::SpriteCom() {
	
	m_power = 0x03;
	
	*m_settings = (CC1101Settings){
	    0x0E,   // FSCTRL1
	    0x00,   // FSCTRL0
	    0x22,   // FREQ2
	    0xB1,   // FREQ1
	    0x3B,   // FREQ0
	    0x0A,   // MDMCFG4
	    0xF8,   // MDMCFG3
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
	    0x22,   // PKTCTRL0  0x02 = infinite packet length, 0x00 = Fixed Packet Size, 0x40 = whitening, 0x20 = PN9
	    0x00,   // ADDR      Device address.
	    0xFF    // PKTLEN    Packet Length (Bytes)
	};
}

SpriteCom::SpriteCom(CC1101Settings *settings) {
	
	m_power = 0x03;
	m_settings = settings;
}

// Set the output power of the transmitter.
SpriteCom::setPower(int tx_power_dbm) {
	
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
			m_power = 0x03; // -30 dBm
}

void SpriteCom::transmit(char bytes[], unsigned int length) {
	
	// For now this just transmits PRN bits
	Radio.strobe(RF_STX);
}

void SpriteCom::radioInit() {
	
	Radio.reset();

	Radio.writeConfiguration(m_Settings);  // Write settings to configuratoin registers
	
	Radio.writePATable(m_power);
}

void SpriteCom::radioSleep() {
	
}