TARGET = FileTransferAndWebsite
CONFIG += c++14
QT += core widgets gui dbus

SOURCES += main.cpp myapp.cpp myengine.cpp myvision.cpp
HEADERS += myapp.h myengine.h myvision.h

NXT_SDK = 2.3.0
AVATAR = avatar.png
MANIFEST = manifest.json
TRANSLATION = translation.json
LICENSE = license.txt

#VAPP_WEBSITE
DEPLOYFILES += $$PWD/qtlogging.ini $$PWD/website

include($(NXT_FRAMEWORK)/qmake/nxt_rio.pri)
