// Include the GSM library
#include <GSM.h>

#define PINNUMBER ""

#define echoPin 2
#define trigPin 3
#define RELAY 4
#define sms 12

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
bool isSent = false; // For not spamming the user

void setup() {
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO");
  pinMode(RELAY, OUTPUT);
  Serial.println("with Relay SPDT");
  pinMode(sms, OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // connection state
  boolean notConnected = true;
  // Start GSM shield
  // If your SIM has PIN, pass it as a parameter of begin() in quotes
  while (notConnected) {
    if (gsmAccess.begin(PINNUMBER) == GSM_READY) {
      notConnected = false;
    } else {
      Serial.println("Not connected");
      delay(1000);
    }
  }
  Serial.println("GSM initialized");
}

void loop() {
//  digitalWrite(cloud, HIGH);
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)

  if (distance >= 200 || distance <= 0) 
  {
    Serial.println("no object detected");
    // Displays the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
	  digitalWrite(RELAY, LOW);
	  digitalWrite(sms, LOW);
    isSent = false;
  } else {
    Serial.println("object detected");
    // Displays the distance on the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
    digitalWrite(RELAY, HIGH); // Turn on the Light bulb
    if (!isSent) {
    	digitalWrite(sms, HIGH); // Send SMS to the user and data to cloud
      SendMessage();  // Send a message to your owner
      isSent = true;
    }  
  }

}

void sendMessage() {
  char remoteNum[20] = "PHONE_NUMBER";
  Serial.println("House Alert:");
  Serial.println("There is someone by your house");

  // send the message
  sms.beginSMS(remoteNum);
  sms.print("There is someone by your house");
  sms.endSMS();
  Serial.println("\nCOMPLETE!\n");
}