////
////#include "Pitches.h"
////#include <Servo.h> 
////Servo myservo;
////Servo myservo1;
////#define NO_SOUND 0 
////
////// Declare IO
////// Joystick
////#define joyInputX		A0  // X-Axis  
////#define joyInputY		A1  // Y-Axis 
////
////#define	servoTop		3	// Servo top
////#define	lazer			5	// Lazer button
////#define speaker			6	// Piezo speaker
////#define	servoBottom		9	// Servo bottom
////#define	musicButton		10	// Button musics
////#define joyButton		12  // Button on joystick
////
////
////#define	numReadings		10
////int
////buttonState1 = 0,
////lastButtonState1 = 0,
////buttonPushCounter1,
////state = 0,
////buttonState = 0,         // current state of the button
////lastButtonState = 0,
////joyx,
////joyy,
////posX = 90,              // variable to store the servo position 
////posY = 90,
////pos_y = 90,
////pos_x = 90,
////music,
////third_time,
////second_time = 3,
////x = 3,
////
////// Smoothing values
////readingsy[numReadings],      // the readings from the analog input
////indexy = 0,                  // the index of the current reading
////totaly = 0,                  // the running total
////averagey = 0,                // the average
////							 //----
////	readingsx[numReadings],      // the readings from the analog input
////	indexx = 0,                  // the index of the current reading
////	totalx = 0,                  // the running total
////	averagex = 0,                // the average
////
////	newpos_y = 90,
////	oldpos_y = 0,
////	newpos_x = 90,
////	oldpos_x = 0,
////	frame = 2500;
////
////
////long interval;
////
////unsigned long
////previousMillis = 0,
////currentMillis,
////timewatch,
////pos_timey,
////pos_timex,
////music_timer,
////rand_seq,
////stopper,
////wait,
////wait_smoothX,
////wait_smoothY;
////
////
////
////
////
////
////
////
//////  MUSIC   
////
////int melody[] = {
////
////	NOTE_E4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_E4,
////	NOTE_E4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_E4,NOTE_E4,NOTE_E4,
////	NOTE_E4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_E4,
////	NOTE_E4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_E4,NOTE_E4,NOTE_E4,
////	NOTE_DS5,NOTE_D5,NOTE_B4,NOTE_A4,NOTE_B4,
////	NOTE_E4,NOTE_G4,NOTE_DS5,NOTE_D5,NOTE_G4,NOTE_B4,
////	NOTE_B4,NOTE_FS5,NOTE_F5,NOTE_B4,NOTE_D5,NOTE_AS5,
////	NOTE_A5,NOTE_F5,NOTE_A5,NOTE_DS6,NOTE_D6,NO_SOUND
////};
////
////
////int noteDurations[] = {
////
////	8,16,16,8,4,8,8,8,
////	8,16,16,8,4,8,8,8,
////	8,16,16,8,4,8,8,8,
////	8,16,16,8,4,8,8,8,
////	8,2,8,8,1,
////	8,4,8,4,8,8,
////	8,8,4,8,4,8,
////	4,8,4,8,3
////};
////
////int pace = 1100; // change pace of music("speedy")
////int duration;
////int Note = 54;
////
/////////////////////////////////////////////////////////////////
////
////void setup()
////{
////	myservo.attach(servoBottom);  // attaches the servo on pin 9 to the servo object
////	myservo1.attach(servoTop);
////	Serial.begin(9600);
////	pinMode(lazer, OUTPUT);
////
////	myservo.write(posX);
////	myservo1.write(posY);
////
////	pinMode(joyButton, INPUT_PULLUP);
////	pinMode(musicButton, INPUT_PULLUP);
////	digitalWrite(joyButton, HIGH); // turn on pull-up resistors
////
////	posY = constrain(posY, -5, 183);
////	buttonState = constrain(buttonState, 0, 1);
////
////	for (int thisReading = 0; thisReading < numReadings; thisReading++)
////		readingsy[thisReading] = 0;
////	for (int thisReading = 0; thisReading < numReadings; thisReading++)
////		readingsx[thisReading] = 0;
////
////
////}
////
////void loop() {
////
////
////	////////// bot Function //////////////////////////////////////
////
////	if ((millis() - wait > 1800UL) && (millis() - stopper > 5000UL)) {
////		//   pos_y = map( pos_y, -5, 185, 35, 180);
////
////		digitalWrite(lazer, HIGH); //Aktivera laser 
////		buttonPushCounter1 = 2;
////
////		//////////// Servo Bottom Y //////////////
////
////		if (pos_y == newpos_y) {
////			oldpos_y = newpos_y;
////			newpos_y = random(35, 180);
////
////			third_time++;
////
////
////			if (third_time % x == 0) {
////				wait = millis();                    //// Avgör slumpen av avbrott!
////				frame = random(1400, 3500);
////				x = random(1, 5);
////			}
////
////		}
////		if (millis() - wait_smoothY > 45UL) {
////
////			if (newpos_y > oldpos_y) {
////				pos_y++;
////			}
////			else {
////				pos_y--;
////			}
////
////			myservo1.write(pos_y);
////			wait_smoothY = millis();
////		}
////
////
////		/////////// Servo Top X /////////////
////
////		if (pos_x == newpos_x) {
////			oldpos_x = newpos_x;
////
////			newpos_x = random(10, 80);
////
////
////		}
////
////		if (millis() - wait_smoothX > 60UL) {
////
////			if (newpos_x > oldpos_x) {
////				pos_x++;
////			}
////			else {
////				pos_x--;
////			}
////
////			myservo.write(pos_x);
////			wait_smoothX = millis();
////		}
////	}
////
////
////	//////////////////////////////////
////
////
////
////
////
////	/// Top Servo   - Joy x
////
////	if (millis() - pos_timey > 30UL) {   // Servo acceleration Y
////										 // Plus
////		if (joyy > 92) {
////			myservo1.write(pos_y--);
////			newpos_y = pos_y;
////			third_time = 0;
////			stopper = millis();
////		}
////		// Minus
////		if (joyy < 90) {
////			myservo1.write(pos_y++);
////			newpos_y = pos_y;
////			third_time = 0;
////			stopper = millis();
////		}
////		pos_timey = millis();
////
////	}
////
////
////	/// Bottom Servo   - Joy x
////
////	if (millis() - pos_timex > 25UL) {    // Servo acceleration X
////										  // Plus
////
////		if (joyx > 88) {
////			myservo.write(pos_x--);
////			newpos_x = pos_x;
////			third_time = 0;
////
////			stopper = millis();
////		}
////
////		// Minus
////		if (joyx < 86) {
////			myservo.write(pos_x++);
////			newpos_x = pos_x;
////			third_time = 0;
////			stopper = millis();
////
////		}
////		pos_timex = millis();
////
////	}
////
////
////	//// Lazer Button
////
////	// read the pushbutton input pin:
////	buttonState1 = digitalRead(joyButton);
////
////	// compare the buttonState to its previous state
////	if (buttonState1 != lastButtonState1) {
////		// if the state has changed, increment the counter
////		if (buttonState1 == HIGH) {
////			// if the current state is HIGH then the button
////			// wend from off to on:
////			buttonPushCounter1++;
////
////		}
////	}
////
////	lastButtonState1 = buttonState1;
////	if (buttonPushCounter1 % 2 == 0) {
////		digitalWrite(lazer, HIGH);
////
////	}
////	else {
////		digitalWrite(lazer, LOW);
////
////	}
////
////	//// music- knappen
////
////	music = digitalRead(musicButton);
////	if ((music == LOW) && (millis() - music_timer > 12000UL)) {
////		Note = 0;
////		music_timer = millis();
////		stopper = millis();
////	}
////
////	if (Note < 54) {
////		currentMillis = millis();
////		interval = (duration * 1.2);
////		if (currentMillis - previousMillis >= interval) {
////			previousMillis = currentMillis;
////			duration = pace / noteDurations[Note];//Adjust duration with the pace of music
////			tone(speaker, melody[Note], duration); //Play note
////			Note++;
////		}
////	}
////
////
////
////
////	//// Serial print - Ser vad som händer
////
////	if (millis() - timewatch > 200UL) {
////		Serial.print("   pos_Y: ");
////		Serial.print(pos_y);
////		Serial.print("   newpos_Y: ");
////		Serial.print(newpos_y);
////		Serial.print("        ");
////		Serial.print("   X: ");
////		Serial.print(pos_x);
////		Serial.print("   New-X: ");
////		Serial.print(newpos_x);
////		Serial.print("        ");
////		Serial.print("   X: ");
////		Serial.print(x);
////		Serial.print("        ");
////		Serial.print("   Pause: ");
////		Serial.println(frame);
////		timewatch = millis();
////	}
////}
////
////void Smoothing(int total, int readings[], int index, short int joyInput, int joyPosValue) {
////	total = total - readings[index];
////	readings[index] = analogRead(joyInput);
////	total = total + readings[index];
////	index = index >= numReadings ? 0 : index + 1;
////	joyPosValue = total / numReadings;
////	joyPosValue = map(joyy, 0, 1011, -5, 185);
////}
////void SmoothingLab() {
////	Smoothing(totaly, readingsy, indexy, joyInputY, joyy);
////	Smoothing(totaly, readingsy, indexy, joyInputY, joyy);
////
//	pos_x = constrain(pos_x, -5, 186);
//	pos_y = constrain(pos_y, -5, 186);
//	newpos_y = constrain(newpos_y, -5, 186);
//	newpos_x = constrain(newpos_x, -5, 186);
//
	/// smoothing y
	//// subtract the last reading:
	//totaly = totaly - readingsy[indexy];
	//// read from the sensor:  
	//readingsy[indexy] = analogread(joyinputy);
	//// add the reading to the total:
	//totaly = totaly + readingsy[indexy];
	//// advance to the next position in the array:  
	//indexy = indexy + 1;

	//// if we're at the end of the array...
	//if (indexy >= numreadings)
	//	// ...wrap around to the beginning:
	//	indexy = 0;

	//// calculate the average:
	//joyy = totaly / numreadings;
	//// send it to the computer as ascii digits   

	//joyy = map(joyy, 0, 1011, -5, 185);
