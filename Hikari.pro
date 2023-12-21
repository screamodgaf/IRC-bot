QT -= gui
QT += network
CONFIG += c++17 console network
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS



#==========



#========


# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
#QMAKE_CXXFLAGS += -O2 -mpreferred-stack-boundary=3 -finline-small-functions -momit-leaf-frame-pointer  ///////// -- -mpreferred-stack-boundary=3  robi blad lol
QMAKE_CXXFLAGS += -O2 -finline-small-functions -momit-leaf-frame-pointer
SOURCES += main.cpp \
    Capitalism/stockmarket.cpp \
    Capitalism/stockreadandsave.cpp \
    Capitalism/capital.cpp \
    Capitalism/capitalismgame.cpp \
    Capitalism/capitalismkasyno.cpp \
    Capitalism/capitalismstorage.cpp \
    antiflood.cpp \
    chatbotgtp.cpp \
    colourmanipulation.cpp \
    englishposts.cpp \
    greeter.cpp \
    ignorednicks.cpp \
    inviteonpart.cpp \
    plugin.cpp \
    removedoublenicks.cpp \
    settings.cpp \
    socket.cpp \
    weather.cpp \
    speech.cpp \
    consoleinput.cpp \
    news1.cpp \
    parser.cpp \
    extract_nickandroom.cpp \
    talking.cpp \
    optionsactivator.cpp \
    replacer.cpp \
    removeduplicates.cpp \
    recordtalks.cpp \
    botspeech.cpp \
    loadlogs.cpp \
    controlirc.cpp \
    weatherapiforecast.cpp \
    weatherforecast.cpp \
    youtube.cpp




HEADERS += \
    Capitalism/stockmarket.h \
    Capitalism/stockreadandsave.h \
    Capitalism/capital.h \
    Capitalism/capitalismgame.h \
    Capitalism/capitalismkasyno.h \
    Capitalism/capitalismstorage.h \
    antiflood.h \
    chatbotgtp.h \
    colourmanipulation.h \
    englishposts.h \
    greeter.h \
    ignorednicks.h \
    inviteonpart.h \
    plugin.h \
    removedoublenicks.h \
    settings.h \
    socket.h \
    weather.h \
    speech.h \
    consoleinput.h \
    news1.h \
    parser.h \
    extract_nickandroom.h \
    talking.h \
    optionsactivator.h \
    replacer.h \
    removeduplicates.h \
    recordtalks.h \
    botspeech.h \
    loadlogs.h \
    controlirc.h \
    weatherapiforecast.h \
    weatherforecast.h \
    youtube.h


INCLUDEPATH += $$PWD/.
INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
