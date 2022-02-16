#include <QLoggingCategory>

// Include nxt framework headers
#include <systemsignals.h>

// Include local header files
#include "myapp.h"

// Declare logging category (lc). LCs can be activated/deactivated in "qtlogging.ini"
// The log messages can be read over REST at http://<YOUR_CAMERA_IP>/vapps/<VAPP_NAME>/log
static QLoggingCategory lc{ "FtpExample" };

/**
 * @brief Serves as the main entry point for our program
 * @param argc count of command line arguments
 * @param argv list of command line arguments
 * @return Exit success
 */
int main(int argc, char *argv[])
{
    // Catch signals to close VApp properly
    IDS::Utils::catchSignalsAndQuit({ SIGQUIT, SIGINT, SIGTERM, SIGHUP });

    // Create the app object
    MyApp app{ argc, argv };

    // Log that the app runs now
    qCInfo(lc) << "Up and running.";

    // Actually start the app. This line will not return until the app object terminates
    return app.exec();
}
