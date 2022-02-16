#pragma once

// Include std headers
#include <memory>

// Include framework headers
#include <engine.h>
#include <resultsourcecollection.h>
#include <roimanager.h>

/**
 * @brief The app-specific engine
 */
class MyEngine : public IDS::NXT::Engine
{
    // This Qt macro is needed for the signal/slot mechanism
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param resultcollection The result collection object
     *
     * This function constructs the engine object, further parameters could be inserted if
     * app-global interaction objects should be used in a multi-engine vision app.
     */
    MyEngine(IDS::NXT::ResultSourceCollection &resultcollection);

protected:
    /**
     * @brief Factory function for vision objects
     * @return Factored vision object
     *
     * This method factors a vision object, which depends on our implementation. This method
     * is called whenever the framework needs a new vision object, these objects are reused.
     * Do not set up changing variables inside of the vision object, this should be done in setupVision.
     */
    virtual std::shared_ptr<IDS::NXT::Vision> factoryVision() override;

    /**
     * @brief Setup of a vision object
     * @param vision Vision object to be set up
     *
     * This method sets up a vision object. This method is called whenever a vision objects needs to be
     * setup, i.e. when a new image should be processed. Inside this method, every variable of the vision
     * object should be set. As the vision object will run in a different thread context, use thread-safe
     * variables, e.g. simple copies.
     */
    virtual void setupVision(std::shared_ptr<IDS::NXT::Vision> vision) override;

    /**
     * @brief Handle the result of a vision object
     * @param vision Finished vision object
     *
     * This method handles a finished vision object and reads the results. These results can then be further
     * processed and/or written to the ResultSourceCollection. After that, the image can be removed from the
     * vision object.
     */
    virtual void handleResult(std::shared_ptr<IDS::NXT::Vision> vision) override;

private:
    /**
     * @brief Collection of results
     *
     * This object stores all results of the image processing.
     */
    IDS::NXT::ResultSourceCollection &_resultcollection;

    /**
     * @brief ROI manager object
     *
     * The ROI manager handles multiple rois (adding, removing, resizing, moving etc.) at runtime.
     * If a VApp uses the ROIManager, the NXT-Cockpit provides a particular ROI-configuration toolbox to handle multiple ROIs.
     */
    IDS::NXT::ROIManager _roiManager;
};
