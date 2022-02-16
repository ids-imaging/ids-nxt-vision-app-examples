#include "myapp.h"
#include <cnnmanager_v2.h>
#include <QLoggingCategory>
#include <QFontDatabase>

static QLoggingCategory lc{ "Persondetector.app" };

using namespace IDS::NXT::CNNv2;
using namespace IDS::NXT;

MyApp::MyApp(int &argc, char **argv)
    : VApp{ argc, argv }
    , _engine{ _resultcollection }
{
    // at this point all result sources must be created.
    auto ressrc = _resultcollection.createSource("inferencetime", ResultType::String);
    ressrc->setUnit(Unit::Milliseconds);

    _resultcollection.createSource("persons", ResultType::Integer);

    // Add font to Qt. No font is shipped with the NXT
    QFontDatabase::addApplicationFont(VApp::vappAppDirectory() + "DejaVuSans.ttf");

    // Connect
    connect(&CnnManager::getInstance(), &CnnManager::installedCnnsChanged, this, &MyApp::installedCnnsChanged);
}

void MyApp::imageAvailable(std::shared_ptr<Hardware::Image> image)
{
    _engine.handleImage(image);
}

void MyApp::abortVision()
{
    _engine.abortVision();
}

void MyApp::installedCnnsChanged()
{
    qCDebug(lc) << "installedCnnsChanged";
    try {
        const auto cnns = CnnManager::getInstance().availableCnns();
        qCDebug(lc) << "Available CNNs:" << cnns << "Enable first one";

        CnnManager::getInstance().enableCnn(cnns.at(0), true);

    } catch (std::runtime_error &e) {
        qCritical(lc) << "Error:" << e.what();
    }
}
