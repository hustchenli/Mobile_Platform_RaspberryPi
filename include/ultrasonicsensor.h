//!
//! \file ultrasonicsensor.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 04-Jun-2018
//! \brief  a sensor class for the sensor that can measure the hight from the ground to the above obstacle
//!
//! when this sensor is called, fisrt, it will emit a ultraschall wave, then the sensor
//! will wait the return of the wave. when the wave is coming, then count the
//! duration of the return wave. using the gived calculation formula, the distance
//! can be solved out. In this sensor the mutex will also be used at the point of
//! value delivering, which can protect the ressorce in the storage.
//!

#ifndef ULTRASONICSENSOR_H
#define ULTRASONICSENSOR_H

#include <QObject>
#include <QMutex>
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QThread>

#include <iostream>

#include <wiringPiI2C.h>
#include <wiringPi.h>

#include "abstractsensor.h"

class UltrasonicSensor: public AbstractSensor
{
    Q_OBJECT

public:
    UltrasonicSensor(int nPinSonic, int nSensorID);
    ~UltrasonicSensor();

public slots:
    //! \brief Reading the sensor value(rewrite the virtual methde)
    //!
    //! transfer the current hight to the variable m_dSensorvalue
    //!
    void slot_readSensor();

private:
    bool initSensor();

    int m_nPinSonic;
};

#endif // ULTRASONICSENSOR_H
