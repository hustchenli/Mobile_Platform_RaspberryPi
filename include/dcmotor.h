//!
//! \file dcmotor.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 04-Jun-2018
//! \brief to control the motor
//!
//! by setting the values of pins to control the direction and speed of the motors' rotation
//!
/*//! \param [in] pinMotorA, pinMotorB     for the direction of the rotation
//! \param [in] pinMotorE                to set PWM value, in order to control the speed of the rotation
//!
//! \param [out] forward, backward, stop     to control the motion of the motors
//! \param [out] setPWM        to change the velosity */
//!
#ifndef DCMOTOR_H
#define DCMOTOR_H

#include <wiringPi.h>
#include <softPwm.h>
#include <iostream>

using namespace std;

class DcMotor
{
public:

    DcMotor(int pinMotorA, int pinMotorB, int pinMotorE);
    ~DcMotor();
    //! \brief  writes the pins to realise moving forward action
    //!
    void forward();
    //! \brief  writes the pins to realise moving backward action
    //!
    void backward();
    //! \brief  writes the pins to realise stop moving action
    //!
    void stop();
    //! \brief  sets the pwm value to the pin that connected with enable pin in the H-Bridge
    //!
    //! \param [in] PWM  expected speed
    void setPWM(double PWM);
    double getPWM();

private:
    int m_nPinMotorA;
    int m_nPinMotorB;
    int m_nPinMotorE;
    double m_nPWM;
    bool initPins();
};

#endif // DCMOTOR_H
