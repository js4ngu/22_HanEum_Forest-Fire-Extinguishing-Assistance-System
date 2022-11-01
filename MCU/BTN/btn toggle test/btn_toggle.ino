bool BTN_toggle();

//BTN
bool current_btn = 0;
bool prev_btn = 0;

//LED
bool LED_STATE = 1;

void setup() {
    pinMode(A0,INPUT);
    pinMode(13,OUTPUT);
    digitalWrite(13, HIGH);
}

void loop(){
    if(BTN_toggle()){
        LED_STATE = !LED_STATE;
        digitalWrite(13, LED_STATE);
    }
}

bool BTN_toggle(){
    current_btn = digitalRead();
    if ( (prev_btn ^ current_btn) == 1){
        return 1;
    }
    prev_btn = current_btn;
}
