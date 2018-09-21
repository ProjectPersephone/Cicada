// 2013 Sprite-specific code here; MorikawaSDK ARTSAT DESPATCH code follows

#define SPRITE
#define USE_SPRITE_RADIO  // ... comment this out to save waiting

#ifdef SPRITE

// ~~~~~~~~~~~~ Sprite (vintage 2013) Magnetometer ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// From https://github.com/kicksat/Sprites/wiki/Code-Examples

#include <SpriteMag.h>

SpriteMag sprite_mag = SpriteMag();


// ~~~~~~~~~~~~ Sprite (vintage 2013) Temperature Sensor ~~~~~~~~~~~~~~~~~~~~~
//
// https://github.com/lfdebrux/TemperatureSensor/tree/master/examples/sprite_temp

#include <temp.h>

// ~~~~~~~~~~~~ Sprite (vintage 2013) Radio ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// https://github.com/kicksat/Sprites/blob/master/Software/Test_Code/RadioData/RadioData.ino

#include <SpriteRadio.h>

/*
  This example code will configure the CC1101 radio core in the CC430 to
  repeatedly transmit a text message. The output signal will be MSK modulated
  at 64 kbps on a 437.24 MHz carrier and bits are encoded by alternating between
  two different 511 bit Gold codes.
*/

unsigned char prn2[64] = {
  0b00000001, 0b01011110, 0b11010100, 0b01100001, 0b00001011, 0b11110011, 0b00110001, 0b01011100,
  0b01100110, 0b10010010, 0b01011011, 0b00101010, 0b11100000, 0b10100011, 0b00000000, 0b11100001,
  0b10111011, 0b10011111, 0b00110001, 0b11001111, 0b11110111, 0b11000000, 0b10110010, 0b01110101,
  0b10101010, 0b10100111, 0b10100101, 0b00010010, 0b00001111, 0b01011011, 0b00000010, 0b00111101,
  0b01001110, 0b01100000, 0b10001110, 0b00010111, 0b00110100, 0b10000101, 0b01100001, 0b01000101,
  0b00000110, 0b10100010, 0b00110110, 0b00101111, 0b10101001, 0b00011111, 0b11010111, 0b11111101,
  0b10011101, 0b01001000, 0b00011001, 0b00011000, 0b10101111, 0b00110110, 0b10010011, 0b00000000,
  0b00010000, 0b10000101, 0b00101000, 0b00011101, 0b01011100, 0b10101111, 0b01100100, 0b11011010
};

unsigned char prn3[64] = {
  0b11111101, 0b00111110, 0b01110111, 0b11010101, 0b00100101, 0b11101111, 0b00101100, 0b01101001,
  0b00101010, 0b11101001, 0b00111100, 0b11000100, 0b00000111, 0b10010011, 0b11000101, 0b00000111,
  0b00110111, 0b00011111, 0b01111011, 0b11010001, 0b10111010, 0b00000111, 0b10010000, 0b00110111,
  0b11011111, 0b01011010, 0b11101101, 0b11001000, 0b10001100, 0b01101001, 0b10010111, 0b00101001,
  0b10101100, 0b11011001, 0b11010110, 0b00011010, 0b11010110, 0b10101000, 0b00000101, 0b11010011,
  0b01101010, 0b11001011, 0b11010110, 0b01010010, 0b00111111, 0b11100111, 0b10000010, 0b10000110,
  0b01101110, 0b10011010, 0b01100101, 0b10100110, 0b00101110, 0b01010100, 0b11110100, 0b01111010,
  0b11001011, 0b00101110, 0b01100011, 0b10111111, 0b01010100, 0b11000100, 0b11010100, 0b01010100
};

//Initialize the radio class, supplying the Gold Codes that correspond to 0 and 1
SpriteRadio sprite_radio = SpriteRadio(prn2, prn3);

// ~~~~~~~~~~~~ Sprite (vintage 2013) Gyro ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Originally:
// https://github.com/kicksat/Sprites/blob/master/Software/Test_Code/GyroDemo/GyroDemo.ino
// 
// It has been modified to eliminate the SpriteRadio interface (already
// supplied, above), the serial setup/message (ditto), the sprintf call
// (the Sprite2 runtime library seems to lack printf's, while some
// debugging output goes out by Serial.print calls instead of radio
// transmission.)

#include <SpriteGyro.h>

SpriteGyro gyro = SpriteGyro();

 
// ~~~~~~~~~~~~ AVR program space compatibility retrofit ~~~~~~~~~~~~~~~~~~~~~
//

#include <avr/pgmspace.h>
#define strncpy_P(s1,s2,n) strncpy(s1,s2,n) // not in avr/pgmspace.h

#endif // SPRITE

// The following code for Arduino 1.0.5 was ported to Arduino 1.5+,
// then modified for vintage-2013 Energia and the KickSat Sprite.
// It compiles under a version of Energia (2013 or so) for the original
// Sprite, and it runs on that platform. Much of the code refers
// to devices on the ARTSAT2 DESPATCH spacecraft, but conditional
// compilation (#ifdef SPRITE) is used, where necessary, to
// substitute for readings from the Sprite's devices.

/*
**      ARTSAT Despatch Application
**
**      Original Copyright (C) 2014 - 2014 HORIGUCHI Junshi.
**                                          http://iridium.jp/
**                                          zap00365@nifty.com
**      Portions Copyright (C) <year> <author>
**                                          <website>
**                                          <e-mail>
**      All rights reserved.
**
**      Version     Arduino
**      Website     http://artsat.jp/
**      E-mail      info@artsat.jp
**
**      This source code is for Arduino IDE.
**      Arduino 1.0.5
**
**      DespatchApp.ino
**
**      ------------------------------------------------------------------------
**
**      GNU GENERAL PUBLIC LICENSE (GPLv3)
**
**      This program is free software: you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation, either version 3 of the License,
**      or (at your option) any later version.
**      This program is distributed in the hope that it will be useful,
**      but WITHOUT ANY WARRANTY; without even the implied warranty of
**      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**      See the GNU General Public License for more details.
**      You should have received a copy of the GNU General Public License
**      along with this program. If not, see <http://www.gnu.org/licenses/>.
**
**      このプログラムはフリーソフトウェアです。あなたはこれをフリーソフトウェア財団によって発行された
**      GNU 一般公衆利用許諾書（バージョン 3 か、それ以降のバージョンのうちどれか）が定める条件の下で
**      再頒布または改変することができます。このプログラムは有用であることを願って頒布されますが、
**      *全くの無保証* です。商業可能性の保証や特定目的への適合性は、言外に示されたものも含め全く存在しません。
**      詳しくは GNU 一般公衆利用許諾書をご覧ください。
**      あなたはこのプログラムと共に GNU 一般公衆利用許諾書のコピーを一部受け取っているはずです。
**      もし受け取っていなければ <http://www.gnu.org/licenses/> をご覧ください。
*/

