/*
  ************************************************************************
  *	twi.c
  *
  *	Arduino core files for MSP430
  *		Copyright (c) 2012 Robert Wessels. All right reserved.
  *
  *
  ***********************************************************************
  Derived from:
  twi.c - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <math.h>
#include <stdlib.h>
#include "Energia.h" // for digitalWrite


#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#include "pins_energia.h"
#include "twi.h"

static volatile uint8_t twi_state;
static volatile uint8_t twi_sendStop;           // should the transaction end with a stop
static volatile uint8_t twi_inRepStart;         // in the middle of a repeated start

static void (*twi_onSlaveTransmit)(void);
static void (*twi_onSlaveReceive)(uint8_t*, int);

static uint8_t twi_masterBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_masterBufferIndex;
static uint8_t twi_masterBufferLength;

static uint8_t twi_txBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_txBufferIndex;
static volatile uint8_t twi_txBufferLength;

static uint8_t twi_rxBuffer[TWI_BUFFER_LENGTH];
static volatile uint8_t twi_rxBufferIndex;

static volatile uint8_t twi_error;

#ifdef __MSP430_HAS_USI__
static uint8_t twi_slarw;
static uint8_t twi_my_addr;
#endif

#ifdef __MSP430_HAS_USCI__
#endif

#ifdef __MSP430_HAS_EUSCI_B0__
#endif


/* 
 * Function twi_init
 * Desc     readys twi pins and sets twi bitrate
 * Input    none
 * Output   none
 */
void twi_init(void)
{

	// initialize state
	twi_state = TWI_IDLE;
	//twi_sendStop = true;		// default value
	twi_inRepStart = false;
 
//	__bic_SR_register(GIE);
	pinMode(TWISDA,INPUT_PULLUP);
	pinMode(TWISCL,INPUT_PULLUP);
	
#ifdef __MSP430_HAS_USI__
	USICTL0 = USIPE6+USIPE7+USIMST+USISWRST;  // SDA/SCL
	USICTL1 = USII2C+USIIE;                   // Enable I2C mode & USI interrupt
	USICKCTL = USIDIV_7+USISSEL_2+USICKPL;    // USI clk: SCL = SMCLK/128
	USICNT |= USIIFGCC;                       // Disable automatic clear control
	USICTL0 &= ~USISWRST;                     // Enable USI
	USICTL1 &= ~USIIFG;                       // Clear pending flag
	__bis_SR_register(GIE);
#endif

#ifdef __MSP430_HAS_USCI__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
    //Disable the USCI module and clears the other bits of control register
    UCB0CTLW0 = UCSWRST;


    //Configure Automatic STOP condition generation
    UCB0CTLW1 &= ~UCASTP_3;
    //UCB0CTLW1 |= autoSTOPGeneration;

    //Byte Count Threshold
    //UCB0TBCNT = byteCounterThreshold;
    /*
     * Configure as I2C master mode.
     * UCMST = Master mode
     * UCMODE_3 = I2C mode
     * UCSYNC = Synchronous mode
     */
    UCB0CTLW0 = UCMST + UCMODE_3 + UCSYNC + UCSWRST;

    //Configure I2C clock source
    UCB0CTLW0 |= (UCSSEL__SMCLK);

    /*
     * Compute the clock divider that achieves the fastest speed less than or
     * equal to the desired speed.  The numerator is biased to favor a larger
     * clock divider so that the resulting clock is always less than or equal
     * to the desired clock, never greater.
     */
    UCB0BRW = (unsigned short)(F_CPU / 400000);
    UCB0CTLW0 &= ~(UCSWRST);
    UCB0IE |= (UCRXIE0|UCTXIE0|UCALIE|UCNACKIFG|UCSTTIFG|UCSTPIFG); // Enable I2C interrupts
#endif
}

/* 
 * Function twi_setAddress
 * Desc     sets slave address and enables interrupt
 * Input    none
 * Output   none
 */
