#include <QLoggingCategory>

// Include nxt framework headers
#include <systemsignals.h>

// Include local header files
#include "myapp.h"

static QLoggingCategory lc{"MultiCNN"};

/**
 * @brief Serves as the main entry point for our program
 * @param argc count of command line arguments
 * @param argv list of command line arguments
 * @return Exit success
 */
int main(int argc, char* argv[]) {
    // Catch Signals to close VApp Properly
    IDS::Utils::catchSignalsAndQuit({SIGQUIT, SIGINT, SIGTERM, SIGHUP});

    // Create the app object
    MyApp app{argc, argv};

    // Log that the app runs now
    qInfo(lc) << "Up and running.";

    // Actually start the app. This line will not return until the app object terminates
    return app.exec();
}
