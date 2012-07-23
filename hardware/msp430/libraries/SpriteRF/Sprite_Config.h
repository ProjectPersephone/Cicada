#ifndef SPRITE_CONFIG_H_
#define SPRITE_CONFIG_H_

/* This header contains all of the various configurations for the Sprite transmitter.
 * Select a configuration by uncommenting the appropriate #define(s).
 */
 
/*------------------------------------------- Frequency --------------------------------------------------*/

/* Choose the transmitter frequency.  Only uncomment one at a time. */

//#define FREQ_435p000_MHZ
//#define FREQ_437p500_MHZ
//#define FREQ_437p525_MHZ
#define FREQ_902p000_MHZ

/*------------------------------------------- Modulation -------------------------------------------------*/

/* MSK modulation at the specified bitrate (kbps), 902 MHz carrier.
 * The data to be transmitted is specified in the PRN_Data.h header file or
 * by using the BUILT_IN_PN9 define below.
 */
 
//#define MSK_35
#define MSK_50
//#define MSK_100
//#define MSK_250
//#define MSK_500

/* Use ASK modulation at a bit rate of 250 kbps.
 * This will allow the transmission to be verified on an oscilloscope.
 * The data to be transmitted is specified in the PRN_Data.h header file.
 */
 
//#define ASK_250


/* Use the transmitter's built in 9 bit PRN generator.
 * This will probably save power as it lets the MCU power down durring transmission.
 * The output will be a PRN sequence of 2^9-1 chips in length.
 */
 
#define BUILT_IN_PN9


/*------------------------  Don't edit register settings below this line. ------------------------*/

#ifdef FREQ_435p000_MHZ
#define SMARTRF_SETTING_FREQ2      0x10
#define SMARTRF_SETTING_FREQ1      0xBB
#define SMARTRF_SETTING_FREQ0      0x13
#endif

#ifdef FREQ_437p500_MHZ
#define SMARTRF_SETTING_FREQ2      0x10
#define SMARTRF_SETTING_FREQ1      0xD3
#define SMARTRF_SETTING_FREQ0      0xB1
#endif

#ifdef FREQ_437p525_MHZ
#define SMARTRF_SETTING_FREQ2      0x10
#define SMARTRF_SETTING_FREQ1      0xD3
#define SMARTRF_SETTING_FREQ0      0xF0
#endif

#ifdef FREQ_902p000_MHZ
#define SMARTRF_SETTING_FREQ2      0x22
#define SMARTRF_SETTING_FREQ1      0xB1
#define SMARTRF_SETTING_FREQ0      0x3B
#endif

#ifdef MSK_500
#define SMARTRF_SETTING_MDMCFG4    0x0E // Modem config - channel bandwidth/baud rate
#define SMARTRF_SETTING_MDMCFG3    0x3B // Modem config - baud rate
#endif

#ifdef MSK_250
#define SMARTRF_SETTING_MDMCFG4    0x0D
#define SMARTRF_SETTING_MDMCFG3    0x3B
#endif

#ifdef MSK_100
#define SMARTRF_SETTING_MDMCFG4    0x0B
#define SMARTRF_SETTING_MDMCFG3    0xF8
#endif

#ifdef MSK_50
#define SMARTRF_SETTING_MDMCFG4    0x0A
#define SMARTRF_SETTING_MDMCFG3    0xF8
#endif

#ifdef MSK_35
#define SMARTRF_SETTING_MDMCFG4    0x0A
#define SMARTRF_SETTING_MDMCFG3    0x61
#endif

//This stuff gets defined for any MSK setup
#define SMARTRF_SETTING_FSCTRL1    0x0E // RX IF freq.
#define SMARTRF_SETTING_FSCTRL0    0x00 // Freq. Offset
#define SMARTRF_SETTING_MDMCFG2    0x70 // (0x70 = MSK, no sync)0x13 // Modem config (0x03 = 32 bit sync word, 0x10 = GFSK)
#define SMARTRF_SETTING_MDMCFG1    0x02 // Modem config (0x20 = 4 preamble bytes)
#define SMARTRF_SETTING_MDMCFG0    0xF8 // Modem config - channel spacing
#define SMARTRF_SETTING_CHANNR     0x00 // Channel number (sets the frequency)
#define SMARTRF_SETTING_DEVIATN    0x07 // Specifies fraction of symbol period over which phase change occurs (1/8 - 8/8) - 0x07 = standard MSK
#define SMARTRF_SETTING_FREND1     0xB6 // Front end RX config
#define SMARTRF_SETTING_FREND0     0x10 // Front end TX config 
#define SMARTRF_SETTING_MCSM0      0x18 // Configures callibration of oscillator
#define SMARTRF_SETTING_FOCCFG     0x1D // Freq. Offset callibration
#define SMARTRF_SETTING_BSCFG      0x1C // Bit synchronization config
#define SMARTRF_SETTING_AGCCTRL2   0xC7
#define SMARTRF_SETTING_AGCCTRL1   0x00
#define SMARTRF_SETTING_AGCCTRL0   0xB0
#define SMARTRF_SETTING_FSCAL3     0xEA
#define SMARTRF_SETTING_FSCAL2     0x2A
#define SMARTRF_SETTING_FSCAL1     0x00
#define SMARTRF_SETTING_FSCAL0     0x1F
#define SMARTRF_SETTING_FSTEST     0x59
#define SMARTRF_SETTING_TEST2      0x88
#define SMARTRF_SETTING_TEST1      0x31
#define SMARTRF_SETTING_TEST0      0x09
#define SMARTRF_SETTING_FIFOTHR    0x07 // 33/32 bytes TX/RX
#define SMARTRF_SETTING_IOCFG2     0x29
#define SMARTRF_SETTING_IOCFG0     0x06
#define SMARTRF_SETTING_PKTCTRL1   0x00 // Packet Automation (0x04 = append status bytes)
#define SMARTRF_SETTING_PKTCTRL0   0x02 // 0x02 = infinite packet length, 0x00 = Fixed Packet Size, 0x40 = whitening, 0x20 = PN9
#define SMARTRF_SETTING_ADDR       0x00 // Device address (doesn't do anything)
#define SMARTRF_SETTING_PKTLEN     0xFF // Packet Length (Bytes)

