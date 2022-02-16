TARGET = persondetector
CONFIG += c++14
QT += core widgets gui dbus

SOURCES += myapp.cpp myvision.cpp myengine.cpp tflite.cpp main.cpp customresultimage.cpp
HEADERS += myapp.h myvision.h myengine.h tflite.h customresultimage.h
DEFINES +=
DISTFILES += license.txt qtlogging.ini

# Add tensorflowLite library
LIBS+= -L$$PWD/lib -ltensorflow-lite_2_3
LIBS+= -ldl
HEADERPATH += $$PWD/include
INCLUDEPATH += $$PWD/include

NXT_SDK = 2.1.0
AVATAR = avatar.png
MANIFEST = manifest.json
TRANSLATION = translation.json
LICENSE = license.txt

# All files/folders which need to be shipped have to be added to DEPLOYFILES
DEPLOYFILES += $$PWD/cnn $$PWD/DejaVuSans.ttf $$PWD/qtlogging.ini

include($(NXT_FRAMEWORK)/qmake/nxt_rio.pri)
