void BTN_toggle();

//BTN
bool current_btn = 0;
bool prev_btn = 0;

//LED
bool LED_STATE = 1;

void setup() {
    Serial.begin(9600);
    pinMode(A0,INPUT);
    pinMode(13,OUTPUT);
    digitalWrite(13, HIGH);
}

void loop(){
    BTN_toggle();
}

void BTN_toggle(){
    current_btn = digitalRead(A0);
    if ( (prev_btn ^ current_btn) == 1){
        Serial.println("TOGGLE");
    }
    prev_btn = current_btn;
}
