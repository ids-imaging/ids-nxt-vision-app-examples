// Include the own header
#include "myengine.h"

// Include framework headers
#include <resultsource.h>
#include <QLoggingCategory>

// Include local headers
#include "myvision.h"

using namespace IDS::NXT::CNNv2;
using namespace IDS::NXT;

static QLoggingCategory lc{ "Persondetector.engine" };

static constexpr int DEFAULT_DETECTION_THRESHOLD = 50;

static const QString MODE_None = QString("None");
static const QString MODE_Pixel = QString("Pixel");
static const QString MODE_Black = QString("Black");

MyEngine::MyEngine(ResultSourceCollection &resultcollection)
    : _resultcollection{ resultcollection }
    , _detectionThreshold{ "detectionthreshold", DEFAULT_DETECTION_THRESHOLD }
    , _pixelmode{ "pixelation", QStringList{ MODE_None, MODE_Pixel, MODE_Black }, MODE_None }
    , _resultImage{ "resultimage" }
{
    // MyEngine constructor. The result collection (e.g. number of people detected) is forwarded by the MyApp object, the other object are defined within this class, such as
    // the result image (could be the image with detected persons pixelated),
    // and the configurables (e.g. a slider for the detection threshold or a drop down menu the pixelation mode).
    _detectionThreshold.setRange(0, 100);

    // The cnnChanged signal is needed here although we only work with a single CNN since it needs to be loaded once upon the Vision App start.
    connect(&CnnManager::getInstance(), &CnnManager::cnnChanged, this, &MyEngine::cnnChanged);
}

std::shared_ptr<Vision> MyEngine::factoryVision()
{
    // Simply construct a vision object, we may give further parameters, such as constant
    // parameters or shared (thread-safe!) objects.

    return std::make_shared<MyVision>();
}

void MyEngine::setupVision(std::shared_ptr<Vision> vision)
{
    // Here we set changing parameters, such as current configurable values
    if (vision) {
        auto obj = std::static_pointer_cast<MyVision>(vision);

        // initialize the CNN included in this example VApp. It is pretrained to detect persons.
        obj->setCnnData(_activeCnn);
        obj->setBoundingBoxProcessing(_currentTFLiteFiles);
        obj->setDetectionThreshold(static_cast<float>(_detectionThreshold) / 100);
    }
}

void MyEngine::handleResult(std::shared_ptr<Vision> vision)
{
    // In this function the result of the myvision object is processed
    // Get the finished vision object
    auto obj = std::static_pointer_cast<MyVision>(vision);

    try {
        // #DETECTION
        qCDebug(lc) << " Get MyVision data";
        // extracting the inference result of the CNN ...
        auto cnnResult = obj->result();
        // ... and some info regarding the cnn model
        auto cnnData = obj->cnnData();

        // Check if result is suited for detection
        if (cnnData.inferenceType() != CnnData::InferenceType::Detection) {
            throw std::runtime_error("CNN is not suited for detection");
        }

        // create custom result image with detections
        _resultImage.createOverlay(obj->image(), cnnResult, _pixelmode);

        // As an additional result we'd like to count the number of detected persons. The corresponding value is added to the result collection
        _resultcollection.addResult("persons", cnnResult.size(), "Number", vision->image());
        // adding the inference time to the result collection
        _resultcollection.addResult("inferencetime", obj->timeNeeded(), "Time", vision->image());
    } catch (const std::runtime_error &e) {
        qCCritical(lc) << "Error handling result: " << e.what();
    }

    // and declare the current image as processed. Now all results will be provided at the corresponding REST ressources by the camera's web server
    _resultcollection.finishedAllParts(obj->image());

    // Remove the image pointer from the vision object and thereby allow the framework to
    // reuse the image buffer.
    obj->setImage(nullptr);
}

void MyEngine::cnnChanged()
{
    // This function is needed to load the included person detection model upon the Vision App start
    qCDebug(lc) << "cnnChanged()";
    try {
        const auto activeCnns = CnnManager::getInstance().activeCnns();
        if (!activeCnns.isEmpty()) {
            // get the active CNN of the Deep Ocean Core
            _activeCnn = activeCnns.at(0);

            if (_activeCnn.inferenceType() == CnnData::InferenceType::Detection) {

                //... and the corresponding box regression TFLite model ...
                const auto currentTfLiteBox = _activeCnn.cnnDirectory() + "/Box.tflite";
                if (!QFile::exists(currentTfLiteBox)) {
                    qCCritical(lc) << "Box.tflite does not exist";
                    return;
                }

                //... and the corresponding box decoding TFLite model ...
                const auto currentTfLiteDecoder = _activeCnn.cnnDirectory() + "/Decoder.tflite";
                if (!QFile::exists(currentTfLiteDecoder)) {
                    qCCritical(lc) << "Decoder.tflite does not exist";
                    return;
                }
                _currentTFLiteFiles = { currentTfLiteBox, currentTfLiteDecoder };
            }
            return;
        } else {
            qCDebug(lc) << "No cnn available";
        }
    } catch (std::runtime_error &e) {
        Q_UNUSED(e)
    }
}
