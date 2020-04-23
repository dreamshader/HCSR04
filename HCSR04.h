/*
 ***************************************************************************
 * HCSR04.h ultrasonic sensor class
 ***************************************************************************
 *
 *   Copyright (C) 2020 Dreamshader (aka Dirk Schanz)
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 ***************************************************************************
 *
 * -------- brief description ---------------------------------------------
 *
 *   contains class and other definition for HCSR04 ultrasonic sensor
 *
 * -------- Todo list -----------------------------------------------------
 *
 * -------- Known bugs ----------------------------------------------------
 *
 * -------- History -------------------------------------------------------
 *
 * 2020/04/23: initial version
 *
 ***************************************************************************
*/

/* ************************************ ************************************ */

#ifndef _HC_SR04_H_
#define _HC_SR04_H_

#include "Arduino.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif


#define HCSR04_RUNSTATE_UNKOWN           0
#define HCSR04_RUNSTATE_CREATED          1
#define HCSR04_RUNSTATE_RUNNING          2
#define HCSR04_RUNSTATE_PAUSED           3
#define HCSR04_RUNSTATE_STOPPED          4
#define HCSR04_RUNSTATE_ENDED            5

#define HCSR04_DISTANCE_UNDEF           -1
#define HCSR04_DISTANCE_RED_FROM         1   // HC-SR04 minimum is 20 mm
#define HCSR04_DISTANCE_RED_TO          30
#define HCSR04_DISTANCE_YELLOW_FROM     30
#define HCSR04_DISTANCE_YELLOW_TO       50
#define HCSR04_DISTANCE_GREEN_FROM      50
#define HCSR04_DISTANCE_GREEN_TO       200

#define HCSR04_LED_ON                 HIGH
#define HCSR04_LED_OFF                 LOW

#define HCSR04_NO_LED                   -1

#define HCSR04_ERR_OK                    0

#define HCSR04_ERR_BASE                -150
#define HCSR04_ERR_NULLP        (HCSR04_ERR_BASE - 1)
#define HCSR04_ERR_RUNSTATE     (HCSR04_ERR_BASE - 2)
#define HCSR04_ERR_DISTANCES    (HCSR04_ERR_BASE - 3)


class HCSR04 {

    public:
        HCSR04( int triggerPin, int echoPin, int redPin = HCSR04_NO_LED, 
                 int yellowPin = HCSR04_NO_LED, int greenPin = HCSR04_NO_LED );

        int pause(void);
        int run(void);
        int stop(void);
        float ping(void);
        int setGreenRange( short from, short to );
        int setYellowRange( short from, short to );
        int setRedRange( short from, short to );

        void signalOn(void);
        void signalOff(void);
        bool signal(void);

    private:
        void checkRange( float distance );
        bool isInRange( short distance, short from, short to );
        bool distancesSet( void );

    protected:
        int   _runState;

        bool  _signal;
        int   _pinTrigger;
        int   _pinEcho;

        int   _pinRedSignal;
        int   _pinYellowSignal;
        int   _pinGreenSignal;

        short _rangeRedFrom;
        short _rangeRedTo;
        short _rangeYellowFrom;
        short _rangeYellowTo;
        short _rangeGreenFrom;
        short _rangeGreenTo;
};



#ifdef __cplusplus
}
#endif

#endif // _HC_SR04_H_
