/*
**      ARTSAT MorikawaSDK
**
**      Original Copyright (C) 2012 - 2012 HORIGUCHI Junshi.
**                                          http://iridium.jp/
**                                          zap00365@nifty.com
**      Portions Copyright (C) 2012 - 2012 OZAKI Naoya.
**                                          naoya.ozaki11@gmail.com
**      Portions Copyright (C) 2012 - 2013 SHINTANI Kodai.
**                                          shinkoko43@gmail.com
**      Portions Copyright (C) 2012 - 2013 TOKITAKE Yuta.
**                                          toki211@gmail.com
**      Portions Copyright (C) 2013 - 2014 HORIGUCHI Junshi.
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
**      TSTMorikawa.cpp
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

#include "TSTMorikawa.h"
#if 0
#include "TimerOne.h"
#include "EEPROM.h"
#include "I2Cm.h"
#include "SPI.h"
#include "fastlz.h"
#include "TSTCriticalSection.h"
#endif

#include "TSTError.h"

#define pgm_read_byte(bp) (* (unsigned char *) (bp))
#define memcmp_P(b1,b2,n) memcmp(b1,b2,n)
extern "C" {

extern unsigned int const __data_start;
extern unsigned int const __data_end;
extern unsigned int const __bss_start;
extern unsigned int const __bss_end;
extern void const* const __brkval;

}
namespace tst {

#define MISSION_MAIN_BAUD   (4800)
#define TIMER_INTERVAL      (80000UL)
#define BOOT_DELAY          (1000)
#define SHUTDOWN_DELAY      (1000)
#define POWERBUS_DELAY      (100)
#define AUDIOBUS_DELAY      (1000)
#define PACKET_SEPARATOR    ('-')
#define PACKET_REQUEST      ('c')
#define PACKET_RESPONSE     ('r')
#define PACKET_POWER        ('p')
#define PACKET_MAIN         ('c')
#define PACKET_MISSION      ('m')
// # -> mode
// ! -> remote
// * -> command
#define PACKET_SEND         ("#-!-m-***")
#define PACKET_RECEIVE      ("#-m-!-***")
#define PACKET_DELIMITER    (" \r\n")
#define PSTR(p) (p)
#define COMMAND_ECHO        (PSTR("eco"))
#define COMMAND_TELEMETRY   (PSTR("smm"))
#define COMMAND_ABNORMALSD  (PSTR("asd"))
#define COMMAND_NORMALSD    (PSTR("nsd"))
#define COMMAND_AUDIOBUSON  (PSTR("don"))
#define COMMAND_AUDIOBUSOFF (PSTR("dof"))

#define UNPROGMEM // until this issue is resolved
#undef OPTION_BUILD_MEMORYLOG
	// until this issue is resolved

static PowerBusType const g_pin[POWER_BUS_LIMIT] UNPROGMEM = {
    PIN_POWER_X,
    PIN_POWER_Y,
    PIN_POWER_Z
};
static char const g_send[] UNPROGMEM = PACKET_SEND;
static char const g_receive[] UNPROGMEM = PACKET_RECEIVE;
static char const g_delimiter[] UNPROGMEM = PACKET_DELIMITER;
#define RAMEND 32767 // until normalized
MemoryInfo TSTMorikawa::_memory = {
    0, RAMEND + 1, 0, RAMEND + 1
};
TSTTrinity<bool> TSTMorikawa::_selftest(false);

/*public static */TSTMorikawa& TSTMorikawa::getInstance(void)
{
    static TSTMorikawa s_singleton;
    
#if defined(OPTION_BUILD_MEMORYLOG)
    saveMemoryLog();
#endif
    return s_singleton;
}

/*public */bool TSTMorikawa::hasAbnormalShutdown(void) const
{
    bool result(false);
    
#if defined(OPTION_BUILD_MEMORYLOG)
    saveMemoryLog();
#endif
    if (_state) {
        result = _shutdown;
    }
    return result;
}

