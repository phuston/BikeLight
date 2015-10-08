 #include <Servo.h>
Servo hServo;
Servo vServo;

byte incomingByte = 0;
// Sets up initial servo positions
byte vServPos = 120; // Vertical (tilt) servo
byte hServPos = 90;  // Horizontal (pan) servo
// Sets up servo step sizes
byte hstepr = 1;
byte vstepr = -2;

float rng_avg = 0;
int rng1 = 0;
int rng2 = 0;
int rng3 = 0;
int rng4 = 0;

byte hrange = 40;

const int sensor = A5;  // Analog input pin that the potentiometer is attached to
float sensorValue, inches, cm;    //Must be of type float for pow()

void setup() {
  hServo.attach(9);
  vServo.attach(10);
  hServo.write(hServPos);
  vServo.write(vServPos);
  Serial.begin(9600);
}


void loop() {
  if (Serial.available() > 0) {
    // Flush out all 'd' values from serial sent by python
    Serial.flush();
    incomingByte = Serial.read();
    
    // Print out values to serial
    Serial.print(rng_avg);
    Serial.print(" ");
    Serial.println(hServPos);
    
    // Get next segment of data
    readData();
  }

}

int readData(){
  // Move pan servo one step size
  hServPos += hstepr;
  hServo.write(hServPos);
  
  // Detect whether the pan servo is at extreme values
  if (hServPos >= 90 + hrange || hServPos <= 90-hrange){
    hstepr*=-1; // Reverse direction of motion if at one of extremes
    vServPos+=vstepr; // Move tilt servo down
    vServo.write(vServPos);
  }
  
  // Delay to give sensor time to settle
  delay(100);
  
  // Takes 4 readings, finds average to get consistent readings
  rng1 = analogRead(sensor);
  delay(10);
  rng2 = analogRead(sensor);
  delay(10);
  rng3 = analogRead(sensor);
  delay(10);
  rng4 = analogRead(sensor);
  
  rng_avg = (rng2+rng3+rng4)/3;
  
  // Return to loop()
  return 0;
}
