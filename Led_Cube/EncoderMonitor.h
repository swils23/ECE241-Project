volatile int encoderPosition; // can change at any time

void EncoderMonitorA(){ // ISR A - Clockwise rotation
    if (digitalRead(2) == digitalRead(3)) encoderPosition++;
    else encoderPosition--;
}

void EncoderMonitorB(){ // ISR B - Counter-clockwise rotation
    if (digitalRead(2) == digitalRead(3)) encoderPosition--;
    else encoderPosition++;
}

void EncoderInitialize(){  // Initialize the encoder monitor
    pinMode(2, INPUT); // pin 2 INPUT
    pinMode(3, INPUT); // pin 3 INPUT
    attachInterrupt(digitalPinToInterrupt(2), EncoderMonitorA, CHANGE); // Interrupt A
    attachInterrupt(digitalPinToInterrupt(3), EncoderMonitorB, CHANGE); // Interrupt B
}
