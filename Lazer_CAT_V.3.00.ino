#include "Pitches.h"
#include <Servo.h> 
Servo ServY;
Servo ServX;
#define NO_SOUND 0 

#pragma region Input declaration

#define joyInputX		A6  // X-Axis  purple
#define joyInputY		A7  // Y-Axis vit

#define	servoTop		5	// Servo top blå
#define	lazer			4	// Lazer button  gul
#define speaker			7	// Piezo speaker green
#define	servoBottom		6	// Servo bottom lila 
#define	musicButton		3	// Button musics
#define joyButton		2  // Button on joystick green/ orange

#pragma endregion

#pragma region Variables

const int numReadings = 10;
int
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
// Smoothing values
readingsy[numReadings],      // the readings from the analog input
indexy = 0,                  // the index of the current reading
totaly = 0,                  // the running total
averagey = 0,                // the average
readingsx[numReadings],      // the readings from the analog input
indexx = 0,                  // the index of the current reading
totalx = 0,                  // the running total
averagex = 0,                // the average
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
#pragma endregion

void setup()
{
	ServX.attach(servoBottom);  // attaches the servo on pin 9 to the servo object
	ServY.attach(servoTop);
	Serial.begin(115200);
	pinMode(lazer, OUTPUT);

	ServX.write(90);
	ServY.write(90);

	pinMode(joyButton, INPUT_PULLUP);
	pinMode(musicButton, INPUT_PULLUP);
	digitalWrite(joyButton, HIGH); // turn on pull-up resistors


	for (int thisReading = 0; thisReading < numReadings; thisReading++)
		readingsy[thisReading] = 0;
	for (int thisReading = 0; thisReading < numReadings; thisReading++)
		readingsx[thisReading] = 0;


}

void loop() {
	//SmoothingJoyValue();
	ResetDefectValues();
	AutoBotMode();
	ManualMode();

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
	if ((buttonMusicVal == LOW) && (millis() - musicTimer > 12000UL)) {
		Note = 0;
		musicTimer = millis();
		timerAutoBot = millis();
	}
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
			servo.write(posServo);

			// reset
			newPosServo = posServo;
			counterPause = 0;
			timerAutoBot = millis();
			timer = millis();
		}
	}
}
bool ReadJoystick() {
	joyX = map(joyX = analogRead(joyInputX), 0, 1023, 0, 32766);
	joyY = map(joyY = analogRead(joyInputY), 0, 1023, 0, 32766);
	// TODO: Move this?!
	int bufferX[] = { 15538, 15338 },
		bufferY[] = { 16427, 16627 };
	if (joyX == 0 ||
		joyY == 0 ||
		joyX == 32766 ||
		joyY == 32766)
	{
		timerMovementDelay = 40;
	}
	else
	{
		timerMovementDelay = 125;
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
void ManualMode() {
	ReadJoystick();

	// Top Servo - Joy X
	MoveServo(
		timerJoyReadX,
		timerMovementDelay,
		joyX,
		15540, // TODO: Update this
		15330,
		posServoTop,
		newPosServoTop,
		ServY);
	// Bottom Servo - Joy Y
	MoveServo(
		timerJoyReadY,
		timerMovementDelay,
		joyY,
		16630,
		16420,
		posServoBottom,
		newPosServoBottom,
		ServX);

	ReadingButtons();
	PlayMusic();
}
void AutoBotMode() {

	// TODO: Do While Loop

	digitalWrite(lazer, HIGH);
	buttonPushCounter = 2;
	do
	{
		// bot Function 
		if ((millis() - wait > randomPauseTime) &&
			(millis() - timerAutoBot > 5000UL)) {
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
//void Smoothing(int& total, int readings[10], int& index, int joyInput, int& joyPos) {
//	total = total - readings[index];
//	readings[index] = analogRead(joyInput);
//	total = total + readings[index];
//	index = index + 1;
//	if (index >= numReadings)
//	{
//		index = 0;
//	}
//	joyPos = total / numReadings;
//}
//void SmoothingJoyValue() {
//	//// Top Joy Y
//	//Smoothing(totaly, readingsy, indexy, joyInputY, joyY);
//	//// Bottom Joy X
//	//Smoothing(totalx, readingsx, indexx, joyInputX, joyX);
//}
void ResetDefectValues() {
	//Saftey measure
	posServoBottom = constrain(posServoBottom, -6, 186);
	posServoTop = constrain(posServoTop, -6, 186);
	newPosServoBottom = constrain(newPosServoBottom, -6, 186);
	newPosServoTop = constrain(newPosServoTop, -6, 186);
}
void PlayMusic() {
	if (Note < 54) {
		currentMillis = millis();
		interval = (duration * 1.2);
		if (currentMillis - previousMillis >= interval) {
			previousMillis = currentMillis;
			duration = pace / noteDurations[Note];//Adjust duration with the pace of music
			tone(speaker, melody[Note], duration); //Play note
			Note++;
		}
	}
}
