#pragma once

// Include framework headers
#include "cnnmanager_v2.h"
#include <vision.h>

/**
 * @brief The app-specific vision object
 */
class MyVision : public IDS::NXT::Vision {
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
     * @brief Getter for the result
     */
    std::vector<std::unique_ptr<IDS::NXT::CNNv2::MultiBuffer>> result();
    QList<IDS::NXT::CNNv2::CnnData> cnnData() const;

    /**
     * @brief Setter for the list of activated CNNs
     */
    void setCnnData(const QList<IDS::NXT::CNNv2::CnnData>& cnnData);

private:
    std::vector<std::unique_ptr<IDS::NXT::CNNv2::MultiBuffer>> _result;
    QList<IDS::NXT::CNNv2::CnnData> _cnnData;
};
