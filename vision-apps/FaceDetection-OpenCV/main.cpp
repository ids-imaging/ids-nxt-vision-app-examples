// include Qt headers
#include <QLoggingCategory>

// include NXT framework headers
#include "systemsignals.h"

// include project headers
#include "faceVapp.h"

static QLoggingCategory lc{ "facedetect" };

/**
 * This is the method, where the VApp starts
 * @brief Main method
 * @param argc Count of command line arguments (with VApps usually zero)
 * @param argv Array of command line arguments
 * @return program exit code
 */
int main(int argc, char *argv[])
{
    // This function ensures that the program is terminated correctly
    IDS::Utils::catchSignalsAndQuit({ SIGQUIT, SIGINT, SIGTERM, SIGHUP });

    // create actual myApp object
    FaceApp app{ argc, argv };

    // log, that we start now
    qCInfo(lc) << "Facedetect up and running.";

    // actually start the app
    return app.exec();
}
