#include "Pitches.h"
#include <Servo.h> 
Servo ServY;
Servo ServX;
#define NO_SOUND 0 

#pragma region Input declaration

#define joyInputX		A6  // X-Axis  purple
#define joyInputY		A7  // Y-Axis vit

#define	servoTop		5	// Servo top blå
#define	servoBottom		6	// Servo bottom lila 
#define	lazer			4	// Lazer button  gul
#define speaker			7	// Piezo speaker green
#define	musicButton		3	// Button musics
#define joyButton		2  // Button on joystick green/ orange

#pragma endregion

#pragma region Variables

const int numReadings = 10;
int bufferX[] = { 15338, 15538 },
bufferY[] = { 16427, 16627 };
int
songTrack = 0,
timerMovementDelay,
buttonState = 1,
lastButtonState = 0,
buttonPushCounter,
joyX,
joyY,
posServoTop = 90,
posServoBottom = 90,
newPosServoBottom = 90,
newPosServoTop = 90,
buttonMusicVal,
counterPause,
second_time = 3,
pauseExecCounter = 2,
randomPauseTime = 1200;

long interval;
unsigned long
previousMillis = 0,
currentMillis,
timewatch,
timerJoyReadY,
timerJoyReadX,
musicTimer,
rand_seq,
timerAutoBot = 0,
wait,
timerSmoothX,
timerSmoothY;
#pragma endregion

#pragma region Music



int melody[][100] = {

  {NOTE_E4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_E4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_E4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_E4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_G4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_DS5,NOTE_D5,NOTE_B4,NOTE_A4,NOTE_B4,
  NOTE_E4,NOTE_G4,NOTE_DS5,NOTE_D5,NOTE_G4,NOTE_B4,
  NOTE_B4,NOTE_FS5,NOTE_F5,NOTE_B4,NOTE_D5,NOTE_AS5,
  NOTE_A5,NOTE_F5,NOTE_A5,NOTE_DS6,NOTE_D6,NO_SOUND},

  { NOTE_E7, NOTE_E7, 0, NOTE_E7,
  0, NOTE_C7, NOTE_E7, 0,
  NOTE_G7, 0, 0,  0,
  NOTE_G6, 0, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0,

  NOTE_C7, 0, 0, NOTE_G6,
  0, 0, NOTE_E6, 0,
  0, NOTE_A6, 0, NOTE_B6,
  0, NOTE_AS6, NOTE_A6, 0,

  NOTE_G6, NOTE_E7, NOTE_G7,
  NOTE_A7, 0, NOTE_F7, NOTE_G7,
  0, NOTE_E7, 0, NOTE_C7,
  NOTE_D7, NOTE_B6, 0, 0 }
};


int noteDurations[][100] = { {

  8,16,16,8,4,8,8,8,
  8,16,16,8,4,8,8,8,
  8,16,16,8,4,8,8,8,
  8,16,16,8,4,8,8,8,
  8,2,8,8,1,
  8,4,8,4,8,8,
  8,8,4,8,4,8,
  4,8,4,8,3},
  { 12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12,

  9, 9, 9,
  12, 12, 12, 12,
  12, 12, 12, 12,
  12, 12, 12, 12 }
};

int pace = 1100; // change pace of music("speedy")
int duration;
int Note = 54;
#pragma endregion

void setup()
{
	ServX.attach(servoBottom);  
	ServY.attach(servoTop);
	Serial.begin(115200);
	pinMode(lazer, OUTPUT);

	ServX.write(90);
	ServY.write(90);

	pinMode(joyButton, INPUT_PULLUP);
	pinMode(musicButton, INPUT_PULLUP);
	digitalWrite(joyButton, HIGH); 


}

void loop() {

	AutoBotMode();
	ManualMode();

}