#include <MorikawaSDK.h>
#ifndef SPRITE
#include "utility/I2Cm.h"
#endif
#include "WordTable.h"

#define TARGET_BUILD_DEBUG
//#define TARGET_DEBUG_AUTO

//#define TARGET_USE_RADIO

#define PIN_MODE_SELECT_MSB         (34)
#define PIN_MODE_SELECT_LSB         (35)
#define PIN_MPU6500_INT             (3)
#define PIN_RADIO_SIGNAL            (5)
#define PIN_RADIO_NOISE             (2)
#define PIN_RSSI                    (A0)
#define I2C_LM75A_BOARD             (0x48)
#define I2C_LM75A_TX                (0x49)
#define I2C_LM75A_BATTERY           (0x4A)
#define I2C_LM75A_WALL              (0x4B)
#define I2C_LM75A_KEEL              (0x4C)
#define I2C_MPU6500                 (0x69)
#define I2C_INA226                  (0x40)
#define SMEM_GYRO_X                 (0x3FA00UL)
#define SMEM_GYRO_Y                 (0x3FA01UL)
#define SMEM_GYRO_Z                 (0x3FA02UL)
#define SMEM_BATTERY_CURRENT        (0x3FA03UL)
#define SMEM_BATTERY_VOLTAGE        (0x3FA04UL)
#define SMEM_RSSI                   (0x3FA05UL)
#define SMEM_BOARD_TEMPERATURE      (0x3FA06UL)
#define EMEM_CONTEXT_INDEX          (0)
#define INDEX_LIMIT                 (0x300)
#define COUNT_LIMIT                 (8)
#define AVERAGE_SIZE                (3)
#ifdef SPRITE
#define RANGE_TEMPERATURE_MAXIMUM   (+180)
#define RANGE_TEMPERATURE_MINIMUM   (-180)
#else
#define RANGE_TEMPERATURE_MAXIMUM   (+250)
#define RANGE_TEMPERATURE_MINIMUM   (-110)
#endif
#define RANGE_ACCEL_MAXIMUM         (+32767)
#define RANGE_ACCEL_MINIMUM         (-32768)
#define RANGE_GYRO_MAXIMUM          (+32767)
#define RANGE_GYRO_MINIMUM          (-32768)
#define RANGE_VOLTAGE_MAXIMUM       (+32767)
#define RANGE_VOLTAGE_MINIMUM       (0)
#define RANGE_CURRENT_MAXIMUM       (+32767)
#define RANGE_CURRENT_MINIMUM       (-32768)
#define RANGE_RSSI_MAXIMUM          (+1023)
#define RANGE_RSSI_MINIMUM          (0)
#define RANGE_RADIO_MAXIMUM         (+32767)
#define RANGE_RADIO_MINIMUM         (0)

#ifdef SPRITE
#define LIMIT_TEMPERATURE_MAXIMUM   (3000) // > 140 C
#define LIMIT_TEMPERATURE_MINIMUM   (0)  // < -170 C
#else
#define LIMIT_TEMPERATURE_MAXIMUM   (19296) // +102 (+51degC)
#define LIMIT_TEMPERATURE_MINIMUM   (7827)  // -24 (-12degC)
#endif

#ifdef SPRITE
#define LIMIT_GYRO_MAXIMUM          (2000) // +1311 (+10deg/sec)
#define LIMIT_GYRO_MINIMUM          (-2000) // -1311 (-10deg/sec)
#else
#define LIMIT_GYRO_MAXIMUM          (17039) // +1311 (+10deg/sec)
#define LIMIT_GYRO_MINIMUM          (15728) // -1311 (-10deg/sec)
#endif

#define LIMIT_CURRENT_MAXIMUM       (18633) // +4500 (+4.5A)
#define LIMIT_CURRENT_MINIMUM       (16133) // -500 (-0.5A)
#define WAIT_LM75A                  (350)   // typical 100ms, limits 300ms
#define WAIT_MPU6500                (300)   // accel = 4 ~ 4000Hz, gyro = 4 ~ 8000Hz
#define WAIT_INA226                 (15)    // typical 140us, limits 154us ~ typical 8.244ms, limits 9.068ms
#define WAIT_ANALOGREAD             (15)    // typical 100us
#define DELAY_RADIO                 (5000)
#define INTERVAL_RADIO              (15000)
#define INVALID_VALUE_4             ("____")
#define INVALID_VALUE_8             ("________")
#define REMAP(v, if, it, of, ot)    (map(constrain((v), (if), (it)), (if), (it), (of), (ot)))

enum ValidEnum {
    VALID_LM75A_BOARD               = 1 << 0,
    VALID_LM75A_TX                  = 1 << 1,
    VALID_LM75A_BATTERY             = 1 << 2,
    VALID_LM75A_WALL                = 1 << 3,
    VALID_LM75A_KEEL                = 1 << 4,
    VALID_MPU6500                   = 1 << 5,
    VALID_INA226                    = 1 << 6
};

