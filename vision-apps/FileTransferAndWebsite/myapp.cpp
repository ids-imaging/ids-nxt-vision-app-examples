#include "myapp.h"

// Include QT headers
#include <QLoggingCategory>

using namespace IDS::NXT;

// Declare logging category (lc). LCs can be activated/deactivated in "qtlogging.ini"
// The log messages can be read over REST at http://<YOUR_CAMERA_IP>/vapps/<VAPP_NAME>/log
static QLoggingCategory lc { "FtpExample.App" };

MyApp::MyApp(int &argc, char **argv)
    : VApp { argc, argv }, _engine { _resultcollection }
{
    // at this point all result sources must be created.
    _resultcollection.createSource("visionduration", ResultType::String);

    qCInfo(lc) << "Result sources created.";
}

void MyApp::imageAvailable(std::shared_ptr<Hardware::Image> image)
{
    _engine.handleImage(image);
}

void MyApp::abortVision()
{
    _engine.abortVision();
}
