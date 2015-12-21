//--------------------------------------------------------------------------
// C++ Source file -- RXListener.cpp
// @author Joshua MacVey
//--------------------------------------------------------------------------

RXListener::RXListener(int rfRxPin, int baudRate, int dataSize, 
		       int upperThreshold, int lowerThreshold): 
		 rfRxPin(rfRxPin), baudRate(baudRate),
		 dataSize(dataSize),
	 	 upperThreshold(upperTheshold), lowerTheshold(lowerThreshold) {
		 Serial.begin(baudRate);
}

RXListener::~RXListener() {}

void RXListener::printSignal(byte* dataHigh, byte* dataLow) {
	Serial.println("HIGH, LOW");
	for (int i = 0; i < dataSize; i++){
		Serial.print(dataHigh[i]);
		Serial.print(",");
		Serial.println(dataLow[i]);
	}
}

int RXListener::calibrateValue(int value) {
	if (value > 45 && value < 55) {
		return 50;
	}
	else if (value > 6) {
		return 1; 
		}
	else {
		return 0;
	}		
}


boolean RXListener::isMatch(byte* data, char highLow, char inOut) {
	switch(highLow) {
	case 'L':
		switch (inOut) {
			case 'I':
				return isMatchHelper(data, lowIn);
			case 'O':
				return isMatchHelper(data, lowOut);
		}
		break;
	case 'H':
		switch (inOut) {
			case 'I':
				return isMatchHelper(data, highIn);
			case 'O':
				return isMatchHelper(data, highOut);
		}
		break;
	}
	return false;
}

void RXListener::sampleSignal(byte* dataHigh, byte* dataLow)
{
//Read and store the rest of the signal into the storedData array
int dataCounter = 0;
  for(int i=0; i<dataSize; i++){
    
    //length of low
    dataCounter=0; //reset the counter
    while(analogRead(rfRxPin)>upperThreshold && dataCounter<maxSignalLength){
      dataCounter++;
    }
    dataLow[i]=dataCounter;
    
    //length of high
    dataCounter=0;//reset the counter
    while(analogRead(rfRxPin)<lowerThreshold && dataCounter<maxSignalLength){
      dataCounter++;
    }
    dataHigh[i]=dataCounter;
}
	for (int i = 0; i < dataSize; i++) {
	// calibrate the values
	dataLow[i] = calibrateValue(dataLow[i]);
	dataHigh[i] = calibrateValue(dataHigh[i]);
	}
	// for testing purposes
   	//printSignal(dataHigh, dataLow);
	//delay(10000);
}

boolean RXListener::isMatchHelper(byte* data, const byte* patternToMatch) { 
	// 8 bit sequence detector
	int matchIndex = 0;
	int matchCounter = 0;
	for (int i = 0; i < dataSize - 8; i++)
		{

		for (int j = i; j < i + 8; j++)
		    {
			if (data[j] == patternToMatch[matchIndex])
				{matchCounter++;
				 matchIndex++;
			 		if (matchCounter == 8) {
					return true;
					} // end if
				} // end pattern match
			else {
				matchCounter = 0;
				j = i + 8; // exit the j loop
				matchIndex = 0;
			     }
		    } // end j-for
	}		// end i-for
	return false;
}
