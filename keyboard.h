#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <QObject>

class Keyboard : public QObject
{
    Q_OBJECT

    QObject *m_parent;

public:
    Keyboard(QObject *parent);

    void queuePress(uint vk, uint sc, uint duration);

public slots:
    void timeout();
    void resetKey(uint vk, uint sc);
};

#endif // KEYBOARD_H
