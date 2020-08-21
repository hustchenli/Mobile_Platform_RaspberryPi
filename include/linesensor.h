//!
//! \file linesensor.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 04-Jun-2018
//! \brief to get the siganl of sensors
//!
//! by using the funktion digitalRead to get the status of the sensor, if the signal is receied,
//! that means the Robot has reached the boundary of the black line.
//!


#ifndef LINESENSOR_H
#define LINESENSOR_H

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



class LineSensor: public AbstractSensor
{
   Q_OBJECT

public:
    LineSensor(int nPinLineSensor, int nSensorID);
    ~LineSensor();
  //  int getLastStatus();
public slots:
    //! \brief Reading the sensor value(rewrite the virtual methde)
    //!
    //! transfer the current  linie sensor value to the variable m_dSensorvalue
    void slot_readSensor();

private:
    bool initSensor(){};
    int m_nPinLineSensor;

};

#endif // LINESENSOR_H
