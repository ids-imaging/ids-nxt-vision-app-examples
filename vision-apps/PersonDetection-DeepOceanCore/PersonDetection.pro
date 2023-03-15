TARGET = persondetector
CONFIG += c++14
QT += core widgets gui dbus

SOURCES += myapp.cpp myvision.cpp myengine.cpp main.cpp customresultimage.cpp \
    inferenceresultprocessing.cpp
HEADERS += myapp.h myvision.h myengine.h customresultimage.h \
    inferenceresultprocessing.h

LIBS+= -ldl

NXT_SDK = 3.0.0
AVATAR = avatar.png
MANIFEST = manifest.json
TRANSLATION = translation.json
LICENSE = license.txt

# All files/folders which need to be shipped have to be added to DEPLOYFILES
DEPLOYFILES += $$PWD/cnn $$PWD/DejaVuSans.ttf $$PWD/qtlogging.ini

include($(NXT_FRAMEWORK)/qmake/nxt_rio.pri)
