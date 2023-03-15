TARGET = MultiCNN
CONFIG += c++14
QT += core widgets gui dbus

SOURCES += myapp.cpp myvision.cpp myengine.cpp main.cpp
HEADERS += myapp.h myvision.h myengine.h

NXT_SDK = 2.6.0
AVATAR = avatar.png
MANIFEST = manifest.json
TRANSLATION = translation.json
LICENSE = license.txt

# add files within source directory which should be shipped (e.g. libraries, configs, ...)
# To ship cnns place them in the folder cnn. They are installed automatically
DEPLOYFILES = $$PWD/qtlogging.ini $$PWD/cnn

include($(NXT_FRAMEWORK)/qmake/nxt_rio.pri)
