
#define MaxPwm 40.0   //maximum PWM value
//#define nLinelength1 300//distance of straight forward moving
//#define nLinelength2 60//distance of straight forwardward moving back
//#define nvelocity1 100//forward velocity
//#define nvelocity2 130//rotation velocity
//#define nAngel       360
//#define nvelocity3 200//backward velocity
#define m_Linecompensation 0.0
#define m_Rotationcompensatiion 2.0

#define VelocitySamplingRate 20
#define diameter 11.9  //distance betweeen 2 wheels
#define WheelDiameter 6.3   //diameter of the wheels
#define ValuePi 3.1415926
#define TlaTimerstoTa 10
//#define KplA 0.5
#define KilA 0.0
#define KdlA 0.0
//#define KplB 0.5
#define KilB 0.0
#define KdlB 0.0

#include "odometrie.h"
double velocityA;
double velocityB;
//double valueA;
//double valueB;
//bool m_bOdoRunning;
//double lastvalueA;
//double lastvalueB;
Odometrie::Odometrie(AbstractSensor *&EncoderLeft, AbstractSensor *&EncoderRight,AbstractSensor\
                     *&ColorSensor, AbstractSensor *&UltrasonicSensor,AbstractSensor*&\
                     LineSensorA, AbstractSensor*& LineSensorB, DcMotor &MotorLeft,\
                     DcMotor &MotorRight):
    m_pVelocitySamplingRate(this),
    RefEncoderLeft(EncoderLeft), RefEncoderRight(EncoderRight),\
    RefColorSensor(ColorSensor),RefUltrasonicSensor(UltrasonicSensor),\
    RefLineSensorA(LineSensorA),RefLineSensorB(LineSensorB),\
    RefMotorLeft(MotorLeft), RefMotorRight(MotorRight)

{
    //m_bOdoRunning= false;
   /* m_pTimerOdometrie = new QTimer();
    m_pTimerOdometrie->setSingleShot(false);
    m_pTimerOdometrie->setInterval(10);
    connect(m_pTimerOdometrie, SIGNAL(timeout()), this, SLOT(slot_Odometrie_Line()));
*/
        //lastvalueA=0.0;
        //lastvalueB=0.0;
        //double Ta = (double)VelocitySamplingRate/(double)1000;
    m_bControlisruning=false;
    m_nLeftVelocity=0.0;
    m_nRightVelocity=0.0;
    m_nLeftDestination=100.0;
    m_nRightDestination=100.0;
    P_DATAA=0.115;
    I_DATAA=2.6;
    D_DATAA=0.000552;
    m_nColor=0.0;    //use colorsensor to control the movment of the robot
    P_DATAB=0.115;
    I_DATAB=2.5;
    D_DATAB=0.0005313;
    //m_pVelocitySamplingRate = new QTimer();
    m_pVelocitySamplingRate.setSingleShot(false);
    m_pVelocitySamplingRate.setInterval(VelocitySamplingRate);
    //m_pVelocitySamplingRate->start();
    connect(&m_pVelocitySamplingRate, SIGNAL(timeout()), this, SLOT(slot_velocitycontrol()));
}
Odometrie::~Odometrie()
{
}

