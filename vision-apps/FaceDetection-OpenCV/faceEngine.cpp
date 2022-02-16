// include the local header
#include "faceEngine.h"

// include Qt headers
#include <QLoggingCategory>

// include NXT framework headers
#include "resultsource.h"

// include project headers
#include "faceVision.h"

static QLoggingCategory lc{ "facedetect.engine" };

FaceEngine::FaceEngine(IDS::NXT::ResultSourceCollection &resultcollection)
    : _resultcollection{ resultcollection }
    , _resImage{ "default" }
    , _scale{ "scale", 5 }
{
    // improve configurable scale
    _scale.setRange(1, 10);

    // load OpenCV cascade classifier
    auto success = _cascade.load("vapp/haarcascade_frontalface_default.xml");

    qCDebug(lc) << "load cascade: " << success;
}

std::shared_ptr<IDS::NXT::Vision> FaceEngine::factoryVision()
{
    // factor project's vision object
    return std::make_shared<FaceVision>(_cascade);
}

void FaceEngine::setupVision(std::shared_ptr<IDS::NXT::Vision> vision)
{
    if (vision) {
        auto obj = std::static_pointer_cast<FaceVision>(vision);
        obj->setScale(_scale);
    }
}

void FaceEngine::handleResult(std::shared_ptr<IDS::NXT::Vision> vision)
{
    // cast generic framework vision class to project's vision class
    auto obj = std::static_pointer_cast<FaceVision>(vision);

    // read found faces
    auto faces = obj->faces();

    // check if any faces found and decide if "good" / "bad"
    if (faces.empty()) {
        _resultcollection.addResult("good", false, "generic", obj->image());
        obj->image()->visionFailed("No face found.", "false");
    } else {
        _resultcollection.addResult("good", true, "generic", obj->image());
        obj->image()->visionOK("Face found.", "good");
    }

    // add decodingtime
    _resultcollection.addResult("decodingtime", obj->timeNeeded(), "time", obj->image());

    // update result images
    _resImage.update(obj->image(), faces, true, _scale);

    // signal that all parts of the image are finished
    _resultcollection.finishedAllParts(obj->image());

    // free image, the buffer may now be reused by the system
    obj->setImage(nullptr);
}
