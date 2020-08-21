#define nRightMotorPinA 3
#define nRightMotorPinB 2
#define nRightMotorPinE 0

#define nRightEncoderPinA 28
#define nRightEncoderPinB 29


#define nLeftMotorPinA 24
#define nLeftMotorPinB 25
#define nLeftMotorPinE 23

#define nLeftEncoderPinA 21
#define nLeftEncoderPinB 22

#define nPinLineSensorA 15
#define nPinLineSensorB 12

#define nUltrasonicSensorPin 27


//#define VelocitySamplingRate 10.0
#define WheelDiameter 6.3
#define ValuePi 3.1415926
#define diameter 11.9

#define LEDPIN 26
#define L01 31.3
#define L02 86
#define L03 54.5
#define L12 54.5
#define L13 86
#define L05 50.5//45.0
#define L10 50.5
#define L06 37.0
//#define L070809 104.7
//#define L0607 69.6
//#define L0809 72.1
//#define L060708 105.9
#define L09 35.8
#define L9999 200
#define D_Compensition 6.8
#define D_CompensitionY 10

/*#define Yellow 0
#define Red 1
#define Green 2
#define ClerkA 0
#define ClerkB 1
*/
#include "mobileplatform.h"


MobilePlatform::MobilePlatform()
    : m_motorLeft(nLeftMotorPinA, nLeftMotorPinB, nLeftMotorPinE)
    , m_motorRight(nRightMotorPinA, nRightMotorPinB, nRightMotorPinE),m_bFollowLine(false)
    , m_bChallengeIsRunning(false),m_timesfromstart(0),m_controlledSvelocity(10),m_controlledCvelocity(33)
    , m_compensationvelocity(22)
{

    /*m_bFollowLine = false;
    m_bChallengeIsRunning=false;
    m_timesfromstart=0;
    m_controlledSvelocity=10;
    m_controlledCvelocity=33;//muss in GUI einstellt
    m_compensationvelocity=22;
    */
    wiringPiSetup();
    pinMode(LEDPIN,OUTPUT);
    //StorageRackstates = Red;
    //m_bOdoRunning = false;
    /*lastvalueA=0.0;
    lastvalueB=0.0;
    Ta = VelocitySamplingRate;
    m_pVelocitySamplingRate = new QTimer();
    m_pVelocitySamplingRate->setSingleShot(false);
    m_pVelocitySamplingRate->setInterval(Ta);
    m_pVelocitySamplingRate->start();
    connect(m_pVelocitySamplingRate, SIGNAL(timeout()), this, SLOT(slot_velocitycal()));//
*/

    m_pLineSensorA  = new LineSensor(nPinLineSensorA, nMapLineSensorA);
    m_pLineSensorB  = new LineSensor(nPinLineSensorB, nMapLineSensorB);
    m_pColorSensor  = new ColorSensor(nMapColorSensor);
    m_pEncoderLeft  = new Encoder(nLeftEncoderPinA, nLeftEncoderPinB, nMapLeftEncoderMotorA);
    m_pEncoderRight = new Encoder(nRightEncoderPinA, nRightEncoderPinB, nMapRightEncoderMotorB);
    m_pSonicSensor  = new UltrasonicSensor(nUltrasonicSensorPin, nMapSonicSensor);

    connectSensorThread(m_pLineSensorA, m_pThreadLineA);
    connectSensorThread(m_pLineSensorB, m_pThreadLineB);
    connectSensorThread(m_pColorSensor, m_pThreadColor);
    connectSensorThread(m_pEncoderLeft, m_pThreadEncoderLeft);
    connectSensorThread(m_pEncoderRight, m_pThreadEncoderRight);
    connectSensorThread(m_pSonicSensor, m_PThreadSonic);

    m_pOdometrie = new Odometrie(m_pEncoderLeft,m_pEncoderRight,m_pColorSensor,m_pSonicSensor,\
                                 m_pLineSensorA,m_pLineSensorB,m_motorLeft,m_motorRight);
    /*m_pThreadOdometrie = new QThread();
    connect(m_pThreadOdometrie,SIGNAL(started()), m_pOdometrie, SLOT(slot_OdometrieStart()));
    connect(m_pOdometrie,SIGNAL(sgn_finished()),m_pThreadOdometrie,SLOT(quit()));
    connect(m_pOdometrie,SIGNAL(sgn_finished()),m_pOdometrie,SLOT(deleteLater()));
    connect(m_pThreadOdometrie,SIGNAL(finished()),m_pThreadOdometrie,SLOT(deleteLater()));
    m_pOdometrie->moveToThread(m_pThreadOdometrie);
*/
    connectControlThread(m_pOdometrie, m_pThreadOdometrie);
    connect(this, SIGNAL(sgn_forwardControlParameters(double,int)), m_pOdometrie,\
            SLOT(slot_updateVelocityControlParamters(double,int)));

    QStateMachine *machine = new QStateMachine();//++++
    QState *stategroup1_doChallenge = new QState();
    QState *state0_Startpoint =new QState(stategroup1_doChallenge);
    QState *state1_GotoLager =new QState(stategroup1_doChallenge);
    QState *state2_IntheLager =new QState(stategroup1_doChallenge);
    QState *state3_GotoClerk = new QState(stategroup1_doChallenge);//direction of turning depends on LED and do Odometrie
    QState *state4_IntheClerkroom = new QState(stategroup1_doChallenge);
    QState *state5_GobackStartpoint = new QState(stategroup1_doChallenge);
    state0_Startpoint->addTransition(this,SIGNAL(sgn_getChallengeStart()),state1_GotoLager);
    state1_GotoLager->addTransition(this,SIGNAL(sgn_atLagerturnpointIn()),state2_IntheLager);
    state2_IntheLager->addTransition(this,SIGNAL(sgn_atLagerturnpointOut()),state3_GotoClerk);
    state3_GotoClerk->addTransition(this,SIGNAL(sgn_atClerkpointIn()),state4_IntheClerkroom);
    state4_IntheClerkroom->addTransition(this,SIGNAL(sgn_atClerkpointOut()),state1_GotoLager);
    state4_IntheClerkroom->addTransition(this,SIGNAL(sgn_allordersweredone()),state5_GobackStartpoint);
    stategroup1_doChallenge->setInitialState(state0_Startpoint);
    machine->addState(stategroup1_doChallenge);

    QFinalState *state_btnQuit= new QFinalState;
    stategroup1_doChallenge->addTransition(this,SIGNAL(sgn_quit()),state_btnQuit);
    machine->addState(state_btnQuit);
    machine->setInitialState(stategroup1_doChallenge);
    machine->start();

    //connect(state0_Startpoint,SIGNAL(entered()),this,SLOT(slot_Gotolager()));
    connect(state1_GotoLager,SIGNAL(entered()),this,SLOT(slot_GotoLager()));
    connect(state2_IntheLager,SIGNAL(entered()),this,SLOT(slot_getWare()));
    connect(state3_GotoClerk,SIGNAL(entered()),this,SLOT(slot_GotoClerk()));
    connect(state4_IntheClerkroom,SIGNAL(entered()),this,SLOT(slot_putdownWare()));
    connect(state5_GobackStartpoint,SIGNAL(entered()),this,SLOT(slot_GobackStartpoint()));

}