void Odometrie::Controlmove( double targetLeftV,  double targetRightV,  double targetLeftD,\
                             double targetRightD, double targetColor, double verticaldistance,\
                             bool line)
{
    std::cout<<"controlmove start"<<endl;
    m_mutex_Controlisruning.lock();
    m_bControlisruning = true;
    m_mutex_Controlisruning.unlock();
    m_nHight=verticaldistance;
    //if(targetLeftV==targetRightV)
    //{lineControl=false;}
   // else
   // {lineControl=true;}
    m_nColor=targetColor;
    m_nLeftVelocity= targetLeftV;
    m_nRightVelocity= targetRightV;
    m_nLeftDestination= targetLeftD;
    m_nRightDestination= targetRightD;
    m_nLastEncoderValueLeft=0.0;
    m_nLastEncoderValueRight=0.0;
    m_nEncoderValueLeft=0;
    m_nEncoderValueRight=0;
    m_nYkLeft=0.0;
    m_nYkRight=0.0;
    m_nLocationYkLeft=0.0;
    m_nLocationYkRight=0.0;
    m_nAbsWaysumLeft=0.0;
    m_nAbsWaysumLeft=0.0;
    m_nWaysumLeft=0.0;
    m_nWaysumLeft=0.0;
    m_nPwmLeft=0.0;
    m_nPwmRight=0.0;
    m_nNewPwmLeft=0.0;
    m_nNewPwmRight=0.0;
    m_nErrorsumLeft=0.0;
    m_nErrorsumRight=0.0;
    m_nLocationErrorSumLeft=0.0;
    m_nLocationErrorSumRight=0.0;
    m_bline=line;
     m_nOrderCounter=0;

    m_nEkLeft = 0.0; //error for left motor
    m_nLastEkLeft=0.0;
    m_nEkRight=0.0;   //error for right motor
    m_nLastEkRight=0.0;
    m_nLocationEkLeft=0.0;
    m_nLocationEkRight=0.0;
    m_nLastLocationEkLeft=0.0;
    m_nLastLocationEkRight=0.0;

    //counter_line1=0;
    //counter_line2=0;
    RefMotorLeft.stop();
    RefMotorRight.stop();
    RefEncoderLeft->resetSensor();
    RefEncoderRight->resetSensor();
    //std::cout<<"waysumA start:"<<waysumA<<"waysumB start"<<waysumB<<endl;
    m_pVelocitySamplingRate.start();
    std::cout<<"control move finish"<<endl;
}

