#pragma once

// Include framework headers
#include "vision.h"
// Include Qt headers
#include <QRect>

/**
 * @brief The app-specific vision object
 */
class gvVision : public IDS::NXT::Vision
{
    // this Qt macro is needed for the signal/slot mechanism
    Q_OBJECT

public:
    /**
     * @brief Constructor
     */
    gvVision() = default;

    /**
     * @brief Start the image processing
     */
    void process() override;
    /**
     * @brief Abort the image processing
     */
    void abort() override;

    /**
     * @brief Getter for the gray value
     * @return gray value (0-255)
     */
    quint64 grayvalue() const;

    /**
     * @brief Setter for the ROI
     * @param roi The ROI QRect object
     */
    void setRoi(const QRect &roi);

private:
    /**
     * @brief last calculated gray value
     */
    quint64 _grayvalue;

    /**
     * @brief ROI object
     */
    QRect _roi;
};
