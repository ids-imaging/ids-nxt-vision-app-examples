// Include Qt headers and qtfix.h which works around a compiler issue with QLoggingCategory
#include <QLoggingCategory>

// Include framework headers
#include "systemsignals.h"

// Include local header files
#include "gvapp.h"

// Instantiate a QLoggingCategory Object
static QLoggingCategory lc{ "grayvalue" };

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
    gvApp app{ argc, argv };

    // Log that the app runs now
    qCInfo(lc) << "Grayvalue up and running.";

    // Actually start the app. This line will not return until the app object terminates
    return app.exec();
}
