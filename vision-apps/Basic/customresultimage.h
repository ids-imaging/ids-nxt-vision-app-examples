// #RESULT_IMAGE
#pragma once
#include <QImage>

#include "image.h"
#include "resultimage.h"

class CustomResultImage : public IDS::NXT::ResultImage
{
public:
    CustomResultImage(const QByteArray &name);

    QImage getImage() const override;

    void setImage(const QImage &image, const std::shared_ptr<IDS::NXT::Hardware::Image> &nxtImage);

private:
    QImage _image;
};