#ifdef ASK_250
#define SMARTRF_SETTING_FSCTRL1    0x0C
#define SMARTRF_SETTING_FSCTRL0    0x00
#define SMARTRF_SETTING_FREQ2      0x22
#define SMARTRF_SETTING_FREQ1      0xB1
#define SMARTRF_SETTING_FREQ0      0x3B
#define SMARTRF_SETTING_MDMCFG4    0xFD
#define SMARTRF_SETTING_MDMCFG3    0x3B
#define SMARTRF_SETTING_MDMCFG2    0x30 // (0x30 = ASK, no sync)0x13 // Modem config
#define SMARTRF_SETTING_MDMCFG1    0x02 // Modem config (0x20 = 4 preamble bytes)
#define SMARTRF_SETTING_MDMCFG0    0xF8
#define SMARTRF_SETTING_CHANNR     0x00
#define SMARTRF_SETTING_DEVIATN    0x15
#define SMARTRF_SETTING_FREND1     0xB6
#define SMARTRF_SETTING_FREND0     0x11
#define SMARTRF_SETTING_MCSM0      0x18
#define SMARTRF_SETTING_FOCCFG     0x1C
#define SMARTRF_SETTING_BSCFG      0x1C
#define SMARTRF_SETTING_AGCCTRL2   0xC7
#define SMARTRF_SETTING_AGCCTRL1   0x00
#define SMARTRF_SETTING_AGCCTRL0   0xB0
#define SMARTRF_SETTING_FSCAL3     0xEA
#define SMARTRF_SETTING_FSCAL2     0x2A
#define SMARTRF_SETTING_FSCAL1     0x00
#define SMARTRF_SETTING_FSCAL0     0x1F
#define SMARTRF_SETTING_FSTEST     0x59
#define SMARTRF_SETTING_TEST2      0x88
#define SMARTRF_SETTING_TEST1      0x31
#define SMARTRF_SETTING_TEST0      0x09
#define SMARTRF_SETTING_FIFOTHR    0x07 // 33/32 bytes TX/RX
#define SMARTRF_SETTING_IOCFG2     0x29
#define SMARTRF_SETTING_IOCFG0     0x06
#define SMARTRF_SETTING_PKTCTRL1   0x00 // Packet Automation (0x04 = append status bytes)
#define SMARTRF_SETTING_PKTCTRL0   0x02 // 0x02 = infinite packet length, 0x00 = Fixed Packet Size, 0x40 = whitening, 0x20 = PN9
#define SMARTRF_SETTING_ADDR       0x00 // Device address (doesn't do anything)
#define SMARTRF_SETTING_PKTLEN     0xFF // Packet Length (Bytes)
#endif

#ifdef BUILT_IN_PN9
#undef SMARTRF_SETTING_PKTCTRL0
#define SMARTRF_SETTING_PKTCTRL0   0x22 // 0x02 = infinite packet length, 0x00 = Fixed Packet Size, 0x40 = whitening, 0x20 = PN9
#endif

// Stuff added from CC430 Example
#define SMARTRF_SETTING_AGCTEST    0x88
#define SMARTRF_SETTING_IOCFG1     0x1E
#define SMARTRF_SETTING_MCSM1      0x3C
#define SMARTRF_SETTING_MCSM2      0x07
#define SMARTRF_SETTING_PTEST      0x7F
#define SMARTRF_SETTING_WOREVT1    0x87   
#define SMARTRF_SETTING_WOREVT0    0x6B   
#define SMARTRF_SETTING_WORCTL     0xF8

#endif /*SPRITE_CONFIG_H_*/
