#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();
unsigned long startTime;
unsigned long endMicros;

const int LED = LED_BUILTIN;
const int TEST_RINGER_BUTTON = 4;   // D4
const int RELAY_PIN = 7;            // D7
const int RF_PIN = 2;               // Pin D2 = interrupt 0
const int HZ = 25;

int ledState = LOW;                 // Initial state used to set the LED off
int buttonState = 0;                // Used to read current button state

void setup() {
	Serial.begin(9600);
	mySwitch.enableReceive(RF_PIN);  	    // RF Receiver on interrupt 0 => pin #D2

	pinMode(TEST_RINGER_BUTTON, INPUT);
	pinMode(LED, OUTPUT);
	pinMode(RELAY_PIN, OUTPUT);

	digitalWrite(7, HIGH);
	Serial.println("System Ready");
}

void loop() {
	buttonState = digitalRead(TEST_RINGER_BUTTON);

	if (mySwitch.available() &&
	    mySwitch.getReceivedBitlength() == 24 || buttonState == HIGH) {
			generateRingtone();
			mySwitch.resetAvailable();
	}
}

void busyLED(byte state) {	
	digitalWrite(LED, state); 
}

/*
  Ring voltage approv 50V @ 25HZ for 2 seconds.
  1000 miliseconds / 25(hz) = 20ms high and 20ms low @ 50% duty cycle

  HARDCODED FOR 25HZ OVER 2 SECONDS
*/
void generateRingtone() {
	int count = 0;
	busyLED(HIGH);

	Serial.println("Ringing");
	startTime = millis();
	for (int i = 0; i < HZ * 2; i++ ) {
		digitalWrite(RELAY_PIN, LOW);
		delay(20);			// Doorbell is ringing
		digitalWrite(7, HIGH);
		delay(20);			// Doorbell is ringing
	count++;
	}
	endMicros = millis();
	busyLED(LOW);

	Serial.print("Ring duration: ");
	Serial.print( (endMicros - startTime) / 1000 );
	Serial.print(" seconds    |    Times cycled: ");
	Serial.println(count);

	// Cool down
	delay(DURATION * 2);
}
