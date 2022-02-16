// Include the own header
#include "myengine.h"

// Include framework headers
#include <resultsource.h>
#include <signaler.h>

// Include local headers
#include "myvision.h"

// Include QT headers
#include <QLoggingCategory>
#include <QDateTime>

using namespace IDS::NXT;

// Declare logging category (lc). LCs can be activated/deactivated in "qtlogging.ini"
// The log messages can be read over REST at http://<YOUR_CAMERA_IP>/vapps/<VAPP_NAME>/log
static QLoggingCategory lc { "FtpExample.Engine" };

MyEngine::MyEngine(ResultSourceCollection &resultcollection)
    : _resultcollection { resultcollection }
    , _sendImageToFTP { "sendftpaction" }
{
    // initialization
    _sendImageToFTP.setDescription("If FTP is enabled, this button triggers the image transmission to the FTP server.");
    updateFtpAction();

    // connect interaction elements
    connect(&_ftpClient, &FTP::FTPClient::enabledChanged, this, &MyEngine::updateFtpAction);
    connect(&_sendImageToFTP, &Action::executionDemanded, this, &MyEngine::sendToFTP);
}

std::shared_ptr<Vision> MyEngine::factoryVision()
{
    // Simply construct a vision object, we may give further parameters, such as not-changing
    // parameters or shared (thread-safe!) objects.
    return std::make_shared<MyVision>();
}

void MyEngine::setupVision(std::shared_ptr<Vision> vision)
{
    // Here we could set changing parameters, such as current configurable values
    if (vision) {
        auto obj = std::static_pointer_cast<MyVision>(vision);
    }
}

void MyEngine::handleResult(std::shared_ptr<Vision> vision)
{
    // Get the finished vision object
    auto obj = std::static_pointer_cast<MyVision>(vision);

    // Store recent image in private buffer
    _recentImg = obj->image()->getQImage().copy();

    // Signal that all parts of the image are finished
    _resultcollection.finishedAllParts(obj->image());

    // Remove the image pointer from the vision object and thereby allow the framework to
    // reuse the image buffer.
    obj->setImage(nullptr);
}

void MyEngine::updateFtpAction()
{
    if (_ftpClient.enabled()) {
        _sendImageToFTP.setEnabled(true);
    } else {
        _sendImageToFTP.setEnabled(false);
    }
}

void MyEngine::sendToFTP()
{
    if (!_recentImg.isNull()) {
        qCInfo(lc) << "Sending image to FTP server.";
        const auto currentTime = QDateTime::currentDateTime().toString(u"MM_dd_yyyy__hh_mm_ss_zzz");
        const auto fileName = "FtpExampleVApp/" + currentTime + ".jpg"; // Directory "FtpExmpleVApp" is created automatically

        auto metaDataMap = QVariantMap {};
        metaDataMap.insert(QStringLiteral("StringKey"), "MyStringValue");
        metaDataMap.insert(QStringLiteral("IntKey"), 42);
        metaDataMap.insert(QStringLiteral("MapKey"), QMap<QString, QVariant> { { "key1", "value1" }, { "key2", "value2" } });

        try {
            _ftpClient.sendToFtp(_recentImg, fileName, metaDataMap);
        } catch (std::runtime_error &e) {
            qCCritical(lc) << "Can not send Image to FTP Server:" << e.what();
        }
    } else {
        qCInfo(lc) << "Can not send Image to FTP Server: No image aquired yet.";
        Signaler::getInstance().postSignal(QStringLiteral("FTP example VApp"), Signaler::Severity::Info,
                                           TranslatedText { "Transmission to FTP failed" },
                                           TranslatedText { "Could not send an image to the FTP server. No image aquired yet." });
    }
}
