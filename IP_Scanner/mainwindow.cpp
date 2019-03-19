#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QNetworkInterface>
#include <string>
#include <cstring>
#include <QDebug>
#include <QString>
#include <QStringList>
#include <QComboBox>
#include <QTcpSocket>
#include <QNetworkProxy>


using namespace std;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}


QStringList MainWindow::IP_Scanner()
{
    QString store_ip;
    QStringList ip_list = {};


    /*
        The QNetworkInterface class provides a listing of the host's IP addresses and network interfaces.
        QNetworkInterface represents one network interface attached to the host where the program is being run.
        Each network interface may contain zero or more IP addresses,
        each of which is optionally associated with a netmask and/or a broadcast address.
        The list of such trios can be obtained with addressEntries().
    */


    QList<QNetworkInterface> interfaces = QNetworkInterface::allInterfaces();

    for(int i=0; i<interfaces.count(); i++)
    {
        /*
        The QNetworkAddressEntry class stores one IP address supported by a network interface,
        along with its associated netmask and broadcast address.
        Each network interface can contain zero or more IP addresses,
        which in turn can be associated with a netmask
        and/or a broadcast address (depending on support from the operating system).
        */
        QList<QNetworkAddressEntry> entries = interfaces.at(i).addressEntries();

        for(int j=0; j<entries.count(); j++)
        {
            if(entries.at(j).ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                // Prints the IP addresses to the console
                qDebug() << entries.at(j).ip().toString();


                // Store the IP addresses in variable store_ip
                store_ip = entries.at(j).ip().toString();


                // Enter the IP addresses into the lists ip_list with NO filter
                //ip_list.append(store_ip);

                // Insert only the good addresses(That return value of "True") to the combo box
                if (IP_filter(store_ip))
                {
                    ip_list.append(store_ip);
                }
            }
        }
    }

    // Print QStringList ( the IP addresses ) to the console
    qDebug() << ip_list;

    // Insert ip addresses to comboBox
    for (int i = 0; i < ip_list.size(); ++i)
    {
        ui->ipList_comboBox->addItem(ip_list.at(i));
    }

    return ip_list;
}


bool MainWindow::IP_filter(QString ip)
{
    int ipParts[4];
    int ipPartsLength = sizeof(ipParts)/sizeof(int);
    QStringList ip_list = {};

    // Remove "." from the ip address
    QStringList ipAfterSplitTo4Parts = ip.split('.');
    qDebug() << ipAfterSplitTo4Parts ;

    for(int i = 0 ; i<ipPartsLength ; i++)
    {
        ipParts[i] = ipAfterSplitTo4Parts.at(i).toInt();
        qDebug() << ipParts[i]; // Print IP after split to the console
    }

    //Check whether the IP address is in range
    // return (ipParts[0] == 192 && ipParts[1] == 168 && ipParts[2] == 53 && ipParts[3] >=1 && ipParts[3] <= 100) ? true : false;
    if(ipParts[0] == 192 && ipParts[1] == 168 && ipParts[2] == 53 && ipParts[3] >=1 && ipParts[3] <= 100) {
        qDebug() << "IP address valid  ! !";
        return true;
    }

    qDebug() << "IP address not in Range ! !";
    return false;

}


void MainWindow::connect_to_network()
{
    QTcpSocket socket; //The QTcpSocket provides a TCP socket.
    QString store_ip;

    store_ip = ui->ipList_comboBox->currentText();

    qDebug() << store_ip;

    socket.setProxy(QNetworkProxy::NoProxy);
    socket.connectToHost(store_ip, 53);// Attempts to make a connection to hostName on the given port. The socket is opened in the given openMode.
    if (socket.waitForConnected()) {
        qDebug()
            << "Local IPv4 address for Internet connectivity is" << socket.localAddress();
    } else {
        qWarning()
            << "Could not determine local IPv4 address:" << socket.errorString();
    }
}


void MainWindow::on_pushButton_clicked()
{
    IP_Scanner();
}


void MainWindow::on_connect_pushButton_clicked()

{
    connect_to_network();
}


void MainWindow::on_filter_pushButton_clicked()
{
    //IP_filter("192.168.53.1");
}
