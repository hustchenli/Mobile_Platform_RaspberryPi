//!
//! \file abstractsensor.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 04-Jun-2018
//! \brief build a abstract class for all sensors
//!
//! in this part all of sensorparameters are included, so that other sensors can inherit
//! the objects from the abstractsensor
//!
#ifndef ABSTRACTSENSOR_H
#define ABSTRACTSENSOR_H

#include <QObject>
#include <QMutex>
#include <QApplication>
#include <QDebug>
#include <QTimer>
#include <QThread>

#include <iostream>

#include <wiringPiI2C.h>
#include <wiringPi.h>
#define nMapLineSensorA 1
#define nMapLineSensorB 2
#define nMapColorSensor 3
#define nMapSonicSensor 4
#define nMapLeftEncoderMotorA 5
#define nMapRightEncoderMotorB 6


class AbstractSensor: public QObject
{
    Q_OBJECT

public:
            AbstractSensor();
    virtual ~AbstractSensor();
    //! \brief get the Value from Sensors
    //!
    //! get the value of sensor through the reading of m_dSensorValue
    //!
    virtual double getValue();
    //! \brief delete the Thread of corresponding Sensors
    //!
    //! emits the finishing signal to activate the Part of deleting progarmm in Mobileplatform
    //!
    virtual void DeleteSensorundThread();
    //! \brief virtual with empty default implementation, here just needed for encoder
    virtual void resetSensor() {}   // virtual with empty default implementation, here just needed for encoder
public slots:
    //! \brief stopping of the Sensor
    //!
    //! turn off the "button" variable of sensor and turn off the Qtimer in the sensor
    //!
    virtual void SensorStop();
protected:
    virtual bool initSensor() = 0;

    int     m_nSensorID;
    int     m_nSamplingRate;
    bool    m_bIsRunning;
    double  m_dSensorValue;


    QMutex  m_mutex_value; // mutex for protecting the SensorValue
    QMutex  m_mutex_reset; // mutex for resetting the sensor (in this programm just the encoder) 
    QTimer* m_pTimerSamplingRate;


public slots:
    //! \brief stopping of the Sensor
    //!
    //! turn off the "button" variable of sensor and turn off the Qtimer in the sensor
    //!
    virtual void slot_readSensor() = 0;

signals:
    void sgn_newSensorData(int SensorID, double Data);
    void sgn_finished();

};

#endif // ABSTRACTSENSOR_H
