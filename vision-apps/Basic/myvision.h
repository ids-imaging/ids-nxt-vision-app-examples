#ifndef MYVISION_H
#define MYVISION_H

// Include Qt headers
#include <QRect>
#include <QImage>

// Include framework headers
#include <vision.h>

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

    /**
     * @brief Setter for the roi rect
     */
    void setRoi(const QRect &roiRect);

    /**
     * @brief Getter for the result image
     */
    QImage getResultImage() const;

private:
    QRect _roiRect;
    QImage _resultImage;
};

#endif // MYVISION_H