/*public */TSTError TSTMorikawa::setup(void)
{
    TSTError log;
    TSTError error(TSTERROR_OK);
    
#if defined(OPTION_BUILD_MEMORYLOG)
    saveMemoryLog();
#endif
    if (!_state) {
        _state = true;
        _index = 0;
        _overflow = false;
        _page = false;
        _receive = false;
        memset(&_telemetry, 0, sizeof(_telemetry));
        _update = false;
        _shutdown = false;
        _audio = true;
#if 0
        _param.time = 0;
        _param.count = 1;
        _param.mode = 0;
        pinMode(PIN_POWER_X, OUTPUT);
        digitalWrite(PIN_POWER_X, LOW);
        pinMode(PIN_POWER_Y, OUTPUT);
        digitalWrite(PIN_POWER_Y, LOW);
        pinMode(PIN_POWER_Z, OUTPUT);
        digitalWrite(PIN_POWER_Z, LOW);
#endif
#if defined(TARGET_DESPATCH_FM1)
        digitalWrite(PIN_POWER_X, HIGH);
#endif
        delay(POWERBUS_DELAY);
#if 0
        initializeI2C();
        initializeSPI();
        if ((log = _shared.setup(this)) != TSTERROR_OK) {
            writeSelfTestLog(offsetof(SelfTestLog, setup_shared), &log, sizeof(log));
        }
        else if ((log = _shared.readBootParam(&_param)) != TSTERROR_OK) {
            writeSelfTestLog(offsetof(SelfTestLog, setup_param), &log, sizeof(log));
        }
        initializeRandom(_param.time);
        if ((log = _fram.setup(this)) != TSTERROR_OK) {
            writeSelfTestLog(offsetof(SelfTestLog, setup_fram), &log, sizeof(log));
        }
        if ((log = _flash.setup(this)) != TSTERROR_OK) {
            writeSelfTestLog(offsetof(SelfTestLog, setup_flash), &log, sizeof(log));
        }
        if ((log = _led.setup(this)) != TSTERROR_OK) {
            writeSelfTestLog(offsetof(SelfTestLog, setup_led), &log, sizeof(log));
        }
        if ((log = _tone.setup(this)) != TSTERROR_OK) {
            writeSelfTestLog(offsetof(SelfTestLog, setup_tone), &log, sizeof(log));
        }
        if ((log = _digitalker.setup(this)) != TSTERROR_OK) {
            writeSelfTestLog(offsetof(SelfTestLog, setup_digitalker), &log, sizeof(log));
        }
        if ((log = _camera.setup(this)) != TSTERROR_OK) {
            writeSelfTestLog(offsetof(SelfTestLog, setup_camera), &log, sizeof(log));
        }
#endif
#if defined(TARGET_INVADER_EM1) || defined(TARGET_INVADER_FM1)
#if 0
        Serial1.begin(MISSION_MAIN_BAUD);
        disableAudioBus();
        Timer1.initialize(TIMER_INTERVAL);
        Timer1.attachInterrupt(&onTimer);
#endif
#endif
        delay(BOOT_DELAY);
    }
    else {
        error = TSTERROR_INVALID_STATE;
    }
    return error;
}

/*public */void TSTMorikawa::cleanup(void)
{
#if defined(OPTION_BUILD_MEMORYLOG)
    saveMemoryLog();
#endif
    if (_state) {
#if 0
        shareSelfTestLog();
#endif
#if defined(TARGET_INVADER_EM1) || defined(TARGET_INVADER_FM1)
#if 0
        Timer1.stop();
        disableAudioBus();
        Serial1.end();
#endif
#endif
#if 0
        _camera.cleanup();
        _digitalker.cleanup();
        _tone.cleanup();
        _led.cleanup();
        _flash.cleanup();
        _fram.cleanup();
        _shared.cleanup();
        digitalWrite(PIN_POWER_Z, LOW);
        digitalWrite(PIN_POWER_Y, LOW);
        digitalWrite(PIN_POWER_X, LOW);
#endif
        delay(POWERBUS_DELAY);
        _state = false;
    }
    return;
}

/*public static */void TSTMorikawa::shutdown(void)
{
    TSTError log;
    
#if defined(OPTION_BUILD_MEMORYLOG)
    saveMemoryLog();
#endif
    getInstance().cleanup();
#if defined(TARGET_INVADER_EM1) || defined(TARGET_INVADER_FM1)
#if 0
    Serial1.begin(MISSION_MAIN_BAUD);
    if ((log = sendRequest(PACKET_MAIN, COMMAND_AUDIOBUSOFF)) != TSTERROR_OK) {
        writeSelfTestLog(offsetof(SelfTestLog, shutdown_audiobusoff), &log, sizeof(log));
    }
    if ((log = sendRequest(PACKET_MAIN, COMMAND_NORMALSD)) != TSTERROR_OK) {
        writeSelfTestLog(offsetof(SelfTestLog, shutdown_normalsd), &log, sizeof(log));
    }
    Serial1.end();
#endif
#endif
    delay(SHUTDOWN_DELAY);
    while (true) {
#if 0
        set_sleep_mode(SLEEP_MODE_PWR_DOWN);
        sleep_mode();
#endif
    }
    return;
}

/*public */void TSTMorikawa::loop(void)
{
#if 0
    TSTCriticalSection mutex;
#endif
    
#if defined(OPTION_BUILD_MEMORYLOG)
    saveMemoryLog();
#endif
    if (_state) {
#if 0
        mutex.lock();
#endif
        if (_receive) {
            _page = !_page;
            _update = true;
            _receive = false;
        }
#if 0
        mutex.unlock();
#endif
        if (_shutdown) {
            shutdown();
        }
#if 0
        _digitalker.loop();
#endif
#if 0
        shareSelfTestLog();
#endif
    }
    return;
}

/*private static */void TSTMorikawa::initializeRandom(unsigned long time)
{
#if 0
    static PinType const s_pin[] UNPROGMEM = {
        A0,
        A1,
        A2,
        A3,
        A4,
        A5,
        A6,
        A7,
        A8,
        A9,
        A10,
        A11,
        A12,
        A13,
        A14,
        A15
    };
#endif
    register long seed;
#if 0
    register PinType pin;
    register int i;
#endif
    
#if defined(OPTION_BUILD_MEMORYLOG)
    saveMemoryLog();
#endif
    seed = 0;
#if 0
    for (i = 0; i < lengthof(s_pin); ++i) {
        pin = pgm_read_byte(&s_pin[i]);
        pinMode(pin, INPUT);
        seed += analogRead(pin);
    }
#endif
    seed ^= time;
    randomSeed(seed);
    return;
}

}// end of namespace
