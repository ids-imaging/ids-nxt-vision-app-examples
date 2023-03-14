// #RESULT_IMAGE
#include "customresultimage.h"

#include <QLoggingCategory>
#include <QPainter>
#include <QFont>

using namespace IDS::NXT;

static QLoggingCategory lc { "Persondetector.CustomResultImage" };

static constexpr int FONT_PIXEL_SIZE = 30;
static constexpr int PEN_WIDTH = 8;

static constexpr int FONT_PIXEL_SIZE_MINIMAL = 5;
static constexpr float FONT_BOX_SCALING_FACTOR = 0.8;

CustomResultImage::CustomResultImage(const QByteArray &name)
    : ResultImage(name)
{
}

void CustomResultImage::createOverlay(const std::shared_ptr<Hardware::Image> &image,
                                      const QList<InferenceResultProcessing::overlayData> &data, const QString &mode)
{
    static const QVector<QColor> colors { _idsBlueLight,
                                          _idsBlue,
                                          _idsGray,
                                          _idsGreen,
                                          _idsYellow,
                                          _idsOrange };

    if (image != nullptr) {
        const auto fullImage = image->getQImage();

        auto imageFormat = fullImage.format();

        if (imageFormat == QImage::Format_Mono or imageFormat == QImage::Format_Grayscale8) {
            imageFormat = QImage::Format_RGB888;
        }
        // Create empty image as base for drawing rects
        QImage outImage(fullImage.width(), fullImage.height(), imageFormat);
        try {
            // set font and fontsize for text in boxes
            QFont dejavu(QStringLiteral("DejaVuSans"));
            dejavu.setPixelSize(FONT_PIXEL_SIZE);

            QPainter painter;
            painter.begin(&outImage);
            // copy sensorimage
            painter.drawImage(0, 0, fullImage);
            auto pen = QPen();
            pen.setWidth(PEN_WIDTH);
            painter.setFont(dejavu);

            auto fontStartFont = painter.font();
            auto fontStartPixelSize = fontStartFont.pixelSize();

            if (!data.empty()) {
                if (mode != QLatin1String("None")) {
                    for (const auto &val : data) // Make persons unrecognizable
                    {
                        auto thisRoi = val.cnnRoi;
                        // fill box
                        if (mode == QLatin1String("Black")) {
                            painter.fillRect(thisRoi, QColor("black"));
                        } else if (mode == QLatin1String("Pixel")) {
                            auto cropped = outImage.copy(thisRoi);
                            auto scaled = cropped.scaled(16, 16);
                            painter.drawImage(thisRoi, scaled);
                        }
                    }
                }
                for (const auto &val : data) // draw box frames and text
                {
                    // Reset Font on every ROI
                    fontStartFont.setPixelSize(fontStartPixelSize);
                    painter.setFont(fontStartFont);

                    auto thisClass = val.inference.at(0).first;
                    auto thisProbability = QString::number(val.inference.at(0).second, 'f', 2).left(4); // limit double values to 2 decimal points
                    auto thisRoi = val.cnnRoi;

                    // class "error" is always red
                    QColor color;
                    if (thisClass.toLower() != QLatin1String("error")) {
                        color = QColor(colors.at(val.color % colors.size()));
                    } else {
                        color = _errorRed;
                    }

                    // draw box
                    pen.setColor(color);
                    painter.setPen(pen);
                    painter.drawRect(thisRoi); //Detected Box
                    // draw textbox
                    auto roiText = thisClass + " " + thisProbability;
                    auto textbox = painter.fontMetrics().boundingRect(roiText);
                    qCDebug(lc) << "Textbox original" << textbox << "Text" << roiText;

                    // calculate textbox size
                    while (thisRoi.width() < textbox.width()) {
                        auto f = painter.font();

                        qCDebug(lc) << f.pixelSize();

                        if (f.pixelSize() < FONT_PIXEL_SIZE_MINIMAL) {
                            qCDebug(lc) << "Font too small";
                            f.setPixelSize(static_cast<int>(static_cast<float>(FONT_PIXEL_SIZE_MINIMAL)));
                            textbox = painter.fontMetrics().boundingRect(roiText);
                            break;
                        }

                        f.setPixelSize(static_cast<int>(static_cast<float>(f.pixelSize()) * FONT_BOX_SCALING_FACTOR));

                        qCDebug(lc) << f.pixelSize();
                        painter.setFont(f);

                        textbox = painter.fontMetrics().boundingRect(roiText);
                    }

                    if (thisRoi.y() >= textbox.height()) {
                        textbox = QRect(thisRoi.x(), thisRoi.y() - textbox.height(), textbox.width(), textbox.height());
                    } else {
                        textbox = QRect(thisRoi.x(), thisRoi.y() + thisRoi.height() - textbox.height(), textbox.width(), textbox.height());
                    }

                    //move textbox to other side if box crosses image border
                    if (!fullImage.rect().contains(textbox, true)) {
                        if (textbox.y() > fullImage.rect().y() / 2) {
                            textbox = QRect(thisRoi.x(), thisRoi.y() + thisRoi.height() - textbox.height(), textbox.width(), textbox.height());
                        } else {
                            textbox = QRect(thisRoi.x(), thisRoi.y() - thisRoi.height() + textbox.height(), textbox.width(), textbox.height());
                        }
                    }
                    // calculate textbox size
                    painter.fillRect(textbox, color);
                    pen.setColor(QColor("black"));
                    painter.setPen(pen);
                    painter.drawText(textbox, Qt::AlignLeft, thisClass);
                    painter.drawText(textbox, Qt::AlignRight, thisProbability);
                }
            }
            // draw boxes to overlay
            painter.end();

            _image = outImage;
        } catch (...) {
            qCDebug(lc) << "Drawing failed.";
        }

        setModified(image->key());
    } else {
        setModified(QLatin1String(""));
    }
}

QImage CustomResultImage::getImage() const
{
    if (!_image.isNull()) {
        return _image;
    }

    return QImage {};
}
