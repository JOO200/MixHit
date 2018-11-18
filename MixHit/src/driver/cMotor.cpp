#include "cMotor.h"
cMotor::cMotor()
{

}

cMotor::cMotor(int pPinPwm, int pPinDrehrichtung)
{
	mPinDrehrichtung = pPinDrehrichtung;
	mPinPwm = pPinPwm;

	pinMode(mPinDrehrichtung, OUTPUT);
	pinMode(mPinPwm, OUTPUT);

	ledcSetup(CHANNEL, PWM_FREQUENCY, RESOLUTION);    //PWM Setup
	ledcAttachPin(mPinPwm, CHANNEL);                //Channelzuweisung zu PWM Pin

	MotorStop();
}

void cMotor::MotorStartR()
{
	if (stateRunning == false)
	{
		Anlauframpe(true);							//True: Drehrichtung rechts
		stateRunning = true;
	}
	else
	{
		digitalWrite(mPinDrehrichtung, HIGH);
		ledcWrite(CHANNEL, 255);
		stateRunning = true;
	}
}

void cMotor::MotorStartL()
{
	if (stateRunning == false)
	{
		Anlauframpe(false);							//True: Drehrichtung rechts
		stateRunning = true;
	}
	else
	{
		digitalWrite(mPinDrehrichtung, LOW);
		ledcWrite(CHANNEL, 255);
		stateRunning = true;
	}
}

void cMotor::MotorStop()
{
	if (stateRunning == true)
	{
		Bremsrampe();							//True: Drehrichtung rechts
		stateRunning = false;
	}
	else
	{
		ledcWrite(CHANNEL, 0);
		stateRunning = false;
	}
}

void cMotor::Anlauframpe(bool Drehrichtung)
{
	digitalWrite(mPinDrehrichtung, Drehrichtung);                   //Drehrichtung bestimmen
	for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle++)   //Motor Anfahrrampe rechtslauf
	{
		ledcWrite(CHANNEL, dutyCycle);
		delayMicroseconds(DelayAnlauframpe);
	}
}

void cMotor::Bremsrampe()
{
	for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle--)   //Motor Abbremsrampe
	{
		ledcWrite(CHANNEL, dutyCycle);
		delayMicroseconds(DelayAnlauframpe);
	}
}