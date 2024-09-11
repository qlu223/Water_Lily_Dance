#include <Servo.h>
#include <NewPing.h>

Servo waterServo;
//North1-4 East5-8 South9-12 West13-16
Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;
Servo servo7;
Servo servo8;
Servo servo9;
Servo servo10;
Servo servo11;
Servo servo12;
Servo servo13;
Servo servo14;
Servo servo15;
Servo servo16;
int crashPin = 30;
#define MAX_DISTANCE 150
#define SONAR_NUM 4

//north = 0; east = 1; south = 2; west =3 
//trig, echo, maxdist
NewPing sonar[SONAR_NUM] = {
  //north
  NewPing(46, 47, MAX_DISTANCE),
  //east
  NewPing(8, 9, MAX_DISTANCE),
  //south
  NewPing(41, 40, MAX_DISTANCE),
  //west
  NewPing(49, 48, MAX_DISTANCE)
};


const int IDLE = 0;
const int WATER = 1;
const int PEOPLE = 2; 

int state = IDLE;

void setup(){
  pinMode(crashPin, INPUT);
  //green/north
  pinMode(46, OUTPUT);
  pinMode(47, INPUT);
  //blue/east
  pinMode(46, OUTPUT);
  pinMode(47, INPUT);
  //yellow/south
  pinMode(41, OUTPUT);
  pinMode(40, INPUT);
  //red/west
  pinMode(49, OUTPUT);
  pinMode(48, INPUT);


  waterServo.attach(37);
  //north servos
	servo1.attach(22);
  servo2.attach(23);
  servo3.attach(24);
  servo4.attach(25);
  //east servos 
  servo5.attach(4);
  servo6.attach(5);
  servo7.attach(6);
  servo8.attach(7);

  //south servos
  servo9.attach(42);
  servo10.attach(43);
  servo11.attach(44);
  servo12.attach(45);

  //west servos
  servo13.attach(50);
  servo14.attach(51);
  servo15.attach(52);
  servo16.attach(53);
  Serial.begin(115200);
}
//calculates the spin based on the cm parameter given by an ultrasonic read
int calculateSpin(int cm){
  int spin;
  if(cm <MAX_DISTANCE && cm >= MAX_DISTANCE-37){
      spin = 93;
    }else if(cm < (MAX_DISTANCE-37) && cm >=(MAX_DISTANCE-75)){
      spin = 89;
    } else if(cm < (MAX_DISTANCE-75) && cm >= (MAX_DISTANCE-112)){
      spin = 85;
    } else if(cm <(MAX_DISTANCE-112) && cm >=(MAX_DISTANCE-130)){
      spin = 80;
    } else if(cm <(MAX_DISTANCE-130) && cm >=(MAX_DISTANCE-138)){
      spin = 75;
    } else{
      spin = 0;
    }
    return spin;
}
/*
  Writes the direction facing servos with the respective direction facing ultrasonic sensor value
  @params int sonar cm distance, bool if idle, int degree of the idle state
*/
//write the north side servos
void writeNorth(int cm, bool isIdle, int degree){
  int spin;
  int isCrash = digitalRead(crashPin);
  if(isCrash == LOW){
    state = WATER;
    return;
  }
  if(isIdle){
    spin = degree;
  } else{
    spin = calculateSpin(cm);
  }
  // Serial.print(spin);
  servo1.write(spin);
  servo2.write(spin);
  servo3.write(spin);
  servo4.write(spin);
}
//write the east side servos
void writeEast(int cm,bool isIdle, int degree){
  int spin;
  int isCrash = digitalRead(crashPin);
  if(isCrash == LOW){
    state = WATER;
    return;
  }
  if(isIdle){
    spin = degree;
  } else{
    spin = calculateSpin(cm);
  }
	servo5.write(spin);
  servo6.write(spin);
  servo7.write(spin);
  servo8.write(spin);	
}
//write the south side servos
void writeSouth(int cm,bool isIdle, int degree){
  int spin;
  int isCrash = digitalRead(crashPin);
  if(isCrash == LOW){
    state = WATER;
    return;
  }
  if(isIdle){
    spin = degree;
  } else{
    spin = calculateSpin(cm);
  }
	servo9.write(spin);
  servo10.write(spin);
  servo11.write(spin);
  servo12.write(spin);	
}
//write the west side servos
void writeWest(int cm,bool isIdle, int degree){
  int spin;
  int isCrash = digitalRead(crashPin);
  if(isCrash == LOW){
    state = WATER;
    return;
  }
  if(isIdle){
    spin = degree;
  } else{
    spin = calculateSpin(cm);
  }
  servo13.write(spin);
  servo14.write(spin);
  servo15.write(spin);
  servo16.write(spin);	
}
/*
  loops through the sonars to see if any of them have been detected
  starts spinnings the servos once detected
*/

bool loopSonars(){
  bool motion = false; 
  int isCrash = digitalRead(crashPin);
  if(isCrash == LOW){
    state = WATER;
    return false;
  }
  for(int i = 0; i < SONAR_NUM; i++){
    delay(10);
    int val = sonar[i].ping_cm();
    // Serial.print(i);
    // Serial.print("=");
    // Serial.print(val);
    // Serial.print("cm ");

    if(sonar[i].ping_cm() != 0){
      motion = true; 
      if(i == 0){
        writeNorth(val, false, 0);
        delay(70);
      }
      if(i == 1){
        writeEast(val, false, 0);
        delay(70);
      }
      if(i == 2){
        writeSouth(val, false, 0);
        delay(70);
      } 
      if(i == 3){
        writeWest(val, false, 0);
        delay(70);
      }
    }
  }
  return motion;
}
/*
  spin all servos, drop a marble go back to idle
*/
void water(){
  // Serial.println("water");
  waterServo.write(90);
  delay(120);
  state = IDLE;
}
//when crash sensor is not triggered and there are no people, servos have stopped or are moving slowly
void idle(){
  waterServo.write(245);
  writeNorth(0, true, 95); 
  writeEast(0,true, 95);
  writeSouth(0,true, 95);
  writeWest(0, true, 95);
  // Serial.println("idle");
  int isCrash = digitalRead(crashPin);
  // Serial.println(isCrash);
  if(isCrash == LOW){
    state = WATER;
  } else if(loopSonars()){
    state = PEOPLE;
  }   
  
 
}
//the closer the person, the faster the servos spin
void people(){
  // Serial.println("people");
  int isCrash = digitalRead(crashPin);
  // Serial.println(isCrash);
  if(isCrash == LOW){
    state = WATER;
  } else if(!loopSonars()){
    state = IDLE; 
  }

}
void loop(){
  int isCrash =digitalRead(crashPin);
  if(isCrash == LOW){
    state = WATER;
  }
  if(state == IDLE){
    idle();
  } else if(state == WATER){
    water();
  } else if(state == PEOPLE){
    people();
  } else{
    Serial.print("bad state: ");
    Serial.println(state);
  }
}
