#include "myapp.h"

using namespace IDS::NXT;

MyApp::MyApp(int &argc, char **argv)
    : VApp { argc, argv }, _engine { _resultcollection }
{
    // at this point all result sources must be created.
    _resultcollection.createSource("visionduration", ResultType::String);
    _resultcollection.createSource("grayvalues", ResultType::String);
}

void MyApp::imageAvailable(std::shared_ptr<Hardware::Image> image)
{
    _engine.handleImage(image);
}

void MyApp::abortVision()
{
    _engine.abortVision();
}
