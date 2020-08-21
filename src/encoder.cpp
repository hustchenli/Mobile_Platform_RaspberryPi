
#define nTicsProRadumdrehung 1000

#include "encoder.h"

Encoder::Encoder(int nPinEncoderA, int nPinEncoderB, int nSensorID) : AbstractSensor()
{
    m_nPinEncoderA = nPinEncoderA;
    m_nPinEncoderB = nPinEncoderB;
    m_nSensorID = nSensorID;
    m_nPulseCounter = 0;
    m_dSensorValue = 0.;
    wiringPiSetup();
    pinMode(m_nPinEncoderA,INPUT);
    pinMode(m_nPinEncoderB,INPUT);
}

Encoder::~Encoder()
{
}


/*bool Encoder::initSensor()
{

        return true;
}*/

void Encoder::resetSensor()
{
    m_nPulseCounter = 0;
    double resetEncoderValue = 0.0;
    m_mutex_value.lock();
    m_dSensorValue = resetEncoderValue;
    m_mutex_value.unlock();
}

void Encoder::slot_readSensor()
{
    m_bIsRunning = true;

    while(m_bIsRunning)
    {
        bool m_bbeforeA = digitalRead(m_nPinEncoderA);//records last value and current value and then make Comparison
        bool m_bafterA = digitalRead(m_nPinEncoderA);
        bool m_bbeforeB = digitalRead(m_nPinEncoderB);
        bool m_bafterB = digitalRead(m_nPinEncoderB);

        while(m_bafterA == m_bbeforeA && m_bafterB == m_bbeforeB && m_bIsRunning)
        {
            m_bafterA = digitalRead(m_nPinEncoderA);
            m_bafterB = digitalRead(m_nPinEncoderB);
        }
        if (m_bbeforeA == m_bafterB)
           { m_mutex_PulseCounter.lock();
            m_nPulseCounter++;
            m_mutex_PulseCounter.unlock();}

        else if (m_bbeforeB == m_bafterA)
        {   m_mutex_PulseCounter.lock();
            m_nPulseCounter--;
            m_mutex_PulseCounter.unlock();
        }
        m_mutex_value.lock();
        //caculate the total grad of the rotation
        m_dSensorValue = (double)m_nPulseCounter/nTicsProRadumdrehung*360;
        m_mutex_value.unlock();        
        QCoreApplication::processEvents();
    }
//    emit sgn_finished();
}
