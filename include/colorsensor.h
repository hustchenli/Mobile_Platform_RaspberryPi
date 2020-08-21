//!
//! \file colorsensor.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 04-Jun-2018
//! \brief to sense the different color
//!
//! take advantage of RGB-color-table to recognize 6 different colors: white, black, green,
//! red, yellow, blue
//!
#ifndef COLORSENSOR_H
#define COLORSENSOR_H

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

class ColorSensor: public AbstractSensor
{
    Q_OBJECT

public:

    ColorSensor(int nSensorID);
    ~ColorSensor();

public slots:
    //! \brief Reading the sensor value(rewrite the virtual methde)
    //!
    //! transfer the current color to the variable m_dSensorvalue
    void slot_readSensor();

private:
    bool initSensor(){};

    int readColor(int nbRed, int nbBlue, int nbGreen,int nbBrightness);

    int m_iDeviceHandleId;

};

#endif // COLORSENSOR_H