void MobilePlatform::StopSensorforDelete()
{
    m_pLineSensorA->SensorStop();
    m_pLineSensorB->SensorStop();
    m_pColorSensor->SensorStop();
    m_pOdometrie->stopControl();
    m_pEncoderLeft->SensorStop();
    m_pEncoderRight->SensorStop();
    m_pSonicSensor->SensorStop();
}

MobilePlatform::~MobilePlatform()
{
    m_pLineSensorA->DeleteSensorundThread();
    m_pLineSensorB->DeleteSensorundThread();
    m_pColorSensor->DeleteSensorundThread();
    m_pOdometrie->finished();
    m_pEncoderLeft->DeleteSensorundThread();
    m_pEncoderRight->DeleteSensorundThread();
    m_pSonicSensor->DeleteSensorundThread();
}

void MobilePlatform::connectSensorThread(AbstractSensor *sensor, QThread *thread)
{
    thread = new QThread();
    connect(thread,SIGNAL(started()), sensor, SLOT(slot_readSensor()));
    connect(sensor,SIGNAL(sgn_finished()),thread,SLOT(quit()));
    connect(sensor,SIGNAL(sgn_finished()),sensor,SLOT(deleteLater()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    sensor->moveToThread(thread);
    thread->start();
}

void MobilePlatform::connectControlThread(Odometrie *contr, QThread *thread)
{
    thread = new QThread();

    //connect(thread,SIGNAL(started()),contr, SLOT(slot_velocitycontrol()));
    connect(contr,SIGNAL(sgn_finished()),thread,SLOT(quit()));
    connect(contr,SIGNAL(sgn_finished()),contr,SLOT(deleteLater()));
    connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
    contr->moveToThread(thread);
    thread->start();
}
AbstractSensor* MobilePlatform::mapSensorIDObject(int nSensorID)
{
    switch (nSensorID)
    {
    case nMapLineSensorA:
        return m_pLineSensorA;
        break;
    case nMapLineSensorB:
        return m_pLineSensorB;
        break;
    case nMapColorSensor:
        return m_pColorSensor;
        break;
    case nMapLeftEncoderMotorA:
        return m_pEncoderLeft;
        break;
    case nMapRightEncoderMotorB:
        return m_pEncoderRight;
        break;
    case nMapSonicSensor:
        return m_pSonicSensor;
        break;
    default:
        return 0;
    }
}

void MobilePlatform::ControlledStraight(double expectedvelocity, double destination, double targetColor,\
                                        double verticaldistance,bool openline)
{  if( m_bChallengeIsRunning)
    {
    std::cout<<"Straight Start"<<endl;
    double nA,nB,nC,nD, color, distance;
    bool m_bline;
    nA=expectedvelocity;
    nB=expectedvelocity;
    nC=destination;
    nD=destination;
    color=targetColor;
    distance=verticaldistance;
    //m_pOdometrie->Controlmove( A,  B,  C, D, color, distance);
    m_bline=openline;
    QMetaObject::invokeMethod(m_pOdometrie,"Controlmove",Qt::BlockingQueuedConnection,
                              Q_ARG(double, nA),
                              Q_ARG(double, nB),
                              Q_ARG(double, nC),
                              Q_ARG(double, nD),
                              Q_ARG(double, color),
                              Q_ARG(double, distance),
                              Q_ARG(bool, m_bline));

    while (m_pOdometrie->m_bControlisruning)
    {
           usleep(10000);
           QCoreApplication::processEvents();
    }
    std::cout<<"Straight finish"<<endl;
    }
}

void MobilePlatform::ControlledTurn(double rotationvelocity, double angle)
{   if( m_bChallengeIsRunning)
    {
    std::cout<<"Turn Start"<<endl;
    double nE,nF,nG,nH;
    //if(angle<180&&angle>0)
  //  {
       // E=0;
        nE=-rotationvelocity;
        nF=rotationvelocity;
   // }
  //  else
    //{
       // E=rvalue;
      //  F=0;
        //F=-rvalue;
   // }
    nG=-(angle/360*diameter*ValuePi);
    nH=(angle/360*diameter*ValuePi);




    QMetaObject::invokeMethod(m_pOdometrie,"Controlmove",Qt::BlockingQueuedConnection,
                              Q_ARG(double, nE),
                              Q_ARG(double, nF),
                              Q_ARG(double, nG),
                              Q_ARG(double, nH),
                              Q_ARG(double, 6.0),
                              Q_ARG(double, -1.0),
                              Q_ARG(bool, false));


    //m_pOdometrie->Controlmove( E,  F, G, H, 6.0, -1);
    //std::cout<<"E ="<<E<<"F ="<<F<<endl;
    while (m_pOdometrie->m_bControlisruning)
    {
           usleep(10000);
           QCoreApplication::processEvents();
    }
    std::cout<<"Turn finish"<<endl;
    }
}

void MobilePlatform::slot_OdometrieStart()
{   m_bOdoRunning=true;
    ControlledStraight(m_controlledSvelocity,L9999,6.0,50,true); //when come across above glass plate
    ControlledTurn(m_controlledCvelocity,-120.0);
    ControlledStraight(m_controlledSvelocity,L9999,3.0,-1,true);
    emit sgn_finished();
}



void MobilePlatform::slot_getNewSensorValues(int SensorID)
{   double value;
    value=mapSensorIDObject(SensorID)->getValue();
    emit sgn_newSensorValues(SensorID,value);
}

void MobilePlatform::slot_resetSensor(int SensorID)
{
    mapSensorIDObject(SensorID)->resetSensor();
}

void MobilePlatform::slot_moveForward()
{
    m_motorLeft.forward();
    m_motorRight.forward();
}

void MobilePlatform::slot_moveBackward()
{
    m_motorLeft.backward();
    m_motorRight.backward();
}

void MobilePlatform::slot_moveLeft()
{
    m_motorLeft.stop();
    m_motorRight.forward();
}

void MobilePlatform::slot_moveSharpLeft()
{
    m_motorLeft.backward();
    m_motorRight.forward();
}

void MobilePlatform::slot_moveRight()
{
    m_motorRight.stop();
    m_motorLeft.forward();
}

void MobilePlatform::slot_moveSharpRight()
{
    m_motorRight.backward();
    m_motorLeft.forward();
}


void MobilePlatform::slot_stopMotion()
{
    m_bOdoRunning=false;
    m_bFollowLine = false;
    m_bChallengeIsRunning=false;
    QMetaObject::invokeMethod(m_pOdometrie, "stopControl", Qt::BlockingQueuedConnection);
    m_motorLeft.stop();
    m_motorRight.stop();

}

void MobilePlatform::slot_setPWM(double value)
{
    m_motorLeft.setPWM(value);
    m_motorRight.setPWM(value);
}

void MobilePlatform::slot_pathChanged(QString path)
{
 m_path= path;
}

void MobilePlatform::slot_followline()
{
    m_bFollowLine = true;

    while (m_bFollowLine)
    {
        QCoreApplication::processEvents();
        if (m_bFollowLine == false)
        {
            break;
        }
        int nStatusSensorA = digitalRead(nPinLineSensorA);
        //std::cout << "Sensor A: " << nStatusSensorA << endl;
        int nStatusSensorB = digitalRead(nPinLineSensorB);
        //std::cout << "Sensor B: " << nStatusSensorB << endl;


        if (nStatusSensorA == 0 && nStatusSensorB == 0) //  "backward"
        {
            slot_moveForward();
        }
        else if (nStatusSensorA == 0 && nStatusSensorB == 1)  //  "left-turn"
        {
            slot_moveForward();
            slot_moveSharpRight();
        }
        else if (nStatusSensorA == 1 && nStatusSensorB == 0) // "right-turn"
        {
            slot_moveForward();
            slot_moveSharpLeft();
        }
        else if (nStatusSensorA == 1 && nStatusSensorB == 1) //  "forward"
        {
            slot_moveForward();
        }
    }

}


void MobilePlatform::slot_XMLparsen()
{
    //getorderarray= Xmlprasen::getXmlFilePath(m_path);
    //orderarray= m_Sorter.firstoptimalOrder(getorderarray);
    orderarray= Xmlprasen::getXmlFilePath(m_path);
    m_bChallengeIsRunning=true;
    cout<<"storage1:"<<orderarray.order.at(0).storageRack<<endl;
    cout<<"warehouseClerk1:"<<orderarray.order.at(0).warehouseClerk<<endl;
    cout<<"storage2:"<<orderarray.order.at(1).storageRack<<endl;
    cout<<"warehouseClerk2:"<<orderarray.order.at(1).warehouseClerk<<endl;
    cout<<"storage3:"<<orderarray.order.at(2).storageRack<<endl;
    cout<<"warehouseClerk3:"<<orderarray.order.at(2).warehouseClerk<<endl;
}

void MobilePlatform::slot_GotoLager()
{
   std::cout<<"gotolager is running"<<endl;
    emit sgn_GotoLager();
       switch(orderarray.order.at(m_timesfromstart).storageRack)
      { case 0:
        {state_Storagerack= RACK_RED;
           break;}
       case 1:
        {state_Storagerack= RACK_GREEN;
        break;}
       case 2:
         {state_Storagerack= RACK_YELLOW;
           break;}
       }

       switch(orderarray.order.at(m_timesfromstart).warehouseClerk)
      { case 0:
        {state_WarehouseClerk=CLERK_A;
           break;}
        case 1:
        {state_WarehouseClerk=CLERK_B;
        break;}
       }

       if(m_timesfromstart!=0)
     {
       switch(orderarray.order.at(m_timesfromstart-1).warehouseClerk)
      { case 0:
        {state_WarehouseClerklast=CLERK_A;
           break;}
        case 1:
        {state_WarehouseClerklast=CLERK_B;
        break;}
       }
      }
       while(m_bChallengeIsRunning)
     { if(m_timesfromstart==0)
       {
      switch(state_Storagerack)
       {
       case RACK_YELLOW:
            {ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true);
            ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
            ControlledTurn(m_controlledCvelocity,67.0);
            ControlledStraight(m_controlledSvelocity,L02,6.0,-1.0,true);
            ControlledTurn(m_controlledCvelocity,-70.0);
            ControlledStraight(m_controlledSvelocity,L03,6.0,-1.0,true);
            ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
            ControlledStraight(m_controlledSvelocity,L05,6.0,-1.0,true);
            ControlledTurn(m_controlledCvelocity,-67.0);
            ControlledStraight(m_controlledSvelocity,L9999,5.0,-1.0,true);// when come across yellow
            ControlledStraight(m_compensationvelocity,D_CompensitionY,6.0,-1.0,false);
            ControlledTurn(m_controlledCvelocity,-67.0);
            break;}
       case RACK_RED:
           { ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true);
            ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
            ControlledTurn(m_controlledCvelocity,67.0);
            ControlledStraight(m_controlledSvelocity,L02,6.0,-1.0,true);
            ControlledTurn(m_controlledCvelocity,-70.0);
            ControlledStraight(m_controlledSvelocity,L03,6.0,-1.0,true);
            ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
            ControlledStraight(m_controlledSvelocity,L05,6.0,-1.0,true);
            ControlledTurn(m_controlledCvelocity,-70.0);
            ControlledStraight(m_controlledSvelocity,L9999,5.0,-1.0,true);// when come across yellow
            ControlledStraight(m_compensationvelocity,D_CompensitionY,6.0,-1.0,false);
            ControlledStraight(m_controlledSvelocity,L9999,2.0,-1.0,true);
            ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
            ControlledTurn(m_controlledCvelocity,-67.0);
            break;}
       case RACK_GREEN:
            {ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true);
            ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
            ControlledTurn(m_controlledCvelocity,-67.0);
            ControlledStraight(m_controlledSvelocity,L13,6.0,-1.0,true);
            ControlledTurn(m_controlledCvelocity,70.0);
            ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true);
            ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
             ControlledStraight(m_controlledSvelocity,L10,6.0,-1.0,true);
            ControlledTurn(m_controlledCvelocity,67.0);
            ControlledStraight(m_controlledSvelocity,L9999,3.0,-1.0,true);// when come across green
            ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
            ControlledTurn(m_controlledCvelocity,-67.0);
            break;}
       default:
            break;
      }
      }

       else

           { switch(state_WarehouseClerklast)
             {
              case CLERK_A:
                   { switch(state_Storagerack)
                    {
                    case RACK_YELLOW:
                      { if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L05,6.0,-1.0,true);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,-67.0);
                       if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L9999,5.0,-1.0,true);// when come across yellow
                       ControlledStraight(m_compensationvelocity,D_CompensitionY,6.0,-1.0,false);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,67.0);
                       break;}
                    case RACK_RED:
                      { if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L05,6.0,-1.0,true);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,-67.0);
                       if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L9999,2.0,-1.0,true);// when come across red
                       ControlledStraight(m_compensationvelocity,D_CompensitionY,6.0,-1.0,false);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,-67.0);
                       break;}
                    case RACK_GREEN:
                      { if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L05,6.0,-1.0,true);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,-67.0);
                       if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L9999,3.0,-1.0,true);// when come across green
                       ControlledStraight(m_compensationvelocity,D_CompensitionY,6.0,-1.0,false);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,67.0);
                       break;}
                   default:
                   {break;}
                    }
                   }

              case CLERK_B:
                  { switch(state_Storagerack)
                   {case RACK_YELLOW:
                      {if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L10,6.0,-1.0,true);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,67.0);
                       if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L9999,5.0,-1.0,true);// when come across yellow
                       ControlledStraight(m_compensationvelocity,D_CompensitionY,6.0,-1.0,false);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,-67.0);
                      break;}
                   case RACK_RED:
                      {if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L10,6.0,-1.0,true);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,67.0);
                       if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L9999,2.0,-1.0,true);// when come across red
                       ControlledStraight(m_compensationvelocity,D_CompensitionY,6.0,-1.0,false);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,67.0);
                      break;}
                   case RACK_GREEN:
                      {if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L10,6.0,-1.0,true);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,67.0);
                       if( m_bChallengeIsRunning)
                       ControlledStraight(m_controlledSvelocity,L9999,3.0,-1.0,true);// when come across green
                       ControlledStraight(m_compensationvelocity,D_CompensitionY,6.0,-1.0,false);
                       if( m_bChallengeIsRunning)
                       ControlledTurn(m_controlledCvelocity,-67.0);

                      break;}
                   default:
                        {break;}
                    }
                   }

               default:
               { break;}
               }
              }
         }
           emit sgn_atLagerturnpointIn();
}


