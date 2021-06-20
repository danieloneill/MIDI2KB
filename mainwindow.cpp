#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QQmlContext>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_settings = new QSettings();

    ui->action_Start->setEnabled(false);
    ui->action_Stop->setEnabled(false);

    ui->editPressDuration->setValidator(new QIntValidator(0, 9999, this));
    ui->editPressDuration->setText( m_settings->value("pressDuration", 50).toString() );

    connect( ui->editPressDuration, &QLineEdit::textEdited, [=]() {
        m_settings->setValue("pressDuration", ui->editPressDuration->text());
    });

    connect( ui->btnScan, &QPushButton::clicked, this, &MainWindow::scanRequested );
    ui->cbDevices->addItem(tr("Click (re)scan to find MIDI devices..."));

    connect( ui->btnConnect, &QPushButton::clicked, this, &MainWindow::connectClicked );
    connect( ui->action_Quit, &QAction::triggered, this, &MainWindow::quitRequested );
    connect( ui->btnEnabled, &QPushButton::toggled, this, &MainWindow::activeChanged );
    connect( ui->btnReset, &QPushButton::clicked, this, &MainWindow::resetClicked );

    ui->eventsQuickWidget->rootContext()->setContextProperty("MainWindow", this);
    ui->eventsQuickWidget->setSource(QUrl("qrc:/events.qml"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::selectedMidiDevice()
{
    return ui->cbDevices->currentData().toString();
}

uint MainWindow::keypressDuration()
{
    return ui->editPressDuration->text().toUInt();
}

void MainWindow::scanResults( QList< QPair< QString, QString > > results )
{
    ui->cbDevices->clear();
    ui->btnConnect->setEnabled(false);

    ui->cbDevices->addItem(tr("Click (re)scan to find MIDI devices..."));
    foreach( auto pair, results )
    {
        ui->cbDevices->addItem(pair.second, pair.first);
        ui->btnConnect->setEnabled(true);
        ui->action_Start->setEnabled(true);
    }
    ui->cbDevices->setCurrentIndex( results.length() > 0 ? 1 : 0 );
}

void MainWindow::setStatus(const QString &message)
{
    ui->statusbar->showMessage(message);
}

void MainWindow::comboChanged()
{
    if( ui->cbDevices->currentIndex() == 0 )
    {
        return ui->btnConnect->setEnabled(false);
    }

    ui->btnConnect->setEnabled(true);
    ui->action_Start->setEnabled(true);
}

bool MainWindow::isActive()
{
    return ui->btnEnabled->isChecked();
}

void MainWindow::connectedChanged(bool tf)
{
    if( tf )
    {
        ui->statusbar->showMessage(tr("MIDI Device connected."));
        ui->btnConnect->setText( tr("&Disconnect") );
        ui->action_Stop->setEnabled(true);
        ui->action_Start->setEnabled(false);
    }
    else
    {
        ui->statusbar->showMessage(tr("MIDI Device DISCONNECTED."));
        ui->btnConnect->setText( tr("&Connect") );
        ui->action_Stop->setEnabled(false);
        ui->action_Start->setEnabled( ui->cbDevices->count() > 0 && ui->cbDevices->currentIndex() > 0 ? true : false );
    }
}
