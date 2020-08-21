#include "dcmotor.h"

DcMotor::DcMotor(int pinMotorA, int pinMotorB, int pinMotorE)
{
    m_nPinMotorA = pinMotorA;
    m_nPinMotorB = pinMotorB;
    m_nPinMotorE = pinMotorE;
    m_nPWM = 20;
    if (!initPins())
    {
        cout << "motorpins" << m_nPinMotorA << ", " << m_nPinMotorB << ", " << m_nPinMotorE << "are false" << endl;
    }
}

DcMotor::~DcMotor()
{
    m_nPinMotorA = 0;
    m_nPinMotorB = 0;
    m_nPinMotorE = 0;
    m_nPWM = 0;
}

bool DcMotor::initPins()
{
    bool bPinsMotorA = (m_nPinMotorA == 3 && m_nPinMotorB == 2 && m_nPinMotorE == 0);
    bool bPinsMotorB = (m_nPinMotorA == 24 && m_nPinMotorB == 25 && m_nPinMotorE == 23);
    if (bPinsMotorA || bPinsMotorB)
    {
        wiringPiSetup();
        pinMode(m_nPinMotorA,OUTPUT);
        pinMode(m_nPinMotorB,OUTPUT);
        pinMode(m_nPinMotorE,OUTPUT);
        softPwmCreate(m_nPinMotorE,0,100);
        return true;
    }
    else
        return false;
}

void DcMotor::forward()
{
   digitalWrite(m_nPinMotorA,1);
   digitalWrite(m_nPinMotorB,0);
}

void DcMotor::backward()
{
   digitalWrite(m_nPinMotorA,0);
   digitalWrite(m_nPinMotorB,1);
}

void DcMotor::stop()
{
    digitalWrite(m_nPinMotorA,0);
    digitalWrite(m_nPinMotorB,0);
}

void DcMotor::setPWM(double PWM)
{
    m_nPWM = PWM;
    softPwmWrite(m_nPinMotorE, PWM);
}

double DcMotor::getPWM()
{
    double nGetPWM = m_nPWM;
    return nGetPWM;
}

