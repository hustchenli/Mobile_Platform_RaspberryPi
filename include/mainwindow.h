//!
//! \file mainwindow.h
//! \author Li Chen, Hongyu Zhao  Team004
//! \date 04-Jun-2018
//! \brief build the GUI for the program
//!
//! build up the GUI, and it can be also operated by keyboard by using the funktion KeyPressEvent
//! and by using of the qcustomplot, the real time velocity can be showed in GUI
//!
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QKeyEvent>
#include <QDebug>
#include <QButtonGroup>
#include <QTimer>
#include <iostream>
#include <odometrie.h>
//#include <qcustomplot.h>

#define nMapLineSensorA 1
#define nMapLineSensorB 2
#define nMapColorSensor 3
#define nMapSonicSensor 4
#define nMapLeftEncoderMotorA 5
#define nMapRightEncoderMotorB 6

#define nMapLeftWheelVelocityKp 7
#define nMapLeftWheelVelocityKi 8
#define nMapLeftWheelVelocityKd 9

#define nMapRightWheelVelocityKp 10
#define nMapRightWheelVelocityKi 11
#define nMapRightWheelVelocityKd 12

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent * e);
    void closeEvent(QCloseEvent *closeevent);

    void createButtonGroup();
    QButtonGroup* m_pReadSensorsButtonGroup;

    QTimer* m_pTimerRequestData;

signals:

    void sgn_forwardButtonClicked();
    void sgn_backwardButtonClicked();
    void sgn_leftButtonClicked();
    void sgn_rightButtonClicked();
    void sgn_stopButtonClicked();
    void sgn_followLineButtonClicked();
    void sgn_speedButtonClicked(double);
    void sgn_OdometrieButtonClicked();
    void sgn_velocityClicked();

    void sgn_requestingNewData(int SensorID);
    void sgn_resetSensor(int SensorID);
    void sgn_velocityControlParameterChanged(double value, int paramterID);

    void sgn_pathChanged(QString);
    void sgn_loadXMLbuttonClicked();
    void sgn_ChallengeStartButtonClicked();
private slots:
    void slot_requestNewData();
    void slot_activeLineEdit(int SensorID, bool checked);
    void slot_mapControlParameterSignal(double value);
    void slot_gotolager();
    void slot_inthelager();
    void slot_gotoclerk();
    void slot_intheclerk();
    void slot_gobackstart();

    void makePlot();
   void slot_realtimeDataSlot();



public slots:
   //! \brief update the Sensor Data on the GUI
   //!
   //! \param [in] SensorID   for switch case
   //! \param [in] Data       received data for sensor with special ID
   void slot_updateData(int SensorID, double Data);
};

#endif // MAINWINDOW_H

