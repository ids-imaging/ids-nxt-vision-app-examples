#pragma once

// include std headers
#include <memory>

// include OpenCV headers

#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"

// include NXT framework headers
#include "vapp.h"
#include "resultsourcecollection.h"

// include project headers
#include "faceEngine.h"
#include "faceResultImage.h"

/**
 * @brief Specific VApp class
 */
class FaceApp : public IDS::NXT::VApp
{
    // IDS::NXT::VApp inherits from QObject, hence this Qt macro is needed here
    Q_OBJECT

public:
    /**
     * @brief C'tor
     * @param argc Count of command line arguments (with VApps usually zero)
     * @param argv Array of command line arguments
     */
    FaceApp(int &argc, char **argv);

protected slots:
    /**
     * @see IDS::NXT::VApp::imageAvailable
     */
    void imageAvailable(std::shared_ptr<IDS::NXT::Hardware::Image> image) override;
    /**
     * @see IDS::NXT::VApp::abortVision
     */
    void abortVision() override;

private:
    /// Result collection
    IDS::NXT::ResultSourceCollection _resultcollection;

    /// VApp-Engine object
    FaceEngine _engine;
};
