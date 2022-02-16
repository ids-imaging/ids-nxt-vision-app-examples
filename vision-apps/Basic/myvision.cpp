// Include the own header
#include "myvision.h"

// Include Qt headers
#include <QDateTime>
#include <QImage>
#include <QLoggingCategory>

// Declare logging category (lc). LCs can be activated/deactivated in "qtlogging.ini"
// The log messages can be read over REST at http://<YOUR_CAMERA_IP>/vapps/<VAPP_NAME>/log
static QLoggingCategory lc{ "BasicExampleVApp.Vision" };

using namespace IDS::NXT;

void MyVision::process()
{
    // get camera image
    auto img = image();

    try {
        // copy roi from camera image to the result image
        _resultImage = img->getQImage().copy(_roiRect);
        // Tell the framework that the image processing is done so the image buffer can be reused
        img->visionOK("", "");
    } catch (...) {
        qCCritical(lc) << "Vision failed, something went wrong.";
        // Tell the framework that the image processing is done so the image buffer can be reused
        img->visionFailed("Vision failed", "Something went wrong.");
    }
}

void MyVision::abort()
{
    // Call the base class
    Vision::abort();
}

void MyVision::setRoi(const QRect &roiRect)
{
    _roiRect = roiRect;
}

QImage MyVision::getResultImage() const
{
    return _resultImage;
}