void twi_setAddress(uint8_t address)
{
  // set twi own (slave) address
#ifdef __MSP430_HAS_USI__
	twi_my_addr = address << 1;
	//TODO: enable start detect interrupt to kick off the state machine
#endif
#ifdef __MSP430_HAS_USCI__
	// UCGCEN = respond to general Call
    UCB0I2COA = address | UCGCEN;
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
	// UCGCEN = respond to general Call
    UCB0I2COA0 = address | UCGCEN;
;
#endif
}

/* 
* Function twi_readFrom
 * Desc     attempts to become twi bus master and read a
 *          series of bytes from a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes to read into array
 * Output   number of bytes read
 */
uint8_t twi_readFrom(uint8_t address, uint8_t* data, uint8_t length, uint8_t sendStop)
{
	uint8_t i;

#ifdef __MSP430_HAS_USI__
	USICTL0 |= USIMST; // USI master mode
#endif
#ifdef __MSP430_HAS_USCI__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
    UCB0CTLW0 = UCSWRST;                      // Enable SW reset
    UCB0CTLW0 |= UCMST + UCMODE_3 + UCSYNC;   // I2C Master, synchronous mode
    UCB0CTLW0 &= ~(UCTR);                     // Configure in receive mode
    UCB0I2CSA = address;                      // Set Slave Address
    UCB0CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
    UCB0IE |= (UCRXIE0|UCTXIE0|UCALIE|UCNACKIFG|UCSTTIFG|UCSTPIFG); // Enable I2C interrupts
#endif
	// ensure data will fit into buffer
	if(TWI_BUFFER_LENGTH < length){
		return 0;
	}

	// initialize buffer iteration vars
	twi_masterBufferIndex = 0;
	twi_masterBufferLength = length-1;  // This is not intuitive, read on...
	// On receive, the previously configured ACK/NACK setting is transmitted in
	// response to the received byte before the interrupt is signalled.
	// Therefor we must actually set NACK when the _next_ to last byte is
	// received, causing that NACK to be sent in response to receiving the last
	// expected byte of data.

#ifdef __MSP430_HAS_USI__
	// build sla+w, slave device address + w bit
	twi_slarw = 1;
	twi_slarw |= address << 1;

	// send start condition
	twi_state = TWI_SND_START;
	// this will trigger an interrupt kicking off the state machine in the isr
	USICTL1 |= USIIFG;
#endif
#ifdef __MSP430_HAS_USCI__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
    twi_state =  TWI_MRX;                     // Master receive mode
    while (UCB0CTLW0 & UCTXSTP);           // Ensure stop condition got sent
    UCB0CTLW0 |= UCTXSTT;                 // I2C start condition
#endif

	// wait for read operation to complete
	while(twi_state != TWI_IDLE){
		continue;
	}

	if (twi_masterBufferIndex < length)
		length = twi_masterBufferIndex;

	// copy twi buffer to data
	for(i = 0; i < length; ++i){
		data[i] = twi_masterBuffer[i];
	}

	return length;
}

