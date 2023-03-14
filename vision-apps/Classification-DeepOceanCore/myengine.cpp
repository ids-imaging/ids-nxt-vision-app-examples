// Include the own header
#include "myengine.h"

// Include framework headers
#include <resultsource.h>
#include <QLoggingCategory>
#include <cmath>

// Include local headers
#include "myvision.h"

using namespace IDS::NXT;
using namespace IDS::NXT::CNNv2;

static QLoggingCategory lc{ "classificationexample.engine" };

MyEngine::MyEngine(IDS::NXT::ResultSourceCollection &resultcollection)
    : _resultcollection{ resultcollection }
{
    // Connect cnnChanged signal of library to local slot
    connect(&IDS::NXT::CNNv2::CnnManager::getInstance(), &IDS::NXT::CNNv2::CnnManager::cnnChanged, this, &MyEngine::cnnChanged);
}

std::shared_ptr<IDS::NXT::Vision> MyEngine::factoryVision()
{
    // Simply construct a vision object, we may give further parameters, such as not-changing
    // parameters or shared (thread-safe!) objects.

    return std::make_shared<MyVision>();
}

void MyEngine::setupVision(std::shared_ptr<IDS::NXT::Vision> vision)
{
    // Here we could set changing parameters, such as current configurable values
    if (vision) {
        auto obj = std::static_pointer_cast<MyVision>(vision);

        // set current activated CNN because it can change during runtime.
        obj->setCnnData(_activeCnn);
    }
}

void MyEngine::handleResult(std::shared_ptr<IDS::NXT::Vision> vision)
{
    // Get the finished vision object
    auto obj = std::static_pointer_cast<MyVision>(vision);

    uint32_t inferencetime;

    try {
        // #CLASSIFICATION
        qCDebug(lc) << " Get MyVision data";
        // extracting the inference result of the CNN
        auto cnnResultList = obj->resultList();

        // Check if result is valid
        if (cnnResultList.isEmpty()) {
            throw std::runtime_error("CNN result not valid");
        }

        // Get Deep Ocean Core processing time
        inferencetime = cnnResultList.first().duration_ms;

        // Check if inference result contains roi result
        if (cnnResultList.first().roiResults.isEmpty()) {
            throw std::runtime_error("CNN result contains no roi result");
        }

        // Make Top 0-4
        for (auto i = 0; (i < 4) && (i < cnnResultList.first().roiResults.first().inferenceProbabilities.size()); i++) {
            // Class
            _resultcollection.addResult("inference", cnnResultList.first().roiResults.first().inferenceProbabilities.at(i).inferenceClass, "Top" + QString::number(i + 1), vision->image());
            // Propability
            _resultcollection.addResult("inference_propability", QString::number(cnnResultList.first().roiResults.first().inferenceProbabilities.at(i).probability, 'f', 2), "Top" + QString::number(i + 1), vision->image());
        }

        _resultcollection.addResult("inferencetime", inferencetime, QStringLiteral("Time"), vision->image());
    } catch (const std::runtime_error &e) {
        qCCritical(lc) << "Error handling result: " << e.what();
        _resultcollection.addResult("inference", e.what(), QStringLiteral("Content1"), vision->image());
    }

    // signal that all parts of the image are finished
    _resultcollection.finishedAllParts(obj->image());

    // Remove the image pointer from the vision object and thereby allow the framework to
    // reuse the image buffer.
    obj->setImage(nullptr);
}

void MyEngine::cnnChanged()
{
    // This function is needed to load the included cnn model upon the Vision App start
    qCDebug(lc) << "cnnChanged()";
    try {
        if (!CnnManager::getInstance().availableCnns().isEmpty()) {
            //Save cnnData to get information about the current cnn.
            _activeCnn = CnnManager::getInstance().activeCnns().at(0);
            return;
        } else {
            qCCritical(lc) << "No cnn available";
        }
    } catch (std::runtime_error &e) {
        qCCritical(lc) << e.what();
    }
}