void MobilePlatform::slot_getWare()
{
    std::cout<<"getWare start"<<endl;
    emit sgn_getWare();
   ControlledStraight(m_controlledSvelocity,L9999,6.0,50,true); //when come across above glass plate
   ControlledTurn(m_controlledCvelocity,-120.0);
   digitalWrite(LEDPIN,1);
   switch(state_Storagerack)
         { case RACK_YELLOW:
              if( m_bChallengeIsRunning)
              ControlledStraight(m_controlledSvelocity,L9999,5.0,-1.0,true);
              ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
              break;
          case RACK_RED:
              if( m_bChallengeIsRunning)
              ControlledStraight(m_controlledSvelocity,L9999,2.0,-1.0,true);
              ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
              break;
          case RACK_GREEN:
              if( m_bChallengeIsRunning)
              ControlledStraight(m_controlledSvelocity,L9999,3.0,-1.0,true);
              ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
              break;
         default:
             break;
          }
       emit sgn_atLagerturnpointOut();

}


void MobilePlatform::slot_GotoClerk()
{
    std::cout<<"gotoclerk"<<endl;
    emit sgn_GotoClerk();
   switch(state_Storagerack)
     {case RACK_YELLOW:
         { switch(state_WarehouseClerk)
            {  case CLERK_A:
               {if( m_bChallengeIsRunning)
               ControlledTurn(m_controlledCvelocity,-67.0);
                if( m_bChallengeIsRunning)
               ControlledStraight(m_controlledSvelocity,L06,6.0,-1.0,true);
               if( m_bChallengeIsRunning)
               ControlledTurn(m_controlledCvelocity,67.0);
               if( m_bChallengeIsRunning)
               ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true);
               ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
               if( m_bChallengeIsRunning)
               ControlledTurn(m_controlledCvelocity,67.0);
                break;}
              case CLERK_B:
               {if( m_bChallengeIsRunning)
               ControlledTurn(m_controlledCvelocity,67.0);
               if( m_bChallengeIsRunning)
               ControlledStraight(m_controlledSvelocity,L9999,2.0,-1.0,true);
               ControlledStraight(m_compensationvelocity,D_CompensitionY,6.0,-1.0,false);
               ControlledStraight(m_controlledSvelocity,L9999,3.0,-1.0,true);
               ControlledStraight(m_compensationvelocity,D_CompensitionY,6.0,-1.0,false);
               ControlledStraight(m_controlledSvelocity,L09,6.0,-1.0,true);
               if( m_bChallengeIsRunning)
               ControlledTurn(m_controlledCvelocity,-67.0);
               if( m_bChallengeIsRunning)
               ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true);
               ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
               if( m_bChallengeIsRunning)
               ControlledTurn(m_controlledCvelocity,-67.0);
                break;}
              default:
                 break;
             }

          break;
         }
     case RACK_RED:
         { switch(state_WarehouseClerk)
              {case CLERK_A:
               {if( m_bChallengeIsRunning)
               ControlledTurn(m_controlledCvelocity,-67);
               if( m_bChallengeIsRunning)
               ControlledStraight(m_controlledSvelocity,L9999,5.0,-1.0,true);
               ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
               ControlledStraight(m_controlledSvelocity,L06,6.0,-1.0,true);
               if( m_bChallengeIsRunning)
               ControlledTurn(m_controlledCvelocity,67);
               ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true);
               ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
               ControlledTurn(m_controlledCvelocity,67);
               break;}
              case CLERK_B:
               {ControlledTurn(m_controlledCvelocity,67.0);
                ControlledStraight(m_controlledSvelocity,L9999,3.0,-1.0,true);
                ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
               ControlledStraight(m_controlledSvelocity,L09,6.0,-1.0,true);
               ControlledTurn(m_controlledCvelocity,-67.0);
               ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true);
               ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
               ControlledTurn(m_controlledCvelocity,-67.0);
               break;}
             default:
               break;
              }

      break;
        }
    case RACK_GREEN:
         { switch(state_WarehouseClerk)
              {case CLERK_A:
               {ControlledTurn(m_controlledCvelocity,-67.0);
               ControlledStraight(m_controlledSvelocity,L9999,2.0,-1.0,true);
               ControlledStraight(m_compensationvelocity,D_CompensitionY,6.0,-1.0,false);
               ControlledStraight(m_controlledSvelocity,L9999,5.0,-1.0,true);
               ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
               ControlledStraight(m_controlledSvelocity,L06,6.0,-1.0,true);
               ControlledTurn(m_controlledCvelocity,67.0);
               ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true);
               ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
               ControlledTurn(m_controlledCvelocity,67.0);
           break;}
              case CLERK_B:
              { ControlledTurn(m_controlledCvelocity,67.0);
               ControlledStraight(m_controlledSvelocity,L09,6.0,-1.0,true);
               ControlledTurn(m_controlledCvelocity,-67.0);
               ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true);
               ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
               ControlledTurn(m_controlledCvelocity,-67.0);
               break;}
             default:
               break;
              }
           break;
       }

     default:
      break;
    }
     emit sgn_atClerkpointIn();

}

