#include "controlsystem.h"

ControlSystem::ControlSystem()
{
    m_pMobilePlatform = new MobilePlatform();
          m_pMainWindow = new MainWindow();

          connectSignalSlots();
          m_pMainWindow->show();
}
ControlSystem::~ControlSystem()
{
    delete m_pMainWindow;
    m_pMobilePlatform->StopSensorforDelete();
    delete m_pMobilePlatform;
}

void ControlSystem::connectSignalSlots()
{
    connect(m_pMainWindow, SIGNAL(sgn_forwardButtonClicked()),   m_pMobilePlatform, SLOT(slot_moveForward()));
    connect(m_pMainWindow, SIGNAL(sgn_backwardButtonClicked()),  m_pMobilePlatform, SLOT(slot_moveBackward()));
    connect(m_pMainWindow, SIGNAL(sgn_leftButtonClicked()),      m_pMobilePlatform, SLOT(slot_moveLeft()));
    connect(m_pMainWindow, SIGNAL(sgn_rightButtonClicked()),     m_pMobilePlatform, SLOT(slot_moveRight()));
    connect(m_pMainWindow, SIGNAL(sgn_stopButtonClicked()),      m_pMobilePlatform, SLOT(slot_stopMotion()));
    connect(m_pMainWindow, SIGNAL(sgn_stopButtonClicked()),      m_pMobilePlatform,SIGNAL(sgn_quit()));
    connect(m_pMainWindow, SIGNAL(sgn_followLineButtonClicked()),m_pMobilePlatform, SLOT(slot_followline()));
    connect(m_pMainWindow, SIGNAL(sgn_speedButtonClicked(double)),  m_pMobilePlatform, SLOT(slot_setPWM(double)));
    connect(m_pMainWindow,SIGNAL(sgn_OdometrieButtonClicked()),     m_pMobilePlatform, SLOT(slot_OdometrieStart()));   
    connect(m_pMainWindow,SIGNAL(sgn_pathChanged(QString)),      m_pMobilePlatform, SLOT(slot_pathChanged(QString)));
    connect(m_pMainWindow,SIGNAL(sgn_loadXMLbuttonClicked()),    m_pMobilePlatform,  SLOT(slot_XMLparsen()));
    //connect(m_pMainWindow,SIGNAL(sgn_velocityClicked()),         m_pMobilePlatform, SLOT(slot_velocitycal()));
    connect(m_pMainWindow, SIGNAL(sgn_velocityControlParameterChanged(double,int)), m_pMobilePlatform, SIGNAL(sgn_forwardControlParameters(double,int)));
    connect(m_pMainWindow,      SIGNAL(sgn_requestingNewData(int)),     m_pMobilePlatform, SLOT(slot_getNewSensorValues(int)));
    connect(m_pMobilePlatform, SIGNAL(sgn_newSensorValues(int,double)),m_pMainWindow,      SLOT(slot_updateData(int,double)));
    connect(m_pMainWindow,      SIGNAL(sgn_resetSensor(int)),           m_pMobilePlatform, SLOT(slot_resetSensor(int)));
    connect(m_pMainWindow, SIGNAL(sgn_ChallengeStartButtonClicked()),m_pMobilePlatform,   SIGNAL(sgn_getChallengeStart()));
    connect(m_pMobilePlatform,SIGNAL(sgn_GotoLager()),m_pMainWindow,SLOT(slot_gotolager()));
    connect(m_pMobilePlatform,SIGNAL(sgn_getWare()),m_pMainWindow,SLOT(slot_inthelager()));
    connect(m_pMobilePlatform,SIGNAL(gn_GotoClerk()),m_pMainWindow,SLOT(slot_gotoclerk()));
    connect(m_pMobilePlatform,SIGNAL(sgn_putdownWare()),m_pMainWindow,SLOT(slot_intheclerk()));
    connect(m_pMobilePlatform,SIGNAL(sgn_GobackStartpoint()),m_pMainWindow,SLOT(slot_gobackstart()));
}
