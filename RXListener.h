//--------------------------------------------------------------------------
// C++ Header file -- RXListener.h
// @author Joshua MacVey
//--------------------------------------------------------------------------

#ifndef RX_LISTENER_H
#define RX_LISTENER_H

class RXListener {

public:

/** 
 * Constructor.
 * The constructor for this RXListener samples and looks for signals matched with the code.
 * @param rfRxPin radio-frequency receiver pin through which RF signals will be read.
 * @param baudRate the specified baudrate to sample the signal at
 * @param dataSize the size of the data arrays (sampling size)
 * @param upperThreshold the uppper bound of the interval to ignore data.
 * @param lowerThreshold the lower bound of the interval to ignore data.
 */
RXListener(int rfRxPin, int baudRate, int dataSize, 
		int upperThreshold, int lowerThreshold); 

/**
 * Destructor.
 * Cleans up after itself.
 */
~RXListener();

/**
 * Utility method to print high and low data signals to terminal
 * @param dataHigh the high data array
 * @param dataLow the low data array
 */
void printSignal(byte* dataHigh, byte* dataLow);

/**
 * Utility method to calibrate values on RF signal.
 * @param value the value to calibrate.
 * @return the calibrated value.
 */
virtual int calibrateValue(int value);

/**
 * Utility method to test for signal match.
 * @param data reference to array where data is stored.
 * @param highLow the description of data high or low
 * @param inOut description for low and high
 * @return true if data is matched; false otherwise.
 */
virtual boolean isMatch(byte* data, char highLow, char inOut);

/**
 * Utility method to sample data on RF line.
 * After this function is called, the data referenced will contain
 * the sample values.
 * @param data reference to an array where data is stored.
 * @param desc description of the data
 */
virtual void sampleSignal(byte* dataHigh, byte* dataLow);


private:

/**
 * Utility method to help test for signal match
 * @param data the data to be matched
 * @param patternToMatch the pattern to match against
 * @return true if pattern matched. false otherwise
 */
boolean isMatchHelper(byte* data, const byte* patternToMatch);

/** Private Data Fields **/
const int baudRate; // supplied by constructor
const int rfRxPin; // supplied by constructor
const int upperThreshold; // supplied by constructor
const int lowerThreshold; // supplied by constructor
const int maxSignalLength = 255;
int dataCounter = 0;
const int dataSize;
};

#endif
