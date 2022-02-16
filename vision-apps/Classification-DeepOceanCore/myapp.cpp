#include "myapp.h"
#include <cnnmanager_v2.h>
#include <QLoggingCategory>
#include <QFontDatabase>

using namespace IDS::NXT;
using namespace IDS::NXT::CNNv2;

static QLoggingCategory lc{ "classificationexample.app" };

MyApp::MyApp(int &argc, char **argv)
    : VApp{ argc, argv }, _engine{ _resultcollection }
{
    // #RESULT_HANDLING
    // At this point all result sources must be created.
    _resultcollection.createSource("inference", ResultType::String);
    _resultcollection.createSource("inference_propability", ResultType::String);
    auto infTime = _resultcollection.createSource("inferencetime", ResultType::String);
    infTime->setUnit(Unit::Milliseconds);

    // Connect installedCnnsChanged signal of library to local slot
    connect(&CnnManager::getInstance(), &CnnManager::installedCnnsChanged, this, &MyApp::installedCnnsChanged);
}

void MyApp::imageAvailable(std::shared_ptr<IDS::NXT::Hardware::Image> image)
{
    _engine.handleImage(image);
}

void MyApp::abortVision()
{
    _engine.abortVision();
}
// This slot is called if the number of installed cnns changed e.g. if an new one is installed.
void MyApp::installedCnnsChanged()
{
    qCDebug(lc) << "installedCnnsChanged";
    try {
        auto cnns = CnnManager::getInstance().availableCnns();
        qCDebug(lc) << "Available CNNs:" << cnns << "Enable first one";

        if (!cnns.empty()) {
            // We ship one cnn with this app and enable it now.
            // If the cnn is enabled successfully, the signal "CnnManager::cnnChanged()" is emitted
            // If an error occures, the signal "CnnManager::cnnError()" is emitted
            CnnManager::getInstance().enableCnn(cnns.at(0), true);
        }
    } catch (std::runtime_error &e) {
        qCritical(lc) << "Error:" << e.what();
    }
}
