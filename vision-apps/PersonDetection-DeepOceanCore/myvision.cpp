// Include the own header
#include "myvision.h"
#include "cnnmanager_v2.h"
#include <stdlib.h>

#include <QImage>
#include <QLoggingCategory>

static QLoggingCategory lc{ "Persondetector.vision" };

using namespace IDS::NXT::CNNv2;

void MyVision::process()
{
    // This function takes care the processing of the current image by the Deep Ocean Core
    try {
        // Get the current image object
        auto img = image();
        qCDebug(lc) << "process " << img->key();

        if (_cnnData) { // check if deep ocean core is correctly initialized
            // #DETECTION
            const auto qImg = img->getQImage();
            // scale the image to the input size of the cnn
            auto scaledImage = qImg.scaled(_cnnData.inputSize(), Qt::IgnoreAspectRatio, Qt::FastTransformation);
            // process image with the CNN configured on the Deep Ocean Core
            auto buffer = _cnnData.processImage(scaledImage);

            // postprocesing with TfLite
            _result = _bbp.processDetections(buffer, qImg.size(), scaledImage.size(),
                                             _cnnData.classes(), _detectionThreshold);

            img->visionOK("", "");
        } else {
            // check if deep ocean core is correctly initialized
            img->visionFailed("Deep ocean core is not initialized", "Deep ocean core is not initialized");
        }
    } catch (std::exception &e) {
        qCCritical(lc) << "Error: " << e.what();
        image()->visionFailed("CNN evaluation failed", "CNN evaluation failed");
    }
}

void MyVision::abort()
{
    // Call the base class
    Vision::abort();
}

CnnData MyVision::cnnData() const
{
    return _cnnData;
}

void MyVision::setCnnData(const CnnData &cnnData)
{
    _cnnData = cnnData;
}

QList<TfLite::overlayData> MyVision::result()
{
    // we need to move the result because it is stored in an unique pointer
    return _result;
}

void MyVision::setBoundingBoxProcessing(const QPair<QString, QString> &TFLiteFiles)
{
    _bbp.setTFLiteFiles(TFLiteFiles);
}

float MyVision::detectionThreshold() const
{
    return _detectionThreshold;
}

void MyVision::setDetectionThreshold(float newDetectionThreshold)
{
    _detectionThreshold = newDetectionThreshold;
}
