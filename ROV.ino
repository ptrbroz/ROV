
//PIN ASSIGNMENTS
#define FRONT_BARS A0
#define LEFT_BARS A1
#define RIGHT_BARS A2
#define POT_SPEED A3

#define MOTOR_LEFT_A 9
#define MOTOR_LEFT_B 10

#define MOTOR_RIGHT_A 3
#define MOTOR_RIGHT_B 11

//CONSTANTS
#define TOUCH_THRESHOLD 100

//GLOBAL VARS
int target = 0;

int FRONT_TOUCH = 0;
int LEFT_TOUCH = 0;
int RIGHT_TOUCH = 0;

//determined by reading pots on setup
int MAX_SPEED = 200;
int MIN_SPEED = 10;

int ROT_SPEED = 0;
int ACCELERATION = 1;
int DECELERATION = 6;
int ACCELERATION_PRESCALER = 10;
int ACCELERATION_COUNTER = 0;
int TOLERANCE = 0;

int LED_COUNTER = 0;
int LED_PRESCALER = 10;

//current speeds
int LEFT_SPEED = 0;
int RIGHT_SPEED = 0;

//targets 
int LEFT_TARGET = 0;
int RIGHT_TARGET = 0;

void setup() {
  // put your setup code here, to run once:

  //pwm setup

  //timer 1 -----------------------------------
  uint8_t TCCR1A_temp = 0b00000000;
  uint8_t TCCR1B_temp = 0b00000000;
  TCCR1A_temp |= 0b11110000; //inverting modes - set on match

  TCCR1A_temp |= 0b00000010;
  TCCR1B_temp |= 0b00011000; //fast pwm, top = ICR1

  TCCR1B_temp |= 0b00000010; //divide clock by 8

  TCCR1A = TCCR1A_temp;
  TCCR1B = TCCR1B_temp;

  ICR1H = 0;
  ICR1L = 255; //set period (top)

  OCR1AH = 0;
  OCR1AL = 255;
  OCR1BH = 0;
  OCR1BL = 255; //set initial duty to 0%

  DDRB |= 0b00000110; //set PB1, PB2 (=D9 D10) output
  // end of timer 1 ------------------------------

  //timer 2 --------------------------------------
  uint8_t TCCR2A_temp = 0b00000000;
  uint8_t TCCR2B_temp = 0b00000000;

  TCCR2A_temp |= 0b11110000; //inverting modes - set on match

  TCCR2A_temp |= 0b00000011; //
  TCCR2B_temp |= 0b00000000; //fast pwm, top = 0xff
  
  TCCR2B_temp |= 0b00000010; //divide clock by 8

  TCCR2A = TCCR2A_temp;
  TCCR2B = TCCR2B_temp;

  OCR2A = 255;
  OCR2B = 255; //set initial duty
  DDRB |= 0b00001000; //set PB3 (=D11) output
  DDRD |= 0b00001000; //set PD3 (=D03) output
  //end of timer 2

  Serial.begin(9600);
  Serial.println("STARTUP");
  pinMode(FRONT_BARS, INPUT);
  pinMode(LEFT_BARS, INPUT);  
  pinMode(RIGHT_BARS, INPUT);
  pinMode(POT_SPEED, INPUT);

  pinMode(13, OUTPUT);
  
  delay(200);
  

  /*int speedReading = analogRead(POT_SPEED);
  Serial.print("\nPOT_S :");
  Serial.print(speedReading);

  Serial.print("\nPOT_A :");
  Serial.print(speedReading);*/

  
}

void read_bars(){
  analogRead(FRONT_BARS);
  delay(15);
  FRONT_TOUCH = ( analogRead(FRONT_BARS) > TOUCH_THRESHOLD );
  
  analogRead(LEFT_BARS);
  delay(15);
  LEFT_TOUCH =  ( analogRead(LEFT_BARS)  > TOUCH_THRESHOLD );

  analogRead(RIGHT_BARS);
  delay(15);
  RIGHT_TOUCH = ( analogRead(RIGHT_BARS) > TOUCH_THRESHOLD );
  
  //delay(15);
  //Serial.println(analogRead(LEFT_BARS));
}

