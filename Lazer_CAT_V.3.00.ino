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

int rangeRestrictX[]{ 10,160 };
int rangeRestrictY[]{ 90,145 };

float warpSpeed = 1.0;

int posX = 0;
int posY = 0;
int lastPosX = 0;
int lastPosY = 0;

int rangeX = rangeRestrictX[1] - rangeRestrictX[0];
int rangeY = rangeRestrictY[1] - rangeRestrictY[0];
///////////////
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
	randomSeed(analogRead(0));

}

void loop() {

	AutoBotMode();
	ManualMode();

}

void ModeHazzan() {

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

}
void ModeFluxiom() {
	digitalWrite(lazer, HIGH);
	float angle = 0;
	int sPosX = posX;
	int sPosY = posY;

	// Random variables
	int selectRandom = random(0, 12);
	int randomDelay = random(10, 51) / warpSpeed;
	int randomDelay2 = random(10, 101) / warpSpeed;
	int randomDelay3 = random(20, 31) / warpSpeed;
	int randomSteps = random(1, 4);
	int randomSteps2 = random(1, 5);
	bool luck = random(0, 2);
	int circleXcenter = random(80, 101);
	int circleYcenter = random(80, 101);

	// Keep things in bounds - needed for the unrestricted functions or you will wind up on the ceiling.
	if (posX<rangeRestrictX[0] || posX>rangeRestrictX[1]) {
		posX = rangeRestrictX[1] - rangeX / 2;
		ServX.write(posX);
	}
	if (posY<rangeRestrictY[0] || posY>rangeRestrictY[1]) {
		posY = rangeRestrictY[1] - rangeY / 2;
		ServY.write(posY);
	}


	switch (selectRandom) {

		// Taunt      
	case 0:
		for (int i = 0; i < 6; i++) {
			posX = random(rangeRestrictX[0] + rangeX / 4, rangeRestrictX[1] - rangeX / 4);
			posY = random(rangeRestrictY[0] + rangeY / 4, rangeRestrictY[1] - rangeY / 4);
			ServX.write(posX);
			ServY.write(posY);
			delay(randomDelay3 * 30 / warpSpeed);
		}
		break;

		// Wobble      
	case 1:
		for (int rad = 5; rad < 15; rad++) {
			for (int i = 5; i > 0; i--) {
				angle = i * 2 * 3.14 / 10;
				lastPosX = posX;
				lastPosY = posY;
				posX = circleXcenter + (cos(angle) * rad);
				posY = circleYcenter + (sin(angle) * rad);
				if (posX > lastPosX) { // Slow things down
					posX = lastPosX + 1;
				}
				else {
					posX = lastPosX - 1;
				}
				if (posY > lastPosY) {
					posY = lastPosY + 1;
				}
				else {
					posY = lastPosY - 1;
				}
				ServX.write(posX);
				ServY.write(posY);

				if (luck) {
					if (sPosX > rangeX / 2) {
						circleXcenter -= 1;
					}
					else {
						circleXcenter += 1;
					}
				}
				else {
					if (sPosY > rangeY / 2) {
						circleYcenter -= 1;
					}
					else {
						circleYcenter += 1;
					}
				}
				delay(randomDelay * 2);
			}
		}
		break;

		// Scan        
	case 2:
		posY = random(rangeRestrictY[0] + rangeY / 10, rangeRestrictY[1] - rangeY / 4);
		ServY.write(posY);
		for (posX = rangeRestrictX[0]; posX <= rangeRestrictX[1]; posX += 1) {
			ServX.write(posX);
			delay(randomDelay);
		}
		for (posX = rangeRestrictX[1]; posX >= rangeRestrictX[0]; posX -= 1) {
			ServX.write(posX);
			delay(randomDelay2);
		}
		break;

		// Zip        
	case 3:
		for (int i = 0; i < 3; i++) {
			posX = random(rangeRestrictX[0], rangeRestrictX[1]);
			posY = random(rangeRestrictY[0], rangeRestrictY[1]);
			ServX.write(posX);
			ServY.write(posY);
			delay(randomDelay2 * 10);
		}
		break;

		// Boomerang        
	case 4:
		lastPosX = posX;
		sPosX = random(rangeRestrictX[0], rangeRestrictX[1]);
		if (posX < sPosX) {
			for (lastPosX; posX <= sPosX; posX++) {
				ServX.write(posX);
				delay(randomDelay3);
			}
		}
		else {
			for (lastPosX; posX >= sPosX; posX--) {
				ServX.write(posX);
				delay(randomDelay3);
			}
		}
		for (posY; posY >= rangeRestrictY[0]; posY--) {
			posY -= 1;
			ServY.write(posY);
			delay(randomDelay3);
		}
		for (posY = rangeRestrictY[0]; posY <= rangeRestrictY[1] - rangeY / 5; posY += 1) {
			if (posY % 2) { // Wobble on the throw
				posX += 1;
				ServX.write(posX);
			}
			else {
				posX -= 1;
				ServX.write(posX);
			}
			ServY.write(posY);
			delay(randomDelay);
		}
		if (luck > 0) { // If we have no luck, the boomerang doesn't come back
			for (posY = rangeRestrictY[1] - rangeY / 5; posY >= rangeRestrictY[0]; posY -= 1) {
				if (posY > rangeRestrictY[1] - rangeY / 2) { // Curve on return
					if (posY % 2) {
						posX += 1;
						ServX.write(posX);
					}
				}
				else {
					if (posY % 2) {
						posX -= 1;
						ServX.write(posX);
					}
				}
				ServY.write(posY);
				delay(randomDelay2);
			}
		}
		break;

		// Creep        
	case 5:
		lastPosX = posX;
		sPosX = (posX + (rangeRestrictX[1] - rangeX / 2)) / 2;
		if (posX < sPosX) {
			for (lastPosX; posX <= sPosX; posX++) {
				ServX.write(posX);
				delay(randomDelay3);
			}
		}
		else {
			for (lastPosX; posX >= sPosX; posX--) {
				ServX.write(posX);
				delay(randomDelay3);
			}
		}
		lastPosY = posY;
		sPosY = (posY + (rangeRestrictY[1] - rangeY / 2)) / 2;
		if (posY < sPosY) {
			for (lastPosY; posY <= sPosY; posY++) {
				ServY.write(posY);
				delay(randomDelay3);
			}
		}
		else {
			for (lastPosY; posY >= sPosY; posY--) {
				ServY.write(posY);
				delay(randomDelay3);
			}
		}
		for (int i = 0; i < 20; i++) {
			if (luck) {
				posX += randomSteps;
				posY += randomSteps;
			}
			else {
				posX -= randomSteps;
				posY -= randomSteps;
			}
			ServX.write(posX);
			ServY.write(posY);
			delay(randomDelay3 * 15 / warpSpeed);
		}
		break;

		// Squiggle        
	case 6:
		for (int i = 0; i < 120; i++) {
			if (i % 2) {
				posX += randomSteps2;
				posY += randomSteps;
			}
			else {
				posX -= randomSteps2;
				posY -= randomSteps;
			}
			ServX.write(posX);
			ServY.write(posY);
			delay(randomSteps * 5 / warpSpeed);
		}
		break;

		// Blink        
	case 7:
		for (int i = 0; i < 10; i++) {
			digitalWrite(lazer, LOW);
			delay(randomDelay + randomDelay2 + (20 / warpSpeed));
			digitalWrite(lazer, HIGH);
			delay(randomDelay + randomDelay2 + (20 / warpSpeed));
		}
		break;

		// Circle                    
	case 8:
		if (luck) {
			for (int rad = 5; rad < 20; rad++) {
				for (int i = 0; i < 10; i++) {
					angle = i * 2 * 3.14 / 10;
					posX = circleXcenter + (cos(angle) * rad);
					posY = circleYcenter + (sin(angle) * rad);
					ServX.write(posX);
					ServY.write(posY);
					delay(randomSteps2 * 5 / warpSpeed);
				}
			}
		}
		else {
			for (int rad = 20; rad > 5; rad--) {
				for (int i = 10; i > 0; i--) {
					angle = i * 2 * 3.14 / 10;
					posX = circleXcenter + (cos(angle) * rad);
					posY = circleYcenter + (sin(angle) * rad);
					ServX.write(posX);
					ServY.write(posY);
					delay(randomSteps2 * 5 / warpSpeed);
				}
			}
		}
		break;

		// ZigZag
	case 9:
		if (luck) {
			for (posX = rangeRestrictX[0]; posX <= rangeRestrictX[0] + rangeX / 10; posX += 1) {
				posY += randomSteps;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay);
			}
			for (posX = rangeRestrictX[0] + rangeX / 10; posX <= rangeRestrictX[0] + rangeX / 5; posX += 1) {
				posY -= randomSteps;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay);
			}
			for (posX = rangeRestrictX[0] + rangeX / 5; posX <= rangeRestrictX[0] + rangeX / 3.3; posX += 1) {
				posY += randomSteps;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay);
			}
			for (posX = rangeRestrictX[0] + rangeX / 3.3; posX <= rangeRestrictX[0] + rangeX / 2.5; posX += 1) {
				posY -= randomSteps;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay);
			}
			for (posX = rangeRestrictX[0] + rangeX / 2.5; posX <= rangeRestrictX[0] + rangeX / 2; posX += 1) {
				posY += randomSteps;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay);
			}
			for (posX = rangeRestrictX[0] + rangeX / 2; posX <= rangeRestrictX[0] + rangeX / 1.65; posX += 1) {
				posY -= randomSteps;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay);
			}
		}
		else {
			for (posY = rangeRestrictY[1] - rangeY / 10; posY >= rangeRestrictY[1] - rangeY / 5; posY -= 1) {
				posX -= randomSteps2;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay2);
			}
			for (posY = rangeRestrictY[1] - rangeY / 5; posY >= rangeRestrictY[1] - rangeY / 3.3; posY -= 1) {
				posX += randomSteps2;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay2);
			}
			for (posY = rangeRestrictY[1] - rangeY / 3.3; posY >= rangeRestrictY[1] - rangeY / 2.5; posY -= 1) {
				posX -= randomSteps2;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay2);
			}
			for (posY = rangeRestrictY[1] - rangeY / 2.5; posY >= rangeRestrictY[1] - rangeY / 2; posY -= 1) {
				posX += randomSteps2;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay2);
			}
			for (posY = rangeRestrictY[1] - rangeY / 2; posY >= rangeRestrictY[1] - rangeY / 1.65; posY -= 1) {
				posX -= randomSteps2;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay2);
			}
			for (posY = rangeRestrictY[1] - rangeY / 1.65; posY >= rangeRestrictY[1] - rangeY / 1.425; posY -= 1) {
				posX += randomSteps2;
				ServX.write(posX);
				ServY.write(posY);
				delay(randomDelay2);
			}
		}
		break;

		// Chase
	case 10:
		posY = random(rangeRestrictY[0] + rangeY / 10, rangeRestrictY[1] - rangeY / 4);
		ServY.write(posY);
		posX = rangeRestrictX[1] - rangeX / 2;
		ServX.write(posX);
		for (int i = 40; i > 0; i--) {
			luck = random(0, 2); // Making our own luck
			if (luck) {
				posX += randomSteps2 * 2;
				ServX.write(posX);
			}
			else {
				posX -= randomSteps * 2;
				ServX.write(posX);
			}
			delay(randomDelay2);
		}
		break;

		// Cool
	case 11:
		if (luck) {
			digitalWrite(lazer, LOW);
			delay(randomSteps2 * 1000 / warpSpeed);
		}
		break;
	} // End switch

}
void AutoBotMode() {

	// TODO: Do While Loop

	digitalWrite(lazer, HIGH);
	buttonPushCounter = 2;
	do
	{
		ModeFluxiom();
		//ModeHazzan();
	} while (!ReadJoystick());
	digitalWrite(lazer, HIGH);

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
			posServo = constrain(posServo, 15, 165);
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
