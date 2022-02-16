// Include the own header
#include "myengine.h"

// Include framework headers
#include <resultsource.h>

// Include local headers
#include "myvision.h"

using namespace IDS::NXT;

MyEngine::MyEngine(ResultSourceCollection &resultcollection)
    : _resultcollection { resultcollection }
{
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
        obj->setROIs(_roiManager.managedROIs());
    }
}

void MyEngine::handleResult(std::shared_ptr<Vision> vision)
{
    // Get the finished vision object
    auto obj = std::static_pointer_cast<MyVision>(vision);

    // Get the result from the vision
    const auto results = obj->getResult();

    // Fill ResultSources of the collection
    _resultcollection.addResult("visionduration", QString::number(obj->timeNeeded()), QStringLiteral("resultPart"), vision->image());
    QString grayValues;
    for (const auto &result : results) {
        grayValues += result.first->name() + ": " + QString::number(result.second);
        if (&result != &results.back()) {
            grayValues += ",  ";
        }
    }
    _resultcollection.addResult("grayvalues", grayValues, QStringLiteral("value"), vision->image());

    // Signal that all parts of the image are finished
    _resultcollection.finishedAllParts(obj->image());

    // Remove the image pointer from the vision object and thereby allow the framework to
    // reuse the image buffer.
    obj->setImage(nullptr);
}
