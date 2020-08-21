//!
//! \file Odometrie.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 25-Jul-2018
//! \brief combined with a new thread, this class can through the pid control achieve the function that makes the mobile move straightly and move around stable.
//!
//!
//! this class includes three follow another executed line or rotation movement. Those three
//! parts of movement are both controled by similar PID controller, which have different control
//! prameters for the speed control. Using the showed on the GUI real time diagramm, the PID parameter
//! could be adjusted to the suitable value(using the qcustomplot widgt). Besides, the position controller are also be used here,
//! when the mobileplatform comes approach to the destination, the velocity will go down and become lower than the start velocity.
//!
#ifndef ODOMETRIE_H
#define ODOMETRIE_H
#include<QApplication>
#include <QObject>
#include<dcmotor.h>
#include<encoder.h>
#include<colorsensor.h>
#include<ultrasonicsensor.h>
#include<linesensor.h>
#include<time.h>
#include<unistd.h>
#include<iostream>
#include<wiringPi.h>
#include <wiringPiI2C.h>
#include<QThread>
#include<abstractsensor.h>
#include<math.h>
#include<QtCore>
extern double velocityA;     //current velocity of left motor; global variable
extern double velocityB;     //current velocity of right motor; global variable

//bool m_bOdoRunning;

#define nMapLeftWheelVelocityKp 7
#define nMapLeftWheelVelocityKi 8
#define nMapLeftWheelVelocityKd 9

#define nMapRightWheelVelocityKp 10
#define nMapRightWheelVelocityKi 11
#define nMapRightWheelVelocityKd 12
class Odometrie:public QObject
{
    Q_OBJECT
    public:
        Odometrie(AbstractSensor*& EncoderLeft, AbstractSensor*& EncoderRight, AbstractSensor\
                  *&ColorSensor,AbstractSensor *&UltrasonicSensor,AbstractSensor*& LineSensorA,\
                  AbstractSensor*& LineSensorB,DcMotor& MotorLeft,DcMotor& MotorRight);
        ~Odometrie();
        void finished();

        bool m_bControlisruning;
        bool m_bline;
    private:
        QTimer m_pVelocitySamplingRate;
        QMutex m_mutex_updateControlParameter;
        QMutex m_mutex_Controlisruning;
        AbstractSensor*& RefEncoderLeft;
        AbstractSensor*& RefEncoderRight;
        AbstractSensor *&RefColorSensor;
        AbstractSensor *&RefUltrasonicSensor;
        AbstractSensor *&RefLineSensorA;
        AbstractSensor *&RefLineSensorB;
        DcMotor& RefMotorLeft;
        DcMotor& RefMotorRight;
       // void Odometrie_Line(double value, double destination);
        //void Odometrie_Rotation(double value,double destination);

        // PID parameter for left motor during straight forward moving
        double P_DATAA ;   //Kp parameter
        double I_DATAA ;     //Ki parameter
        double D_DATAA ; //Kd parameter
        // PID parameter for right motor during straight forward moving
        double P_DATAB  ;      //Kp parameter
        double I_DATAB  ;    //Ki parameter
        double D_DATAB  ;   //Kd parameter
       /*
        // PID parameter for left motor during turning
        #define P_DATAA2 0.110  //Kp parameter
        #define I_DATAA2 0      //Ki parameter
        #define D_DATAA2  0.0045052//Kd parameter
        // PID parameter for right motor during turning
        #define P_DATAB2 0.094 //Kp parameter
        #define I_DATAB2 0     //Ki parameter
        #define D_DATAB2  0.004//Kd parameter
        */
        double m_nLeftVelocity;
        double m_nRightVelocity;
        double m_nLeftDestination;
        double m_nRightDestination;
        double m_nErrorsumLeft;
        double m_nErrorsumRight;
        double m_nEncoderValueLeft;        //data of the left encoder; global variable
        double m_nEncoderValueRight;        //data of the right encoder; global variable
        double m_nLastEncoderValueLeft;
        double m_nLastEncoderValueRight;
        double m_nCurrentcolor;
        double m_nColor;
        double m_nCurrentHight;
        double m_nHight;
        double m_nYkLeft;
        double m_nYkRight;
        double m_nLocationYkLeft;
        double m_nLocationYkRight;
        double m_nAbsWaysumLeft;
        double m_nAbsWaysumRight;
        double m_nWaysumLeft;
        double m_nWaysumRight;
        double m_nPwmLeft;
        double m_nPwmRight;
        double m_nNewPwmLeft;
        double m_nNewPwmRight;
        double m_nEkLeft; //error for left motor
        double m_nLastEkLeft;
        double m_nEkRight;   //error for right motor
        double m_nLastEkRight;
        double m_nLocationErrorSumLeft;
        double m_nLocationErrorSumRight;
        double m_nLocationEkLeft;
        double m_nLocationEkRight;
        double m_nLastLocationEkLeft;
        double m_nLastLocationEkRight;
        double m_nLineLeft;
        double m_nLineRight;
        double m_nOrderCounter;
        double m_nLocationKpLeft;
        double m_nLocationKpRight;
       // int counter_line1;
        //int counter_line2;
        //double velocityA;
        //double velocityB;

public slots:
        //! \brief update the parameters of the PID controller
        //! \param [in] value   Kp,Ki or Kd value
        //! \param [in] parameterID
        void slot_updateVelocityControlParamters(double value, int parameterID);
        //! \brief achieve the velocitycontrol through pid control(called by timer) and folow algorithm
        void slot_velocitycontrol();
        //! \brief stops the velocity control through stop button on GUI or the satisfaction of the expected condition
        void stopControl();
        //! \brief Initialzition the parameters for pid controled movement or follow line movement
        //!
        //! the input parameters are showed as followed, and those expected condition can not be satisfied at the same time.
        //!
        //! \param [in] m_nLeftVelocity      expected velocity of left wheel
        //! \param [in] m_nRightVelocity     expected velocity of right wheel
        //! \param [in] m_nLeftDestination   expected destination of left wheel
        //! \param [in] m_nRightDestination  expected destination of right wheel
        //! \param [in] targetcolor          expected color
        //! \param [in] verticaldistance     expected vertical hight
        //! \param [in] line                 follow line function open or not
        void Controlmove(double m_nLeftVelocity, double m_nRightVelocity, double m_nLeftDestination, double m_nRightDestination, double targetcolor,\
                         double verticaldistance,bool line);
signals:
        void sgn_finished();//all the orders are finished
};

#endif // ODOMETRIE_H
