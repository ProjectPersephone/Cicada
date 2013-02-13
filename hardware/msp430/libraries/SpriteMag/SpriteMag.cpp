/*
  SpriteRadio.cpp - An Energia library for the HMC5883L

*/

#include "SpriteMag.h"
#include "utility/HMC5883L_Config.h"
#include "TI_USCI_I2C_master.h"

SpriteMag::SpriteMag() {
	m_biasx = 0;
	m_biasy = 0;
	m_biasz = 0;
}

SpriteMag::SpriteMag(MagneticField bias) {
	m_biasx = bias.x;
	m_biasy = bias.y;
	m_biasz = bias.z;
}

void SpriteMag::init() {

	m_sendBufer[0] = 0x00;
	m_sendBufer[1] = (MAG_SAMPLES_AVE|MAG_DATA_RATE|MAG_MEAS_MODE);

	TI_USCI_I2C_transmitinit(MAG_ADDRESS, I2C_PRESCALE);
	TI_USCI_I2C_transmit(m_sendBufer, 2);

	m_sendBufer[0] = 0x01; m_sendBufer[1] = MAG_GAIN;
	TI_USCI_I2C_transmit(m_sendBufer, 2);

	m_sendBufer[0] = 0x02; m_sendBufer[1] = MAG_OPER_MODE;
	TI_USCI_I2C_transmit(m_sendBufer, 2);
}

MagneticField SpriteMag::read() {

	m_sendBufer[0] = 0x03;

	TI_USCI_I2C_transmitinit(MAG_ADDRESS, I2C_PRESCALE);
	TI_USCI_I2C_transmit(m_sendBufer, 1);

	TI_USCI_I2C_receiveinit(MAG_ADDRESS, I2C_PRESCALE);
	TI_USCI_I2C_receive(m_receiveBuffer, 6);

	MagneticField b;
	b.x = (m_receiveBuffer[0] << 8) | m_receiveBuffer[1]; //+ m_biasx;
	b.y = (m_receiveBuffer[2] << 8) | m_receiveBuffer[3]; //+ m_biasy;
	b.z = (m_receiveBuffer[4] << 8) | m_receiveBuffer[5]; //+ m_biasz;

	return b;
}
