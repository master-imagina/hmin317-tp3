INCLUDEPATH += $$PWD

QT += network

SOURCES += $$PWD/openglwindow.cpp \
    gamewindow.cpp calendar.cpp

HEADERS += $$PWD/openglwindow.h \
    $$PWD/gamewindow.h calendar.h

SOURCES += \
    main.cpp

target.path = $$PWD
INSTALLS += target
QMAKE_MAC_SDK = macosx10.11

RESOURCES += \
    gestionnaire.qrc
