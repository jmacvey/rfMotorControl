//--------------------------------------------------------------------------
// C++ Source File - frMotor.cpp
// @author Joshua MacVey
//--------------------------------------------------------------------------

#include "FRMotor.h"


FRMotor::FRMotor(int rfRxPin, const byte* lowIn, const byte* lowOut,
	const byte* highOut, const byte* highIn, int baudRate, int dataSize,
	int upperThreshold, int lowerThreshold): rfRxPin(rfRxPin),
	lowIn(lowIn), lowOut(lowOut), highOut(highOut), highIn(highIn) {
// initialize pin outputs
pinMode(ch_A_Pin, OUTPUT);
pinMode(ch_A_Brake, OUTPUT);
pinMode(ch_B_Pin, OUTPUT);
pinMode(ch_B_Brake, OUTPUT);
pinMode(ledPin, OUTPUT);
rxManager = new RXListener(rfRxPin, baudRate, dataSize, upperThreshold, lowerThreshold);
}

FRMotor::~FRMotor() {
	delete[] lowIn;
	delete[] lowOut;
	delete[] highOut;
	delete[] highIn;
}

void FRMotor::start_ch(char channel) {

if (channel == 'A'){
	// forward direction
	digitalWrite(ch_A_Pin, HIGH);
	// disengage brake
	digitalWrite(ch_A_Brake, LOW);
	setSpeed('A', 255);
	}
else if (channel == 'B'){
	// forward direction
	digitalWrite(ch_B_Pin, HIGH);
	// disengage brake
	digitalWrite(ch_B_Brake, LOW);
	setSpeed('B', 255);
	}
}

void FRMotor::stop_ch(char channel) {

if (channel == 'A') {
	// engage brake
	digitalWrite(ch_A_Brake, HIGH);
	digitalWrite(ch_A_Speed, 0);
	digitalWrite(ch_A_Pin, LOW);
	}
else if (channel == 'B') {
	// engage brake
	digitalWrite(ch_B_Brake, HIGH);
	digitalWrite(ch_B_Speed, 0);
	digitalWrite(ch_B_Pin, LOW);
	}
}

void FRMotor::setSpeed(char channel, int speed)
{
if (channel == 'A') {
	digitalWrite(ch_A_Speed, speed);
	}
else if (channel == 'B') {
	digitalWrite(ch_B_Speed, speed);
	}
}

void FRMotor::listenRX() {
	byte highData[dataSize];
	byte lowData[dataSize];
	// sample the signal
	digitalWrite(ledPin, HIGH);
	rxManager->sampleSignal(highData, lowData);
	// get flags for signal receipts.
	boolean idle = isIdle(lowData, highData);
	
	// change the output based on boolean flags.
	if (!idle)
		{
		//printSignal(highData, lowData);
		boolean highOutMatch = rxManager->isMatch(highData, 'H', 'O');
		boolean lowOutMatch = rxManager->isMatch(lowData, 'L', 'O');
		boolean highInMatch = rxManager->isMatch(highData, 'H', 'I');
		boolean lowInMatch = rxManager->isMatch(lowData, 'L', 'I');

		if (highOutMatch && lowOutMatch){
			setMotorOut();
			} // end if
		else if (highInMatch && lowInMatch) {
			setMotorIn();
			} // end if
	}

	else {
			idleMotor();
	}
}


void FRMotor::idleMotor() {
	stop_ch('A');
	stop_ch('B');
	ch_B_On = false;
	ch_A_On = false;
}

void FRMotor::setMotorOut() {
	stop_ch('B');
	ch_B_On = false;
	delay(100);
	if (!ch_A_On){
		start_ch('A');
		ch_A_On = true;}
}

void FRMotor::setMotorIn() {
	stop_ch('A');
	ch_A_On = false;
	delay(100);
	if (!ch_B_On){
		start_ch('B');
		ch_B_On = true;}
}

boolean FRMotor::isIdle(byte* lowData, byte* highData) {
	for (int i = 0; i < dataSize; i++) {
		if (lowData[i] != 0) {
			return false;
		} // end if
	}// end for
	return true;
}

void FRMotor::testLoop() {
start_ch('A');
delay(2000);
stop_ch('A');
delay(2000);
start_ch('B');
delay(2000);
stop_ch('B');
delay(2000);
}



