#include <Servo.h>
#include <NewPing.h>

Servo waterServo;
Servo north1;
Servo north2;
Servo east1;
Servo east2;
Servo south1;
Servo south2;
Servo west1;
Servo west2;

Servo northEast;
Servo northWest;
Servo southEast;
Servo southWest;

Servo center1;
Servo center2;
Servo center3;
Servo center4;

//set the maximum distance and number of ultrasonic sensors
#define MAX_DISTANCE 60
#define SONAR_NUM 4

//initialize the ultrasonic sensors
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

void setup() {
  // put your setup code here, to run once:
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

}
//If one side is detected, set to 1, otherwise 0
static int northOn;
static int eastOn;
static int southOn;
static int westOn;

//if all sides are on, detach all servos along the outside, spin center 
void spinCenter(){
  north1.detach();
  north2.detach();
  east1.detach();
  east2.detach();
  south1.detach();
  south2.detach();
  west1.detach();
  west2.detach();
  northEast.detach();
  northWest.detach();
  southEast.detach();
  southWest.detach();

  center1.attach(42);
  center2.attach(44);
  center3.attach(25);
  center4.attach(22);

  center1.write(0);
  center2.write(180);
  center3.write(0);
  center4.write(180);

  //wait 7 seconds
  delay(7000);
  center1.detach();
  center2.detach();
  center3.detach();
  center4.detach();
}
/*
  Writes respective side servos
  @param: cm is the distance that the ultrasonic has detected an object
*/
//writes the north side servo and all the behavior connected to it
void writeNorth(int cm){
  //if adjacent sides are off, turn off the respective corner servo
  if(!eastOn){
    northEast.detach();
  } 
  if(!westOn){
    northWest.detach();
  }
  northOn = 1;
  //if all servos are on, change state to water and return
  if(northOn && eastOn && southOn && westOn){
    state = WATER;
    return;
  }
  //if side is on and either of the adjacent sides are on, detach side servos and 
  // attach respective corner servo.
  if(northOn && (eastOn || westOn)){
    north1.detach();
    north2.detach();
    if(eastOn){
      east1.detach();
      east2.detach();
      northEast.attach(7);
      northEast.write(0);
      delay(15);
    }
    if(westOn){
      west1.detach();
      west2.detach();
      northWest.attach(50);
      northWest.write(0);
      delay(15);
    }    
  }
  //else if neither adjacent side is on, start spinning the two direction servos 
  else if(cm <= MAX_DISTANCE){
    north1.attach(24);
    north2.attach(23);
    int spin = 0;
    north1.write(spin);
    north2.write(spin);
    delay(15);
  } 
}
//write south servos
void writeSouth(int cm){
  if(!eastOn){
    southEast.detach();
  } 
  if(!westOn){
    southWest.detach();
  }
  southOn = 1;
  if(northOn && eastOn && southOn && westOn){
    state = WATER;
    return;
  }
  if(southOn && (eastOn || westOn)){
    south1.detach();
    south2.detach();
    if(eastOn){
      east1.detach();
      east2.detach();
      southEast.attach(4);
      southEast.write(0);
      delay(15);
    }
    if(westOn){
      west1.detach();
      west2.detach();
      southWest.attach(53);
      southWest.write(0);
      delay(15);
    }    
  } else 
  if(cm <= MAX_DISTANCE){
    south1.attach(43);
    south2.attach(45);
    int spin = 0;
    south1.write(spin);
    south2.write(spin);
    delay(15);
  } 
  else if(northOn && eastOn && southOn && westOn){
    state = WATER;
  }
}
//write east servos
void writeEast(int cm){
  if(!northOn){
    northEast.detach();
  } 
  if(!southOn){
    southEast.detach();
  }
  eastOn = 1;
  Serial.println(eastOn);
  Serial.println(northOn);
  Serial.println(westOn);
  if(northOn && eastOn && southOn && westOn){
    state = WATER;
    return;
  }
  if(eastOn && (northOn || southOn)){
    east1.detach();
    east2.detach();
    if(northOn){
      north1.detach();
      north2.detach();
      northEast.attach(7);
      northEast.write(0);
      delay(15);
    }
    if(southOn){
      south1.detach();
      south2.detach();
      southEast.attach(4);
      southEast.write(0);
      delay(15);
    }    
  } else
  if(cm < MAX_DISTANCE){
    east1.attach(5);
    east2.attach(6);
    int spin = 0;
    east1.write(spin);
    east2.write(spin);
    delay(10);
  } 
  else if(northOn && eastOn && southOn && westOn){
    state = WATER;
  }
}
//write west servos
void writeWest(int cm){
  if(!northOn){
    northWest.detach();
  } 
  if(!southOn){
    southWest.detach();
  }
  westOn = 1;
  if(northOn && eastOn && southOn && westOn){
    state = WATER;
    return;
  }
  if(westOn && (northOn ||southOn)){
    west1.detach();
    west2.detach();
    if(northOn){
      north1.detach();
      north2.detach();
      northWest.attach(50);
      northWest.write(0);
      delay(10);
    }
    if(southOn){
      south1.detach();
      south2.detach();
      southWest.attach(53);
      southWest.write(0);
      delay(10);
    }    
  } else
  if(cm < MAX_DISTANCE){
    west1.attach(52);
    west2.attach(51);
    int spin = 0; 
    west1.write(spin);
    west2.write(spin);
    delay(10);
  } 

}

//spins the center servos and drops a marble
void water(){
  spinCenter();
  northOn = 0;
  eastOn = 0;
  southOn = 0;
  westOn = 0;
  waterServo.write(90);
  delay(155);
  waterServo.write(245);
  delay(5000);
  state = IDLE;
}
/*
  Loops through the 4 ultrasonic sensors.
  If there is an object detected, motion is set to true and respective side is called.
  returns true if there is movement, false otherwise. 
*/
bool loopSonars(){
  bool motion = false;
  for(int i = 0; i < SONAR_NUM; i++){
    delay(10);
    int val = sonar[i].ping_cm();
    //if sonar is not 0, there is an object
    if(val != 0){
      motion = true;
      if(i == 0){
        writeNorth(val);
        delay(15);
      
      }
      if(i == 1){
        writeEast(val);
        delay(15);
      }
      if(i == 2){
        writeSouth(val);
        delay(15);
      } 
      if(i == 3){
        writeWest(val);
        delay(15);
      }
    } else{
      if(i== 0){
        north1.detach();
        north2.detach();
        northOn = 0;
      }
      if(i == 1){
        east1.detach();
        east2.detach();
        eastOn = 0;
      }
      if(i == 2){
        south1.detach();
        south2.detach();
        southOn = 0;
      } 
      if(i == 3){
        west1.detach();
        west2.detach();
        westOn = 0;
      }
    }
  }
  return motion;
}
//Closes the marble servo, if each side is on, state is set to WATER, otherwise loop through servos and if
//loopSonars returns true, set state to PEOPLE
void idle(){
  waterServo.write(245);
  if(northOn && eastOn && southOn && westOn){
    state = WATER;
  } else if(loopSonars()){
    state = PEOPLE;
  }   
  
}
//If each side is on, state is set to WATER, otherwise loop through servos and if loopSonars is 
//false, set state to IDLE
void people(){
  if(northOn && eastOn && southOn && westOn){
    state = WATER;
  } else if(!loopSonars()){
    state = IDLE; 
  }
}

void loop() {
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
