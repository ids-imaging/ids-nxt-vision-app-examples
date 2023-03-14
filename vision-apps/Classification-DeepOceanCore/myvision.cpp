// Include the own header
#include "myvision.h"
#include "cnnmanager_v2.h"
#include <stdlib.h>

#include <QImage>
#include <QLoggingCategory>

using namespace IDS::NXT::CNNv2;

static QLoggingCategory lc { "classificationexample.vision" };

void MyVision::process()
{
    try {
        // Get the image data
        const auto img = image();
        qCDebug(lc) << "process " << img->key();

        if (_cnnData) { // check if deep ocean core is correct initialized
            // #CLASSIFICATION

            // process full image
            QList<IDS::NXT::CnnHelper::NamedRoi> rois { { QStringLiteral("full"), QRect(QPoint(0, 0), img->imageSize()) } };

            // process image with deep ocean core.
            IDS::NXT::CnnHelper::InferenceParameters inferenceParameters;
            inferenceParameters.threshold = 0.0;
            inferenceParameters.enableHeatMap = false;
            inferenceParameters.transformationMode = IDS::NXT::CnnHelper::TransformationMode::FastTransformation;
            _resultList = _cnnData.processImage(img, rois, inferenceParameters);

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

IDS::NXT::CnnHelper::InferenceResultList MyVision::resultList()
{
    return _resultList;
}
