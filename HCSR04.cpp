/*
 ***************************************************************************
 * HCSR04.cpp ultrasonic sensor class
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
 *   contains class implementation for HCSR04 ultrasonic sensor
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

#include "HCSR04.h"

/*
 * *************************************************************************
 * HCSR04::HCSR04( int triggerPin, int echoPin, int redPin, 
 *                 int yellowPin, int greenPin )
 * ------------------------------------------------------------------
 * int triggerPin : pin to use as the trigger pin
 * int echoPin    : pin to receive the echo signal
 * int redPin     : pin of the red status LED if any
 * int yellowPin  : pin of the yellow status LED if any
 * int greenPin   : pin of the green status LED if any
 * ------------------------------------------------------------------
 * create an HCSR04 ultrasonic object
 * returns --
 * *************************************************************************
*/
HCSR04::HCSR04( int triggerPin, int echoPin, int redPin, 
                int yellowPin, int greenPin )
{

    if( (_pinTrigger      = triggerPin) != HCSR04_NO_LED )
    {
        pinMode( _pinTrigger, OUTPUT );
    }

    if( (_pinEcho         = echoPin) != HCSR04_NO_LED )
    {
        pinMode( _pinEcho, INPUT );
    }

    if( (_pinRedSignal    = redPin) != HCSR04_NO_LED )
    {
        pinMode( _pinRedSignal, OUTPUT );
    }

    if( (_pinYellowSignal = yellowPin) != HCSR04_NO_LED )
    {
        pinMode( _pinYellowSignal, OUTPUT );
    }

    if( (_pinGreenSignal  = greenPin) != HCSR04_NO_LED )
    {
        pinMode( _pinGreenSignal, OUTPUT );
    }

    signalOff();

    setGreenRange( HCSR04_DISTANCE_GREEN_FROM, HCSR04_DISTANCE_GREEN_TO );
    setYellowRange( HCSR04_DISTANCE_YELLOW_FROM, HCSR04_DISTANCE_YELLOW_TO );
    setRedRange( HCSR04_DISTANCE_RED_FROM, HCSR04_DISTANCE_RED_TO );

    signalOn();

    _runState = HCSR04_RUNSTATE_CREATED;

}

/*
 * *************************************************************************
 * int HCSR04::pause(void)
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * set runstate to HCSR04_RUNSTATE_PAUSED
 * returns new runstate
 * *************************************************************************
*/
int HCSR04::pause(void)
{
    int retVal = HCSR04_ERR_OK;

    retVal = (_runState = HCSR04_RUNSTATE_PAUSED);

    return( retVal );
}

/*
 * *************************************************************************
 * int HCSR04::run(void)
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * set runstate to HCSR04_RUNSTATE_RUNNING
 * returns new runstate
 * *************************************************************************
*/
int HCSR04::run(void)
{
    int retVal = HCSR04_ERR_OK;

    signalOn();
    retVal = (_runState = HCSR04_RUNSTATE_RUNNING);

    return( retVal );
}

/*
 * *************************************************************************
 * int HCSR04::stop(void)
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * set runstate to HCSR04_RUNSTATE_STOPPED
 * returns new runstate
 * *************************************************************************
*/
int HCSR04::stop(void)
{
    int retVal = HCSR04_ERR_OK;

    retVal = (_runState = HCSR04_RUNSTATE_STOPPED);

    return( retVal );
}

/*
 * *************************************************************************
 * bool HCSR04::distancesSet( void )
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * check whether distances are set
 * returns true if distances are set and false otherwise
 * *************************************************************************
*/
bool HCSR04::distancesSet( void )
{
    bool retVal = false;

    if( _rangeRedFrom    != HCSR04_DISTANCE_UNDEF && _rangeRedFrom    > 0 &&
        _rangeRedTo      != HCSR04_DISTANCE_UNDEF && _rangeRedTo      > 0 &&
        _rangeYellowFrom != HCSR04_DISTANCE_UNDEF && _rangeYellowFrom > 0 &&
        _rangeYellowTo   != HCSR04_DISTANCE_UNDEF && _rangeYellowTo   > 0 &&
        _rangeGreenFrom  != HCSR04_DISTANCE_UNDEF && _rangeGreenFrom  > 0 &&
        _rangeGreenTo    != HCSR04_DISTANCE_UNDEF && _rangeGreenTo    > 0 )
    {
        retVal = true;
    }

    return( retVal );
}

/*
 * *************************************************************************
 * bool HCSR04::isInRange( short distance, short from, short to )
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * check whether a value is in range
 * returns true if in range, false otherwise
 * *************************************************************************
*/
bool HCSR04::isInRange( short distance, short from, short to )
{
    bool retVal = false;

    if( from <= to )    // from is less or equal to
    {
        if( distance >= from && distance <= to )
        {
            retVal = true;
        }
    }
    else
    {
        if( distance <= from && distance >= to )
        {
            retVal = true;
        }
    }

    return( retVal );
}

