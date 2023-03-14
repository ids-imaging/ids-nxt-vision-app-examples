#ifndef CUSTOMRESULTIMAGE_H
#define CUSTOMRESULTIMAGE_H

#include <QImage>

#include "image.h"
#include "resultimage.h"

#include "inferenceresultprocessing.h"

/**
  * Creates an resultimage with custom overlay
**/
class CustomResultImage : public IDS::NXT::ResultImage
{
public:
    CustomResultImage(const QByteArray &name);
    // Create Overlay with detected objects
    void createOverlay(const std::shared_ptr<IDS::NXT::Hardware::Image> &image,
                       const QList<InferenceResultProcessing::overlayData> &data, const QString &mode);

    QImage getImage() const override;

private:
    QImage _image;

    const QColor _idsBlueLight = QColor(119, 203, 210);
    const QColor _idsBlue = QColor(0, 138, 150);
    const QColor _idsGreen = QColor(81, 192, 119);
    const QColor _idsGray = QColor(202, 202, 201);
    const QColor _idsYellow = QColor(255, 204, 96);
    const QColor _idsOrange = QColor(255, 173, 100);
    const QColor _errorRed = QColor("red");
};

#endif // CUSTOMRESULTIMAGE_H
