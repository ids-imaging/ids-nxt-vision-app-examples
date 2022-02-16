#pragma once

// Include framework headers
#include <vision.h>
#include <configurableroi.h>

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
     * @brief Set the ROIs
     * @param rois Map of managed rois
     */
    void setROIs(const QMap<QString, std::shared_ptr<IDS::NXT::ConfigurableROI>> &rois);

    /**
     * @brief Get the calculated gray values
     * @return List of gray values with corresponding roi
     */
    QList<QPair<std::shared_ptr<IDS::NXT::ConfigurableROI>, qint64>> getResult();

private:
    QMap<QString, std::shared_ptr<IDS::NXT::ConfigurableROI>> _rois;
    QList<QPair<std::shared_ptr<IDS::NXT::ConfigurableROI>, qint64>> _result;
};
