CONFIG += c++14
QT += core widgets gui dbus

TARGET = classification

SOURCES += myapp.cpp myvision.cpp myengine.cpp main.cpp
HEADERS += myapp.h myvision.h myengine.h

DEFINES +=
DISTFILES +=
#Mandatory params
NXT_SDK = 2.1.0
AVATAR = avatar.png
MANIFEST = manifest.json
TRANSLATION = translation.json

#Optional params
LICENSE = license.txt
# add files within source directory which should be shipped (e.g. libraries, configs, ...)
# To ship cnns place them in the folder cnn. They are installed automatically
DEPLOYFILES = $$PWD/qtlogging.ini $$PWD/cnn

include($(NXT_FRAMEWORK)/qmake/nxt_rio.pri)
