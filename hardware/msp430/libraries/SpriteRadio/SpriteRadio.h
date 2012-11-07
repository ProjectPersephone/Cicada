/*
  SpriteRadio.h - An Energia library for transmitting data using the CC430 series of devices
  
  by Zac Manchester

*/

#ifndef SpriteRadio_h
#define SpriteRadio_h

#include "utility/CC430Radio.h"

class SpriteRadio {
  public:
	
	// Constructor - optionally supply radio register settings
	SpriteRadio();
	SpriteRadio(CC1101Settings *settings);
	
	// Set the transmitter power level. Default is 10 dBm.
	void setPower(int tx_power_dbm);

    // Right now this Transmits the given byte array exactly
	// In the future it should apply FEC before transmitting
    void transmit(unsigned char bytes[], unsigned int length);

	// Initialize the radio - must be called before transmitting
    void txInit();

	// Put the radio in low power mode - call after transmitting
	void sleep();
	
  private:
	CC1101Settings *m_settings;
	char m_power;
	
};

#endif //SpriteRadio_h