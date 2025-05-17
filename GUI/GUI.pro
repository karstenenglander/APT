QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += C:/Users/karst/vcpkg/installed/x64-windows/include
LIBS += -LC:/Users/karst/vcpkg/installed/x64-windows/lib -lsqlite3
INCLUDEPATH += C:/Users/karst/Documents/CS/APT/include
LIBS += -LC:/Users/karst/Documents/CS/APT/lib
LIBS += -LC:/Users/karst/vcpkg/installed/x64-windows/lib -lexiv2






# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    C:/Users/karst/Documents/CS/APT/src/DatabaseManager.cpp \
    C:/Users/karst/Documents/CS/APT/src/DatabaseFunctions.cpp \
    C:/Users/karst/Documents/CS/APT/src/ImageProcessor.cpp \
    C:/Users/karst/Documents/CS/APT/src/TimeUtils.cpp

HEADERS += \
    mainwindow.h \
    C:/Users/karst/Documents/CS/APT/include/DatabaseManager.h \
    C:/Users/karst/Documents/CS/APT/include/DatabaseFunctions.h \
    C:/Users/karst/Documents/CS/APT/include/ImageProcessor.h \
    C:/Users/karst/Documents/CS/APT/include/TimeUtils.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
