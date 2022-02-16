// Include the own header
#include "gvapp.h"
#include <QLoggingCategory>

using namespace IDS::NXT;

static QLoggingCategory lc{ "grayvalue.app" };

gvApp::gvApp(int &argc, char **argv)
    : VApp(argc, argv), _resultcollection{}, _engine{ _resultcollection }
{
    // #RESULT_HANDLING
    // Every result source has an identifier which must be unique and consist of
    // The sources are created in the collection, the return value can be used for further settings
    auto gv = _resultcollection.createSource("grayvalue", ResultType::Integer);

    // Further settings can include
    //  - range of possible values
    //  - warning ranges ("RangeWarningN")
    //  - error ranges ("RangeErrorN")
    //  - unit (SI units)
    gv->setRange(0, 255);

    // Create result source. (bright is true if grayvalue is higher than threshold)
    _resultcollection.createSource("bright", ResultType::Boolean);
}

void gvApp::imageAvailable(std::shared_ptr<Hardware::Image> image)
{
    // Every image is given to the engine. If multiple engines exist, we can decide which engine is used
    // Also, the pointer can simply be destructed, when the image should be ignored.
    _engine.handleImage(image);
}

void gvApp::abortVision()
{
    // All engines must be aborted
    _engine.abortVision();
}