////
////
////
////
////
////
////
////
////	///// Smoothing X
////	//// subtract the last reading:
////	//totalx = totalx - readingsx[indexx];
////	//// read from the sensor:  
////	//readingsx[indexx] = analogRead(joyInputX);
////	//// add the reading to the total:
////	//totalx = totalx + readingsx[indexx];
////	//// advance to the next position in the array:  
////	//indexx = indexx + 1;
////
////	//// if we're at the end of the array...
////	//if (indexx >= numReadings)
////	//	// ...wrap around to the beginning:
////	//	indexx = 0;
////
////	//// calculate the average:
////	//joyx = totalx / numReadings;
////	//// send it to the computer as ASCII digits   
////	/////  
////
////	//joyx = map(joyx, 0, 1011, -5, 185);
////
////
////
////
////}

//void ReadMove(
//	unsigned long timer,
//	int timeoutRead,
//	int joyPos,
//	int upperBuffer,
//	int lowerBuffer,
//	int posServo,
//	int newPosServo,
//	Servo servo) 
//{
//	if (millis() - timer > timeoutRead) {
//		if (joyPos > upperBuffer || joyPos < lowerBuffer) {
//
//			int movement = 0;
//
//			if (joyPos > upperBuffer) {
//				movement = -1;
//			}
//			else
//				if (joyPos < lowerBuffer) {
//				movement = +1;
//			}
//			posServo += movement;
//			servo.write(posServo);
//
//			// reset
//			newPosServo = posServo;
//			counterPause = 0;
//			robotTimer = millis();
//			timeoutRead = millis();
//		}
//}