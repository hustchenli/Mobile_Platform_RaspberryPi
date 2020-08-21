#define nDefaultSamplingRate 1
#include <abstractsensor.h>

AbstractSensor::AbstractSensor()
{

    m_bIsRunning = false;// the switch of sensor
    m_nSamplingRate = nDefaultSamplingRate; // initialzition of the Samplingrate
    m_pTimerSamplingRate = new QTimer();
    m_pTimerSamplingRate->setInterval(m_nSamplingRate);
    m_pTimerSamplingRate->setSingleShot(false);
    connect(m_pTimerSamplingRate, SIGNAL(timeout()), this, SLOT(slot_readSensor()));

}

AbstractSensor::~AbstractSensor()
{
}

double AbstractSensor::getValue()
{
    double value;
    m_mutex_value.lock();
    value = m_dSensorValue;
    m_mutex_value.unlock();
    return value;
}
void AbstractSensor::SensorStop()
{
    //std::cout<<"bbbbb: "<<QThread::currentThreadId()<<std::endl;
    if(m_bIsRunning)
    {
        m_bIsRunning = false;
//        qDebug() << "m_bIsRuning : false";
    }

    if (m_pTimerSamplingRate->isActive())
    {
 //        qDebug() << "SensorQTimer : stop";
    }
}

void AbstractSensor::DeleteSensorundThread()
{

    emit sgn_finished();
//    qDebug() << "Signal : finished";
}
