#define nRefreshRateGUIData 30 // 30 miliseconds

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include  <QTime>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //makePlot();
    createButtonGroup(); //selbst programmierte funktion

    connect(ui->forwardButton,    SIGNAL(clicked()),         this, SIGNAL(sgn_forwardButtonClicked()));
    connect(ui->backwardButton,   SIGNAL(clicked()),         this, SIGNAL(sgn_backwardButtonClicked()));
    connect(ui->leftButton,       SIGNAL(clicked()),         this, SIGNAL(sgn_leftButtonClicked()));
    connect(ui->rightButton,      SIGNAL(clicked()),         this, SIGNAL(sgn_rightButtonClicked()));
    connect(ui->stopButton,       SIGNAL(clicked()),         this, SIGNAL(sgn_stopButtonClicked()));
    connect(ui->followLineButton, SIGNAL(clicked()),         this, SIGNAL(sgn_followLineButtonClicked()));
    connect(ui->doubleSpinBox,          SIGNAL(valueChanged(double)), this, SIGNAL(sgn_speedButtonClicked(double)));
    connect(ui->lineEdit_path, SIGNAL(textChanged(QString)),this,SIGNAL(sgn_pathChanged(QString)));
    connect(ui->pushButton_path,  SIGNAL(clicked()),           this,SIGNAL(sgn_loadXMLbuttonClicked()));
    connect(ui->Odometrie, SIGNAL(clicked()),this, SIGNAL(sgn_OdometrieButtonClicked()));//  +++++++++++++++++++++++
    connect(ui->ChallengeStartButton,SIGNAL(clicked()),     this,SIGNAL(sgn_ChallengeStartButtonClicked()));
    //connect(ui->velocity,SIGNAL(clicked()),this, SIGNAL(sgn_velocityClicked()));//++++
    connect(m_pReadSensorsButtonGroup, SIGNAL(buttonToggled(int,bool)), this, SLOT(slot_activeLineEdit(int, bool)));

    m_pTimerRequestData = new QTimer();
    m_pTimerRequestData->setInterval(nRefreshRateGUIData);
    m_pTimerRequestData->setSingleShot(false);
    connect(m_pTimerRequestData, SIGNAL(timeout()), this, SLOT(slot_requestNewData()));
    //connect(m_pTimerRequestData, SIGNAL(timeout()), this, SLOT(slot_realtimeDataSlot()));//for realtime plotting
    m_pTimerRequestData->start();
    connect(ui->doubleSpinBox_KpLeft, SIGNAL(valueChanged(double)), this, SLOT(slot_mapControlParameterSignal(double)));
    connect(ui->doubleSpinBox_KiLeft, SIGNAL(valueChanged(double)), this, SLOT(slot_mapControlParameterSignal(double)));
    connect(ui->doubleSpinBox_KdLeft, SIGNAL(valueChanged(double)), this, SLOT(slot_mapControlParameterSignal(double)));
    connect(ui->doubleSpinBox_KpRight, SIGNAL(valueChanged(double)), this, SLOT(slot_mapControlParameterSignal(double)));
    connect(ui->doubleSpinBox_KiRight, SIGNAL(valueChanged(double)), this, SLOT(slot_mapControlParameterSignal(double)));
    connect(ui->doubleSpinBox_KdRight, SIGNAL(valueChanged(double)), this, SLOT(slot_mapControlParameterSignal(double)));
}


MainWindow::~MainWindow()
{
    //m_pTimerRequestData->stop();
    delete ui;
}

void MainWindow::createButtonGroup()
{
    m_pReadSensorsButtonGroup = new QButtonGroup(this);
    m_pReadSensorsButtonGroup->setExclusive(false);// set 2 stable status(checked)
    m_pReadSensorsButtonGroup->addButton(ui->checkBox_readLineSensorA, nMapLineSensorA);
    m_pReadSensorsButtonGroup->addButton(ui->checkBox_readLineSensorB, nMapLineSensorB);
    m_pReadSensorsButtonGroup->addButton(ui->checkBox_readColorSensor, nMapColorSensor);
    m_pReadSensorsButtonGroup->addButton(ui->checkBox_readEncoderA, nMapLeftEncoderMotorA);
    m_pReadSensorsButtonGroup->addButton(ui->checkBox_readEncoderB, nMapRightEncoderMotorB);
    m_pReadSensorsButtonGroup->addButton(ui->checkBox_readSonicSensor, nMapSonicSensor);
}


