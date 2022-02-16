// Include the own header
#include "myvision.h"

// Include Qt headers
#include <QDateTime>
#include <QImage>

using namespace IDS::NXT;

void MyVision::process()
{
    auto img = image();

    _result.clear();
    for (auto roiIter = _rois.constBegin(); roiIter != _rois.constEnd(); ++roiIter) {
        // Initialize counters and variable
        qint64 gv_total = 0;
        qint64 gv_count = 0;

        // copy roi from camera image to the result image
        const auto roiImg = img->getQImage().copy(roiIter.value()->getQRect());
        // Interate over every fourth pixel
        for (auto x = 0; x < roiImg.width(); x += 4) {
            for (int y = 0; y < roiImg.height(); y += 4) {
                // Add the pixel's gray value to the sum variable
                gv_total += roiImg.pixelColor(x, y).lightness();
                // Increase the pixel counter
                gv_count++;
            }
        }

        // Calculate the average gray value
        auto gv = static_cast<qint64>(gv_total / gv_count);
        // Add result to list
        auto roiResult = QPair<std::shared_ptr<ConfigurableROI>, qint64>{ roiIter.value(), gv };
        _result.append(roiResult);
    }

    img->visionOK("", "");
}

void MyVision::abort()
{
    // Call the base class
    Vision::abort();
}

void MyVision::setROIs(const QMap<QString, std::shared_ptr<ConfigurableROI>> &rois)
{
    _rois = std::move(rois);
}

QList<QPair<std::shared_ptr<ConfigurableROI>, qint64>> MyVision::getResult()
{
    return _result;
}
