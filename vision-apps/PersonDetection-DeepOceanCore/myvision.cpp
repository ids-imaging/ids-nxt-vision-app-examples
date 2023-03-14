// Include the own header
#include "myvision.h"
#include "cnnmanager_v2.h"
#include "libutils_cnnhelper.h"

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

            // process image with the CNN configured on the Deep Ocean Core
            QList<IDS::NXT::CnnHelper::NamedRoi> rois;
            rois.append(IDS::NXT::CnnHelper::NamedRoi { "full", qImg.rect() });

            IDS::NXT::CnnHelper::InferenceParameters inferenceParameters;
            inferenceParameters.threshold = _detectionThreshold;
            inferenceParameters.enableHeatMap = false;
            inferenceParameters.transformationMode = IDS::NXT::CnnHelper::TransformationMode::FastTransformation;

            const auto result = _cnnData.processImage(qImg, rois, inferenceParameters);

            _result = InferenceResultProcessing::createResult(result.first(), result.first().inputRoi.roi.size(), cnnData().classes());

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

QList<InferenceResultProcessing::overlayData> MyVision::result()
{
    // we need to move the result because it is stored in an unique pointer
    return _result;
}

double MyVision::detectionThreshold() const
{
    return _detectionThreshold;
}

void MyVision::setDetectionThreshold(double newDetectionThreshold)
{
    _detectionThreshold = newDetectionThreshold;
}
