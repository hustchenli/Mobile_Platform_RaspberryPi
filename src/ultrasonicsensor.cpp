#define nSamplingRateUltrasonicSensor 50//miliseconds
#define SensortoGround 12.0 //cm
#define nFactor 58


#include "ultrasonicsensor.h"

UltrasonicSensor::UltrasonicSensor(int nPinSonic, int nSensorID) : AbstractSensor()
{
    m_nPinSonic = nPinSonic;
    m_nSensorID = nSensorID;
    m_nSamplingRate = nSamplingRateUltrasonicSensor;
    m_pTimerSamplingRate->setInterval(m_nSamplingRate);
    m_pTimerSamplingRate->start();
    initSensor();
}

UltrasonicSensor::~UltrasonicSensor()
{
}

bool UltrasonicSensor::initSensor()
{   
        wiringPiSetup();
        return true;
}


void UltrasonicSensor::slot_readSensor()
{

    int nTimebefore = 0;
    int nTimeafter = 0;
    int nTimeDifference = 0;

    pinMode(m_nPinSonic,OUTPUT);
    digitalWrite(m_nPinSonic,LOW);
    delayMicroseconds(2);
    digitalWrite(m_nPinSonic,HIGH);
    delayMicroseconds(10);
    digitalWrite(m_nPinSonic,LOW);
    pinMode(m_nPinSonic,INPUT);

    while(!digitalRead(m_nPinSonic))// jump out from the loop when the Echo Back
    {

    }
    nTimebefore = micros();

    while(digitalRead(m_nPinSonic))
    {

    }
    nTimeafter = micros();
    m_mutex_value.lock();
    nTimeDifference = nTimeafter - nTimebefore;
    if (nTimeDifference > 38000) // too high means that there is no obstacle
        nTimeDifference = 0;
    m_dSensorValue = SensortoGround+ nTimeDifference / nFactor;
    m_mutex_value.unlock();
}

