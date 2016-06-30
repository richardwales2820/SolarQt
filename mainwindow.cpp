#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->sendButton, SIGNAL(released()), this, SLOT (handleButton()));

    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, infoList)
        ui->ports_combo->addItem(info.portName());
}

void MainWindow::handleButton()
{
    QSerialPort *serial = new QSerialPort(this);
    QString name = ui->ports_combo->currentText();

    serial->setPortName(name);
    serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    QDate selectedDate = ui->calendar->selectedDate();
    QTime selectedTime = ui->timebox->time();

    int dayOfWeek = selectedDate.dayOfWeek();
    int day = selectedDate.day();
    int month = selectedDate.month();
    int year = selectedDate.year();

    int seconds = selectedTime.second();
    int minutes = selectedTime.minute();
    int hour = selectedTime.hour();

    //---------- QByteArray from ints ----------//
    QByteArray dayOfWeekQb = dayOfWeekQb.number(dayOfWeek);
    QByteArray dayQb = dayQb.number(day);
    QByteArray monthQb = monthQb.number(month);
    QByteArray yearQb = yearQb.number(year);
    QByteArray secondsQb = secondsQb.number(seconds);
    QByteArray minutesQb = minutesQb.number(minutes);
    QByteArray hourQb = hourQb.number(hour);

    //---------- QByteArray tags for serial ----------//
    QByteArray tagA("A");
    QByteArray tagB("B");
    QByteArray tagC("C");
    QByteArray tagD("D");
    QByteArray tagE("E");
    QByteArray tagF("F");
    QByteArray tagG("G");

    serial->write(tagA);
    serial->write(secondsQb);
    delay();

    serial->write(tagB);
    serial->write(minutesQb);
    delay();

    serial->write(tagC);
    serial->write(hourQb);
    delay();

    serial->write(tagD);
    serial->write(dayQb);
    delay();

    serial->write(tagE);
    serial->write(monthQb);
    delay();

    serial->write(tagF);
    serial->write(yearQb);
    delay();

    serial->write(tagG);
    serial->write(dayOfWeekQb);

    delete(serial);
}

void MainWindow::delay()
{
    QTime dieTime= QTime::currentTime().addSecs(1);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

MainWindow::~MainWindow()
{
    delete ui;
}