void  Odometrie:: slot_velocitycontrol()
{  // RefEncoderLeft->resetSensor();
   // RefEncoderRight->resetSensor();
   // valueA=0;
    //valueB=0;
  // lastvalueA=0;
  // lastvalueB=0;
    //m_bOdoRunning= true;
    std::cout<<"slot velicity control starts"<<endl;

    m_nEncoderValueLeft=RefEncoderLeft->getValue();
    m_nEncoderValueRight=RefEncoderRight->getValue();
    m_nCurrentcolor=RefColorSensor->getValue();
    m_nCurrentHight=RefUltrasonicSensor->getValue();
    m_nAbsWaysumLeft=fabs(m_nEncoderValueLeft/360*ValuePi*WheelDiameter);
    m_nAbsWaysumLeft=fabs(m_nEncoderValueRight/360*ValuePi*WheelDiameter);
    m_nLineLeft=RefLineSensorA->getValue();
    m_nLineRight=RefLineSensorB->getValue();
    if(m_bline)
  {  // QCoreApplication::processEvents();
          if(m_nLineLeft==1&&m_nLineRight==1)

       {
              cout<<" 1  1"<<endl;
         m_nPwmLeft=m_nLeftVelocity;
         m_nPwmRight=m_nRightVelocity;
         RefMotorLeft.setPWM(1.2*m_nPwmLeft);
         RefMotorLeft.forward();
         RefMotorRight.setPWM(m_nPwmRight);
         RefMotorRight.forward();
         //counter_line1=0;
         //counter_line2=0;
          }

          else if(m_nLineLeft==1&&m_nLineRight==0)
        {
                cout<<" 1  0"<<endl;
          m_nPwmLeft=m_nLeftVelocity;//-1/qPow(1.02,counter_line2)
          m_nPwmRight=m_nRightVelocity;//qPow(1.01,counter_line2)
          //pwmB=K_LineB*counter_line2*ykB;

     //usleep(10000);
          RefMotorLeft.setPWM(1.2*m_nPwmLeft);
          RefMotorRight.setPWM(m_nPwmRight);
          RefMotorRight.forward();
          RefMotorLeft.backward();
         // RefMotorLeft.forward();
          //RefMotorRight.forward();
    //counter_line2++;
       }
       else if(m_nLineLeft==0&&m_nLineRight==1)
       {//cout<<" 0  1"<<endl;
     //pwmA=K_LineA*counter_line1*ykA;
       m_nPwmLeft=m_nLeftVelocity;//1/qPow(1.01,counter_line1)*
       m_nPwmRight=m_nRightVelocity;//-1/qPow(1.02,counter_line1)*

     //usleep(10000);
       RefMotorLeft.setPWM(1.2*m_nPwmLeft);
       RefMotorRight.setPWM(m_nPwmRight);
       RefMotorLeft.forward();
       RefMotorRight.backward();
       //RefMotorRight.forward();
     //  RefMotorLeft.forward();



       //counter_line1++;
       }
         /* else if(lineA==0&&lineB==0)
       {
          RefMotorLeft.setPWM(1.212*pwmA);
          RefMotorRight.setPWM(pwmB);
          RefMotorLeft.backward();
          RefMotorRight.backward();

        }*/
 /*   else if(lineA==0&&lineB==0)
   {
  // cout<<" 0  0"<<endl;
    /*  if(fabs(counter_line1-counter_line2)>250)
    {
              RefMotorLeft.stop();
              RefMotorRight.stop();
              cout<<"counter_line1-counter_line2"<<counter_line1-counter_line2<<endl;
             if(counter_line1>counter_line2)
             { pwmA=0.5*LeftV;
              pwmB=RightV;}
             else
              {pwmA=0.5*LeftV;
              pwmB=RightV;}

       RefMotorLeft.setPWM(1.212*pwmA);
       RefMotorRight.setPWM(pwmB);
       RefMotorLeft.backward();
       RefMotorRight.backward();
       counter_line1=0;
       counter_line2=0;
    }
   */
      //else
      //{
         // RefMotorLeft.setPWM(1.212*pwmA);
          //RefMotorLeft.forward();
          //RefMotorRight.setPWM(pwmB);
          //RefMotorRight.forward();
        //  stopControl();
      //     RefMotorLeft.stop();
 //     RefMotorRight.stop();
      //}



     if(m_nAbsWaysumLeft+m_nAbsWaysumLeft-m_nLeftDestination-m_nRightDestination>=0||(m_nCurrentcolor==m_nColor)||(m_nHight>=m_nCurrentHight))
     { stopControl();
      }
 }
else
    {
        double tDistance=m_nHight;
        double tColor=m_nColor;
        double S_leftvelocity= m_nLeftVelocity;
        double S_rightvelocity= m_nRightVelocity;
        double S_leftdestination=m_nLeftDestination;
        double S_rightdestination=m_nRightDestination;
        m_nLocationKpLeft=fabs(S_leftvelocity/S_leftdestination);
        //cout<<"S_leftvelocity"<<S_leftvelocity<<"S_leftdestination"<<S_leftdestination<<endl;
        m_nLocationKpRight=fabs((double)S_rightvelocity/(double)S_rightdestination);
        double KpA= P_DATAA;
        double KiA= I_DATAA;
        double KdA= D_DATAA;

        double KpB= P_DATAB;
        double KiB= I_DATAB;
        double KdB= D_DATAB;
        double Ta=(double)VelocitySamplingRate/1000;
        //velocityA=(valueA-lastvalueA)*1000/Ta;
        //velocityB=(valueB-lastvalueB)*1000/Ta;
        velocityA=(m_nEncoderValueLeft-m_nLastEncoderValueLeft)/360*ValuePi*WheelDiameter/Ta;  //unit: cm/s
        velocityB=(m_nEncoderValueRight-m_nLastEncoderValueRight)/360*ValuePi*WheelDiameter/Ta;
       // cout<<"velocity calculate"<<endl;
        m_nLastEncoderValueLeft=m_nEncoderValueLeft;
        m_nLastEncoderValueRight=m_nEncoderValueRight;
      m_nOrderCounter++;
    if(m_nOrderCounter==TlaTimerstoTa)
    {m_nWaysumLeft=m_nEncoderValueLeft/360*ValuePi*WheelDiameter;
    m_nWaysumLeft=m_nEncoderValueRight/360*ValuePi*WheelDiameter;
    m_nLocationErrorSumLeft=m_nLocationErrorSumLeft+m_nLocationEkLeft;
    m_nLocationErrorSumRight=m_nLocationErrorSumRight+m_nLocationEkRight;
    m_nLocationEkLeft=S_leftdestination-m_nWaysumLeft;
    m_nLocationEkRight=S_rightdestination-m_nWaysumLeft;
    if((m_nAbsWaysumLeft+m_nAbsWaysumLeft)/(fabs(S_leftdestination)+fabs(S_rightdestination))<=0.7)
    {m_nLocationYkLeft=m_nLocationKpLeft*m_nLocationEkLeft; //+KilA*Ta*TlaTimerstoTa*esumlA+KdlA/Ta/TlaTimerstoTa*(eklA-ekl_1A);
     m_nLocationYkRight=m_nLocationKpRight*m_nLocationEkRight;  //+KilB*Ta*TlaTimerstoTa*esumlB+KdlB/Ta/TlaTimerstoTa*(eklB-ekl_1B);
    }
    else {m_nLocationYkLeft=0.3*S_leftdestination; m_nLocationYkRight=0.3*S_rightdestination;}
     /*ekl_1A=eklA;
     ekl_1B=eklB;*/
    m_nOrderCounter=0;
    }
    //std::cout<<"waysum_A:"<<waysum_A<<"waysum_B"<<waysum_B<<endl;
     //std::cout<<"eklA="<<eklA<<"eklB="<<eklB<<endl;
     //std::cout<<"yklA="<<yklA<<"yklB="<<yklB<<endl;
     m_nEkLeft=m_nLocationYkLeft-velocityA;
     m_nEkRight=m_nLocationYkRight-velocityB;
    //ekA=S_leftvelocity-velocityA;
    //ekB=S_rightvelocity-velocityB;
    m_nErrorsumLeft=m_nErrorsumLeft+m_nEkLeft;
    m_nErrorsumRight=m_nErrorsumRight+m_nEkRight;
    m_nYkLeft= KpA*m_nEkLeft+ KiA*Ta*m_nErrorsumLeft+ KdA/Ta*(m_nEkLeft-m_nLastEkLeft);
    m_nYkRight= KpB*m_nEkRight+ KiB*Ta*m_nErrorsumRight+ KdB/Ta*(m_nEkRight-m_nLastEkRight);

    m_nPwmLeft=m_nYkLeft;
    m_nPwmRight=m_nYkRight;

    //std::cout<<"ykA="<<ykA<<"ykB="<<ykB<<endl;
    if(((m_nAbsWaysumLeft+m_nAbsWaysumLeft-fabs(S_leftdestination)-fabs(S_rightdestination))>=0)\
        ||(tColor==m_nCurrentcolor)||(tDistance>=m_nCurrentHight)||(m_nLineLeft==0&&m_nLineRight==0))
    {
       // cout<<"waysumA:"<<waysumA<<"waysumB"<<waysumB<<endl;
        //std::cout<<"slot velicity control finished"<<endl;
        velocityA=0;
        velocityB=0;
        m_nEncoderValueLeft=0;
        m_nEncoderValueRight=0;
        m_nLastEncoderValueLeft=0.0;
        m_nLastEncoderValueRight=0.0;
        m_nLocationYkLeft=0.0;
        m_nLocationYkRight=0.0;
        m_nYkLeft=0.0;
        m_nYkRight=0.0;
        m_nAbsWaysumLeft=0.0;
        m_nWaysumLeft=0.0;
        m_nAbsWaysumLeft=0.0;
        m_nWaysumLeft=0.0;
        m_nPwmLeft=0.0;
        m_nPwmRight=0.0;
        m_nNewPwmLeft=0.0;
        m_nNewPwmRight=0.0;
        m_nErrorsumLeft=0.0;
        m_nErrorsumRight=0.0;
        m_nLocationErrorSumLeft=0.0;
        m_nLocationErrorSumRight=0.0;
        m_nOrderCounter=0;
        stopControl();
    }
    if (m_nPwmLeft > MaxPwm)
       {
           m_nNewPwmLeft = MaxPwm;
           RefMotorLeft.setPWM(m_nNewPwmLeft);
           RefMotorLeft.forward();
       }
    else if(m_nPwmLeft<-MaxPwm)
    {
        m_nNewPwmLeft = MaxPwm;
        RefMotorLeft.setPWM(m_nNewPwmLeft);
        RefMotorLeft.backward();
    }
    else if(m_nPwmLeft>=-MaxPwm&&m_nPwmLeft<0)
    {
        m_nNewPwmLeft = fabs(m_nPwmLeft);
        RefMotorLeft.setPWM(m_nNewPwmLeft);
        RefMotorLeft.backward();
    }
    else
    {
        m_nNewPwmLeft=m_nPwmLeft;
        RefMotorLeft.setPWM(m_nNewPwmLeft);
        RefMotorLeft.forward();
    }
    if (m_nPwmRight > MaxPwm)
       {
           m_nNewPwmRight = MaxPwm;
           RefMotorRight.setPWM(m_nNewPwmRight);
           RefMotorRight.forward();
       }
    else if(m_nPwmRight<-MaxPwm)
    {
        m_nNewPwmRight = MaxPwm;
        RefMotorRight.setPWM(m_nNewPwmRight);
        RefMotorRight.backward();
    }
    else if(m_nPwmRight>=-MaxPwm&&m_nPwmRight<0)
    {
        m_nNewPwmRight = fabs(m_nPwmRight);
        RefMotorRight.setPWM(m_nNewPwmRight);
        RefMotorRight.backward();
    }
    else
    {
        m_nNewPwmRight=m_nPwmRight;
        RefMotorRight.setPWM(m_nNewPwmRight);
        RefMotorRight.forward();
    }
    m_nLastEkLeft=m_nEkLeft;
    m_nLastEkRight=m_nEkRight;
    }
   // std::cout<<"slot velicity control one time"<<endl;
    //requirement=(waysumA+waysumB-2*S_destination+m_Linecompensation)<0?true:false;
    //}
    //RefMotorLeft.stop();
   // RefMotorRight.stop();
    //sleep(3);
    //waysumA=0;
   // waysumB=0;
    //cout<<"waysumA:"<<waysumA<<"waysumB"<<waysumB<<endl;
}

