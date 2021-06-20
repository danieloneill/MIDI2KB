#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    QSettings   *m_settings;

    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QString selectedMidiDevice();
    uint keypressDuration();

private:
    Ui::MainWindow *ui;

public slots:
    void scanResults( QList< QPair< QString, QString > > results );
    void setStatus(const QString &message);

    void connectedChanged(bool tf);

protected slots:
    void comboChanged();
    bool isActive();

signals:
    void quitRequested();
    void resetClicked();
    void connectClicked();
    void scanRequested();
    void queueKey(uint vkey);
    void activeChanged();

    // Simply passed from Listener signal to Quick widgets:
    void midiEvent(uint note, uint velocity);
};

#endif // MAINWINDOW_H
