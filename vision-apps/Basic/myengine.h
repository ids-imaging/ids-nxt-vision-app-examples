#ifndef MYENGINE_H
#define MYENGINE_H

// Include std headers
#include <memory>

// Include framework headers
#include "customresultimage.h"
#include <engine.h>
#include <resultsourcecollection.h>
#include <configurableroi.h>
#include <configurableenum.h>
#include <configurableint64.h>
#include <action.h>

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
     * processed and/or written to the ResultSourceCollection. After that, the image can be
     * removed (deleteted / set to nullptr) from the vision object.
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
     * @brief Region of interest
     *
     * The ROI can be modified by the user at runtime via REST or the NXT Cockpit
     */
    IDS::NXT::ConfigurableROI _roi;

    /**
     * @brief Result image
     *
     * This image is displayed separately in the NXT Cockpit
     * In this example we will use the result image to display the ROI image
     */
    CustomResultImage _resultImage;

    /**
     * @brief Add a configurable enumeration to the VApp
     *
     * An enumeration object is displayed as drop down menu in the NXT Cockpit
     * In this example we will use the enum to modify the trigger mode settings
     */
    IDS::NXT::ConfigurableEnum _enum;

    /**
     * @brief Add a configurable integer to the VApp
     *
     * A configurable integer object is displayed as slider in the NXT Cockpit
     * In this example we will use the slider to modify the red gain value
     */
    std::unique_ptr<IDS::NXT::ConfigurableInt64> _slider;

    /**
     * @brief Add an action to the VApp
     *
     * In the NXT Cockpit actions are displayed as buttons
     * In this example we will use a button to trigger the image aquisition
     */
    IDS::NXT::Action _triggerButton;

private slots:
    void changeTriggerMode(const QString &newMode);
    void triggerImageAquisition();
    void setRedGain(qint64 redGain);
    void synchronizeEnumWithTriggerMode();
    void synchronizeSliderWithRedGain();
};

#endif // MYENGINE_H
