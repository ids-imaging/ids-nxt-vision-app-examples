#include "myapp.h"

#include "signaler.h"

// Include QT headers
#include <QLoggingCategory>

using namespace IDS::NXT;

// Declare logging category (lc). LCs can be activated/deactivated in "qtlogging.ini"
// The log messages can be read over REST at http://<YOUR_CAMERA_IP>/vapps/<VAPP_NAME>/log
static QLoggingCategory lc{ "BasicExampleVApp.App" };

MyApp::MyApp(int &argc, char **argv)
    : VApp{ argc, argv }
    , _engine{ _resultcollection }
    , _writeLogMessage{ std::make_shared<Action>("logmessageaction") }
{
    // initialization. Set the description of the action element (Button in NXT-Cockpit)
    _writeLogMessage->setDescription("Clicking on this button writes an example message to the system log.");

    // #SIGNAL_CONNECTION
    // Connect the signal of the action object to a VApp method
    connect(_writeLogMessage.get(), &Action::executionDemanded, this, &MyApp::writeLogMessage);

    // #RESULT_HANDLING
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

void MyApp::writeLogMessage()
{
    // #SYSTEM_LOG
    // The system log can be read with the NXT Cockpit or via REST at http://<YOUR_CAMERA_IP>/signals/list
    // In this example, the "TranslatedText" is read from the file "translation.json".
    // This is optional, it is also possible to hand over the log message directly as string.
    Signaler::getInstance().postSignal("VAppBasic",
                                       Signaler::Severity::Info,
                                       FrameworkApplication::manifest().getTranslatedText("Language.examplelogmessage.Brief"),
                                       FrameworkApplication::manifest().getTranslatedText("Language.examplelogmessage.Description"));
}
