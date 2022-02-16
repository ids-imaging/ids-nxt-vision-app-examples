// Include the own header
#include "myvision.h"

// Include Qt headers
#include <QLoggingCategory>

// Declare logging category (lc). LCs can be activated/deactivated in "qtlogging.ini"
// The log messages can be read over REST at http://<YOUR_CAMERA_IP>/vapps/<VAPP_NAME>/log
static QLoggingCategory lc { "FtpExample.Vision" };

void MyVision::process()
{
    // get camera image
    auto img = image();

    try {
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
