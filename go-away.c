#define echoPin 2
#define trigPin 3
#define RELAY 4
#define sms 12

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
bool isSent = false; // For not spamming the user

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO");
  pinMode(RELAY, OUTPUT);
  Serial.println("with Relay SPDT");
  Serial.begin(9600); // // Serial Communication is starting with 9600 of baudrate speed
  pinMode(sms, OUTPUT);
}

void loop() {
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
        isSent = true;
    }  
  }
}