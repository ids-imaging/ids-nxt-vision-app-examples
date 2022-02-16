#pragma once

// Include framework headers
#include <vapp.h>
#include <resultsourcecollection.h>
#include <configurableenum.h>
#include <action.h>
#include <configurablefile.h>

// Include own headers
#include "myengine.h"

/**
 * @brief The app-specific app object
 *
 * The app object serves as central object in the vision app. It's inheritance of VApp gives basic system
 * functionality, such as listing in the REST-Interface / GUI and, probably most importantly allows the
 * usage of interaction elements such as actions, configurables etc.
 */
class MyApp : public IDS::NXT::VApp
{
    // This Qt macro is needed for the signal/slot mechanism
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param argc count of command line arguments
     * @param argv list of command line arguments
     */
    MyApp(int &argc, char **argv);

protected slots:
    /**
     * @brief Called for new images
     * @param image new image
     *
     * This function is called whenever there's a new image which is to be processed by this vision app.
     * The image is provided as a shared pointer. As long as this pointer is kept alive, the image data will
     * not be overwritten by the framework. For an effective software freerun though, the framework must be
     * told that the image processing is done, which can be done with the methods visionDone, visionOK or
     * visionFailed of the image object. Alternatively, the pointer can simply be destructed, which signalizes
     * that the vision app ignored the image and is nearly equivalent with a call to visionDone.
     */
    void imageAvailable(std::shared_ptr<IDS::NXT::Hardware::Image> image) override;

    /**
     * @brief Called for abort-requests
     *
     * This function is called whenever the framework requires the vision to abort all executing image processing.
     */
    void abortVision() override;

private slots:
    /**
     * @brief This slot should be called if the number of installed cnns changed e.g. if an new one is installed.
     *
     * Connect it to the corresponding framework signal
     */
    void installedCnnsChanged();

    /**
     * @brief Slot for enabling a cnn
     */
    void enableCnn();

    /**
     * @brief Slot for disabling a cnn
     */
    void disableCnn();

    /**
     * @brief This slot should be called when the active cnn has changed
     */
    void cnnChanged();

    /**
     * @brief This slot should be called when a cnn file is added
     */
    void cnnfileWritten();

    /**
     * @brief This slot should be called when the current cnn file should be removed
     */
    void deleteCurrentCnn();

private:
    /**
     * @brief Collection of results
     *
     * This object stores all results of the image processing. It is held as a shared pointer as it needs to be
     * used in any engine available.
     */
    IDS::NXT::ResultSourceCollection _resultcollection;

    MyEngine _engine;

    std::unique_ptr<IDS::NXT::ConfigurableFile> _cnnPackage;
    std::unique_ptr<IDS::NXT::ConfigurableEnum> _installedCnns;
    IDS::NXT::Action _enableCnnAction;
    IDS::NXT::Action _disableCnnAction;

    std::mutex _installLock; // Lock to prevent installing a multiple cnns at the same time

    void updateCnnFileDeletableProperty();
};