static void samplingSensorCold(void);
static void samplingSensorHot(void);
#ifndef SPRITE
static void samplingRadio(void);   // no radio sampling in Sprites (yet)
#endif
static void clear(int param[AVERAGE_SIZE]);
static int average(int param[AVERAGE_SIZE]);
static void saveIndex(unsigned int param);
static unsigned int loadIndex(void);
#ifndef SPRITE   // serious problems with type name accessibility
static TSTError checkLM75A(int i2c);
static TSTError checkMPU6500(int i2c);
static TSTError checkINA226(int i2c);
static TSTError calibrateLM75A(int i2c);
static TSTError calibrateMPU6500(int i2c);
static TSTError calibrateINA226(int i2c);
static TSTError getLM75A(int i2c, int* temperature);
static TSTError getMPU6500(int i2c, int* ax, int* ay, int* az, int* gx, int* gy, int* gz);
static TSTError getINA226(int i2c, int* voltage, int* current);
#else
static char checkLM75A(int i2c);
static char checkMPU6500(int i2c);
static char checkINA226(int i2c);
static char calibrateLM75A(int i2c);
static char calibrateMPU6500(int i2c);
static char calibrateINA226(int i2c);
static char getLM75A(int i2c, int* temperature);
static char getMPU6500(int i2c, int* ax, int* ay, int* az, int* gx, int* gy, int* gz);
static char getINA226(int i2c, int* voltage, int* current);
#endif
static void remapRange(int value, int minimum, int maximum, int* result, bool* remap);
static void makeBaudotFixed(char* buffer, char const PROGMEM* value);
static void makeBaudotState(char* buffer, int loop, int temperature, int rssi, int voltage);
static void makeBaudotColor(char* buffer, int lvalue, bool lvalid, int rvalue, bool rvalid);
static void makeBaudotRhythm(char* buffer, int lvalue, bool lvalid, int rvalue, bool rvalid);
static void dump(TSTError error, int hint);


static void dump(tst::TSTError error, int hint)
{
    static char s_text[16];
    
    // Sprite library seems not to have printf functions

#ifdef TARGET_BUILD_DEBUG
#ifndef SPRITE
    snprintf(s_text, sizeof(s_text), "SOS E%02x H%03d", error, hint);
    Serial.println(s_text);
#else
    Serial.print ("SOS E"); Serial.print (error);
    Serial.print (" H");Serial.println(hint);
#endif
#endif
    // Morikawa.setText(TEXT_DEBUG, s_text);
    return;
}


// moved up here, maybe forward ref was part of the problem, along with the mysterious need
// to qualify with "tst::" on error param
TSTError __debug__(tst::TSTError error, int hint)
{
    static TSTTrinity<TSTError> s_error(TSTERROR_OK);
    
    if (error != TSTERROR_OK) {
        if (s_error == TSTERROR_OK) {
            dump(error, hint);
            s_error = error;
        }
    }
    return error;
}



static TSTTrinity<unsigned char> g_valid;   // bit field as ValidEnum
static TSTTrinity<int> g_board_t;           // [0, 32768), -1 is invalid
static TSTTrinity<int> g_tx_t;              // [0, 32768), -1 is invalid
static TSTTrinity<int> g_battery_t;         // [0, 32768), -1 is invalid
static TSTTrinity<int> g_wall_t;            // [0, 32768), -1 is invalid
static TSTTrinity<int> g_keel_t;            // [0, 32768), -1 is invalid
static TSTTrinity<int> g_gyro_x;            // [0, 32768), -1 is invalid
static TSTTrinity<int> g_gyro_y;            // [0, 32768), -1 is invalid
static TSTTrinity<int> g_gyro_z;            // [0, 32768), -1 is invalid
static TSTTrinity<int> g_battery_v;         // [0, 32768), -1 is invalid
static TSTTrinity<int> g_battery_c;         // [0, 32768), -1 is invalid
static TSTTrinity<int> g_rssi;              // [0, 32768), -1 is invalid
static TSTTrinity<int> g_radio_s;           // [0, 32768), -1 is invalid
static TSTTrinity<int> g_radio_n;           // [0, 32768), -1 is invalid
static TSTTrinity<bool> g_sensor;           // [false, true]
static TSTTrinity<bool> g_poem;             // [false, true]
static TSTTrinity<unsigned int> g_index;    // [0, 0x300)
static TSTTrinity<unsigned int> g_count;    // [0, 8)

void setup(void)
{
#ifdef TARGET_BUILD_DEBUG
    Serial.begin(9600);
#endif;

    if (__debug__(Morikawa.setup(), 0) == TSTERROR_OK) {
#ifdef SPRITE
#ifdef TARGET_BUILD_DEBUG
      Serial.println ("Sprite is DESPATCHING ....");
      delay (300);
#endif

      sprite_radio.txInit();
      gyro.init();
      premult = 10; // for temp, global var?
      mainTempCalibrate();
      mainTempRead(); // first reading is usually spurious

      sprite_mag.init();
#endif

#ifdef TARGET_BUILD_DEBUG
        //SharedMemory_setup();
        //SharedMemory_loop();
#endif

#ifdef SPRITE
#else
        __debug__(Morikawa.enablePowerBus(POWER_BUS_Y), 1);
        pinMode(PIN_MODE_SELECT_MSB, INPUT);
        pinMode(PIN_MODE_SELECT_LSB, INPUT);
        pinMode(PIN_MPU6500_INT, INPUT);
        pinMode(PIN_RADIO_SIGNAL, INPUT);
        pinMode(PIN_RADIO_NOISE, INPUT);
        pinMode(PIN_RSSI, INPUT);
#endif
        g_valid = 0;
        g_board_t = -1;
        g_tx_t = -1;
        g_battery_t = -1;
        g_wall_t = -1;
        g_keel_t = -1;
        g_gyro_x = -1;
        g_gyro_y = -1;
        g_gyro_z = -1;
        g_battery_v = -1;
        g_battery_c = -1;
        g_rssi = -1;
        g_radio_s = -1;
        g_radio_n = -1;
        g_sensor = true;
        g_poem = true;
        g_index = 0;
        g_count = 0;
#ifndef SPRITE
        if (__debug__(checkLM75A(I2C_LM75A_BOARD), 10) == TSTERROR_OK) {
            if (__debug__(calibrateLM75A(I2C_LM75A_BOARD), 11) == TSTERROR_OK) {
#endif
                g_valid |= VALID_LM75A_BOARD;
#ifndef SPRITE
            }
        }
        if (__debug__(checkLM75A(I2C_LM75A_TX), 12) == TSTERROR_OK) {
            if (__debug__(calibrateLM75A(I2C_LM75A_TX), 13) == TSTERROR_OK) {
#endif
                g_valid |= VALID_LM75A_TX;
#ifndef SPRITE
            }
        }
        if (__debug__(checkLM75A(I2C_LM75A_BATTERY), 14) == TSTERROR_OK) {
            if (__debug__(calibrateLM75A(I2C_LM75A_BATTERY), 15) == TSTERROR_OK) {
#endif
                g_valid |= VALID_LM75A_BATTERY;
#ifndef SPRITE
            }
        }
        if (__debug__(checkLM75A(I2C_LM75A_WALL), 16) == TSTERROR_OK) {
            if (__debug__(calibrateLM75A(I2C_LM75A_WALL), 17) == TSTERROR_OK) {
#endif
                g_valid |= VALID_LM75A_WALL;
#ifndef SPRITE
            }
        }
        if (__debug__(checkLM75A(I2C_LM75A_KEEL), 18) == TSTERROR_OK) {
            if (__debug__(calibrateLM75A(I2C_LM75A_KEEL), 19) == TSTERROR_OK) {
#endif
                g_valid |= VALID_LM75A_KEEL;
#ifndef SPRITE
            }
        }
#endif
        if (__debug__(checkMPU6500(I2C_MPU6500), 20) == TSTERROR_OK) {
            if (__debug__(calibrateMPU6500(I2C_MPU6500), 21) == TSTERROR_OK) {
                g_valid |= VALID_MPU6500;
            }
        }
        if (__debug__(checkINA226(I2C_INA226), 22) == TSTERROR_OK) {
            if (__debug__(calibrateINA226(I2C_INA226), 23) == TSTERROR_OK) {
                g_valid |= VALID_INA226;
            }
#ifndef SPRITE  // no index saving in Sprites (yet)
        g_index = loadIndex();
        saveIndex(g_index);
#endif
#ifdef TARGET_BUILD_DEBUG
        Serial.print("valid = 0b");
        Serial.println(g_valid, BIN);
        Serial.print("index = ");
        Serial.println(g_index);
#endif
        }
    }
    else {
        Morikawa.shutdown();
    }
    return;
}