/*
 * Function twi_writeTo
 * Desc     attempts to become twi bus master and write a
 *          series of bytes to a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes in array
 *          wait: boolean indicating to wait for write or not
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 */
uint8_t twi_writeTo(uint8_t address, uint8_t* data, uint8_t length, uint8_t wait, uint8_t sendStop)
{
	uint8_t i;
	twi_error = TWI_ERRROR_NO_ERROR;
#ifdef __MSP430_HAS_USI__
	USICTL0 |= USIMST; // USI master mode
#endif
#ifdef __MSP430_HAS_USCI__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
    UCB0CTLW0 = UCSWRST;                      // Enable SW reset
    UCB0CTLW0 |= UCMST + UCMODE_3 + UCSYNC;   // I2C Master, synchronous mode
    UCB0CTLW0 |= UCTR;                        // Configure in transmit mode
    UCB0I2CSA = address;                      // Set Slave Address
    UCB0CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
    UCB0IE |= (UCRXIE0|UCTXIE0|UCALIE|UCNACKIFG|UCSTTIFG|UCSTPIFG); // Enable I2C interrupts
#endif
	if(length == 0) {
		return 0;
	}

	// ensure data will fit into buffer
	if(length > TWI_BUFFER_LENGTH){
		return TWI_ERROR_BUF_TO_LONG;
	}


	// initialize buffer iteration vars
	twi_masterBufferIndex = 0;
	twi_masterBufferLength = length;

	// copy data to twi buffer
	for(i = 0; i < length; ++i){
		twi_masterBuffer[i] = data[i];
	}

#ifdef __MSP430_HAS_USI__
	// build sla+w, slave device address + w bit
	twi_slarw = 0;
	twi_slarw |= address << 1;
	
	twi_state = TWI_SND_START;
	// this will trigger an interrupt kicking off the state machine in the isr
	USICTL1 |= USIIFG;
#endif
#ifdef __MSP430_HAS_USCI__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
    twi_state =  TWI_MTX;                     // Master Transmit mode
    while (UCB0CTLW0 & UCTXSTP);           // Ensure stop condition got sent
    UCB0CTLW0 |= UCTXSTT;                  // I2C start condition
#endif

	while(twi_state != TWI_IDLE)
	{
		continue;
	}

	return twi_error;
}

/*
 * Function twi_transmit
 * Desc     fills slave tx buffer with data
 *          must be called in slave tx event callback
 * Input    data: pointer to byte array
 *          length: number of bytes in array
 * Output   1 length too long for buffer
 *          2 not slave transmitter
 *          0 ok
 */
uint8_t twi_transmit(const uint8_t* data, uint8_t length)
{
  uint8_t i;

#ifdef __MSP430_HAS_USI__
#endif
#ifdef __MSP430_HAS_USCI__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
//  UCB0CTLW0 = UCSWRST;                      // Enable SW reset
//  UCB0CTLW0 |= UCMODE_3 + UCSYNC;           // I2C Slave, synchronous mode
//  UCB0CTLW0 &= ~(UCTR);                     // Slave requires receive Mode
//  UCB0CTLW0 &= ~UCSWRST;                    // Clear SW reset, resume operation
//  UCB0IE |= (UCRXIE0|UCTXIE0|UCALIE|UCNACKIFG|UCSTTIFG|UCSTPIFG); // Enable I2C interrupts
  twi_state =  TWI_STX;                     // Slave transmit mode
#endif
  // ensure data will fit into buffer
  if(TWI_BUFFER_LENGTH < length){
    return 1;
  }
  // set length and copy data into tx buffer
  twi_txBufferLength = length;
  for(i = 0; i < length; ++i){
    twi_txBuffer[i] = data[i];
  }

  return 0;
}

/* 
 * Function twi_attachSlaveRxEvent
 * Desc     sets function called before a slave read operation
 * Input    function: callback function to use
 * Output   none
 */
void twi_attachSlaveRxEvent( void (*function)(uint8_t*, int) )
{
  twi_onSlaveReceive = function;
}

/* 
 * Function twi_attachSlaveTxEvent
 * Desc     sets function called before a slave write operation
 * Input    function: callback function to use
 * Output   none
 */
void twi_attachSlaveTxEvent( void (*function)(void) )
{
  twi_onSlaveTransmit = function;
}

/* 
 * Function twi_start
 * Desc     sends start condition
 * Input    tx: 1 indicated tx, 0 indicated rx
 * Output   none
 */
void twi_start()
{
}

void twi_send() {
#ifdef __MSP430_HAS_USI__
	USISRL = 0xaa;          // Load data byte
	USICNT |=  0x08;              // Bit counter = 8, start TX
#endif
#ifdef __MSP430_HAS_USCI__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
#endif
}
/*
 * Function twi_stop
 * Desc     relinquishes bus master status
 * Input    none
 * Output   none
 */