void set_pwm_direct(int leftSpeed, int rightSpeed){
  uint8_t invertedLeft = 255 - abs(leftSpeed);
  uint8_t invertedRight = 255 - abs(rightSpeed);
  if(leftSpeed < 0){
    OCR1AH = 0;
    OCR1AL = invertedLeft;
    OCR1BH = 0;
    OCR1BL = 255;
  }
  else{
    OCR1AH = 0;
    OCR1AL = 255;
    OCR1BH = 0;
    OCR1BL = invertedLeft;
  }

  if(rightSpeed < 0){
    OCR2B = invertedRight;
    OCR2A = 255;
  }
  else{
    OCR2B = 255;
    OCR2A = invertedRight;
  }
  return;
}

void print_touches(){
  Serial.print(LEFT_TOUCH);
  Serial.print(" ");
  Serial.print(FRONT_TOUCH);
  Serial.print(" ");
  Serial.print(RIGHT_TOUCH);
  Serial.println("");
}

void loop() {
  // put your main code here, to run repeatedly:

  read_bars();
  //print_touches();

  if(FRONT_TOUCH){
    if(LEFT_TOUCH){
      //todo  
      int x = 0;
    }
    else if(RIGHT_TOUCH){
      //todo
      int x = 0;
    }
    else{
      LEFT_TARGET = MAX_SPEED;
      RIGHT_TARGET = MAX_SPEED;
    }
  }
  else if(RIGHT_TOUCH){
    LEFT_TARGET = MAX_SPEED;
    RIGHT_TARGET = -MAX_SPEED;
  }
  else if(LEFT_TOUCH){
    LEFT_TARGET = -MAX_SPEED;
    RIGHT_TARGET = MAX_SPEED;
  }
  else{
    LEFT_TARGET = 0;
    RIGHT_TARGET = 0;
  }

  
  //print_touches();

  //control
  
  ACCELERATION_COUNTER++;

  if(ACCELERATION_COUNTER >= ACCELERATION_PRESCALER){
    int leftDir = 1;
    int rightDir = 1;

    if(LEFT_TARGET < LEFT_SPEED){
      leftDir = -1;
    }
    if(RIGHT_TARGET < RIGHT_SPEED){
      rightDir = -1;
    }

    if(LEFT_TARGET == 0){
      LEFT_SPEED += DECELERATION*leftDir;
    }
    else{
      LEFT_SPEED += ACCELERATION*leftDir;
    }

    if(RIGHT_TARGET == 0){
      RIGHT_SPEED += DECELERATION*rightDir;
    }
    else{
      RIGHT_SPEED += ACCELERATION*rightDir;
    }
    
    if(abs(LEFT_SPEED) < MIN_SPEED){
      if(LEFT_TARGET == 0){
        LEFT_SPEED = 0;
      }
      else{
        LEFT_SPEED = leftDir*MIN_SPEED;
      }
    }

  if(abs(RIGHT_SPEED) < MIN_SPEED){
      if(RIGHT_TARGET == 0){
        RIGHT_SPEED = 0;
      }
      else{
        RIGHT_SPEED = rightDir*MIN_SPEED;
      }
    }

    //set_pwm_direct(LEFT_SPEED, RIGHT_SPEED);
    OCR2B = 255 - RIGHT_SPEED;
    OCR2A = 255;
    
    int printSpeeds = 0;
    if(printSpeeds){
      Serial.print(LEFT_SPEED);
      Serial.print(" ");
      Serial.print(RIGHT_SPEED);
      Serial.println("");
    }
    
  }

  LED_COUNTER++;
  if(LED_COUNTER >= LED_PRESCALER){
    LED_COUNTER = 0;
    digitalWrite(13, 1);
  }
  else{
    digitalWrite(13, 0);
  }



}
