//!
//! \file controlsystem.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 04-Jun-2018
//! \brief to connect the GUI and member funktion
//!
//! this part is designed to build the connection between moblieplatform and mainwindow, for example connect
//! the click signal of button or value change of the spinbox with the funktion of the mobileplatform
//!
// \param [in,out] m_pMoblilePlatform          pointing to moblieplatform
// \param [in,out] m_pMainWindow               pointing to mainwindow


#ifndef CONTROLSYSTEM_H
#define CONTROLSYSTEM_H

#include "mainwindow.h"
#include "mobileplatform.h"
#include <QObject>
#include <QString>

class ControlSystem : public QObject
{
    Q_OBJECT

public:

    ControlSystem();
    ~ControlSystem();

private:
    MobilePlatform *m_pMobilePlatform;
    MainWindow *m_pMainWindow;

    void connectSignalSlots();
};

#endif // CONTROLSYSTEM_H