void twi_stop(void)
{
#ifdef __MSP430_HAS_USI__
	USICTL0 |= USIOE;
	USISRL = 0x00;
	USICNT |=  0x01;
#endif
#ifdef __MSP430_HAS_USCI__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
#endif
}

/* 
 * Function twi_releaseBus
 * Desc     releases bus control
 * Input    none
 * Output   none
 */
void twi_releaseBus(void)
{

}

#if 0
//__attribute__((interrupt(USI_VECTOR)))
void USI_ISR(void)
{
	switch(twi_state){
	// All Master
	case TWI_SND_START:// sent start condition
		USISRL = 0x00;                // Generate Start Condition...
		USICTL0 |= USIGE+USIOE;
		USICTL0 &= ~USIGE;
		USISRL = twi_slarw;
		USICNT = (USICNT & 0xE0) + 0x08;
		twi_state = TWI_RECV_SLA_ACK;

		break;
	case TWI_RECV_SLA_ACK: // reveive (N)ACK

		USICTL0 &= ~USIOE; // SDA = input
		USICNT |= 0x01; // Bit counter=1
		twi_state = TWI_PROC_SLA_ACK;
		break;
	case TWI_PROC_SLA_ACK:
		if ((USISRL & 0x01) || (twi_masterBufferIndex == twi_masterBufferLength)){
			if(twi_masterBufferIndex == 0) {
				//we haven't advance so must be an address NACK
				twi_error = TWI_ERROR_ADDR_NACK;
			} else if (twi_masterBufferIndex < twi_masterBufferLength){
				twi_error = TWI_ERROR_DATA_NACK;
			}
			twi_stop();
			twi_state = TWI_EXIT;
		} else { // ACK received
			USICTL0 |= USIOE;
			USISRL = twi_masterBuffer[twi_masterBufferIndex++];
			USICNT |=  0x08;
			twi_state = TWI_RECV_SLA_ACK;
		}
		// if receive data set counter to 8 and SDA to in
		// then go to receive data processing stage

		break;
	//case TWI_PROC_SLA_DATA:
	//	if not the last byte,
	case TWI_EXIT:
		USISRL = 0x0FF; // USISRL = 1 to drive SDA high
		USICTL0 |= USIGE; // Transparent latch enabled
		USICTL0 &= ~(USIGE+USIOE); // Latch/SDA output disabled
		twi_state = TWI_IDLE; //Idle
		break;
	}

	USICTL1 &= ~USIIFG; // !!!Clear pending flag!!!

}
#endif

void send_start()
{
#ifdef __MSP430_HAS_USI__
	USISRL = 0x00;                // Generate Start Condition...
	USICTL0 |= USIGE+USIOE;
	USICTL0 &= ~USIGE;
	USISRL = twi_slarw;
	USICNT = (USICNT & 0xE0) + 0x08;
#endif
#ifdef __MSP430_HAS_USCI__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
#endif
}

