// include the local header
#include "faceResultImage.h"

// include Qt headers
#include <QPainter>

using namespace IDS::NXT;

FaceResultImage::FaceResultImage(const QByteArray &name)
    : ResultImage(name)
{
}

QImage FaceResultImage::getImage() const
{
    return _image;
}

void FaceResultImage::update(std::shared_ptr<Hardware::Image> image, const std::vector<cv::Rect> &faces, bool good, double scale)
{
    if (!faces.empty()) {
        // get the current image and convert it to RGB32
        _image = image->getQImage().convertToFormat(QImage::Format_RGB32);

        // setup a QPainter to draw the found faces later
        QPainter painter;
        painter.begin(&_image);

        // setup a QPen which defines the color and width of the later drawn rectangle
        auto pen = QPen(Qt::green);
        pen.setWidth(5);

        // iterate over all faces in the vector
        for (const auto &face : faces) {
            if (painter.isActive() && good) {
                // add the pen to the painter
                painter.setPen(pen);

                // draw the rectangle on the found position
                painter.drawRect(face.x * scale, face.y * scale, face.width * scale, face.height * scale);
            }
        }

        painter.end();
    } else {
        _image = image->getQImage().convertToFormat(QImage::Format_RGB32);
    }

    setModified(image->key());
}
