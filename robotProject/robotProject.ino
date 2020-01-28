
/* Gabor sc*/
int line_follow_sensor_pin = A1; //connected to analog 
int line_left_side_max_value = 0;
int line_right_side_max_value = 0;
int line_middle_starter_value=0;
int line_middle_avg_value = 0;
int line_difference_value = 0;

/* Attila sc */
int E1 = 11;
int M1 = 10;
int E2 = 9;
int M2 = 8;

int middleDistanceSensor = A0;
int rightDistanceSensor = A2;

int delay = 200;

boolean isInitialized = false;
boolean noObstickle = true;

void setup(){
  
  Serial.begin(9600);
  
  pinMode(line_folow_sensor_pin, INPUT);
  pinMode(middleDistanceSensor, INPUT);
  pinMode(rightDistanceSensor, INPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
}

void loop(){

   if(!isInitialized){
    initialize();
  }

  startFollowingLine();

}

void startFollowingLine(){
  forward();

 while(noObstickle){
  
  int line_sensor_new = analogRead(line_folow_sensor_value);

  if(line_sensor_new > (line_middle_starter_value + line_difference_value) || (line_sensor_new < line_middle_starter_value + line_difference_value)){

    int turnVelocity =  line_sensor_new - line_middle_starter_value;
    
    turn(turnVelocity); 
  }
  
 } 
}
void startFollowingLine(){
    

 while(noObstickle){
  goForward(80);
  
  int line_sensor_new = analogRead(line_folow_sensor_value);

  if(line_sensor_new > (line_middle_starter_value + line_difference_value) || (line_sensor_new < line_middle_starter_value + line_difference_value)){

    int turnVelocity =  line_sensor_new - line_middle_starter_value;
    
    turn(turnVelocity); 
  }
 } 
}

boolean noObstickle{
  int middleSign = analogRead(middleDistanceSensor);

  //Serial.println(middleSign);
  if (middleSign > 400) {
    stopMotor();
    //delay(10000);
    int val = turnUntilMaxVal();
    goAroundObject(val);
    delay(10000);
  }else{
    true;
  }

}

int turnUntilMaxVal() {
 //Search maximum value
  int maxVal = analogRead(rightDistanceSensor);
  while (true) {
    turnLeft(40);
    delay(200);
    int val = analogRead(rightDistanceSensor);
    //Serial.println(maxVal);
    if (val > maxVal) {
      maxVal = val;
    }
    Serial.println(val);
    //Local max found, rotate
    if (maxVal - 280 > val) {
      //go back to loal max
      while (true) {
        turnRight(40);
        delay(200);
        if (analogRead(rightDistanceSensor) > maxVal - 40 && analogRead(rightDistanceSensor) < maxVal + 40) {
          stopMotor();
          delay(10000);
          return analogRead(rightDistanceSensor);
        }
      }
    }
  }
} 

void goAroundObject(int distance) {
  while (true) {
    int v = p(0.7, distance, analogRead(rightDistanceSensor));
    goForward(180);
    delay(200);
    if ( v < 0) {
      Serial.println(v);
      turnRight(v);
      delay(200);
    } else {
      Serial.println(v);
      turnLeft(v);
      delay(200);
    }
  }

}

void keepDistance() {
  int v = p(0.5, 250, analogRead(middleDistanceSensor));

  if ( v < 0) {
    goBackward(v);
  } else {
    goForward(v);
  }
}

int p(float k, int setPoint, int reading) {
  int v = k * (setPoint - reading);
  return v;
}


void turn(int turnVelocity){

  if(line_middle_starter_value < line_right_side_max_value){
      if(line_middle_starter_value < turnVelocity > line_left_side_max_value){
        turnLeft(turnVelocity);
      }
      if(line_middle_starter_value > turnVelocity < line_left_side_max_value){
        turnRight(turnVelocity);
      }
   }

   if(line_middle_starter_value > line_right_side_max_value){
     if(line_middle_starter_value < turnVelocity > line_right_side_max_value){
       turnRight(turnVelocity);
     }
     if(line_middle_starter_value > turnVelocity < line_right_side_max_value){
       turnLeft(turnVelocity);
     }
   }
}

void initialize(){
  // get the line value
  line_middle_starter_value = analogRead(line_follow_sensor_value);
  delay(100);
  
  // turn left for X and get the left side data
  turnLeft(30);
  line_left_side_max_value = analogRead(line_follow_sensor_value);
  delay(100);

  //turn right for X and get the left side data
  turnRight(60);
  line_right_side_max_value = analogRead(line_follow_sensor_value);
  delay(100);

  line_middle_avg_value = (line_left_side_max_value - line_right_side_max_value) * -1;

  line_difference_value = (line_middle_starter_value - line_middle_avg_value) * -1;

  initialized = true;

}

};
