#include <QDebug>
#include <QHash>
#include <QPair>

#include "listener.h"

#include <windows.h>
#include <winuser.h>

#include "mainwindow.h"
#include "keyboard.h"
#include "QMidiIn.h"
#include "QMidiFile.h"

Listener::Listener(MainWindow *parent)
    : QObject(parent),
      m_mainwindow(parent)
{
    m_active = false;
    m_midi = new QMidiIn(this);
    m_keyboard = new Keyboard(this);

    QObject::connect( m_midi, &QMidiIn::midiEvent, this, &Listener::midiEventHandler );
}

Listener::~Listener()
{
    m_midi->stop();
    m_midi->deleteLater();
    m_keyboard->deleteLater();
}

void Listener::queueKey(uint vkey)
{
    uint duration = m_mainwindow->keypressDuration();
    uint scode = MapVirtualKeyA(vkey, MAPVK_VK_TO_VSC);
    //qDebug() << "Got key " << vkey << scode << duration;
    m_keyboard->queuePress(vkey, scode, duration);
}

void Listener::connectClicked()
{
    if( m_active )
        stop();
    else
        start(m_mainwindow->selectedMidiDevice());
}

bool Listener::start(const QString &device)
{
    qDebug() << "MIDI connection to device "+device+": " << m_midi->connect(device);

    m_midi->start();
    m_active = true;

    emit connectedChanged( m_midi->isConnected() );
    return m_midi->isConnected();
}

bool Listener::stop()
{
    m_midi->stop();
    m_active = false;

    emit connectedChanged(false);
    return m_midi->isConnected();
}

void Listener::midiEventHandler(quint32 message, quint32 timing)
{
    Q_UNUSED(timing)
    QMidiEvent event;
    event.setMessage(message);

    if( event.type() == -1 )
        return;

    emit midiEvent( event.note(), event.velocity() );
}

void Listener::rescan()
{
    QList< QPair< QString, QString >> devs;

    auto bleh = QMidiIn::devices();
    foreach( QString k, bleh.keys() )
        devs.append( QPair<QString, QString>( k, bleh[k] ) );

    emit devicesScanned(devs);
    emit message(tr("Scan complete, %1 devices found.").arg(devs.length()));
}
