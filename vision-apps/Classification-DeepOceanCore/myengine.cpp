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
        // extracting the inference result of the CNN ...
        auto cnnResult = obj->result();
        // ... and some info regarding the cnn model
        auto cnnData = obj->cnnData();

        // Check if result is valid
        if (!cnnResult) {
            throw std::runtime_error("CNN result not valid");
        }

        // Get the output buffers of the CNN
        const auto &allBuffers = cnnResult->allBuffers();

        // Check if result is suited for classification
        if (cnnData.inferenceType() != IDS::NXT::CNNv2::CnnData::InferenceType::Classification || allBuffers.size() != 1) {
            throw std::runtime_error(
                    "Error: CNN is not suited for classification or output buffer set is not specified correctly.");
        }

        // Get Deep Ocean Core processing time
        inferencetime = cnnResult->inferenceTime();

        // Convert result to double and map corresponding class;
        QList<QPair<QString, double>> result_classes;

        // Prealloc memory for speed improvement
        result_classes.reserve(cnnData.classes().size());
        double expSum = 0;

        // Classification cnns have only one output Buffer
        auto currentCnnOutputBuffer = allBuffers.at(0);

        // Extract all classes with classification probability from the CNN
        for (qint32 cnt = 0; cnt < cnnData.classes().size(); cnt++) {
            auto currentClass = cnnData.classes().at(cnt);
            // Apply exponential function for softmax calculation.
            //We need to cast the value to double before exp processing because the numbers can get huge, or tiny
            double currentVal = std::exp(static_cast<double>(currentCnnOutputBuffer.data[cnt]));

            // Sum up values for softmax dividor
            expSum += currentVal;
            result_classes.append(qMakePair(currentClass, currentVal));
        }
        // free cnn buffer because we don't need it anymore
        cnnResult = nullptr;

        // sort classes
        qSort(result_classes.begin(), result_classes.end(), ProbabilityComparer());

        // Make Top 0-4
        for (auto i = 0; (i < 4) && (i < cnnData.classes().size()); i++) {
            // Class
            _resultcollection.addResult("inference", (result_classes[i].first), "Top" + QString::number(i + 1), vision->image());
            // Propability with softmax creation
            _resultcollection.addResult("inference_propability", QString::number(result_classes[i].second / expSum, 'f', 2), "Top" + QString::number(i + 1), vision->image());
        }

        _resultcollection.addResult("inferencetime", inferencetime, "Time", vision->image());
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
