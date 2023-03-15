TARGET = MultiROI
CONFIG += c++14
QT += core widgets gui dbus

SOURCES += myapp.cpp myvision.cpp myengine.cpp main.cpp
HEADERS += myapp.h myvision.h myengine.h

NXT_SDK = 2.3.0
AVATAR = avatar.png
MANIFEST = manifest.json
TRANSLATION = translation.json
LICENSE = license.txt

DEPLOYFILES += $$PWD/qtlogging.ini

include($(NXT_FRAMEWORK)/qmake/nxt_rio.pri)
