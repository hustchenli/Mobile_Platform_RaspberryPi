//!
//! \file encoder.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 04-Jun-2018
//! \brief measures the rotation angle of two wheels.
//!
//!  Using the low /high voltage change of A,B hall sensor to count the rotation grad.
//! 1200 Tics means a circle. To aviod the high rotation velocity, the sampling interval
//! should setd as small as possible.
//!
#ifndef ENCODER_H
#define ENCODER_H

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

using namespace std;
class Encoder: public AbstractSensor
{
    Q_OBJECT

public:
    //! \brief initializition of Encoder
    //!
    //! \param [in] nPinEncoderA
    //! \param [in] nPinEncoderB
    //! \param [in] nSensorID
    Encoder(int nPinEncoderA, int nPinEncoderB, int nSensorID);
    ~Encoder();
    //! \brief Reseting the sensor value(rewrite the virtual methde)
    //!
    void resetSensor();

public slots:
    //! \brief Reading the sensor value(rewrite the virtual methde)
    //!
    //! transfer the current encoder value to the variable m_dSensorvalue
    void slot_readSensor();

private:

    bool initSensor(){};

    int     m_nPinEncoderA;
    int     m_nPinEncoderB;
    int     m_nPulseCounter;
    QMutex m_mutex_PulseCounter;

};

#endif // ENCODER_H
