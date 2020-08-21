#define nSamplingrateLineSensor 50 //in milliseconds

#include "linesensor.h"

LineSensor::LineSensor(int nPinLineSensor, int nSensorID) : AbstractSensor()
{
    m_nPinLineSensor = nPinLineSensor;
    m_nSensorID = nSensorID;
    m_nSamplingRate = nSamplingrateLineSensor;
    m_pTimerSamplingRate->setInterval(m_nSamplingRate);
    m_pTimerSamplingRate->start();
    wiringPiSetup();
    pinMode(m_nPinLineSensor, INPUT);
}

LineSensor::~LineSensor()
{
}


void LineSensor::slot_readSensor()
{  //QCoreApplication::processEvents();
    //std::cout<<"cccc: "<<QThread::currentThreadId()<<std::endl;
    m_mutex_value.lock();
    m_dSensorValue = digitalRead(m_nPinLineSensor);
    m_mutex_value.unlock();
 }

/*bool LineSensor::initSensor()
{  

        return true;
}*/

/*int LineSensor::getLastStatus()
{
    return digitalRead(m_nPinLineSensor);
}*/
