#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//HTTP core includes
#include <QtNetwork>
#include <QSystemTrayIcon>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QVector>

//OpenScope device includes
#include "osDevice/osDevice.h"

#ifndef QT_NO_SYSTEMTRAYICON
#include <QMainWindow>
#include "httpClient/httpClient.h"
#include "uartClient/uartInfo.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //Devices
    OsDevice* devices[8];
    int devicesHead = 0;
    OsDevice* activeDevice;
    OsDevice* deviceOne;
    OsDevice* deviceTwo;

    QComboBox *deviceDropDown;

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void onHttpAddDeviceRelease();
    void onUartAddDeviceRelease();
    void onDeviceDropDownCurrentIndexChanged(int);

private:
    //UI Elements
    Ui::MainWindow *ui;
    QLineEdit *httpAddress;
    QPushButton *httpAddDevice;

    QLineEdit *uartAddress;
    QPushButton *uartAddDevice;

    //QComboBox *deviceDropDown;

    //System Tray
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    void createWindowActions();
    void createTrayIcon();

    //UART
    UartInfo *uartInfo;


};

#endif // QT_NO_SYSTEMTRAYICON
#endif // MAINWINDOW_H
