#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    // Setup the UI components
    ui->setupUi(this);

    // Connect the push button release signal to the handleButton function
    connect(ui->sendButton, SIGNAL(released()), this, SLOT (handleButton()));

    // Loop through the serial ports and add connected ones to the combobox
    QList<QSerialPortInfo> infoList = QSerialPortInfo::availablePorts();
    foreach (const QSerialPortInfo &info, infoList)
        ui->ports_combo->addItem(info.portName());
}

// When the button is pressed, opens serial communication with the selected port
// from the combobox, then gets the date and time items from the widgets.
// The data is sent, one piece at a time after a certain tag bit
void MainWindow::handleButton()
{
    // new SerialPort pointer that holds the relevant port
    QSerialPort *serial = new QSerialPort(this);
    // Gets the port from the combobox
    QString name = ui->ports_combo->currentText();

    // Opens the port with the necessary settings (Baud, data bits, etc)
    serial->setPortName(name);
    serial->open(QIODevice::ReadWrite);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    // Get all of the data from the widgets, pretty intuitive from here
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

    // Send all of the data over serial with a tag prepended
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

    // Free the memory associated with the serial pointer after it is used
    delete(serial);
}

// Delay function I found that will wait one second in the program before continuing to execute more code
// Used to wait to send more data until the MCU has processed the old bits
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