void ManualMode() {

	do
	{
		if (ReadJoystick())
		{
			// Top Servo - Joy X
			MoveServo(
				timerJoyReadX,
				timerMovementDelay,
				joyX,
				bufferX[1],
				bufferX[0],
				posServoTop,
				newPosServoTop,
				ServY);
			// Bottom Servo - Joy Y
			MoveServo(
				timerJoyReadY,
				timerMovementDelay,
				joyY,
				bufferY[1],
				bufferY[0],
				posServoBottom,
				newPosServoBottom,
				ServX);
		};
		ReadingButtons();
		PlayMusic();

	} while ((millis() - timerAutoBot < 5000UL));

}
void AutoBotMode() {

	// TODO: Do While Loop

	digitalWrite(lazer, HIGH);
	buttonPushCounter = 2;
	do
	{
		// bot Function 
		if (millis() - wait > randomPauseTime) {
			// Servo Bottom X
			if (posServoBottom == newPosServoBottom) {
				newPosServoBottom = random(10, 160);
				counterPause++;
				//// Pause Module
				if (counterPause % pauseExecCounter == 0) {
					wait = millis();
					randomPauseTime = random(1400, 3500);
					pauseExecCounter = random(1, 5);
					counterPause = 0;
				}
			}
			// Move X
			if (millis() - timerSmoothY > 59UL) {

				if (newPosServoBottom > posServoBottom) {
					posServoBottom += 1;
				}
				else if (newPosServoBottom < posServoBottom) {
					posServoBottom -= 1;
				}
				ServX.write(posServoBottom);
				timerSmoothY = millis();
			}


			// Servo Top Y
			if (posServoTop == newPosServoTop) {
				newPosServoTop = random(90, 145);
			}

			// Move Y
			if (millis() - timerSmoothX > 45UL) {

				if (newPosServoTop > posServoTop) {
					posServoTop++;
				}
				else if (newPosServoTop < posServoTop) {
					posServoTop--;
				}
				ServY.write(posServoTop);
				timerSmoothX = millis();
			}
		}
	} while (!ReadJoystick());

	//Serial.print("   Top_Y: ");
	//Serial.print(posServoTop);
	//Serial.print("   newpos Y: ");
	//Serial.print(newPosServoTop);
	//Serial.print("        ");
	//Serial.print("   Bottom X: ");
	//Serial.print(posServoBottom);
	//Serial.print("   newpos X: ");
	//Serial.println(newPosServoBottom);
}
bool ReadJoystick() {
	joyX = map(joyX = analogRead(joyInputX), 0, 1023, 0, 32766);
	joyY = map(joyY = analogRead(joyInputY), 0, 1023, 0, 32766);
	if (joyX == 0 ||
		joyY == 0 ||
		joyX == 32766 ||
		joyY == 32766)
	{
		timerMovementDelay = 25;
	}
	else
	{
		timerMovementDelay = 200;
	}

	if (joyX < bufferX[0] ||
		joyY < bufferY[0] ||
		joyX > bufferX[1] ||
		joyY > bufferY[1]
		)
	{
		return true;
	}
	else
		return false;
}
void MoveServo(unsigned long& timer, const int timeoutRead, int& joyPos, int upperBuffer, int lowerBuffer, int& posServo, int& newPosServo, Servo& servo)
{
	if (millis() - timer > timeoutRead) {
		if (joyPos > upperBuffer || joyPos < lowerBuffer) {
			if (joyPos > upperBuffer) {
				posServo--;
			}
			else
			if (joyPos < lowerBuffer) {
				posServo++;
			}
			posServo = constrain(posServo, -6, 186);
			servo.write(posServo);

			// reset
			newPosServo = posServo;
			counterPause = 0;
			timerAutoBot = millis();
			timer = millis();
		}
	}
}
void PlayMusic() {
	if (Note < 54) {
		currentMillis = millis();
		interval = (duration * 1.2);
		if (currentMillis - previousMillis >= interval) {
			previousMillis = currentMillis;
			duration = pace / noteDurations[songTrack][Note];//Adjust duration with the pace of music
			tone(speaker, melody[songTrack][Note], duration); //Play note
			Note++;
		}
	}
}
void ReadingButtons() {

	// Lazer
	buttonState = digitalRead(joyButton);

	if (buttonState != lastButtonState) {
		if (buttonState == HIGH) {
			buttonPushCounter++;
		}
		lastButtonState = buttonState;
	}

	if (buttonPushCounter % 2 == 0) {
		digitalWrite(lazer, HIGH);
	}
	else {
		digitalWrite(lazer, LOW);
	}

	// Music
	buttonMusicVal = digitalRead(musicButton);
	if ((buttonMusicVal == LOW) && (millis() - musicTimer > 5000UL)) {
		Note = 0;
		songTrack++;
		songTrack = songTrack > 1 ? 0 : songTrack;
		musicTimer = millis();
		timerAutoBot = millis();
	}
}
