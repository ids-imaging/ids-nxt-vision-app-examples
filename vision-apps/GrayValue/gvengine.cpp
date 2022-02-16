// Include the own header
#include "gvengine.h"

// Include framework headers
#include "resultsource.h"

// Include local headers
#include "gvvision.h"

#include <QLoggingCategory>

using namespace IDS::NXT;

static QLoggingCategory lc{ "grayvalue.engine" };

gvEngine::gvEngine(ResultSourceCollection &resultcollection)
    : _resultcollection{ resultcollection }
    , _threshold{ "threshold", 128 } // Setup the threshold configurable with identifier and default value
    , _roi{ "roi" } // Setup the ROI with identifier
{
    // Setup further settings for the threshold configurable
    _threshold.setRange(0, 255);
    _threshold.setRange("RangeWarning0", 200, 255);
    // Connect a change event of the configurable to the slot
    connect(&_threshold, &ConfigurableInt64::changed, this, &gvEngine::onThresholdChanged);

    // Call the slot to handle the current value of threshold
    onThresholdChanged(_threshold);
}

std::shared_ptr<Vision> gvEngine::factoryVision()
{
    // Simply construct a vision object, we may give further parameters, such as not-changing
    // parameters or shared (thread-safe!) objects.
    return std::make_shared<gvVision>();
}

void gvEngine::setupVision(std::shared_ptr<Vision> vision)
{
    // Here we could set changing parameters, such as current configurable values
    if (vision) {
        auto obj = std::static_pointer_cast<gvVision>(vision);
        // Passing the ROI to the Vision object
        obj->setRoi(_roi.getQRect());
    }
}

void gvEngine::handleResult(std::shared_ptr<Vision> vision)
{
    // Get the finished vision object
    auto obj = std::static_pointer_cast<gvVision>(vision);

    // Do further processing, decide if gray value is higher than the set threshold
    const auto bright = (obj->grayvalue() > _thresholdValue);

    // Add all results to the collection
    // #RESULT_HIGHLIGHTING
    Resultformat format;
    format.setHighlight(true);
    format.setTextColor("black");
    if (bright) {
        format.setBackgroundColor("Green");
        _resultcollection.addResult("bright", bright, "Code#0", obj->image(), format);
    } else {
        format.setBackgroundColor("red");
        _resultcollection.addResult("bright", bright, "Code#0", obj->image(), format);
    }
    _resultcollection.addResult("grayvalue", obj->grayvalue(), "Code#0", obj->image());

    // Signal that all parts of the image are finished
    _resultcollection.finishedAllParts(obj->image());

    // Remove the image pointer from the vision object and thereby allow the framework to
    // reuse the image buffer.
    obj->setImage(nullptr);
}

void gvEngine::onThresholdChanged(quint64 value)
{
    qCDebug(lc) << "Threshold changed to " << value;
    _thresholdValue = value;
}
