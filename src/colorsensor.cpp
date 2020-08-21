#define nSamplingRateColorSensor 5//miliseconds

#include "colorsensor.h"


ColorSensor::ColorSensor(int nSensorID) : AbstractSensor()
{

    m_nSensorID = nSensorID;
    m_nSamplingRate = nSamplingRateColorSensor;//update the SamplingRate
    m_pTimerSamplingRate->setInterval(m_nSamplingRate);
    m_pTimerSamplingRate->start();
    m_iDeviceHandleId = wiringPiI2CSetup(0x29);
    wiringPiI2CWriteReg8(m_iDeviceHandleId,0x80|0x00,0x01|0x02);


}

ColorSensor::~ColorSensor()
{
}
void ColorSensor::slot_readSensor()
{  // QCoreApplication::processEvents();
    int nRed = wiringPiI2CReadReg16(m_iDeviceHandleId,0x80|0x16);
    int nGreen = wiringPiI2CReadReg16(m_iDeviceHandleId,0x80|0x18);
    int nBlue = wiringPiI2CReadReg16(m_iDeviceHandleId,0x80|0x1A);

    int nBrightness = wiringPiI2CReadReg16(m_iDeviceHandleId,0x80|0x14);
    m_mutex_value.lock();
    m_dSensorValue = (double)readColor(nRed, nGreen, nBlue,nBrightness);
    m_mutex_value.unlock();
}

int ColorSensor::readColor(int nbRed, int nbGreen, int nbBlue,int nbBrightness)
{   //QCoreApplication::processEvents();
    int nColor;//records the number of the current Color
    double dRedPercent = (double)nbRed/(nbRed+nbGreen+nbBlue);
    double dGreenPercent = (double)nbGreen/(nbRed+nbGreen+nbBlue);
    double dBluePercent = (double)nbBlue/(nbRed+nbGreen+nbBlue);
    //std::cout<<"dRedPercent:"<<dRedPercent<<" dGreenPercent:"<<dGreenPercent<<" dBluePercent:"<<dBluePercent<<endl;

    if (dRedPercent < 0.40 && dRedPercent > 0.28 && dGreenPercent < 0.38 && dGreenPercent > 0.28 && dBluePercent < 0.38 && dBluePercent > 0.26&&nbBrightness<30000)
    {
        nColor = 1;// black
    }
    else
    {
        if (nbRed > nbGreen)
        {
           if (nbRed>nbBlue)
            nColor = 2;// red
           else nColor = 4;//blue
        }
        else
        {
            if(nbGreen > nbBlue)
             nColor = 3;//green
            else nColor = 4;//blue
        }

        if (dBluePercent < 0.20 && dGreenPercent > 0.33 && dRedPercent > (dGreenPercent-0.02))
            nColor = 5;//yellow
        if (dRedPercent < 0.36 && dGreenPercent < 0.36 && dBluePercent < 0.36&&nbBrightness>30000)
            nColor = 0;//white
    }
    return nColor;

}