void loop(void)
{
    bool handle;
    char buffer[16];
    int lvalue;
    int rvalue;
    bool lvalid;
    bool rvalid;
    unsigned long address;
#ifdef TARGET_BUILD_DEBUG
    int debug(Serial.read());
#endif
    int i;
    
    Morikawa.loop();
    // delay (300);
    
#ifdef SPRITE    
#ifdef TARGET_BUILD_DEBUG
    delay (300);
    {
    // AngularVelocity w = gyro.read();
    // Serial.print("gyro x,y,z: ");
    // Serial.print ("<"); Serial.print(w.x);
    // Serial.print (","); Serial.print(w.y);
    // Serial.print (","); Serial.print(w.z);
    // Serial.println (">");
    // //  float wz = w.z/14.375; // why?
    }
    {
    uint32_t temp = mainTempRead();
    Serial.print("temp: "); Serial.print(temp / premult); Serial.print('.'); Serial.print(temp % premult); Serial.println(" deg");
    }
    {
    MagneticField b = sprite_mag.read();
    Serial.print("mag <x,y,z> ="); 
    Serial.print ("<"); Serial.print(b.x);
    Serial.print (","); Serial.print(b.y);
    Serial.print (","); Serial.print(b.z);
    Serial.println (">");
    // delay(250);
    }
#endif // TARGET_BUILD_DEBUG
#endif // SPRITE

#ifdef TARGET_BUILD_DEBUG
#ifndef SPRITE // Sprite serial input doesn't look like it's working anyway
    if (debug == 'r' || debug == 'R') {
        g_index = 0;
        saveIndex(g_index);
        g_count = 0;
        Serial.print("index = ");
        Serial.println(g_index);
    }
#endif
#endif

#ifdef SPRITE
    handle = true;
#else
    handle = false;
#endif
 
#ifdef TARGET_BUILD_DEBUG
#ifndef SPRITE  // Sprite serial input problems
    if (debug == 's' || debug == 'S') {
        handle = true;
    }
#endif
#else
#ifndef SPRITE
    if (digitalRead(PIN_MODE_SELECT_MSB) == HIGH) {
        handle = true;
    }
#endif
#endif
    if (handle) {
        if (!g_sensor) {
            samplingSensorHot();
            buffer[0] = g_gyro_x >> 7;
            buffer[1] = g_gyro_y >> 7;
            buffer[2] = g_gyro_z >> 7;
            buffer[3] = g_battery_c >> 7;
            buffer[4] = g_battery_v >> 7;
            buffer[5] = g_rssi >> 7;
            buffer[6] = g_board_t >> 7;
            __debug__(Morikawa.writeSharedMemory(SMEM_GYRO_X, buffer, SMEM_BOARD_TEMPERATURE - SMEM_GYRO_X + 1), 30);

#ifdef TARGET_BUILD_DEBUG
            Serial.print("sensor = 0x");
            for (i = 0; i < 6; ++i) {
                Serial.print(static_cast<unsigned char>(buffer[i]), HEX);
                Serial.print(", 0x");
            }
            Serial.println(static_cast<unsigned char>(buffer[6]), HEX);
#endif
            g_sensor = true;
        }
    }
#ifndef SPRITE
    else if (g_sensor) {
        samplingSensorCold();
        g_sensor = false;
    }
#endif
    handle = false;
#ifdef TARGET_BUILD_DEBUG
#ifndef SPRITE // Sprite serial input not working?
    if (debug == 'p' || debug == 'P') {
        handle = true;
    }
#endif
#ifdef TARGET_DEBUG_AUTO
    if (millis() / 15000 % 2) {
        handle = true;
    }
#endif
#else
#ifndef SPRITE
    if (digitalRead(PIN_MODE_SELECT_LSB) == HIGH) {
        handle = true;
    }
#endif
#endif

#ifdef SPRITE
   Serial.print ("g_count="); Serial.println (g_count);
#endif

#ifdef SPRITE
   handle = true;
   g_poem = false;
#endif

    if (handle) {
        if (!g_poem) {
            memset(buffer, 0, sizeof(buffer));
            switch (g_count) {
                case 0:
                    makeBaudotFixed(buffer, PSTR("jqF1Lznn"));
                    break;
                case 1:
                    makeBaudotState(buffer, g_index << 5, g_board_t, g_rssi, g_battery_v);
                    break;
                case 2:
                    remapRange(g_tx_t, LIMIT_TEMPERATURE_MINIMUM, LIMIT_TEMPERATURE_MAXIMUM, &lvalue, &lvalid);
                    remapRange(g_keel_t, LIMIT_TEMPERATURE_MINIMUM, LIMIT_TEMPERATURE_MAXIMUM, &rvalue, &rvalid);
                    makeBaudotColor(buffer, lvalue, lvalid, rvalue, rvalid);
                    break;
                case 3:
                    remapRange(g_battery_t, LIMIT_TEMPERATURE_MINIMUM, LIMIT_TEMPERATURE_MAXIMUM, &lvalue, &lvalid);
                    remapRange(g_wall_t, LIMIT_TEMPERATURE_MINIMUM, LIMIT_TEMPERATURE_MAXIMUM, &rvalue, &rvalid);
                    makeBaudotColor(buffer, lvalue, lvalid, rvalue, rvalid);
                    break;
                case 4:
                    remapRange(g_gyro_x, LIMIT_GYRO_MINIMUM, LIMIT_GYRO_MAXIMUM, &lvalue, &lvalid);
                    remapRange(g_gyro_y, LIMIT_GYRO_MINIMUM, LIMIT_GYRO_MAXIMUM, &rvalue, &rvalid);
                    makeBaudotRhythm(buffer, lvalue, lvalid, rvalue, rvalid);
                    break;
                case 5:
                    remapRange(g_gyro_z, LIMIT_GYRO_MINIMUM, LIMIT_GYRO_MAXIMUM, &lvalue, &lvalid);
                    remapRange(g_battery_c, LIMIT_CURRENT_MINIMUM, LIMIT_CURRENT_MAXIMUM, &rvalue, &rvalid);
                    makeBaudotRhythm(buffer, lvalue, lvalid, rvalue, rvalid);
                    break;
                case 6:
                    makeBaudotFixed(buffer, PSTR("artsatF2"));
                    break;
                case 7:
                    makeBaudotFixed(buffer, PSTR("despatch"));
                    break;
                default:
                    // nop
                    break;
            }
            address = (static_cast<unsigned long>(g_index) << 8) | (static_cast<unsigned long>(g_count) << 4);
#ifdef SPRITE
#ifdef USE_SPRITE_RADIO
	    sprite_radio.transmit(buffer, sizeof(buffer));
#endif
#else
            __debug__(Morikawa.writeSharedMemory(address, buffer, sizeof(buffer)), 31);
#endif

#ifdef TARGET_BUILD_DEBUG
            Serial.print("poem [0x");
            Serial.print(address, HEX);
            Serial.print("] = ");
            for (i = 0; i < lengthof(buffer); ++i) {
                if (0 <= buffer[i] && buffer[i] < 32) {
                    Serial.print("<0x");
                    Serial.print(static_cast<unsigned char>(buffer[i]), HEX);
                    Serial.print(">");
                }
                else {
                    Serial.print(buffer[i]);
                }
            }
            Serial.println("");
#endif
            switch (g_count) {
                case 0:
                    samplingSensorHot();
#ifdef TARGET_USE_RADIO
                    samplingRadio();
#endif
                    break;
                default:
                    // nop
                    break;
            }
            if (++g_count >= COUNT_LIMIT) {
                if (++g_index >= INDEX_LIMIT) {
                    g_index = 0;
                }
#ifndef SPRITE
                saveIndex(g_index); // no index saving working in Sprites (yet)
#endif
                g_count = 0;
            }
            g_poem = true;
        }
    }
#ifndef SPRITE  // handle == true means we never get here
    else if (g_poem) {
        switch (g_count) {
            case 0:
                samplingSensorCold();
                break;
            default:
                // nop
                break;
        }
        g_poem = false;
    }
#endif
    return;   
}

