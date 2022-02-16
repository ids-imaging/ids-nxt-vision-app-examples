// Include the own header
#include "gvvision.h"

// Include Qt headers
#include <QImage>
#include <QLoggingCategory>

using namespace IDS::NXT;

static QLoggingCategory lc{ "grayvalue.vision" };

void gvVision::process()
{
    // Get the image data
    auto img = image()->getQImage();

    // #ROI_PROCESSING
    // extract ROI from image
    auto roi_qimg = img.copy(_roi);

    // Calculate average gray value
    int stepSize = 32; // Pixel step size for calculating the gray value
    // handle too small ROI
    if (roi_qimg.width() < stepSize || roi_qimg.height() < stepSize) {
        stepSize = 1;
    }

    // Initialize counters and variable
    qint64 gv_total = 0;
    qint64 gv_count = 0;

    // Interate with stepSize over ROI image
    for (int x = 0; x < roi_qimg.width(); x += stepSize) {
        for (int y = 0; y < roi_qimg.height(); y += stepSize) {
            // Add the pixel's gray value to the sum variable
            gv_total += roi_qimg.pixelColor(x, y).lightness();
            // Increase the pixel counter
            gv_count++;
        }
    }

    // Calculate the average
    if (gv_count > 0) {
        _grayvalue = gv_total / gv_count;
        image()->visionOK("", "");
    } else // No values, prevents division through zero
    {
        _grayvalue = 0;
        image()->visionFailed("No values in roi", "No values in roi");
    }
}

void gvVision::abort()
{
    // Call the base class
    Vision::abort();
}

quint64 gvVision::grayvalue() const
{
    // Return the data
    return _grayvalue;
}

void gvVision::setRoi(const QRect &roi)
{
    _roi = roi;
}
