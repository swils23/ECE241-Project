enum state {ButtonIdle, ButtonWait, ButtonLow};
state ButtonState;
unsigned long ButtonTime;

int ButtonInitialize(){
    pinMode(4, INPUT); // Button Input
    ButtonState = ButtonIdle;
    pinMode(13, OUTPUT); // Debug LED
}

int ButtonNextState(int InputPin){
    switch (ButtonState){
    // Nothing happening
        case ButtonIdle:
            if (InputPin == LOW){
                ButtonTime = millis();
                ButtonState = ButtonWait;
                PORTB |= 0x20; // DBG; LED ON
            }
            break;

    // Pressed (wait for 5ms)
        case ButtonWait:
            if (InputPin == HIGH){ // not pressed
                ButtonState = ButtonIdle; // idle
                PORTB &= ~0x20; // DBG; LED OFF
            } else if (millis() - ButtonTime >= 5){ // pressed for 5ms
                ButtonState = ButtonLow; // pressed
                PORTB &= ~0x20; // DBG; LED OFF
                return 1; // return button pressed
            }
            break;

    // Until released
        case ButtonLow:
            if (InputPin == HIGH){
                ButtonState = ButtonIdle; // idle
                return 2; // return button released
            }
            break;    
    }
    // Nothing happened    
    return 0; // return nothing happened
}
