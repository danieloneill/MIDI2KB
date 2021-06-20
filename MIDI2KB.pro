QT += gui widgets quickwidgets quickcontrols2

CONFIG += c++11 console
CONFIG -= app_bundle testlib

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
LIBS += -lwinmm

SOURCES += \
        QMidiFile.cpp \
        QMidiIn.cpp \
        QMidiOut.cpp \
        QMidi_Win32.cpp \
        keyboard.cpp \
        listener.cpp \
        main.cpp \
        mainwindow.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    QMidiFile.h \
    QMidiIn.h \
    QMidiOut.h \
    keyboard.h \
    listener.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    HeaderButton.qml \
    events.qml \
    packages/org.Reinsoft.MIDI2KB/meta/gpl-3.0.txt \
    packages/org.Reinsoft.MIDI2KB/meta/installscript.qs \
    packages/org.Reinsoft.MIDI2KB/meta/package.xml

RESOURCES += \
    resources.qrc

#INSTALLER = installer
#
#INPUT = $$PWD/config/config.xml# $$PWD/packages
#wininstall.input = INPUT
#wininstall.output = $$INSTALLER
#wininstall.commands = C:/Qt/QtIFW-4.1.1/bin/binarycreator --offline-only -c $$PWD/config/config.xml -p $$PWD/packages MIDI2KBInstaller
#wininstall.CONFIG += target_predeps no_link combine
#
#QMAKE_EXTRA_COMPILERS += wininstall