void delay10mTimes(unsigned int times)
{
    for (; times > 0; --times) {
        delay(10);
        Morikawa.loop();
    }
    return;
}

#ifndef SPRITE
static void samplingSensorCold(void)
{
    int value[AVERAGE_SIZE];
    int i;
    
    for (i = 0; i < AVERAGE_SIZE; ++i) {
        delay(WAIT_ANALOGREAD);
        value[i] = REMAP(analogRead(PIN_RSSI), RANGE_RSSI_MINIMUM, RANGE_RSSI_MAXIMUM, 0, 32767);
#ifdef TARGET_BUILD_DEBUG
        Serial.print("    analogRead RSSI = ");
        Serial.println(value[i]);
#endif
    }
    g_rssi = average(value);
    return;
}
#endif

static void samplingSensorHot(void)
{
    int value[6][AVERAGE_SIZE];
    int i;
    
    for (i = 0; i < 5; ++i) {
        clear(value[i]);
    }
    for (i = 0; i < AVERAGE_SIZE; ++i) {
        delay(WAIT_LM75A);
        if (g_valid & VALID_LM75A_BOARD) {
            __debug__(getLM75A(I2C_LM75A_BOARD, &value[0][i]), 50);
        }
        if (g_valid & VALID_LM75A_TX) {
            __debug__(getLM75A(I2C_LM75A_TX, &value[1][i]), 51);
        }
        if (g_valid & VALID_LM75A_BATTERY) {
            __debug__(getLM75A(I2C_LM75A_BATTERY, &value[2][i]), 52);
        }
        if (g_valid & VALID_LM75A_WALL) {
            __debug__(getLM75A(I2C_LM75A_WALL, &value[3][i]), 53);
        }
        if (g_valid & VALID_LM75A_KEEL) {
            __debug__(getLM75A(I2C_LM75A_KEEL, &value[4][i]), 54);
        }
#ifdef TARGET_BUILD_DEBUG
#if 000
        Serial.print("    LM75A Board, TX, Battery, Wall, Keel = ");
        Serial.println(value[0][i]);
        // Serial.print(", TX = ");
        Serial.println(value[1][i]);
        // Serial.print(", Battery = ");
        Serial.println(value[2][i]);
        // Serial.print(", Wall = ");
        Serial.println(value[3][i]);
        // Serial.print(", Keel = ");
        Serial.println(value[4][i]);
#endif
#endif
    }
    g_board_t = average(value[0]);
    g_tx_t = average(value[1]);
    g_battery_t = average(value[2]);
    g_wall_t = average(value[3]);
    g_keel_t = average(value[4]);
    for (i = 0; i < 6; ++i) {
        clear(value[i]);
    }
    for (i = 0; i < AVERAGE_SIZE; ++i) {
        delay(WAIT_MPU6500);
        if (g_valid & VALID_MPU6500) {
            __debug__(getMPU6500(I2C_MPU6500, &value[0][i], &value[1][i], &value[2][i], &value[3][i], &value[4][i], &value[5][i]), 60);
        }
#ifdef TARGET_BUILD_DEBUG
#if 000
        Serial.println("    MPU6500 Accel X,Y,Z, gyro X/Y/Z = ");
        Serial.println(value[0][i]);
        // Serial.print(", Y = ");
        Serial.println(value[1][i]);
        // Serial.print(", Z = ");
        Serial.println(value[2][i]);
        // Serial.print(", Gyro X = ");
        Serial.println(value[3][i]);
        // Serial.print(", Y = ");
        Serial.println(value[4][i]);
        // Serial.print(", Z = ");
        Serial.println(value[5][i]);
#endif
#endif
    }
    g_gyro_x = average(value[3]);
    g_gyro_y = average(value[4]);
    g_gyro_z = average(value[5]);
    for (i = 0; i < 2; ++i) {
        clear(value[i]);
    }
    for (i = 0; i < AVERAGE_SIZE; ++i) {
        delay(WAIT_INA226);
        if (g_valid & VALID_INA226) {
            __debug__(getINA226(I2C_INA226, &value[0][i], &value[1][i]), 70);
        }
#ifdef TARGET_BUILD_DEBUG
#if 000
        Serial.print("    INA226 Voltage = ");
        Serial.print(value[0][i]);
        Serial.print(", Current = ");
        Serial.println(value[1][i]);
#endif
#endif
    }
    g_battery_v = average(value[0]);
    g_battery_c = average(value[1]);
    return;
}

