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
    current_btn = digitalRead(A0);
    Serial.println(current_btn);
}
