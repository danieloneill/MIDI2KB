#ifndef LISTENER_H
#define LISTENER_H

#include <QHash>
#include <QPair>
#include <QObject>

class MainWindow;
class Keyboard;
class QMidiIn;
class Listener : public QObject
{
    Q_OBJECT

    MainWindow  *m_mainwindow;
    Keyboard    *m_keyboard;
    QMidiIn     *m_midi;

    bool        m_active;

public:
    explicit Listener(MainWindow *parent = nullptr);
    ~Listener();

public slots:
    void midiEventHandler(quint32 message, quint32 timing);
    void connectClicked();

    bool start(const QString &device);
    bool stop();

    void rescan();

    void queueKey(uint vkey);

signals:
    void devicesScanned(QList< QPair<QString, QString>> devices);
    void connectedChanged(bool tf);
    void midiEvent(uint note, uint velocity);

    void message(const QString &message);

protected:
    QHash< uint, QPair< uint, uint > > m_codeMap;
};

#endif // LISTENER_H
