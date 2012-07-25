/*
  SpriteRF.h - A basic Energia library for interfacing with the CC1101 radio core on the CC430 series of devices
  
  by Zac Manchester

*/

#ifndef SpriteRF_h
#define SpriteRF_h

// CC1101 configuration registers.  See data sheet for details: http://www.ti.com/lit/ds/symlink/cc1101.pdf
typedef struct {
	unsigned char fsctrl1;   // Frequency synthesizer control.
    unsigned char fsctrl0;   // Frequency synthesizer control.
    unsigned char freq2;     // Frequency control word, high byte.
    unsigned char freq1;     // Frequency control word, middle byte.
    unsigned char freq0;     // Frequency control word, low byte.
    unsigned char mdmcfg4;   // Modem configuration.
    unsigned char mdmcfg3;   // Modem configuration.
    unsigned char mdmcfg2;   // Modem configuration.
    unsigned char mdmcfg1;   // Modem configuration.
    unsigned char mdmcfg0;   // Modem configuration.
    unsigned char channr;    // Channel number.
    unsigned char deviatn;   // Modem deviation setting (when FSK modulation is enabled).
    unsigned char frend1;    // Front end RX configuration.
    unsigned char frend0;    // Front end RX configuration.
    unsigned char mcsm0;     // Main Radio Control State Machine configuration.
    unsigned char foccfg;    // Frequency Offset Compensation Configuration.
    unsigned char bscfg;     // Bit synchronization Configuration.
    unsigned char agcctrl2;  // AGC control.
    unsigned char agcctrl1;  // AGC control.
    unsigned char agcctrl0;  // AGC control.
    unsigned char fscal3;    // Frequency synthesizer calibration.
    unsigned char fscal2;    // Frequency synthesizer calibration.
    unsigned char fscal1;    // Frequency synthesizer calibration.
    unsigned char fscal0;    // Frequency synthesizer calibration.
    unsigned char fstest;    // Frequency synthesizer calibration control
    unsigned char test2;     // Various test settings.
    unsigned char test1;     // Various test settings.
    unsigned char test0;     // Various test settings.
    unsigned char fifothr;   // RXFIFO and TXFIFO thresholds.
    unsigned char iocfg2;    // GDO2 output pin configuration
    unsigned char iocfg0;    // GDO0 output pin configuration
    unsigned char pktctrl1;  // Packet automation control.
    unsigned char pktctrl0;  // Packet automation control.
    unsigned char addr;      // Device address.
    unsigned char pktlen;    // Packet length.
} CC1101Settings;

class SpriteRF {
  public:

    // Initialize the radio - must be called before transmitting
    void init(CC1101Settings *settings, int tx_power_dbm);

    // Transmit the given byte array exactly (no FEC or packet handling stuff)
    void transmit(char bytes[], unsigned int length);

	// Put the radio in low power mode - call after transmitting
	void sleep();
	
  private:
	
	// Send a command to the radio - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
	unsigned char strobe(unsigned char command);
	
	// Read a single byte from the radio register - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
	unsigned char readSingleReg(unsigned char address);
	
	// Write a single byte to the radio register - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
	void writeSingleReg(unsigned char address, unsigned char value);
	
	// Write the RF configuration settings to the radio
	void writeConfiguration(CC1101Settings *settings);
	
	// Write the RF output power setting - adapted from TI example code: http://www.ti.com/lit/an/slaa465b/slaa465b.pdf
	void writePATable(unsigned char value)
	
};

extern SpriteRF Radio;

#endif //SpriteRF_h
