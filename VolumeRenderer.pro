QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

QMAKE_MACOSX_DEPLOYMENT_TARGET = 13.0

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    fileloader.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    fileloader.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


# DCMTK Library
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/release/ -lcmr
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/debug/ -lcmr
else:unix: LIBS += -L$$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/ -lcmr

INCLUDEPATH += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/include
DEPENDPATH += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/release/libcmr.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/debug/libcmr.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/release/cmr.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/debug/cmr.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/libcmr.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/release/ -ldcmimage
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/debug/ -ldcmimage
else:unix: LIBS += -L$$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/ -ldcmimage

INCLUDEPATH += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/include
DEPENDPATH += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/release/libdcmimage.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/debug/libdcmimage.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/release/dcmimage.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/debug/dcmimage.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/libdcmimage.a

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/release/ -ldcmimgle
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/debug/ -ldcmimgle
else:unix: LIBS += -L$$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/ -ldcmimgle

INCLUDEPATH += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/include
DEPENDPATH += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/release/libdcmimgle.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/debug/libdcmimgle.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/release/dcmimgle.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/debug/dcmimgle.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../../opt/homebrew/Cellar/dcmtk/3.6.7/lib/libdcmimgle.a