void Odometrie::stopControl()
{
    //QMetaObject::invokeMethod(&m_pVelocitySamplingRate, "stop", Qt::BlockingQueuedConnection);
    //cout<<"stopcontrol!!!!!"<<endl;
    m_bline=false;
    RefMotorLeft.stop();
    RefMotorRight.stop();
    m_pVelocitySamplingRate.stop();
    m_mutex_Controlisruning.lock();
    m_bControlisruning = false;
    m_mutex_Controlisruning.unlock();
    RefEncoderLeft->resetSensor();
    RefEncoderRight->resetSensor();
    usleep(1000000);


}
void Odometrie::finished()
{
    emit sgn_finished();
}

void Odometrie::slot_updateVelocityControlParamters(double value, int parameterID)
{
    // brauche ich hier die mutexe?
    // ich glaube nicht, da die variable private ist und von außen nur üver temporäre variable "value" zugegriffen wird
    switch (parameterID)
    {
    case nMapLeftWheelVelocityKp:
        m_mutex_updateControlParameter.lock();
        P_DATAA = value;
//        qDebug() << "m_dKpLeftVelocityControl: " << m_dKpLeftVelocityControl;
        m_mutex_updateControlParameter.unlock();
        break;
    case nMapLeftWheelVelocityKi:
        m_mutex_updateControlParameter.lock();
        I_DATAA = value;
//        qDebug() << "m_dKiLeftVelocityControl: " << value;
        m_mutex_updateControlParameter.unlock();
        break;
    case nMapLeftWheelVelocityKd:
        m_mutex_updateControlParameter.lock();
        D_DATAA = value;
//        qDebug() << "m_dKdLeftVelocityControl: " << value;
        m_mutex_updateControlParameter.unlock();
        break;
    case nMapRightWheelVelocityKp:
        m_mutex_updateControlParameter.lock();
        P_DATAB = value;
//        qDebug() << "m_dKpRightVelocityControl: " << m_dKpRightVelocityControl;
        m_mutex_updateControlParameter.unlock();
        break;
    case nMapRightWheelVelocityKi:
        m_mutex_updateControlParameter.lock();
        I_DATAB = value;
//        qDebug() << "m_dKiRightVelocityControl: " << value;
        m_mutex_updateControlParameter.unlock();
        break;
    case nMapRightWheelVelocityKd:
        m_mutex_updateControlParameter.lock();
        D_DATAB = value;
//        qDebug() << "m_dKdRightVelocityControl: " << value;
        m_mutex_updateControlParameter.unlock();
        break;
    default:
        qDebug() << "error in Control::slot_updatVelocityControlParamters().";
    }
}


