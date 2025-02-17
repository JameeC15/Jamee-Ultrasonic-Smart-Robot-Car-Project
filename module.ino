#include <Servo.h>
Servo myservo;

int Echo_Pin = 6;  // ultrasonic module   ECHO to A0
int Trig_Pin = 7;  // ultrasonic module  TRIG to A1

const int IN1 = 2;   
const int IN2 = 3;  // front right motor
const int IN3 = ;  // front left motor
const int IN4 = ;  

volatile int D_mix;
volatile int D_mid;
volatile int D_max;
volatile int Front_Distance;
volatile int Left_Distance;
volatile int Right_Distance;

// Function Prototypes
void go_forward();
void go_backward();
void rotate_left();
void rotate_right();
void stopMotors();
float checkdistance();
void Detect_Left_and_Right__distance();
void Ultrasonic_obstacle_avoidance();
void Obstacle_Avoidance_Main();

void setup() {
  myservo.attach(9);
  Serial.begin(9600);
  D_mix = 10;
  D_mid = 20;
  D_max = 100;
  Front_Distance = 0;
  Left_Distance = 0;
  Right_Distance = 0;
  myservo.write(90);
  pinMode(Trig_Pin, OUTPUT);
  pinMode(Echo_Pin, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  Obstacle_Avoidance_Main();
}

float checkdistance() {
  digitalWrite(Trig_Pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig_Pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig_Pin, LOW);
  float distance = pulseIn(Echo_Pin, HIGH) / 58.00;
  delay(10);
  return distance;
}

void Detect_Left_and_Right__distance() {
  myservo.write(180);
  delay(400);
  Left_Distance = checkdistance();
  delay(600);
  Serial.print("Left_Distance: ");
  Serial.println(Left_Distance);
  
  myservo.write(0);
  delay(400);
  Right_Distance = checkdistance();
  delay(600);
  Serial.print("Right_Distance: ");
  Serial.println(Right_Distance);
  
  myservo.write(90);
}

void Ultrasonic_obstacle_avoidance() {
  Front_Distance = checkdistance();
  
  if ((Front_Distance < 25) && (Front_Distance > 0)) {  
    stopMotors();
    delay(100);
    
    // Move backward for 1 second before deciding where to turn
    go_backward();
    delay(650);  
    stopMotors();
    delay(200);
    
    myservo.write(180);
    delay(500);
    Left_Distance = checkdistance();
    delay(100);
    
    myservo.write(0);
    delay(500);
    Right_Distance = checkdistance();
    delay(100);
    
    if (Left_Distance > Right_Distance) {
      rotate_left();
      myservo.write(90);
      delay(300); 
    } else {
      rotate_right();
      myservo.write(90);
      delay(300); 
    }
  } else {
    go_forward();
  }
}

void Obstacle_Avoidance_Main() {
  Ultrasonic_obstacle_avoidance();
}

void go_forward() {
  digitalWrite(IN1, HIGH); 
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW); 
  digitalWrite(IN3, HIGH);
}

void go_backward() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, HIGH); 
  digitalWrite(IN3, LOW);
}

void rotate_left() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);  
  digitalWrite(IN3, HIGH);
}

void rotate_right() {
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);
  digitalWrite(IN4, LOW);  
  digitalWrite(IN3, LOW);
}

void stopMotors() {  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(IN3, LOW);
}

