#include <QLoggingCategory>

// Include framework headers
#include "systemsignals.h"

// Include local header files
#include "myapp.h"

// Instantiate a QLoggingCategory Object
static QLoggingCategory lc{ "classificationexample" };

/**
 * @brief Serves as the main entry point for our program
 * @param argc count of command line arguments
 * @param argv list of command line arguments
 * @return Exit success
 */
int main(int argc, char *argv[])
{
    // This function ensures that the program is terminated correctly
    IDS::Utils::catchSignalsAndQuit({ SIGQUIT, SIGINT, SIGTERM, SIGHUP });

    // Create the app object
    MyApp app{ argc, argv };

    // Log that the app runs now
    qCInfo(lc) << "Classification example up and running.";

    // Actually start the app. This line will not return until the app object terminates
    return app.exec();
}
