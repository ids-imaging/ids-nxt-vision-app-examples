#ifndef MYVISION_H
#define MYVISION_H
// Include framework headers
#include <vision.h>
#include <cnnmanager_v2.h>

#include <QImage>

#include "tflite.h"

/**
 * @brief The app-specific vision object
 */
class MyVision : public IDS::NXT::Vision
{
    // this Qt macro is needed for the signal/slot mechanism
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    MyVision() = default;

    /**
     * @brief Start the image processing
     */
    void process() override;
    /**
     * @brief Abort the image processing
     */
    void abort() override;

    // getter and setter for current processing
    QList<TfLite::overlayData> result();

    IDS::NXT::CNNv2::CnnData cnnData() const;
    void setCnnData(const IDS::NXT::CNNv2::CnnData &cnnData);

    void setBoundingBoxProcessing(const QPair<QString, QString> &TFLiteFiles);

    float detectionThreshold() const;
    void setDetectionThreshold(float newDetectionThreshold);

private:
    QList<TfLite::overlayData> _result;

    IDS::NXT::CNNv2::CnnData _cnnData;
    BoundingBoxesProcessing _bbp;
    float _detectionThreshold;
};

#endif // MYVISION_H
