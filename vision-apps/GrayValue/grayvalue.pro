TARGET = grayvalue
CONFIG += c++14
QT += core widgets gui dbus

SOURCES += main.cpp gvapp.cpp gvengine.cpp gvvision.cpp
HEADERS += gvapp.h gvengine.h gvvision.h

#Mandatory params
NXT_SDK = 2.3.0
AVATAR = avatar.png
MANIFEST = manifest.json
TRANSLATION = translation.json

#Optional params
LICENSE = license.txt
# add files within source directory which should be shipped (e.g. libraries, configs, ...)
DEPLOYFILES = $$PWD/qtlogging.ini

include($(NXT_FRAMEWORK)/qmake/nxt_rio.pri)