#ifndef SPRITE // no samplingRadio on Sprites (yet)
static void samplingRadio(void)
{
    unsigned long signalCount;
    unsigned long noiseCount;
    bool signalLock;
    bool noiseLock;
    unsigned long base;
    unsigned long time;
    
    __debug__(Morikawa.enablePowerBus(POWER_BUS_Z), 80);
    digitalWrite(PIN_RADIO_SIGNAL, HIGH);
    digitalWrite(PIN_RADIO_NOISE, HIGH);
    delay(DELAY_RADIO);
    signalCount = 0;
    noiseCount = 0;
    signalLock = false;
    noiseLock = false;
    base = millis();
    while (true) {
        time = millis();
        if (time >= base) {
            if (time - base >= INTERVAL_RADIO) {
                break;
            }
        }
        else if (time >= base + INTERVAL_RADIO) {
            break;
        }
        if (!digitalRead(PIN_RADIO_SIGNAL)) {
            if (!signalLock) {
                ++signalCount;
                signalLock = true;
            }
        }
        else {
            signalLock = false;
        }
        if (!digitalRead(PIN_RADIO_NOISE)) {
            if (!noiseLock) {
                ++noiseCount;
                noiseLock = true;
            }
        }
        else {
            noiseLock = false;
        }
    }
#ifdef TARGET_BUILD_DEBUG
    Serial.print("    PGT5 Signal = ");
    Serial.print(signalCount);
    Serial.print(", Noise = ");
    Serial.println(noiseCount);
#endif
    g_radio_s = REMAP(signalCount, RANGE_RADIO_MINIMUM, RANGE_RADIO_MAXIMUM, 0, 32767);
    g_radio_n = REMAP(noiseCount, RANGE_RADIO_MINIMUM, RANGE_RADIO_MAXIMUM, 0, 32767);
    digitalWrite(PIN_RADIO_NOISE, LOW);
    digitalWrite(PIN_RADIO_SIGNAL, LOW);
    Morikawa.disablePowerBus(POWER_BUS_Z);
    return;
}
#endif

static void clear(int param[/*AVERAGE_SIZE*/])
{
    int i;
    
    for (i = 0; i < AVERAGE_SIZE; ++i) {
        param[i] = -1;
    }
    return;
}

static int average(int param[/*AVERAGE_SIZE*/])
{
    long sum;
    int count;
    int i;
    int result(-1);
    
    sum = 0;
    count = 0;
    for (i = 0; i < AVERAGE_SIZE; ++i) {
        if (param[i] >= 0) {
            sum += param[i];
            ++count;
        }
    }
    if (count > 0) {
        result = sum / count;
    }
    return result;
}

#ifndef SPRITE // no index saving on Sprites (yet)
static void saveIndex(unsigned int param)
{
    int i;
    
    for (i = 0; i < 3; ++i) {
        __debug__(Morikawa.writeEEPROM(EMEM_CONTEXT_INDEX + i * sizeof(param), &param, sizeof(param)), 100);
    }
    return;
}

static unsigned int loadIndex(void)
{
    bool valid[3];
    unsigned int data[3];
    int i;
    unsigned int result(0);
    for (i = 0; i < 3; ++i) {
        valid[i] = false;
        if (__debug__(Morikawa.readEEPROM(EMEM_CONTEXT_INDEX + i * sizeof(data[i]), &data[i], sizeof(data[i])), 200) == TSTERROR_OK) {
            if (0 <= data[i] && data[i] < INDEX_LIMIT) {
                valid[i] = true;
            }
            else {
                __debug__(TSTERROR_INVALID_FORMAT, 201);
            }
        }
    }
    if (valid[0] && valid[1] && data[0] == data[1]) {
        result = data[0];
    }
    else if (valid[1] && valid[2] && data[1] == data[2]) {
        result = data[1];
    }
    else if (valid[2] && valid[0] && data[2] == data[0]) {
        result = data[2];
    }
    else if (valid[0]) {
        result = data[0];
    }
    else if (valid[1]) {
        result = data[1];
    }
    else if (valid[2]) {
        result = data[2];
    }
    return result;
}
#endif

