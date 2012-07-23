#include "cc430x613x.h"
#include "cc430x613x_PMM.h"
#include "RF1A.h"
#include "SmartRF.h"

#include "Sprite_Config.h"
#include "PRN_Data.h"

unsigned int index;
unsigned int remaining;
unsigned char bytesToWrite;

void setClock();

void main( void )
{
    // Stop watchdog timer to prevent time out reset
    WDTCTL = WDTPW + WDTHOLD;

    // Increase PMMCOREV level to 2 in order to avoid low voltage error 
    // when the RF core is enabled
    SetVCore(2);
  
    ResetRadioCore();
  
    //Write SmartRF Config
    WriteBurstReg(IOCFG2, (unsigned char*)RF1A_REGISTER_CONFIG, CONF_REG_SIZE);
    
    //This sets the output power of the transmitter.
    //0x03 = -30 dBm, 0x0E = -20 dBm, 0x51 = 0 dBm, 0xC3 = +10 dBm, 0xC0 = +12 dBm
    WritePATable(0xC3);

#ifdef BUILT_IN_PN9

    //Send transmit command to radio
    Strobe(RF_STX);
        
    //Put the MCU in low power mode
    //__bis_SR_register( LPM4_bits );
    
#else

	//increase CPU frequency to 8MHz
	setClock();
	
	//Interrupt crap that came from the TI Example Code
	RF1AIES |= BIT9;                          
  	RF1AIFG &= ~BIT9;
  	
	//Load TX FIFO with first 64 bytes of PRNData
	WriteBurstReg(RF_TXFIFOWR, PRNData, 64);
	
	index = 64;
	remaining = DATA_SIZE - 64;
	
	//Send transmit command to radio
    Strobe(RF_STX);

    while (1)
    {  	
    	bytesToWrite = 64 - (ReadSingleReg(TXBYTES) & 0x3F);
    	
    	if (remaining > bytesToWrite)
    	{
    		WriteBurstReg(RF_TXFIFOWR, &PRNData[index], bytesToWrite);
    		index = index + bytesToWrite;
    		remaining = remaining - bytesToWrite;
    	}
    	else
    	{
    		if (remaining == 0)
    		{
    			WriteBurstReg(RF_TXFIFOWR, &PRNData[0], bytesToWrite);
    			index = bytesToWrite;
    			remaining = DATA_SIZE - bytesToWrite;
    		}
    		else //remaining != 0
    		{
       			WriteBurstReg(RF_TXFIFOWR, &PRNData[index], remaining);
    			index = 0;
    			remaining = DATA_SIZE;
    		}
    	}
    }
    
#endif    
}

//Taken from TI example code - sets CPU frequency to 8 MHz
void setClock()
{
	UCSCTL3 |= SELREF_2;                      // Set DCO FLL reference = REFO
	UCSCTL4 |= SELA_2;                        // Set ACLK = REFO

	__bis_SR_register(SCG0);                  // Disable the FLL control loop
	UCSCTL0 = 0x0000;                         // Set lowest possible DCOx, MODx
	UCSCTL1 = DCORSEL_5;                      // Select DCO range 16MHz operation
	UCSCTL2 = FLLD_1 + 249;                   // Set DCO Multiplier for 8MHz
                                            // (N + 1) * FLLRef = Fdco
                                            // (249 + 1) * 32768 = 8MHz
                                            // Set FLL Div = fDCOCLK/2
	__bic_SR_register(SCG0);                  // Enable the FLL control loop

	// Worst-case settling time for the DCO when the DCO range bits have been
	// changed is n x 32 x 32 x f_MCLK / f_FLL_reference. See UCS chapter in 5xx
  	// UG for optimization.
  	// 32 x 32 x 8 MHz / 32,768 Hz = 250000 = MCLK cycles for DCO to settle
  	__delay_cycles(250000);
	
  	// Loop until XT1,XT2 & DCO fault flag is cleared
  	do
  	{
  		UCSCTL7 &= ~(XT2OFFG + XT1LFOFFG + XT1HFOFFG + DCOFFG);
                                           		// Clear XT2,XT1,DCO fault flags
    	SFRIFG1 &= ~OFIFG;                      // Clear fault flags
  	}while (SFRIFG1&OFIFG);                   	// Test oscillator fault flag
}
