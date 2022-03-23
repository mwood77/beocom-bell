#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

const int LED = LED_BUILTIN;
const int TEST_RINGER_BUTTON = 4;        // D4
const int RELAY_PIN = 7;                 // D7
const int RF_PIN = 0;                    // Pin D2 = interrupt 0
int ledState = LOW;                      // Initial state used to set the LED off
int buttonState = 0;                     // Used to read current button state
const int RING_DURATION = 2000;          // How long you want your ringer to be triggered

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

void generateRingtone() {
  Serial.println(F("Ringing"));

	busyLED(HIGH);
  digitalWrite(RELAY_PIN, LOW); 
  delay(RING_DURATION);		          	   // Doorbell is ringing  	
  digitalWrite(7, HIGH);
	busyLED(LOW);

	Serial.println(F("Hang-up"));

	// Cool down
	delay(4000);
}