void MainWindow::slot_activeLineEdit(int SensorID, bool checked)
{
    switch (SensorID)
    {
    case nMapLineSensorA:
        ui->lineEdit_lineSensorA->setEnabled(checked);
        ui->lineEdit_speedA->setEnabled(checked);
        break;
    case nMapLineSensorB:
        ui->lineEdit_lineSensorB->setEnabled(checked);
        ui->lineEdit_speedB->setEnabled(checked);
        break;
    case nMapColorSensor:
        ui->lineEdit_colorSensor->setEnabled(checked);
        break;
    case nMapLeftEncoderMotorA:
        ui->lineEdit_encoderA->setEnabled(checked);
        if (checked)
            emit sgn_resetSensor(nMapLeftEncoderMotorA);
        break;
    case nMapRightEncoderMotorB:
        ui->lineEdit_encoderB->setEnabled(checked);
        if (checked)
            emit sgn_resetSensor(nMapRightEncoderMotorB);
        break;
    case nMapSonicSensor:
        ui->lineEdit_verticalDistance->setEnabled(checked);
        break;
    default:
        break;
    }
}

void MainWindow::slot_requestNewData()
{
    if (m_pReadSensorsButtonGroup->button(nMapLineSensorA)->isChecked())
    {
        emit sgn_requestingNewData(nMapLineSensorA);
    }

    if (m_pReadSensorsButtonGroup->button(nMapLineSensorB)->isChecked())
    {
        emit sgn_requestingNewData(nMapLineSensorB);
    }

    if (m_pReadSensorsButtonGroup->button(nMapColorSensor)->isChecked())
    {
        emit sgn_requestingNewData(nMapColorSensor);
    }

    if (m_pReadSensorsButtonGroup->button(nMapLeftEncoderMotorA)->isChecked())
    {
        emit sgn_requestingNewData(nMapLeftEncoderMotorA);
    }

    if (m_pReadSensorsButtonGroup->button(nMapRightEncoderMotorB)->isChecked())
    {
        emit sgn_requestingNewData(nMapRightEncoderMotorB);
    }

    if (m_pReadSensorsButtonGroup->button(nMapSonicSensor)->isChecked())
    {
        emit sgn_requestingNewData(nMapSonicSensor);
    }


}

//QTime t1;
//t1= QTime::currentTime();
void MainWindow::slot_updateData(int SensorID, double sensorData)
{   //t0;
    QString str = QString::number(sensorData);  // translate the number value into string


    QString strA = QString::number(0);//---

    QString strB = QString::number(0);
    switch (SensorID)
    {
    case nMapLineSensorA:
        if (sensorData == 1.0)
            ui->lineEdit_lineSensorA->setText("Black");
        else if (sensorData == 0.0)
            ui->lineEdit_lineSensorA->setText("White");
        break;
    case nMapLineSensorB:
        if (sensorData == 1.0)
            ui->lineEdit_lineSensorB->setText("Black");
        else if (sensorData == 0.0)
            ui->lineEdit_lineSensorB->setText("White");
        else
            std::cout << "Error in RightLineSensorB" << std::endl;
        break;
    case nMapColorSensor:
        if (sensorData == 0.0)
            ui->lineEdit_colorSensor->setText("White");
        else if (sensorData == 1.0)
            ui->lineEdit_colorSensor->setText("Black");
        else if (sensorData == 2.0)
            ui->lineEdit_colorSensor->setText("Red");
        else if (sensorData == 3.0)
            ui->lineEdit_colorSensor->setText("Green");
        else if (sensorData == 4.0)
            ui->lineEdit_colorSensor->setText("Blue");
        else if (sensorData == 5.0)
            ui->lineEdit_colorSensor->setText("Yellow");
        break;
    case nMapLeftEncoderMotorA:
        ui->lineEdit_encoderA->setText(str);       
        ui->lineEdit_speedA->setText(strA);
        break;
    case nMapRightEncoderMotorB:
        ui->lineEdit_encoderB->setText(str);
        ui->lineEdit_speedB->setText(strB);
        break;
    case nMapSonicSensor:
        ui->lineEdit_verticalDistance->setText(str);
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *closeevent) {
    ui->stopButton->clicked();
    closeevent->accept();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_W:
        ui->forwardButton->clicked();
        break;
    case Qt::Key_S:
        ui->backwardButton->clicked();
        break;
    case Qt::Key_A:
        ui->leftButton->clicked();
        break;
    case Qt::Key_D:
        ui->rightButton->clicked();
        break;
    case Qt::Key_Q:
        ui->stopButton->clicked();
        break;
    case Qt::Key_E:
        ui->followLineButton->clicked();
        break;
    case Qt::Key_Escape:
        ui->stopButton->clicked();
        this->close();
        break;
    default: QWidget::keyPressEvent(event);
    }
}

