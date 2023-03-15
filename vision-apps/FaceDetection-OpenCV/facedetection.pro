CONFIG += c++14
QT += core widgets gui dbus

TARGET = faceDetection

SOURCES += main.cpp \
    faceEngine.cpp \
    faceResultImage.cpp \
    faceVision.cpp \
    faceVapp.cpp

HEADERS += \
    faceEngine.h \
    faceResultImage.h \
    faceVision.h \
    faceVapp.h

DISTFILES += \
    README \
    haarcascade_frontalface.xml

DEFINES +=

#Mandatory params
NXT_SDK = 2.1.0
AVATAR = avatar.png
MANIFEST = manifest.json
TRANSLATION = translation.json

#Optional params
LICENSE = license.txt

# Note: Paths have to be absolute
# add files within source directory which should be shipped (e.g. libraries, configs, ...)
DEPLOYFILES += $$PWD/qtlogging.ini
DEPLOYFILES += $$PWD/opencv/share/opencv4/haarcascades/haarcascade_frontalface_default.xml

# Note: Symlinks are not supported on Windows. Only deploy needed files.
# For Debbuging, all files (.so/.so.4/.so.4.4) have to be in the lib folder
# This example uses OpenCV 4.4.0. If you use an other version, update the corresponding libraries

DEPLOYFILES += $$PWD/opencv/lib/libopencv_core.so.4.4
DEPLOYFILES += $$PWD/opencv/lib/libopencv_objdetect.so.4.4
DEPLOYFILES += $$PWD/opencv/lib/libopencv_imgproc.so.4.4
DEPLOYFILES += $$PWD/opencv/lib/libopencv_calib3d.so.4.4
DEPLOYFILES += $$PWD/opencv/lib/libopencv_features2d.so.4.4
DEPLOYFILES += $$PWD/opencv/lib/libopencv_flann.so.4.4


#include opencv
INCLUDEPATH += $$PWD/opencv/include/opencv4
HEADERPATH += $$PWD/opencv/include/opencv4

LIBS += -L$$PWD/opencv/lib -lopencv_objdetect
LIBS += -L$$PWD/opencv/lib -lopencv_core
LIBS += -L$$PWD/opencv/lib -lopencv_imgproc
LIBS += -L$$PWD/opencv/lib -lopencv_calib3d
LIBS += -L$$PWD/opencv/lib -lopencv_features2d
LIBS += -L$$PWD/opencv/lib -lopencv_flann

include($(NXT_FRAMEWORK)/qmake/nxt_rio.pri)
