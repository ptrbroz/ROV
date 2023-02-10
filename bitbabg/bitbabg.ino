

#define MOTOR_RIGHT_A 3
#define MOTOR_RIGHT_B 11

int counter = 0;
int dutyOn = 5;
int dutyOff = 15;

void setup() {
  // put your setup code here, to run once:
  pinMode(MOTOR_RIGHT_A, OUTPUT);
  pinMode(MOTOR_RIGHT_B, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(MOTOR_RIGHT_A, 0);
  counter++;
  if(counter <= dutyOn){
    digitalWrite(MOTOR_RIGHT_B, 1);
  }
  else{
    digitalWrite(MOTOR_RIGHT_B, 0);
  }

  if(counter > dutyOn + dutyOff){
    counter = 0;
  }

  delay(1);
}
