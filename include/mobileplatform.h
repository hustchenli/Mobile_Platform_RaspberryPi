//!
//! \file mobileplatform.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 04-Jun-2018
//! \brief to build new instances, and connect the sensor with Threads
//!
//! this part is designed to use different Threads, which can be implemented appoximately simultaneously,
//! to get values from different sensors, and the controlling of motors and follow-the-line
//! algorithm are also included. Besides, in Challenge task, a statemachine is designed in this
//! class. Through a lots of times of calling to the methodes in the Odometrie class, will the
//! total task be done.
//!

#ifndef MOBILEPLATFORM_H
#define MOBILEPLATFORM_H

#include <QObject>
#include <QCoreApplication>
#include <QThread>
#include <QTimer>
#include <QtXml>
#include <QState>
#include <QFinalState>


#include <iostream>
#include <time.h>
#include <wiringPi.h>
#include <unistd.h>
#include <qstatemachine.h>


#include "dcmotor.h"
#include "abstractsensor.h"
#include "linesensor.h"
#include "colorsensor.h"
#include "encoder.h"
#include "ultrasonicsensor.h"
#include "odometrie.h"
#include "xmlparsen.h"
#include "sorter.h"

#define nMapLineSensorA 1
#define nMapLineSensorB 2
#define nMapColorSensor 3
#define nMapSonicSensor 4
#define nMapLeftEncoderMotorA 5
#define nMapRightEncoderMotorB 6

#define nLinelength1 80//distance of straight forward moving
#define nLinelength2 100//distance of straight forwardward moving back
#define nvelocity1 50//forward velocity
#define nvelocity2 130//rotation velocity
#define nAngel       360
#define nvelocity3 200//backward velocity

using namespace std;

class MobilePlatform : public QObject //Damit Signals und Slots funktioniert muss "MobilePlattform" die Eigenschaften von "QObejct" erben
{
    Q_OBJECT // Makro fuer QObjects

public:

    MobilePlatform();
    ~MobilePlatform();
    //! \brief Stop all the sensors and prepare for the deletion of the thread
    //!
    void StopSensorforDelete();
    //void Odometrie_Line(double value, double destination);//+++++++++++++++++++++++++++
    //void Odometrie_Rotation(double value,double destination);//++++++++++++++++++++++++    void Odometrie_Line(double value, double destination);//+++++++++++++++++++++++++++


private:

    DcMotor m_motorLeft; // motor left
    DcMotor m_motorRight; // motor right
    QMutex m_mutex_getVelocity;
    Sorter m_Sorter;
    int m_nPWM; // controls velocity of motors by pulse-width-modulation
    bool m_bFollowLine; // condition that controls execution of autonomous line following
    double m_nTa;//
    /*enum StorageRackstates
    {
        Yellow,
        Red,
        Green
    };
*/  //!brief pid or folow line algorithm controled straight driving
    void ControlledStraight(double value, double destination, double targetColor, double verticaldistance,bool openline);
    //!brief pid controled rotation
    void ControlledTurn(double rvalue, double angle);
    void connectSensorThread(AbstractSensor *sensor , QThread *thread);
    void connectControlThread(Odometrie *contr, QThread *thread);
    AbstractSensor* mapSensorIDObject(int SensorID);

    AbstractSensor* m_pLineSensorA;
    AbstractSensor* m_pLineSensorB;
    AbstractSensor* m_pColorSensor;
    AbstractSensor* m_pEncoderLeft;
    AbstractSensor* m_pEncoderRight;
    AbstractSensor* m_pSonicSensor;
    Odometrie* m_pOdometrie;
    QThread* m_pThreadLineA;
    QThread* m_pThreadLineB;
    QThread* m_pThreadColor;
    QThread* m_pThreadEncoderLeft;
    QThread* m_pThreadEncoderRight;
    QThread* m_PThreadSonic;
    QThread* m_pThreadOdometrie;

    //QTimer*  m_pVelocitySamplingRate;//+++++++
    bool m_bOdoRunning;
    int m_timesfromstart;
    bool m_bChallengeIsRunning;
    QString m_path;
    OrderList orderarray;
    OrderList getorderarray;
    double m_controlledSvelocity;
    double m_controlledCvelocity;
    double m_compensationvelocity;

    enum StorageRack
    {
        UNKNOWN_RACK = -1,
        RACK_RED,
        RACK_GREEN,
        RACK_YELLOW
    };

    enum WarehouseClerk
    {
        UNKNOWN_CLERK = -1,
        CLERK_A,
        CLERK_B
    };
    StorageRack state_Storagerack=UNKNOWN_RACK;
    WarehouseClerk state_WarehouseClerk=UNKNOWN_CLERK;
    WarehouseClerk state_WarehouseClerklast=UNKNOWN_CLERK;

public slots:
    //! \brief mobileplatform move forward with the setted pwm value
    void slot_moveForward();
    //! \brief mobileplatform move backward with the setted pwm value
    void slot_moveBackward();
    //! \brief mobileplatform turn left with the setted pwm value
    void slot_moveLeft();
    //! \brief mobileplatform sharply turn left with the setted pwm value
    void slot_moveSharpLeft();
    //! \brief mobileplatform turn right with the setted pwm value
    void slot_moveRight();
    //! \brief mobileplatform sharply turn right with the setted pwm value
    void slot_moveSharpRight();
    //! \brief when stop button on the GUI was pressed, stop the Movement of the Mobileplatform
    void slot_stopMotion();
     //! \brief set the PWM value to control the velocity
    void slot_setPWM(double value);
     //! \brief when the path editor on the GUI have been changed,read in the new path
    void slot_pathChanged(QString path);
    //!  \brief execute the follow line algorithum, make the mobileplatform walk along the black line
    void slot_followline();
     //! \brief get the current sensor value in a Sensor with the got Sensor ID
    void slot_getNewSensorValues(int SensorID);
     //! \brief reset sensors and initialized for follw running
    void slot_resetSensor(int SensorID);
    //! \brief to finidh odometrie task
    void slot_OdometrieStart();
    //! \brief parse the new path and save it in the Pi
    void slot_XMLparsen();
    //! \brief to finish task from orignal go to warehouse or from Clerk A/B go to warehouse
    void slot_GotoLager();
    //! \brief get Ware in the Warehouse and go back to the exit of the Warehouse
    void slot_getWare();
    //! \brief make the mobileplatform run to the clerkA/B
    void slot_GotoClerk();
    //! \brief put down ware into the clerkhouse and go back to the exit of the Clerkhouse
    void slot_putdownWare();
    //! \brief when all the suborders are finished, jump out the loop and go back from the Clerk Exit
    //! to the start point
    void slot_GobackStartpoint();

signals:
    void sgn_newSensorValues(int nSensorID, double Data);
    void sgn_forwardControlParameters(double value, int parameterID);
    void sgn_finished();
    void sgn_getChallengeStart();
    void sgn_atLagerturnpointIn();
    void sgn_atLagerturnpointOut();
    void sgn_atClerkpointIn();
    void sgn_atClerkpointOut();
    void sgn_allordersweredone();
    void sgn_quit();
    void sgn_GotoLager();
    void sgn_getWare();
    void sgn_GotoClerk();
    void sgn_putdownWare();
    void sgn_GobackStartpoint();
};

#endif // MOBILEPLATFORM_H

