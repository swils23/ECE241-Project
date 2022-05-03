volatile int encoderPosition; // volatile - can change at any time

void EncoderMonitorA(){ // ISR A
    if (digitalRead(2) == digitalRead(3)) encoderPosition++;
    else encoderPosition--;
}

void EncoderMonitorB(){ // ISR B
    if (digitalRead(2) == digitalRead(3)) encoderPosition--;
    else encoderPosition++;
}

void EncoderInitialize(){ 
    pinMode(2, INPUT); // pin 2 INPUT
    pinMode(3, INPUT); // pin 3 INPUT
    attachInterrupt(digitalPinToInterrupt(2), EncoderMonitorA, CHANGE); // Interrupt A
    attachInterrupt(digitalPinToInterrupt(3), EncoderMonitorB, CHANGE); // Interrupt B
}