static char checkLM75A(int i2c)
{
    TSTError error(TSTERROR_OK);
#ifndef SPRITE
    I2Cm.clear();
    I2Cm.write(0b00000111);
    if (I2Cm.send(i2c, false) == 0) {
        if (I2Cm.receive(i2c, 1, true) >= 1) {
            if (I2Cm.read() != 0xA1) {
                error = TSTERROR_INVALID_FORMAT;
            }
        }
        else {
            error = TSTERROR_FAILED;
        }
    }
    else {
        error = TSTERROR_FAILED;
    }
#endif
    return error;
}

static char checkMPU6500(int i2c)
{
    TSTError error(TSTERROR_OK);
#ifndef SPRITE
    I2Cm.clear();
    I2Cm.write(0x75);
    if (I2Cm.send(i2c, false) == 0) {
        if (I2Cm.receive(i2c, 1, true) >= 1) {
            if (I2Cm.read() != 0x70) {
                error = TSTERROR_INVALID_FORMAT;
            }
        }
        else {
            error = TSTERROR_FAILED;
        }
    }
    else {
        error = TSTERROR_FAILED;
    }
#endif
    return error;
}

static char checkINA226(int i2c)
{
    TSTError error(TSTERROR_OK);
#ifndef SPRITE    
    I2Cm.clear();
    I2Cm.write(0xFF);
    if (I2Cm.send(i2c, false) == 0) {
        if (I2Cm.receive(i2c, 2, true) >= 2) {
            if (I2Cm.read() != 0x22 || I2Cm.read() != 0x60) {
                error = TSTERROR_INVALID_FORMAT;
            }
        }
        else {
            error = TSTERROR_FAILED;
        }
    }
    else {
        error = TSTERROR_FAILED;
    }
#endif
    return error;
}

#ifndef SPRITE
static char calibrateLM75A(int i2c)
{
    TSTError error(TSTERROR_OK);
    
    // power on default is all ok
    return error;
}
#endif

static char calibrateMPU6500(int i2c)
{
    TSTError error(TSTERROR_OK);
    
    // power on default : accel = +-2g, gyro = +-250deg/sec
    return error;
}

static char calibrateINA226(int i2c)
{
    TSTError error(TSTERROR_OK);
#ifndef SPRITE
    I2Cm.clear();
    I2Cm.write(0x00);
    I2Cm.write(0b00000011);
    I2Cm.write(0b00100111);
    if (I2Cm.send(i2c, true) == 0) {
        I2Cm.clear();
        I2Cm.write(0x05);
        I2Cm.write(0x14);
        I2Cm.write(0x00);
        if (I2Cm.send(i2c, true) != 0) {
            error = TSTERROR_FAILED;
        }
    }
    else {
        error = TSTERROR_FAILED;
    }
#endif
    return error;
}

static char getLM75A(int i2c, int* temperature)
{
    short value;
    TSTError error(TSTERROR_OK);
#ifdef SPRITE
     // Same temp sensor no matter what; DESPATCH had several temp sensors.
     // Some noise in these readings though; use the noise somehow?
  if (temperature) {
	unsigned int raw_temp = cc430Read();
        // Serial.print ("raw temp: "); Serial.println (raw_temp);
	// *temperature = mainTempRead();
	*temperature = raw_temp;
  }
#else
    I2Cm.clear();
    I2Cm.write(0b00000000);
    if (I2Cm.send(i2c, false) == 0) {
        if (I2Cm.receive(i2c, 2, true) >= 2) {
            value = ((I2Cm.read() << 8) | (I2Cm.read() << 0)) >> 7;
            if (temperature != NULL) {
                *temperature = REMAP(value, RANGE_TEMPERATURE_MINIMUM, RANGE_TEMPERATURE_MAXIMUM, 0, 32767);
            }
        }
        else {
            error = TSTERROR_FAILED;
        }
    }
    else {
        error = TSTERROR_FAILED;
    }
#endif
    return error;
}

static char getMPU6500(int i2c, int* ax, int* ay, int* az, int* gx, int* gy, int* gz)
{
#ifndef SPRITE
    short value[7];
    int i;
#endif
    TSTError error(TSTERROR_OK);

#ifdef SPRITE
  AngularVelocity w = gyro.read();
# define SETIFNOTNULL(p,v) if (p) *(p) = v

   Serial.print("getMPU6500 faker: gyro <x,y,z>: ");
   Serial.print (": <"); Serial.print(w.x);
   Serial.print (",");  Serial.print (w.y);
   Serial.print (",");  Serial.print (w.z);
   Serial.println(">");
  SETIFNOTNULL(ax,w.x);
  SETIFNOTNULL(ay,w.y);
  SETIFNOTNULL(az,w.z);
  SETIFNOTNULL(gx,w.x);
  SETIFNOTNULL(gy,w.y);
  SETIFNOTNULL(gz,w.z);
#else    
    I2Cm.clear();
    I2Cm.write(0x3B);
    if (I2Cm.send(i2c, false) == 0) {
        if (I2Cm.receive(i2c, 2 * lengthof(value), true) >= 2 * lengthof(value)) {
            for (i = 0; i < lengthof(value); ++i) {
                value[i] = (I2Cm.read() << 8) | (I2Cm.read() << 0);
            }
            if (ax != NULL) {
                *ax = REMAP(value[0], RANGE_ACCEL_MINIMUM, RANGE_ACCEL_MAXIMUM, 0, 32767);
            }
            if (ay != NULL) {
                *ay = REMAP(value[1], RANGE_ACCEL_MINIMUM, RANGE_ACCEL_MAXIMUM, 0, 32767);
            }
            if (az != NULL) {
                *az = REMAP(value[2], RANGE_ACCEL_MINIMUM, RANGE_ACCEL_MAXIMUM, 0, 32767);
            }
            if (gx != NULL) {
                *gx = REMAP(value[4], RANGE_GYRO_MINIMUM, RANGE_GYRO_MAXIMUM, 0, 32767);
            }
            if (gy != NULL) {
                *gy = REMAP(value[5], RANGE_GYRO_MINIMUM, RANGE_GYRO_MAXIMUM, 0, 32767);
            }
            if (gz != NULL) {
                *gz = REMAP(value[6], RANGE_GYRO_MINIMUM, RANGE_GYRO_MAXIMUM, 0, 32767);
            }
        }
        else {
            error = TSTERROR_FAILED;
        }
    }
    else {
        error = TSTERROR_FAILED;
    }
#endif
    return error;
}

