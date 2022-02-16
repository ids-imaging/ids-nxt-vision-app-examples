// Include the own header
#include "myengine.h"

// Include framework headers
#include <resultsource.h>
#include <trigger.h>
#include <triggertype.h>
#include <color.h>

// Include local headers
#include "myvision.h"

using namespace IDS::NXT;

// Include QT headers
#include <QLoggingCategory>

// Declare logging category (lc). LCs can be activated/deactivated in "qtlogging.ini"
// The log messages can be read over REST at http://<YOUR_CAMERA_IP>/vapps/<VAPP_NAME>/log
static QLoggingCategory lc { "BasicExampleVApp.Engine" };

MyEngine::MyEngine(ResultSourceCollection &resultcollection)
    : _resultcollection { resultcollection }
    , _roi { "myroi" }
    , _resultImage { "default" }
    , _enum { "myenum", { "Hardware", "Software", "Freerun" }, "Freerun" }
    , _triggerButton { "triggerbutton", false }
{
    // initialization
    synchronizeEnumWithTriggerMode();
    _triggerButton.setDescription("If the trigger mode is set to software, this button triggers the image aquisition.");
    if (Hardware::Trigger::getInstance().triggerType().source() == TriggerType::Sources::Software) {
        _triggerButton.setEnabled(true);
    } else {
        _triggerButton.setEnabled(false);
    }

    if (Hardware::Color::isAvailable()) {
        // Initialize slider
        _slider = std::make_unique<IDS::NXT::ConfigurableInt64>("myslider");
        _slider->setUnit(Unit::Percent);
        _slider->setRange(0, 100);
        // Connect signals regarding the red gain
        connect(_slider.get(), &ConfigurableInt64::changed, this, &MyEngine::setRedGain);
        connect(&Hardware::Color::getInstance(), &Hardware::Color::changed, this, &MyEngine::synchronizeSliderWithRedGain);
        synchronizeSliderWithRedGain();
    } else {
        _slider = nullptr;
    }

    // #SIGNAL_CONNECTION
    // connect interaction elements
    connect(&_enum, &ConfigurableEnum::changed, this, &MyEngine::changeTriggerMode);
    connect(&Hardware::Trigger::getInstance(), &Hardware::Trigger::changed, this, &MyEngine::synchronizeEnumWithTriggerMode);
    connect(&_triggerButton, &Action::executionDemanded, this, &MyEngine::triggerImageAquisition);
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
        obj->setRoi(_roi.getQRect());
    }
}

void MyEngine::handleResult(std::shared_ptr<Vision> vision)
{
    // Get the finished vision object
    auto obj = std::static_pointer_cast<MyVision>(vision);

    // #RESULT_IMAGE
    // Get the result image
    _resultImage.setImage(obj->getResultImage(), obj->image());

    // #RESULT_HANDLING
    // Fill ResultSources of the collection
    _resultcollection.addResult("visionduration", QString::number(obj->timeNeeded()), "resultPart", vision->image());

    // Signal that all parts of the image are finished
    _resultcollection.finishedAllParts(obj->image());

    // Remove the image pointer from the vision object and thereby allow the framework to
    // reuse the image buffer.
    obj->setImage(nullptr);
}

void MyEngine::changeTriggerMode(const QString &newMode)
{
    // #CAMERA_PARAMS
    // set camera trigger mode and enable/disable trigger button
    if (newMode == "Hardware") {
        Hardware::Trigger::getInstance().setTriggerType(TriggerType::Hardware);
        _triggerButton.setEnabled(false);
    } else if (newMode == "Software") {
        Hardware::Trigger::getInstance().setTriggerType(TriggerType::Software);
        _triggerButton.setEnabled(true);
    } else if (newMode == "Freerun") {
        Hardware::Trigger::getInstance().setTriggerType(TriggerType::Freerun);
        _triggerButton.setEnabled(false);
    }

    qCInfo(lc) << "Changed trigger mode to:" << newMode;
}

void MyEngine::triggerImageAquisition()
{
    Hardware::Trigger::getInstance().trigger();
}

void MyEngine::setRedGain(qint64 redGain)
{
    // #CAMERA_PARAMS
    Hardware::Color::getInstance().setRedGain(redGain);
}

void MyEngine::synchronizeEnumWithTriggerMode()
{
    // synchronize the enum with the current trigger mode
    switch (Hardware::Trigger::getInstance().triggerType().source()) {
    case TriggerType::Sources::Freerun:
        _enum = QString { "Freerun" };
        break;
    case TriggerType::Sources::Hardware:
        _enum = QString { "Hardware" };
        break;
    case TriggerType::Sources::Software:
        _enum = QString { "Software" };
        break;
    default:
        _enum = QString { "Freerun" };
    }
}

void MyEngine::synchronizeSliderWithRedGain()
{
    // synchronize the slider with the current gain value
    _slider->operator=(Hardware::Color::getInstance().redGain());
}
