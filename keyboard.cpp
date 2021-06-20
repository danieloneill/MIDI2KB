// Because the SendInput function is only supported in
// Windows 2000 and later, WINVER needs to be set as
// follows so that SendInput gets defined when windows.h
// is included below.
#define WINVER 0x0500
#include <windows.h>

#include <QTimer>
#include <QDebug>

#include "keyboard.h"

Keyboard::Keyboard(QObject *parent)
    : QObject(parent),
      m_parent(parent)
{
}

void Keyboard::queuePress(uint vk, uint sc, uint duration)
{
    // This structure will be used to create the keyboard
    // input event.
    INPUT ip;

    // Set up a generic keyboard event.
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = sc; // hardware scan code for key
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    // Press the key
    ip.ki.wVk = vk;
    ip.ki.dwFlags = 0; // 0 for key press
    SendInput(1, &ip, sizeof(INPUT));

    QTimer *t = new QTimer(this);
    t->setProperty("VKEY", vk);
    t->setProperty("SCODE", sc);
    QTimer::connect( t, &QTimer::timeout, this, &Keyboard::timeout );
    t->setSingleShot(true);
    t->start(duration);
}

void Keyboard::timeout()
{
    QTimer *t = qobject_cast<QTimer *>(sender());

    uint nvk = t->property("VKEY").toUInt();
    uint nsc = t->property("SCODE").toUInt();

    resetKey(nvk, nsc);

    t->deleteLater();
}

void Keyboard::resetKey(uint vk, uint sc)
{
    // Release the key
    INPUT nip;

    // Set up a generic keyboard event.
    nip.type = INPUT_KEYBOARD;
    nip.ki.wScan = sc; // hardware scan code for key
    nip.ki.time = 0;
    nip.ki.dwExtraInfo = 0;

    // Press the key
    nip.ki.wVk = vk;
    nip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
    SendInput(1, &nip, sizeof(INPUT));

    //qDebug() << "Keypress" << vk << "/" << sc << "released.";
}