static char getINA226(int i2c, int* voltage, int* current)
{
    short bvalue;
    short cvalue;
    TSTError error(TSTERROR_OK);
#ifdef SPRITE    
  if (voltage) *voltage=0;  // TO DO: get Sprite voltage
  if (current) *current=0;  // TO DO: get Sprite current
#else
    I2Cm.clear();
    I2Cm.write(0x02);
    if (I2Cm.send(i2c, false) == 0) {
        if (I2Cm.receive(i2c, 2, true) >= 2) {
            if ((bvalue = (I2Cm.read() << 8) | (I2Cm.read() << 0)) >= 0) {
                I2Cm.clear();
                I2Cm.write(0x04);
                if (I2Cm.send(i2c, false) == 0) {
                    if (I2Cm.receive(i2c, 2, true) >= 2) {
                        cvalue = (I2Cm.read() << 8) | (I2Cm.read() << 0);
                        if (voltage != NULL) {
                            *voltage = REMAP(static_cast<long>(bvalue) * 125 / 100, RANGE_VOLTAGE_MINIMUM, RANGE_VOLTAGE_MAXIMUM, 0, 32767);
                        }
                        if (current != NULL) {
                            *current = REMAP(cvalue, RANGE_CURRENT_MINIMUM, RANGE_CURRENT_MAXIMUM, 0, 32767);
                        }
                    }
                    else {
                        error = TSTERROR_FAILED;
                    }
                }
                else {
                    error = TSTERROR_FAILED;
                }
            }
            else {
                error = TSTERROR_INVALID_FORMAT;
            }
        }
        else {
            error = TSTERROR_FAILED;
        }
    }
    else {
        error = TSTERROR_FAILED;
    }
#endif
    return error;
}

static void remapRange(int value, int minimum, int maximum, int* result, bool* remap)
{
    if (minimum <= value && value <= maximum) {
        *result = REMAP(value, minimum, maximum, 0, 32767);
        *remap = true;
    }
    else {
        *result = value;
        *remap = false;
    }
    return;
}

// static void makeBaudotFixed(char* buffer, char const PROGMEM* value) // g++ tokenization problem?
static void makeBaudotFixed(char* buffer, char const * value)
{
    buffer[0] = 'L';
    strncpy_P(&buffer[1], (value != NULL) ? (value) : (PSTR(INVALID_VALUE_8)), 8);
    buffer[9] = 'N';
    return;
}

static void makeBaudotState(char* buffer, int loop, int temperature, int rssi, int voltage)
{
    buffer[0] = 'L';
    buffer[1] = (loop >> 10) & 0b00011111;
    buffer[2] = (loop >> 5) & 0b00011111;
    buffer[3] = (temperature >> 10) & 0b00011111;
    buffer[4] = (temperature >> 5) & 0b00011111;
    buffer[5] = (rssi >> 10) & 0b00011111;
    buffer[6] = (rssi >> 5) & 0b00011111;
    buffer[7] = (voltage >> 10) & 0b00011111;
    buffer[8] = (voltage >> 5) & 0b00011111;
    buffer[9] = 'N';
    return;
}

static void makeBaudotColor(char* buffer, int lvalue, bool lvalid, int rvalue, bool rvalid)
{
    buffer[0] = 'L';
    if (lvalid) {
        lvalue >>= 9;
        strncpy_P(&buffer[1], (0 <= lvalue && lvalue < lengthof(g_color)) ? (reinterpret_cast<PGM_P>(pgm_read_word(&g_color[lvalue]))) : (PSTR(INVALID_VALUE_4)), 4);
    }
    else {
        buffer[1] =                  0b00000000;
        buffer[2] = (lvalue >> 10) & 0b00011111;
        buffer[3] = (lvalue >>  5) & 0b00011111;
        buffer[4] = (lvalue >>  0) & 0b00011111;
    }
    if (rvalid) {
        rvalue >>= 9;
        strncpy_P(&buffer[5], (0 <= rvalue && rvalue < lengthof(g_color)) ? (reinterpret_cast<PGM_P>(pgm_read_word(&g_color[rvalue]))) : (PSTR(INVALID_VALUE_4)), 4);
    }
    else {
        buffer[5] =                  0b00000000;
        buffer[6] = (rvalue >> 10) & 0b00011111;
        buffer[7] = (rvalue >>  5) & 0b00011111;
        buffer[8] = (rvalue >>  0) & 0b00011111;
    }
    buffer[9] = 'N';
    return;
}

static void makeBaudotRhythm(char* buffer, int lvalue, bool lvalid, int rvalue, bool rvalid)
{
    buffer[0] = 'L';
    if (lvalid) {
        lvalue >>= 8;
        strncpy_P(&buffer[1], (0 <= lvalue && lvalue < lengthof(g_rhythm)) ? (reinterpret_cast<PGM_P>(pgm_read_word(&g_rhythm[lvalue]))) : (PSTR(INVALID_VALUE_4)), 4);
    }
    else {
        buffer[1] =                  0b00000000;
        buffer[2] = (lvalue >> 10) & 0b00011111;
        buffer[3] = (lvalue >>  5) & 0b00011111;
        buffer[4] = (lvalue >>  0) & 0b00011111;
    }
    if (rvalid) {
        rvalue >>= 8;
        strncpy_P(&buffer[5], (0 <= rvalue && rvalue < lengthof(g_rhythm)) ? (reinterpret_cast<PGM_P>(pgm_read_word(&g_rhythm[rvalue]))) : (PSTR(INVALID_VALUE_4)), 4);
    }
    else {
        buffer[5] =                  0b00000000;
        buffer[6] = (rvalue >> 10) & 0b00011111;
        buffer[7] = (rvalue >>  5) & 0b00011111;
        buffer[8] = (rvalue >>  0) & 0b00011111;
    }
    buffer[9] = 'N';
    return;
}
