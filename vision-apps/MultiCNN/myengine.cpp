// Include the own header
#include "myengine.h"

// Include framework headers
#include <resultsource.h>
#include <QLoggingCategory>
#include <cmath>

// Include local headers
#include "myvision.h"

using namespace IDS::NXT;

static QLoggingCategory lc{ "MultiCNN.engine" };

MyEngine::MyEngine(ResultSourceCollection &resultcollection)
    : _resultcollection{ resultcollection }
{
    // Connect cnnChanged signal of the framework to local slot
    connect(&CNNv2::CnnManager::getInstance(), &CNNv2::CnnManager::cnnChanged, this, &MyEngine::cnnChanged);
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

        // set current activated CNN because it can change during runtime.
        obj->setCnnData(_activeCnns);
    }
}

void MyEngine::handleResult(std::shared_ptr<Vision> vision)
{
    // Get the finished vision object
    auto obj = std::static_pointer_cast<MyVision>(vision);

    QList<uint32_t> inferencetime;

    try {
        qCDebug(lc) << " Get MyVision data";
        // extracting the inference result of the CNN ...
        auto allCnnResult = obj->result();
        // ... and some info regarding the cnn model
        auto allCnnData = obj->cnnData();

        if (allCnnResult.size() != static_cast<quint32>(allCnnData.size())) {
            throw std::runtime_error("CNN result not valid");
        }

        for (quint32 i = 0; i < allCnnResult.size(); i++) {
            // Check if result is valid
            if (!allCnnResult[i]) {
                throw std::runtime_error("CNN result not valid");
            }

            // Check if result is suited for classification
            if (allCnnData[i].inferenceType() != CNNv2::CnnData::InferenceType::Classification) {
                throw std::runtime_error("CNN is not suited for classification");
            }

            // Get Deep Ocean Core processing time
            inferencetime.append(allCnnResult[i]->inferenceTime());

            // Convert result to double and map corresponding class;
            QList<QPair<QString, double>> result_classes;

            // Prealloc memory for speed improvement
            result_classes.reserve(allCnnData[i].classes().size());

            // Classification cnns have only one output Buffer
            auto currentCnnOutputBuffer = allCnnResult[i]->allBuffers().at(0);

            const auto currentCnnClasses = allCnnData[i].classes();

            double expSum = 0;
            for (qint32 cnt = 0; cnt < currentCnnClasses.size(); cnt++) {
                auto currentClass = currentCnnClasses.at(cnt);
                // Apply exponential function for softmax calculation;
                double currentVal = std::exp(static_cast<double>(currentCnnOutputBuffer.data[cnt]));

                // Sum up values for softmax dividor
                expSum += currentVal;
                result_classes.append(qMakePair(currentClass, currentVal));
            }
            // free cnn buffer because we don't need it anymore
            allCnnResult[i] = nullptr;

            // sort classes
            qSort(result_classes.begin(), result_classes.end(), CNNv2::ProbabilityComparer());

            // calculate probability
            result_classes[0].second /= expSum;

            // add result to collection
            _resultcollection.addResult("inference", result_classes[0].first, QStringLiteral("CNN%1").arg(i + 1), vision->image());
            _resultcollection.addResult("inference_propability", QString::number(result_classes[0].second, 'f', 2), QStringLiteral("CNN%1").arg(i + 1), vision->image());
            _resultcollection.addResult("inferencetime", inferencetime[i], QStringLiteral("CNN%1").arg(i + 1), vision->image());
            _resultcollection.addResult("cnn", allCnnData[i].name(), QStringLiteral("CNN%1").arg(i + 1), vision->image());
        }
    } catch (const std::runtime_error &e) {
        qCCritical(lc) << "Error handling result: " << e.what();
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
        if (!CNNv2::CnnManager::getInstance().availableCnns().isEmpty()) {
            const auto activeCnns = CNNv2::CnnManager::getInstance().activeCnns();
            if (activeCnns != _activeCnns) {
                _activeCnns = activeCnns;
            }
        } else {
            qCCritical(lc) << "No cnn available";
            _activeCnns.clear();
        }
    } catch (std::runtime_error &e) {
        qCCritical(lc) << e.what();
    }
}
