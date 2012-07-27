/*
  SpriteCom.h - An Energia library for transmitting data using the CC430 series of devices
  
  by Zac Manchester

*/

#ifndef SpriteCom_h
#define SpriteCom_h

class SpriteCom {
  public:
	
	// Constructor - optionally supply radio register settings
	SpriteCom();
	SpriteCom(CC1101Settings *settings);
	
	// Set the transmitter power level. Default is -30 dBm.
	void setPower(int tx_power_dbm);

    // Right now this Transmits the given byte array exactly
	// In the future it should apply FEC before transmitting
    void transmit(char bytes[], unsigned int length);

	// Initialize the radio - must be called before transmitting
    void radioInit();

	// Put the radio in low power mode - call after transmitting
	void radioSleep();
	
  private:
	CC1101Settings *m_settings;
	char m_power;
	
};

#endif //SpriteCom_h