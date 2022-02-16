#include "faceVision.h"
#include "color.h"

#include <QDateTime>
#include <QImage>

#include "opencv2/objdetect.hpp"

FaceVision::FaceVision(const cv::CascadeClassifier &cascade)
    : _cascade { cascade }
{
}

void FaceVision::process()
{
    // find faces with OpenCV
    // read image
    cv::Mat img;
    if (IDS::NXT::Hardware::Color::isAvailable()) {
        cv::Mat color(image()->height(), image()->width(), CV_8UC3, image()->buffer());
        cv::cvtColor(color, img, cv::COLOR_BGR2GRAY);
    } else {
        cv::Mat { image()->height(), image()->width(), CV_8UC1, image()->buffer() }.copyTo(img);
    }

    // resize the image
    auto fx = 1.0 / _scale;
    cv::Mat smallImg;
    cv::resize(img, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR);
    cv::equalizeHist(smallImg, smallImg);

    // detect faces of different sizes using cascade classifier
    _cascade.detectMultiScale(smallImg, _faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
}

void FaceVision::abort()
{
    // forward abort signal to base class
    Vision::abort();
}

std::vector<cv::Rect> FaceVision::faces() const
{
    return _faces;
}

void FaceVision::setScale(double s)
{
    _scale = s;
}
