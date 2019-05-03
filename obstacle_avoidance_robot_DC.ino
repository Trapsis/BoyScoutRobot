/*  Obstacle avoidance robot source code designed 
 *  for the Robotics merit badge for the Boy Scouts
 *  This is for the DC motor / motor controller shield version
 *  ESP8266 NodeMCU
 * 
 *   By Bill Caterino  -  ZigZone Automation
 * 
 * 
 * 
 */
#include <Wire.h> 

#define DIRA 0
#define PWMA 5
#define DIRB 2
#define PWMB 4

int DCgo = 1023;  // 0 (stop) - 1023 (max)
int DCstop = 0;

int trigPin = 14;    // Trigger
int echoPin = 12;    // Echo

void setup() {
  Serial.begin (9600);

  //Define pins for DC motor dirction and speed control
  pinMode(DIRA, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(DIRB, OUTPUT);
  pinMode(PWMB, OUTPUT);
  
  //Define HC-SR04 Ultrasonic sensor inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  //Make sure motors are stopped
  brake();
  delay (3000);  // Pause for a bit after powering on so you have time to put the robot down

}

void loop() {
  Serial.print("Forward / distance: ");
  forward();
  long dist = getDistance();
  Serial.println(dist);
  if (dist < 4){ //There is an object less than 4 inches from front of robot
    Serial.println("object detected!");
    long leftDistance, rightDistance;
    brake();
    delay(500);
    backward();
    delay(800);
    left();
    delay(300);
    brake();
    leftDistance = getDistance();
    Serial.print("Left distance: ");
    Serial.println(leftDistance);
    right();
    delay(600);
    brake();
    rightDistance = getDistance();
    Serial.print("Right distance: ");
    Serial.println(rightDistance);
    if (leftDistance > rightDistance){ //There is more space to manuver to the left
      //let's go left!
      left();
      Serial.println("Going Left");
      delay(600);
    }
    //otherwise we just stay pointed in this direction (right)
    Serial.println("Not turning (staying right)");
  }

}

long getDistance(){
  long duration;
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  return (duration/2) / 74;   // Divide by 74 or multiply by 0.0135
}

void backward(){      
  digitalWrite(DIRA,0); //Set backward direction A
  digitalWrite(DIRB,0); //Set backward direction B 
  analogWrite(PWMA,DCgo); //Turn on motor A
  analogWrite(PWMB,DCgo); //Turn on motor B   
}

void left(){     
  digitalWrite(DIRA,0); //Set backward direction A
  digitalWrite(DIRB,1); //Set forward direction B 
  analogWrite(PWMA,DCgo); //Turn on motor A
  analogWrite(PWMB,DCgo); //Turn on motor B   
}

void right(){      
  digitalWrite(DIRA,1); //Set forward direction A
  digitalWrite(DIRB,0); //Set backward direction B 
  analogWrite(PWMA,DCgo); //Turn on motor A
  analogWrite(PWMB,DCgo); //Turn on motor B   
}

void forward(){
  digitalWrite(DIRA,1); //Set forward direction A
  digitalWrite(DIRB,1); //Set forward direction B 
  analogWrite(PWMA,DCgo); //Turn on motor A
  analogWrite(PWMB,DCgo); //Turn on motor B         
}
void brake(){   
  digitalWrite(DIRA,1); //Set forward direction A
  digitalWrite(DIRB,1); //Set forward direction B 
  analogWrite(PWMA,DCstop); //Turn off motor A
  analogWrite(PWMB,DCstop); //Turn off motor B  
}