#ifdef __MSP430_HAS_USI__
__attribute__((interrupt(USI_VECTOR)))
void USI_ISR(void)
{
	switch(twi_state){

	// Master transmit / receive
	case TWI_SND_START:
		send_start();
		twi_state = TWI_PREP_SLA_ADDR_ACK;
		break;
	case TWI_PREP_SLA_ADDR_ACK: // reveive (N)ACK
		USICTL0 &= ~USIOE; // SDA = input
		USICNT |= 0x01; // Bit counter=1
		twi_state = TWI_MT_PROC_ADDR_ACK;
		break;
	case TWI_MT_PROC_ADDR_ACK:
		if (USISRL & 0x01) {
			twi_error = TWI_ERROR_ADDR_NACK;
			twi_stop();
			twi_state = TWI_EXIT;
			break;
		}
		if(twi_slarw & 1)
			goto mtre;
		else
			goto mtpd;
		// else fall through and process data ACK;
	case TWI_MT_PREP_DATA_ACK: // reveive (N)ACK
		USICTL0 &= ~USIOE; // SDA = input
		USICNT |= 0x01; // Bit counter=1
		twi_state = TWI_MT_PROC_DATA_ACK;
		break;
	case TWI_MT_PROC_DATA_ACK:
mtpd:
		if (USISRL & 0x01) {
			twi_error = TWI_ERROR_DATA_NACK;
			twi_stop();
			twi_state = TWI_EXIT;
			break;
		}

		if(twi_masterBufferIndex == twi_masterBufferLength) {
			twi_stop();
			twi_state = TWI_EXIT;
			break;
		}

		USICTL0 |= USIOE;
		USISRL = twi_masterBuffer[twi_masterBufferIndex++];
		USICNT |=  0x08;
		twi_state = TWI_MT_PREP_DATA_ACK;
		break;
	// Master receiver
mtre:
	case TWI_MR_PREP_DATA_RECV:
		USICTL0 &= ~USIOE; // SDA input
		USICNT |=  0x08; // Bit counter = 8, RX data
		twi_state = TWI_MR_PROC_DATA_RECV;
		break;
	case TWI_MR_PROC_DATA_RECV:
		USICTL0 |= USIOE; // SDA output
		twi_masterBuffer[twi_masterBufferIndex++] = USISRL;
		if(twi_masterBufferIndex > twi_masterBufferLength ) {
			USISRL = 0xFF; // that was the last byte send NACK
			twi_state = TWI_MR_PREP_STOP;
		} else {
			USISRL = 0x00; // keep on receiving and send ACK
			twi_state = TWI_MR_PREP_DATA_RECV;
		}
		USICNT |= 0x01;
		break;
	case TWI_MR_PREP_STOP:
		twi_stop();
		twi_state = TWI_EXIT;
		break;
	// Slave receiver
	// Slave transmitter
	// All
	case TWI_EXIT:
		USISRL = 0x0FF; // USISRL = 1 to drive SDA high
		USICTL0 |= USIGE; // Transparent latch enabled
		USICTL0 &= ~(USIGE+USIOE); // Latch/SDA output disabled
		twi_state = TWI_IDLE; //Idle
		break;

	default:
		break;//should not happen handle error
	}

	USICTL1 &= ~USIIFG; // !!!Clear pending flag!!!

}
#endif
#ifdef __MSP430_HAS_USCI__
#endif
#ifdef __MSP430_HAS_EUSCI_B0__
__attribute__((interrupt(USCI_B0_VECTOR)))
void USCI_B0_ISR(void)
{
//	switch(twi_state){
  switch(UCB0IV){
    case USCI_NONE:            // No Interrupt pending
      break;
    case USCI_I2C_UCALIFG:     // USCI I2C Mode: UCALIFG
      // enter slave transmitter mode
      twi_state = TWI_STX;
      // ready the tx buffer index for iteration
      twi_txBufferIndex = 0;
      // set tx buffer length to be zero, to verify if user changes it
      twi_txBufferLength = 0;
      // request for txBuffer to be filled and length to be set
      // note: user must call twi_transmit(bytes, length) to do this
      twi_onSlaveTransmit();
      // if they didn't change buffer & length, initialize it
      if(0 == twi_txBufferLength){
        twi_txBufferLength = 1;
        twi_txBuffer[0] = 0x00;
      }
      // transmit first byte from buffer, fall
      // copy data to output register
      UCB0TXBUF = twi_txBuffer[twi_txBufferIndex++];
      // if there is more to send, ack, otherwise nack
      if(twi_txBufferIndex < twi_txBufferLength){
        //twi_reply(1);
      }else{
		UCB0CTLW0 |= UCTXNACK;                // Generate NACK condition
      }
      // leave slave receiver state
      twi_state = TWI_IDLE;
	  
      break;
    case USCI_I2C_UCNACKIFG:   // USCI I2C Mode: UCNACKIFG
        // leave slave receiver state
        twi_state = TWI_IDLE;
		twi_error = TWI_ERROR_DATA_NACK;
        __bic_SR_register_on_exit(CPUOFF); // Exit LPM0                 
      break;
    case USCI_I2C_UCSTTIFG:    // USCI I2C Mode: UCSTTIFG
	  if (twi_state ==  TWI_IDLE)
		twi_state =  TWI_SRX;      // Slave receive mode
      break;
    case USCI_I2C_UCSTPIFG:    // USCI I2C Mode: UCSTPIFG
		twi_state =  TWI_IDLE; // IDLE mode
        __bic_SR_register_on_exit(CPUOFF); // Exit LPM0                 
      break;
    case USCI_I2C_UCRXIFG3:    // USCI I2C Mode: UCRXIFG3
      break;
    case USCI_I2C_UCTXIFG3:    // USCI I2C Mode: UCTXIFG3
      break;
    case USCI_I2C_UCRXIFG2:    // USCI I2C Mode: UCRXIFG2
      break;
    case USCI_I2C_UCTXIFG2:    // USCI I2C Mode: UCTXIFG2
      break;
    case USCI_I2C_UCRXIFG1:    // USCI I2C Mode: UCRXIFG1
      break;
    case USCI_I2C_UCTXIFG1:    // USCI I2C Mode: UCTXIFG1
      break;
    case USCI_I2C_UCRXIFG0:    // USCI I2C Mode: UCRXIFG0
		if (twi_state ==  TWI_MRX) {      // Master receive mode
			twi_masterBuffer[twi_masterBufferIndex++] = UCB0RXBUF; // Get RX data
			if(twi_masterBufferIndex == twi_masterBufferLength ) 
				UCB0CTLW0 |= UCTXSTP;  // Generate I2C stop condition
			if(twi_masterBufferIndex > twi_masterBufferLength ) {
				twi_state = TWI_IDLE; //Idle
			} else {
			}
		} else {
			// if there is still room in the rx buffer
			if(twi_rxBufferIndex < TWI_BUFFER_LENGTH){
				// put byte in buffer and ack
				twi_rxBuffer[twi_rxBufferIndex++] = UCB0RXBUF;
			}else{
				// otherwise nack
				UCB0CTLW0 |= UCTXNACK;                // Generate NACK condition
			}
		}
      break;
    case USCI_I2C_UCTXIFG0:    // USCI I2C Mode: UCTXIFG0
		if (twi_state == TWI_MTX) {      // Master receive mode
			// if there is data to send, send it, otherwise stop 
			if(twi_masterBufferIndex < twi_masterBufferLength){
				// copy data to output register and ack
				UCB0TXBUF = twi_txBuffer[twi_txBufferIndex++];                 // Transmit data at address PTxData
			}else{
			   if (twi_sendStop)
				UCB0CTLW0 |= UCTXSTP;                // Generate I2C stop condition
			   else {
				 twi_inRepStart = true;   // we're gonna send the START
				 // don't enable the interrupt. We'll generate the start, but we 
				 // avoid handling the interrupt until we're in the next transaction,
				 // at the point where we would normally issue the start.
				 UCB0CTLW0 |= UCTXSTT;        
				 twi_state = TWI_IDLE;
			   }
			}
		} else {
			// copy data to output register
			UCB0TXBUF = twi_txBuffer[twi_txBufferIndex++];
			// if there is more to send, ack, otherwise nack
			if(twi_txBufferIndex < twi_txBufferLength){
			}else{
				UCB0CTLW0 |= UCTXNACK;                // Generate NACK condition
			}
		}
      break;
    case USCI_I2C_UCBCNTIFG:   // USCI I2C Mode: UCBCNTIFG
      break;
    case USCI_I2C_UCCLTOIFG:   // USCI I2C Mode: UCCLTOIFG/
      break;
    case USCI_I2C_UCBIT9IFG:   // USCI I2C Mode: UCBIT9IFG
      break;
	}
} 
#endif

