#pragma once

// include std headers
#include <memory>

// include opencv headers
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

// include NXT framework headers
#include "engine.h"
#include "configurableint64.h"
#include "resultsourcecollection.h"

// include project headers
#include "faceResultImage.h"

/**
 * @brief Specific Engine class
 */
class FaceEngine : public IDS::NXT::Engine
{
    // IDS::NXT::Engine inherits from QObject, hence this Qt macro is needed here
    Q_OBJECT

public:
    /**
     * @brief C'tor
     * @param resultCollection ResultSourceCollection of the VApp
     */
    FaceEngine(IDS::NXT::ResultSourceCollection &resultcollection);

protected:
    /**
     * @see IDS::NXT::Engine::factoryVision
     */
    virtual std::shared_ptr<IDS::NXT::Vision> factoryVision() override;
    /**
     * @see IDS::NXT::Engine::setupVision
     */
    virtual void setupVision(std::shared_ptr<IDS::NXT::Vision> vision) override;
    /**
     * @see IDS::NXT::Engine::handleResult
     */
    virtual void handleResult(std::shared_ptr<IDS::NXT::Vision> vision) override;

private:
    /// Reference to the ResultSourceCollection of the VApp
    IDS::NXT::ResultSourceCollection &_resultcollection;

    /// ResultImage, displaying contours around found faces
    FaceResultImage _resImage;

    /// Configurable, scaling of face detection
    IDS::NXT::ConfigurableInt64 _scale;

    /// OpenCV classifier object
    cv::CascadeClassifier _cascade;
};