void MainWindow::slot_mapControlParameterSignal(double value)
{
//    QDoubleSpinBox* spinBox = sender();
//    QString spinBox = obj->objectName();
//    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox>(sender());
//    QDoubleSpinBox spinBox = qobject_cast<QDoubleSpinBox>(sender());
    QDoubleSpinBox *spinBox = qobject_cast<QDoubleSpinBox *>(sender());
    if (spinBox == ui->doubleSpinBox_KpLeft)
        emit sgn_velocityControlParameterChanged(value, nMapLeftWheelVelocityKp);
    else if (spinBox == ui->doubleSpinBox_KiLeft)
        emit sgn_velocityControlParameterChanged(value, nMapLeftWheelVelocityKi);
    else if (spinBox == ui->doubleSpinBox_KdLeft)
        emit sgn_velocityControlParameterChanged(value, nMapLeftWheelVelocityKd);
    else if (spinBox == ui->doubleSpinBox_KpRight)
        emit sgn_velocityControlParameterChanged(value, nMapRightWheelVelocityKp);
    else if (spinBox == ui->doubleSpinBox_KiRight)
        emit sgn_velocityControlParameterChanged(value, nMapRightWheelVelocityKi);
    else if (spinBox == ui->doubleSpinBox_KdRight)
        emit sgn_velocityControlParameterChanged(value, nMapRightWheelVelocityKd);
    else
        qDebug()<< "Error in MainWindow::slot_mapControlParameterSignal(double value).";
}

void MainWindow:: slot_gotolager()
{
  ui->lineEdit_workphase->setText("gotolager");

}

void MainWindow:: slot_inthelager()
{
  ui->lineEdit_workphase->setText("inthelager");
}
void MainWindow:: slot_gotoclerk()
{
  ui->lineEdit_workphase->setText("gotocler");
}
void MainWindow:: slot_intheclerk()
{
  ui->lineEdit_workphase->setText("intheclerk");
}
void MainWindow:: slot_gobackstart()
{
  ui->lineEdit_workphase->setText("gobackstart");
}
  void MainWindow::makePlot()
{
    //cout<<"makeplot"<<endl;
    QTimer dataTimer;
    dataTimer.setSingleShot(false);
    ui->customPlot->addGraph(); // blue line
    ui->customPlot->graph(0)->setPen(QPen(QColor(40, 110, 255)));
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");
    ui->customPlot->xAxis->setTicker(timeTicker);
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(-10.0, 120.0);

    // make left and bottom axes transfer their ranges to right and top axes:
    connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    //connect(&dataTimer, SIGNAL(timeout()), this, SLOT(slot_realtimeDataSlot()));
    //dataTimer.start(0); // Interval 0 means to refresh as fast as possible
    //cout<<"makeplot end"<<endl;

}

void MainWindow::slot_realtimeDataSlot()
{   //cout<<"plot start"<<endl;
    static QTime time(QTime::currentTime());
    // calculate two new data points:
    double key = time.elapsed()/1000.0; // time elapsed since start of demo, in seconds
    static double lastPointKey = 0;
    if (key-lastPointKey > 0.002) // at most add point every 2 ms
    {
      // add data to lines:
      ui->customPlot->graph(0)->addData(key, velocityA);
      ui->customPlot->graph(1)->addData(key, velocityB);
      // rescale value (vertical) axis to fit the current data:
      //ui->customPlot->graph(0)->rescaleValueAxis();
      //ui->customPlot->graph(1)->rescaleValueAxis(true);
      lastPointKey = key;
    }
    // make key axis range scroll with the data (at a constant range size of 8):
    ui->customPlot->xAxis->setRange(key,8, Qt::AlignRight);
    ui->customPlot->replot();

    // calculate frames per second:
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
      ui->statusBar->showMessage(
            QString("%1 FPS, Total Data points: %2")
            .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
            .arg(ui->customPlot->graph(0)->data()->size()+ui->customPlot->graph(1)->data()->size())
            , 0);
      lastFpsKey = key;
      frameCount = 0;
    }
}


