// Include the own header
#include "myvision.h"
#include "cnnmanager_v2.h"
#include <stdlib.h>

#include <QImage>
#include <QLoggingCategory>

using namespace IDS::NXT;

static QLoggingCategory lc{"MultiCNN.vision"};

void MyVision::process() {
    try {
        // Get the image data
        auto img = image();

        qCDebug(lc) << "process " << img->key();

        if (!_cnnData.empty()) { // check if deep ocean core is correct initialized
            for (auto& cnn : _cnnData) {
                if (cnn) {
                    // process image with deep ocean core.
                    // Scale the image to the input size of the cnn. If you don't scale it the NXT Framework will scale
                    // it which  can lower performance
                    _result.push_back(cnn.processImage(img->getQImage().scaled(cnn.inputSize(),
                                                                               Qt::IgnoreAspectRatio,
                                                                               Qt::FastTransformation),
                                                       QStringLiteral("Classification")));
                }
            }

            img->visionOK("", "");
        } else { // Deep ocean core is not initialized. This can happen if no cnn is ativated.
            image()->visionFailed("Deep ocean core is not initialized", "Deep ocean core is not initialized");
        }
    } catch (const std::exception& e) {
        qCCritical(lc) << "Error: " << e.what();
        image()->visionFailed("CNN evaluation failed", "CNN evaluation failed");
    }
}

void MyVision::abort() {
    // Call the base class
    Vision::abort();
}

QList<CNNv2::CnnData> MyVision::cnnData() const {
    return _cnnData;
}

void MyVision::setCnnData(const QList<CNNv2::CnnData>& cnnData) {
    _cnnData = cnnData;
}

std::vector<std::unique_ptr<CNNv2::MultiBuffer>> MyVision::result() {
    // we need to move the result because it is stored in an unique pointer.
    // To free the buffer destroy this object.
    return std::move(_result);
}