void MobilePlatform::slot_putdownWare()
{  std::cout<<"putdown ware"<<endl;
    emit sgn_putdownWare();
   ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true); //when come across end blue
   ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
   ControlledTurn(m_controlledCvelocity,-120.0);
   digitalWrite(LEDPIN,0);
   ControlledStraight(m_controlledSvelocity,L9999,4.0,-1.0,true);
   ControlledStraight(m_compensationvelocity,D_Compensition,6.0,-1.0,false);
   if(m_timesfromstart==(orderarray.order.size()-1))
      emit sgn_allordersweredone();
   else
      m_timesfromstart++;
      emit sgn_atClerkpointOut();
}

void MobilePlatform::slot_GobackStartpoint()
{  std::cout<<"goback start point"<<endl;
    emit sgn_GobackStartpoint();
  switch(state_WarehouseClerk)
    {
    case CLERK_A:
        {ControlledTurn(m_controlledCvelocity,67.0);
        ControlledStraight(m_controlledSvelocity,L03,6.0,-1.0,true);
        ControlledTurn(m_controlledCvelocity,67.0);
        ControlledStraight(m_controlledSvelocity,L02,6.0,-1.0,true);
        ControlledTurn(m_controlledCvelocity,-67.0);
        ControlledStraight(m_controlledSvelocity,L01,6.0,-1.0,true);
         break;}
    case CLERK_B:
        {ControlledTurn(m_controlledCvelocity,-67.0);
        ControlledStraight(m_controlledSvelocity,L12,6.0,-1.0,true);
        ControlledTurn(m_controlledCvelocity,-67.0);
        ControlledStraight(m_controlledSvelocity,L13,6.0,-1.0,true);
        ControlledTurn(m_controlledCvelocity,67.0);
        ControlledStraight(m_controlledSvelocity,L01,6.0,-1.0,true);
          break;}

  default:
       break;
     }
      emit sgn_quit();


}

