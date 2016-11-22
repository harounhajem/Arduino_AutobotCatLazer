#include "pitches.h"
#define NO_SOUND 0 // make the rests in music
#include <Servo.h> 
Servo myservo;
Servo myservo1;  

// Joystik
int 
JOYS_VRX_DIO = A0,    // X-Axis  
JOYS_VRY_DIO = A1,    // Y-Axis 
JOYS_SW_DIO = 12;     // Button 

const int 
lazer = 5,
musik_knapp = 10,
numReadings = 10;

int 
buttonState1 = 0,     
lastButtonState1 = 0, 
laser_knappen,
buttonPushCounter1,
hastighet = 50,
posX = 90,              // variable to store the servo position 
posY = 90,
state = 0,
buttonState = 0,         // current state of the button
lastButtonState = 0,
musik_value,
joyx,
joyy,
hastighetx, 
hastighety,
pos_y = 90,
pos_x = 90,
musik,
third_time,

readingsy[numReadings],      // the readings from the analog input
indexy = 0,                  // the index of the current reading
totaly = 0,                  // the running total
averagey = 0,                // the average

readingsx[numReadings],      // the readings from the analog input
indexx = 0,                  // the index of the current reading
totalx = 0,                  // the running total
averagex = 0;                // the average

long interval;

unsigned long 
previousMillis = 0,
currentMillis,
joyxmove,
joyymove,
timewatch,
pos_timey,
pos_timex,
music_timer,
rand_seq,
stopper,
wait,
wait_smooth,
waity,
wait_smoothy;








//  MUSIC   

int melody[] = {

  NOTE_E4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_E4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_E4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_E4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_DS5,NOTE_D5,NOTE_B4,NOTE_A4,NOTE_B4,
  NOTE_E4,NOTE_G4,NOTE_DS5,NOTE_D5,NOTE_G4,NOTE_B4,
  NOTE_B4,NOTE_FS5,NOTE_F5,NOTE_B4,NOTE_D5,NOTE_AS5,
  NOTE_A5,NOTE_F5,NOTE_A5,NOTE_DS6,NOTE_D6,NO_SOUND
};


int noteDurations[] = {

  8,16,16,8,4,8,8,8,
  8,16,16,8,4,8,8,8,
  8,16,16,8,4,8,8,8,
  8,16,16,8,4,8,8,8,
  8,2,8,8,1,
  8,4,8,4,8,8,
  8,8,4,8,4,8,
  4,8,4,8,3
};

int pace = 1100; // change pace of music("speedy")
int duration;
int Note = 54;

/////////////////////////////////////////////////////////////


int 
newpos_y=90,
oldpos_y=0,
newpos_x =90, 
oldpos_x = 0,
frame;




void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object
  myservo1.attach(3);
  Serial.begin(9600);
  pinMode(lazer, OUTPUT); 
  
  myservo.write(posX);
  myservo1.write(posY);
  
  pinMode(JOYS_SW_DIO, INPUT_PULLUP); 
  pinMode(musik_knapp, INPUT_PULLUP);
  digitalWrite(JOYS_SW_DIO, HIGH); // turn on pull-up resistors

  posY = constrain(posY, -5,183);  
  buttonState = constrain(buttonState, 0,1);
  
    for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readingsy[thisReading] = 0;   
    for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readingsx[thisReading] = 0;   

  
} 

void loop(){ 

  
digitalWrite(lazer, HIGH);
pos_x = constrain(pos_x, -5,185);
pos_y = constrain(pos_y, -5,185);



if((millis() - wait > frame) && (millis() - stopper > 10000UL)){    
  
  ///// hur gör man ett bra avbrott?!
  ///// Varannan gång, eller var tredje, vem bestämmer över avbrottet? 

        /////////// Servo Topp /////////
                
                  if(pos_y == newpos_y){
                    oldpos_y = newpos_y;
                    newpos_y = random(10,80);
                      
                    }
                  if(millis() - wait_smoothy > 30UL){
                          
                        if(newpos_y > oldpos_y){
                          pos_y++;
                          } 
                        else{
                          pos_y--;
                          }
                          
                          myservo1.write(pos_y);
                          wait_smoothy = millis();
                          }
                        
                
                /////////// Servo Bottom /////////////
                 
                  if(pos_x == newpos_x){
                    oldpos_x = newpos_x;
                    newpos_x = random(35,180);
                    
                    third_time++;
                    
                    
                    if(third_time % 3 == 0 ){
                    wait = millis();                    //// Avgör slumpen av avbrott!
                    frame = random(1500, 3000);
                    }
                    
                    }
                 
                    if(millis() - wait_smooth > 20UL){
                          
                        if(newpos_x > oldpos_x){
                          pos_x++;
                          } 
                          else{
                          pos_x--;
                          }
                          
                          myservo.write(pos_x);
                          wait_smooth = millis();
                          }
                 
                        
                //////////////////////////////////
}


//// Serial print - Ser vad som händer

if(millis() - timewatch > 200UL){
  Serial.print("   Y: ");
  Serial.print(pos_y);
  Serial.print("   New-Y: ");
  Serial.print(newpos_y);
  Serial.print("   X: ");
  Serial.print(pos_x);
  Serial.print("   New-X: ");
  Serial.println(newpos_x);
  
  timewatch = millis();
 }
}
