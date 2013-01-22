/*
  SpriteRadio.h - An Energia library for the HMC5993L

*/

#ifndef SpriteMag_h
#define SpriteMag_h

typedef struct MagneticField {
	int x;
	int y;
	int z;
} MagneticField;

class SpriteMag {
  public:
	// Constructor
	SpriteMag();
	SpriteMag(MagneticField bias);

	void init();

	MagneticField read();
	
  private:
  	unsigned char m_sendBufer[2];
  	unsigned char m_receiveBuffer[6];
	
	int m_biasx;
	int m_biasy;
	int m_biasz;

};

#endif //SpriteMag_h