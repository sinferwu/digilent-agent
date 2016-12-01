#include <QDesktopServices>
#include <QUrl>
#include <QDebug>

#include "agent.h"


Agent::Agent()
{
    this->httpCapable = true;
    this->majorVersion = 0;
    this->minorVersion = 1;
    this->patchVersion = 0;

    //Initialize devices array with null pointers
    this->activeDevice = 0;
    /*
    this->deviceCount = 0;
    for(int i=0; i < MAX_DEVICE_COUNT; i++)
    {
        devices[i] = 0;
    }
    */
    //Uart
    this->uartInfo = new UartInfo();
}

Agent::~Agent(){
    qDebug("Agent Descructor");
    if(this->activeDevice != 0)
    {
        qDebug("Freeing Active Device");
        delete this->activeDevice;
        this->activeDevice = 0;
    }
}

QVector<QString> Agent::enumerateDevices() {
    //---------- UART ----------
    QVector<QString> devices = QVector<QString>();
    if(this->activeDevice != 0)
    {
        devices.append(this->activeDevice->name);
    }
    uartInfo->refreshPortInfo();    
    for(int i=0; i<uartInfo->ports.count(); i++)
    {
        if(!uartInfo->ports[i].isBusy())
        {
            QString portName = uartInfo->ports[i].portName();
            if(this->activeDevice != 0)
            {
                qDebug() << this->activeDevice->name;
                qDebug() << portName;

                if(this->activeDevice->name != portName){
                    devices.append(portName);
                }
            }
            else
            {
                devices.append(portName);
            }
         }
    }

    //---------- HTTP ----------
    //TODO?

    return devices;
}

QByteArray Agent::getVersion() {
    return QByteArray(QString("%1.%2.%3").arg(majorVersion).arg(minorVersion).arg(patchVersion).toUtf8());
}

int Agent::getMajorVersion() {
    return this->majorVersion;
}

int Agent::getMinorVersion() {
    return this->minorVersion;
}

int Agent::getPatchVersion() {
    return this->patchVersion;
}

bool Agent::launchWfl() {
    return QDesktopServices::openUrl(QUrl("http://openscope.s3-website-us-west-2.amazonaws.com/"));
}

bool Agent::setActiveDeviceByName(QString deviceName) {

    QVector<QString> devices = enumerateDevices();
    if(this->activeDevice != 0)
    {
        if(this->activeDevice->name == deviceName)
        {
            return true;
        } else {
            delete this->activeDevice;
            this->activeDevice = 0;
        }
    }
    //Check if devices still exists and is not busy
    for(int i=0; i<devices.size(); i++)
    {
        if(devices[i] == deviceName)
        {
            this->activeDevice = new WflUartDevice(deviceName);
            this->activeDevice->name = deviceName;
            this->activeDevice->execCommand("{\"mode\":\"JSON\"}\r\n");

            return true;
        }
    }

    //Selected device does not exist
    return false;
}
