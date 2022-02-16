#pragma once

// include std headers
#include <memory>
#include <vector>

// include Qt headers
#include <QImage>

// include OpenCV headers
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

// include NXT framework headers
#include "image.h"
#include "resultimage.h"

/**
 * @brief Specific ResultImage class
 */
class FaceResultImage : public IDS::NXT::ResultImage
{
    // IDS::NXT::ResultImage inherits from QObject, hence this Qt macro is needed here
    Q_OBJECT

public:
    /**
     * @brief C'tor
     * @param name Name of the ResultImage, must follow default guidelines for interaction element naming
     */
    FaceResultImage(const QByteArray &name);

    /**
     * @brief Update image data
     * @note This is called often and should be short
     * @see FaceResultImage::getImage
     * @param image Corresponding image
     * @param faces found faces
     * @param good true if any faces were found
     * @param scale scale of OpenCV processing
     */
    void update(std::shared_ptr<IDS::NXT::Hardware::Image> image, const std::vector<cv::Rect> &faces, bool good, double scale);

private:
    /**
     * @brief Getter for the image data
     * @note This is called less often than ::update, and may be longer (i.e. do the drawing here)
     * @return result image with marked faces
     */
    QImage getImage() const override;

    /// Local storage of the image
    QImage _image;
};
