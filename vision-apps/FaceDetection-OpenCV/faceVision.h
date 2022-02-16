#pragma once

// Include framework headers
#include "vision.h"

// include std headers
#include <vector>

// include OpenCV headers
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

/**
 * @brief The app-specific vision object
 */
class FaceVision : public IDS::NXT::Vision
{
    // this Qt macro is needed for the signal/slot mechanism
    Q_OBJECT

public:
    /**
     * @brief C'tor
     * @param cascade
     */
    FaceVision(const cv::CascadeClassifier &cascade);

    /**
     * @see IDS::NXT::Vision::process
     */
    void process() override;
    /**
     * @see IDS::NXT::Vision::abort
     */
    void abort() override;

    /**
     * @brief Getter for the found faces
     * @return found faces
     */
    std::vector<cv::Rect> faces() const;

    /**
     * @brief Setter for the OpenCV detection scaling
     * @param s scaling factor
     */
    void setScale(double s);

private:
    /// Found faces
    std::vector<cv::Rect> _faces;

    /// OpenCV cascade classifier
    cv::CascadeClassifier _cascade;

    /// OpenCV detection scaling
    double _scale;
};
