// Include the local header
#include "faceVapp.h"

using namespace IDS::NXT;

FaceApp::FaceApp(int &argc, char **argv)
    : VApp(argc, argv)
    , _resultcollection{}
    , _engine{ _resultcollection }
{
    // create result sources
    _resultcollection.createSource("good", ResultType::Boolean);

    auto decodingtime = _resultcollection.createSource("decodingtime", "Integer");
    decodingtime->setUnit(Unit::Milliseconds);
}

void FaceApp::imageAvailable(std::shared_ptr<Hardware::Image> image)
{
    // as there's only one engine, simply forward the image
    _engine.handleImage(image);
}

void FaceApp::abortVision()
{
    // as there's only one engine, simply forward the abort event
    _engine.abortVision();
}