/*
 * *************************************************************************
 * void HCSR04::checkRange( float distance )
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * check distance in which range it is and signal it
 * returns --
 * *************************************************************************
*/
void HCSR04::checkRange( float distance )
{
    if( _signal )
    {
        short checkValue = (short) distance;

        if( isInRange( checkValue, _rangeRedFrom, _rangeRedTo ))
        {
            digitalWrite(_pinRedSignal, HCSR04_LED_ON);
            digitalWrite(_pinYellowSignal, HCSR04_LED_OFF);
            digitalWrite(_pinGreenSignal, HCSR04_LED_OFF);
        }
        else
        {
            if( isInRange( checkValue, _rangeYellowFrom, _rangeYellowTo ))
            {
                digitalWrite(_pinRedSignal, HCSR04_LED_OFF);
                digitalWrite(_pinYellowSignal, HCSR04_LED_ON);
                digitalWrite(_pinGreenSignal, HCSR04_LED_OFF);
            }
            else
            {
                if( isInRange( checkValue, _rangeGreenFrom, _rangeGreenTo ))
                {
                    digitalWrite(_pinRedSignal, HCSR04_LED_OFF);
                    digitalWrite(_pinYellowSignal, HCSR04_LED_OFF);
                    digitalWrite(_pinGreenSignal, HCSR04_LED_ON);
                }
            }
        }
    }
    else
    {
        digitalWrite(_pinRedSignal, HCSR04_LED_OFF);
        digitalWrite(_pinYellowSignal, HCSR04_LED_OFF);
        digitalWrite(_pinGreenSignal, HCSR04_LED_OFF);
    }
}

/*
 * *************************************************************************
 * float HCSR04::ping(void)
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * send a ping an calculate the distance from an object, if any
 * returns the distance in mm
 * *************************************************************************
*/
float HCSR04::ping(void)
{
    float retVal;
    long duration;

    if( _runState == HCSR04_RUNSTATE_RUNNING )
    {
        digitalWrite(_pinTrigger, LOW);     //
        delayMicroseconds(2);               //
        digitalWrite(_pinTrigger, HIGH);    //
//
        delayMicroseconds(10);              //
        digitalWrite(_pinTrigger, LOW);     //
        duration = pulseIn(_pinEcho, HIGH); //

//        retVal = (dauer/2) * 0.3432;        // mm
//        retVal = (duration/2) / 2.91;        // mm

        retVal = (duration/2) * 0.3432; // mm
    }
    else
    {
        retVal = (float) HCSR04_ERR_RUNSTATE;
    }

    checkRange( retVal );

    return(retVal); // return calculated distance in mm
}

/*
 * *************************************************************************
 * int HCSR04::setGreenRange( short from, short to )
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * set the green range
 * returns --
 * *************************************************************************
*/
int HCSR04::setGreenRange( short from, short to )
{
    int retVal = HCSR04_ERR_OK;

    _rangeGreenFrom = from;
    _rangeGreenTo   = to;

    return(retVal);
}

/*
 * *************************************************************************
 * int HCSR04::setYellowRange( short from, short to )
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * set the yellow range
 * returns --
 * *************************************************************************
*/
int HCSR04::setYellowRange( short from, short to )
{
    int retVal = HCSR04_ERR_OK;

    _rangeYellowFrom = from;
    _rangeYellowTo   = to;

    return(retVal);
}

/*
 * *************************************************************************
 * int HCSR04::setRedRange( short from, short to )
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * set the red (critical) range
 * returns --
 * *************************************************************************
*/
int HCSR04::setRedRange( short from, short to )
{
    int retVal = HCSR04_ERR_OK;

    _rangeRedFrom = from;
    _rangeRedTo   = to;

    return(retVal);
}

/*
 * *************************************************************************
 * void HCSR04::signalOn(void)
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * turn signalling on
 * returns --
 * *************************************************************************
*/
void HCSR04::signalOn(void)
{
    
    _signal = distancesSet();
}

/*
 * *************************************************************************
 * void HCSR04::signalOff(void)
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * turn signalling off
 * returns --
 * *************************************************************************
*/
void HCSR04::signalOff(void)
{
    _signal = false;
}

/*
 * *************************************************************************
 * bool HCSR04::signal(void)
 * ------------------------------------------------------------------
 * 
 * ------------------------------------------------------------------
 * get signalling status
 * returns signal on or off (true resp. false)
 * *************************************************************************
*/
bool HCSR04::signal(void)
{
    return(_signal);
}

/* ************************************ ************************************ */

