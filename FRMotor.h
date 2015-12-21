#ifndef FR_MOTOR_H
#define FR_MOTOR_H

#include "Arduino.h"

class FRMotor {
public:
/** 
 * Constructor.
 * The constructor for this motor relay sets necessary pins
 * on the Arduino.
 * @param rfRxPin radio-frequency receiver pin through which RF signals will be read.
 * @param lowIn low-length in code
 * @param lowOut low-length out code
 * @param highOut high-length out code
 * @param highIn high-length in code
 * @param baudRate the specified baudrate to sample the signal at
 * @param dataSize the size of the data arrays (sampling size)
 * @param upperThreshold the uppper bound of the interval to ignore data.
 * @param lowerThreshold the lower bound of the interval to ignore data.
 */
FRMotor(int rfRxPin, const byte* lowIn, const byte* lowOut,
	const byte* highOut, const byte* highIn, int baudRate, int dataSize, 
	int upperThreshold, int lowerThreshold);

/**
 * Destructor.
 * Cleans up after itself.
 */
~FRMotor();

/**
 * Sets motor to out.
 * Channel "A" is arbitrarily defined to be "out" on the motor.
 */
void setMotorOut();

/**
 * Sets motor to in
 * Channel "B" is arbitrarily defined to be "in" on the motor.
 */
void setMotorIn();

/**
 * Idles the motor.
 */
void idleMotor();

/**
 * Opens electrical path for a channel on the Motor Shield.
 * @param channel the channel to be opened.
 */
void start_ch(char channel);

/**
 * Closes electrical path for a channel on the motor shield.
 * @param channel the channel to be closed.
 */
void stop_ch(char channel);

/**
 * Sets the speed for a channel on the motor shield.
 * @param channel the channel whose speed is to be set.
 * @param speed value ranging from 0-255
 */
void setSpeed(char channel, int speed);

/**
 * Listens for an RF signal and sets motor to forward
 * or reverse based on the signal value.
 */
void listenRX();

void testLoop();

private:
/**
 * Utility method to calibrate values on RF signal.
 * @param value the value to calibrate.
 * @return the calibrated value.4
 */
int calibrateValue(int value);

/**
 * Utility method to sample data on RF line.
 * After this function is called, the data referenced will contain
 * the sample values.
 * @param data reference to an array where data is stored.
 * @param desc description of the data
 */
void sampleSignal(byte* dataHigh, byte* dataLow);

/**
 * Utility method to test for signal match.
 * @param data reference to array where data is stored.
 * @param highLow the description of data high or low
 * @param inOut description for low and high
 * @return true if data is matched; false otherwise.
 */
boolean isMatch(byte* data, char highLow, char inOut);

/**
 * Utility method to help test for signal match
 * @param data the data to be matched
 * @param signalToMatch the pattern to match against
 * @return true if pattern matched. false otherwise
 */
boolean isMatchHelper(byte* data, const byte* patternToMatch);

/**
 * Utility method to test for idle motor.
 * @param lowData the low data set
 * @param highData the high data set
 * @return true if idle, false otherwise.
 */
boolean isIdle(byte* lowData, byte* highData);

/** Private Data Fields **/
const int ledPin = 13;
const int rfRxPin; // supplied by constructor
const int ch_A_Pin = 12;
const int ch_A_Brake = 9;
const int ch_A_Speed = 3;
const int ch_B_Pin = 13;
const int ch_B_Brake = 8;
const int ch_B_Speed = 11;

// signal codes
const byte* lowIn;
const byte* lowOut;
const byte* highOut;
const byte* highIn;

// flags for checking motor
boolean ch_A_On = false;
boolean ch_B_On = false;

// RX class to handle remote signals
RXListener rxManager;
};

#endif
