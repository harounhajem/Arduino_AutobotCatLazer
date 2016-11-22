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
musik_knapp = 10;

int 
buttonState1 = 0,     
lastButtonState1 = 0, 
laser_knappen,
buttonPushCounter1,
hastighet = 100,
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
musik;

long interval;

unsigned long 
previousMillis = 0,
currentMillis,
joyxmove,
joyymove,
timewatch;









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
int Note;

/////////////////////////////////////////////////////////////

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
} 


void loop(){ 

  
 
//// Topp Servo -  Joy y
  
    if(millis() - joyymove > hastighet){      
        joyy = analogRead(A1);    
        joyy = map( joyy, 0, 1023, -5, 188);  
        myservo1.write(joyy);    
        joyymove = millis();
    }

/// Bottom Servo   - Joy x
  
    if(millis() - joyxmove > hastighet){
          joyx = analogRead(A0);
    joyx = map( joyx, 1023, 0, -5, 183);  
        myservo.write(joyx);    
        joyxmove = millis();
    }
    

//// Lazer Button

  // read the pushbutton input pin:
  buttonState1 = digitalRead(JOYS_SW_DIO);

  // compare the buttonState to its previous state
  if (buttonState1 != lastButtonState1) {
    // if the state has changed, increment the counter
    if (buttonState1 == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      buttonPushCounter1++;

    }
    }

  lastButtonState1 = buttonState1;
if (buttonPushCounter1 % 2 == 0) {
    digitalWrite(lazer, HIGH);
  } else {
    digitalWrite(lazer, LOW);
  }


//// Musik- knappen

musik = digitalRead(musik_knapp);
      if(musik == HIGH){
           while(Note <54){ 
             currentMillis = millis();
             interval = (duration * 1.2);
            if(currentMillis - previousMillis >= interval) {
              previousMillis = currentMillis;                
              duration = pace/noteDurations[Note];//Adjust duration with the pace of music
              tone(6, melody[Note],duration); //Play note
              Note++;
              }
             }
}

//// Serial print - Ser vad som händer

if(millis() - timewatch > 200UL){
  Serial.print("       joyy: ");
  Serial.print(joyy);
  Serial.print("       joyx: ");
  Serial.print(joyx);
  Serial.print("       Musiken is ON: ");
  Serial.println(musik);
  timewatch = millis();
 }
 
}
