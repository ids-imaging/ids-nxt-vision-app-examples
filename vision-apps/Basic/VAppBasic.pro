TARGET = VAppBasic
CONFIG += c++14
QT += core widgets gui dbus

SOURCES += customresultimage.cpp main.cpp myapp.cpp myengine.cpp myvision.cpp
HEADERS += customresultimage.h myapp.h myengine.h myvision.h

NXT_SDK = 2.1.0
AVATAR = avatar.png
MANIFEST = manifest.json
TRANSLATION = translation.json
LICENSE = license.txt

DEPLOYFILES += $$PWD/qtlogging.ini

include($(NXT_FRAMEWORK)/qmake/nxt_rio.pri)
