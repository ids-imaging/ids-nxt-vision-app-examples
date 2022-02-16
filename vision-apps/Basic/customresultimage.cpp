// #RESULT_IMAGE
#include "customresultimage.h"

using namespace IDS::NXT;

CustomResultImage::CustomResultImage(const QByteArray &name)
    : ResultImage(name)
{
}

QImage CustomResultImage::getImage() const
{
    if (!_image.isNull()) {
        return _image;
    }

    return QImage{};
}

void CustomResultImage::setImage(const QImage &image, const std::shared_ptr<Hardware::Image> &nxtImage)
{
    _image = image;

    setModified(nxtImage->key());
}
