#include "myapp.h"
#include <cnnmanager_v2.h>
#include <QFontDatabase>
#include <QLoggingCategory>

static QLoggingCategory lc{"MultiCNN.app"};

using namespace IDS::NXT;

MyApp::MyApp(int& argc, char** argv)
  : VApp{argc, argv}
  , _engine{_resultcollection}
  , _cnnPackage{std::make_unique<ConfigurableFile>("cnnpackage", "cnn")}
  , _enableCnnAction{"enable"}
  , _disableCnnAction{"disable"}
  , _vappInfo{"cnninfo"} {
    // initialization
    QList<TranslatedText> filtercnn;
    filtercnn.append("IDS NXT rio/rome classification model |*.rcla; *.cnn");
    _cnnPackage->setFilter(filtercnn);
    _cnnPackage->setWritable(true);

    updateCnnFileDeletableProperty();

    const auto cnns = CNNv2::CnnManager::getInstance().availableCnns();
    _installedCnns = std::make_unique<ConfigurableEnum>("cnns", cnns, cnns.at(0));

    _vappInfo.setInfo(FrameworkApplication::manifest().getTranslatedText(QStringLiteral("Language.noCnnActive.Title")));

    // At this point all result sources must be created.
    _resultcollection.createSource("inference", ResultType::String);
    _resultcollection.createSource("inference_propability", ResultType::String);
    auto infTime = _resultcollection.createSource("inferencetime", ResultType::Integer);
    infTime->setUnit(Unit::Milliseconds);
    _resultcollection.createSource("cnn", ResultType::String);

    // Connect framework signals with VApp slots
    connect(&CNNv2::CnnManager::getInstance(),
            &CNNv2::CnnManager::installedCnnsChanged,
            this,
            &MyApp::installedCnnsChanged);
    connect(&CNNv2::CnnManager::getInstance(), &CNNv2::CnnManager::cnnChanged, this, &MyApp::cnnChanged);
    connect(_cnnPackage.get(), &ConfigurableFile::written, this, &MyApp::cnnfileWritten);
    connect(_cnnPackage.get(), &ConfigurableFile::deleted, this, &MyApp::deleteCurrentCnn);
    connect(&_enableCnnAction, &Action::executionDemanded, this, &MyApp::enableCnn);
    connect(&_disableCnnAction, &Action::executionDemanded, this, &MyApp::disableCnn);

    // Enable loading multiple CNNs
    CNNv2::CnnManager::getInstance().enableLoadingMultipleCnns(true);
}

void MyApp::imageAvailable(std::shared_ptr<Hardware::Image> image) {
    _engine.handleImage(image);
}

void MyApp::abortVision() {
    _engine.abortVision();
}

void MyApp::installedCnnsChanged() {
    qCDebug(lc) << "installedCnnsChanged";
    try {
        auto cnns = CNNv2::CnnManager::getInstance().availableCnns();
        qCDebug(lc) << "Available CNNs:" << cnns;

        _installedCnns->setValues(cnns);

        updateCnnFileDeletableProperty();
    } catch (std::runtime_error& e) {
        qCritical(lc) << "Error:" << e.what();
    }
}

void MyApp::enableCnn() {
    qCDebug(lc) << "enable" << _installedCnns->operator QString();
    CNNv2::CnnManager::getInstance().enableCnn(_installedCnns->operator QString(), true);
}

void MyApp::disableCnn() {
    qCDebug(lc) << "disable" << _installedCnns->operator QString();
    CNNv2::CnnManager::getInstance().enableCnn(_installedCnns->operator QString(), false);
}

void MyApp::cnnChanged() {
    qCDebug(lc) << "cnnChanged";
    const auto activeCnns = CNNv2::CnnManager::getInstance().activeCnns();

    if (activeCnns.isEmpty()) {
        _vappInfo.setInfo(
            FrameworkApplication::manifest().getTranslatedText(QStringLiteral("Language.noCnnActive.Title")));
    } else {
        static VAppInfoElement::Info info;

        QStringList cnns;
        cnns.reserve(activeCnns.size());
        for (const auto& cnn : activeCnns) {
            cnns.append(cnn.name());
        }
        const auto infoText = TranslatedText{
            QMap<QString, QString>{{"en", "<b>Active CNNs</b><br>• " + cnns.join(QStringLiteral("<br>• "))},
                                   {"de", "<b>Aktive CNNs</b><br>• " + cnns.join(QStringLiteral("<br>• "))}}};

        info.text = infoText;
        info.category = VAppInfoElement::InfoCategory::Info;

        _vappInfo.setInfo(info);
    }
}

void MyApp::cnnfileWritten() {
    std::lock_guard<std::mutex> lock(_installLock);

    CNNv2::CnnManager::getInstance().addCnn(_cnnPackage->absoluteFilePath());
}

void MyApp::deleteCurrentCnn() {
    try {
        CNNv2::CnnManager::getInstance().removeCnn(_installedCnns->operator QString());
    } catch (std::runtime_error& e) {
        qInfo(lc) << e.what();
    }
}

void MyApp::updateCnnFileDeletableProperty() {
    if (CNNv2::CnnManager::getInstance().availableCnns().size() > 1) {
        _cnnPackage->setDeletable(true);
    } else {
        _cnnPackage->setDeletable(false);
    }
}
