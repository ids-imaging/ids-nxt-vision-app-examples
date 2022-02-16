// Include the own header
#include "myvision.h"
#include "cnnmanager_v2.h"
#include <stdlib.h>

#include <QImage>
#include <QLoggingCategory>

using namespace IDS::NXT::CNNv2;

static QLoggingCategory lc{ "classificationexample.vision" };

void MyVision::process()
{
    try {
        // Get the image data
        const auto img = image();
        qCDebug(lc) << "process " << img->key();

        if (_cnnData) { // check if deep ocean core is correct initialized
            // #CLASSIFICATION
            // process image with deep ocean core.
            // Scale the image to the input size of the cnn. If you don't scale it the NXT Framework will scale it which  can lower performance
            _result = _cnnData.processImage(img->getQImage().scaled(_cnnData.inputSize(), Qt::IgnoreAspectRatio, Qt::FastTransformation));

            img->visionOK("", "");
        } else { // Deep ocean core is not initialized. This can happen if no cnn is ativated.
            image()->visionFailed("Deep ocean core is not initialized", "Deep ocean core is not initialized");
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

IDS::NXT::CNNv2::CnnData MyVision::cnnData() const
{
    return _cnnData;
}

void MyVision::setCnnData(const CnnData &cnnData)
{
    _cnnData = cnnData;
}

std::unique_ptr<MultiBuffer> MyVision::result()
{
    // we need to move the result because it is stored in an unique pointer.
    // To free the buffer destroy this object.
    return std::move(_result);
}
