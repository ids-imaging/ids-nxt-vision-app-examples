// #DETECTION
#include "tflite.h"

#include <fstream>
#include <QLoggingCategory>
#include <QJsonArray>
#include <QJsonObject>

static QLoggingCategory lc { "Persondetector.tflite" };

using namespace IDS::NXT::CNNv2;

TfLite::TfLite(const std::string &ftlitemodel)
// This class takes care of the TFLite part of the included person detection model. For detection architectures,
// we process the largest part of the model, the back bone, with the Deep Ocean Core. Only for the very last operations,
// box regression and non-maximum supression, we use TFLlite models running on the camera's ARM cores.
// IDS NXT lighthouse (or Ferry) already takes care of generating the models, here, we only manage input and output buffers
// This part is generic and can be used with any detection model trained on IDS NXT lighthouse or translated with IDS NXT Ferry
{
    // Loading the TFLite model upon object construction
    _model = tflite::FlatBufferModel::BuildFromFile(ftlitemodel.c_str());
    if (_model == nullptr) {
        throw std::runtime_error("can not create Model: " + ftlitemodel);
    }

    // Building the interpreter
    tflite::ops::builtin::BuiltinOpResolver resolver;
    tflite::InterpreterBuilder builder(*_model, resolver);
    builder(&_interpreter);
    if (_interpreter == nullptr) {
        qCDebug(lc) << "can not create interpreter";
        throw std::runtime_error("can not create interpreter");
    }

    _interpreter->SetNumThreads(1);

    // Allocate tensor buffers.
    if (!(_interpreter->AllocateTensors() == kTfLiteOk)) {
        throw std::runtime_error("can not allocate Tensors");
    }
}

std::vector<std::vector<float>> TfLite::doBoxRegression(const std::vector<ResultBuffer> &buffers)
{
    // This function takes the CNN result from the Deep Ocean Core buffer and feeds it into TFLite buffer for the box regression

    std::vector<int> inputIndices = _interpreter->inputs();

    // all Deep Ocean Core result buffers are copied into the allocated TFLite input buffers
    for (uint64_t i = 0; i < inputIndices.size(); i++) {
        auto inputTensor = _interpreter->typed_tensor<float>(inputIndices[i]);

        auto tfBufferName = QString(_interpreter->GetInputName(i));

        // Match Deep Ocean Core buffername with TFLite buffername
        bool bufferFound = false;
        for (const auto &buf : buffers) {
            if (buf.name == tfBufferName) {
                //qCDebug(lc) << "It's a match!";
                memcpy(inputTensor, buf.data, buf.values * sizeof(float));
                bufferFound = true;
                break;
            }
        }

        if (!bufferFound) {
            qCCritical(lc) << "Can not match deep ocean Buffer to tfLite Buffer" << tfBufferName;
            throw std::runtime_error("Can not match deep ocean Buffer to tfLite Buffer");
        }
    }

    // Invoking the TFLite interpreter
    if (!(_interpreter->Invoke() == kTfLiteOk)) {
        throw std::runtime_error("can not invoke");
    }

    // and grabbing the result
    std::vector<int> outputIndices = _interpreter->outputs();
    std::vector<std::vector<float>> result;
    result.resize(static_cast<uint64_t>(outputIndices.size()));

    int bufferIndex = outputIndices.size();

    for (const auto &outputIndice : outputIndices) {
        auto outputDimension = _interpreter->tensor(outputIndice)->dims;
        auto outputTensor = _interpreter->typed_tensor<float>(outputIndice);

        // Buffer Order is inverted
        bufferIndex--;

        auto values = outputDimension->data[1] * outputDimension->data[2];

        result[bufferIndex].reserve(static_cast<uint64_t>(values));
        result[bufferIndex].insert(result[bufferIndex].begin(), &outputTensor[0], &outputTensor[values]);
    }

    return result;
}

TfLite::boundingBoxes TfLite::doBoxDecoding(const std::vector<std::vector<float>> &boxProposals, float threshold)
{
    //This function takes the result of the box regression and runs the TFLite model for box decoding

    boundingBoxes detections;

    std::vector<int> inputIndices = _interpreter->inputs();

    const int coordinates = 4;
    int bufferIndex = 0;

    // all input buffers are again copied to the allocated input buffers of the box decoding model
    for (const auto &inputIndex : inputIndices) {
        auto decoderInput = _interpreter->typed_tensor<float>(inputIndex);
        memcpy(decoderInput, boxProposals.at(bufferIndex).data(), boxProposals.at(bufferIndex).size() * sizeof(float));
        bufferIndex++;
    }

    //and the TFLite interpreter is invoked, as before with the box regression
    if (!(_interpreter->Invoke() == kTfLiteOk)) {
        throw std::runtime_error("can not invoke");
    }

    // getting the results
    std::vector<int> outputIndices = _interpreter->outputs();

    auto detectionCoordinates = _interpreter->typed_tensor<float>(outputIndices[0]);
    auto detectionIndices = _interpreter->typed_tensor<float>(outputIndices[1]);
    auto detectionScores = _interpreter->typed_tensor<float>(outputIndices[2]);

    auto dimension = static_cast<int>(_interpreter->tensor(outputIndices[1])->dims->data[0]);

    // Only detections with scores higher then the adjustable threshold are copied and forwarded as the final result
    for (int i = 0; i < dimension; i++) {
        if (detectionScores[i] > threshold) {
            std::vector<float> thisCoordinate;

            thisCoordinate.reserve(coordinates);

            for (int j = 0; j < coordinates; j++) {
                thisCoordinate.push_back(detectionCoordinates[i * coordinates + j]);
            }
            detections.boxCoordinates.push_back(thisCoordinate);
            detections.classIndices.push_back(static_cast<int>(detectionIndices[i]));
            detections.classScores.push_back(detectionScores[i]);
        }
    }
    detections.numberDetections = detections.boxCoordinates.size();

    return detections;
}

QList<TfLite::overlayData> TfLite::createResult(const boundingBoxes &detections, const QSize &full, const QSize &small, const QStringList &classes)
{
    // This function scales and prepares the result from doBoxDecoding(...) to be rendered onto the original image
    auto width_factor = (float)full.width() / (float)small.width();
    auto height_factor = (float)full.height() / (float)small.height();

    QList<TfLite::overlayData> drawData;

    auto limitRect = [full](const QRect &rect) {
        auto fullRect = QRect(QPoint(0, 0), full);

        return fullRect.intersected(rect);
    };

    for (int cnt = 0; cnt < detections.numberDetections; cnt++) {
        QPair<QString, float> currentResult;

        auto classText = classes[detections.classIndices.at(cnt)];
        auto detection = detections.boxCoordinates.at(cnt);

        auto point1s = detection.at(0) * static_cast<float>(small.width());
        auto point2s = detection.at(1) * static_cast<float>(small.height());
        auto point3s = detection.at(2) * static_cast<float>(small.width());
        auto point4s = detection.at(3) * static_cast<float>(small.height());
        //qCDebug(lc) << "points << "point1s << point2s<< point3s << point4s;

        auto point1f = static_cast<int>(point1s * width_factor);
        auto point2f = static_cast<int>(point2s * height_factor);
        auto point3f = static_cast<int>(point3s * width_factor);
        auto point4f = static_cast<int>(point4s * height_factor);
        //qCDebug(lc) << point1f << point2f<< point3f << point4f;

        const auto rect = QRect(QPoint(point1f, point2f), QPoint(point3f, point4f));
        currentResult = QPair<QString, float>(classText, detections.classScores.at(cnt));

        QList<QPair<QString, float>> drawResult;

        drawResult.append(currentResult);

        TfLite::overlayData dat;
        dat.cnnRoi = limitRect(rect);
        dat.inference = drawResult;
        // get indice of of class within resultlist. Used to choose color for the rectangles in customresultimage
        dat.color = classes.indexOf(drawResult.at(0).first);
        drawData.append(dat);
    }

    return drawData;
}

QJsonArray TfLite::createResultJson(const QList<overlayData> &input)
{
    // formats the output of createResult(...) as json for the result collection and the REST interface
    QJsonArray allResults;

    for (const auto &dat : input) {
        auto thisRoi = roiToJson(dat.cnnRoi);

        QJsonArray inferenceResult;
        for (const auto &val : dat.inference) {
            QJsonObject thisResult;
            thisResult["Class"] = val.first;
            thisResult["Probability"] = QString::number(val.second, 'f', 2).toDouble();

            inferenceResult.append(thisResult);
        }

        QJsonArray inference;
        inference.append(inferenceResult);
        thisRoi["Inference"] = inference;

        allResults.append(thisRoi);
    }
    return allResults;
}

QJsonObject TfLite::roiToJson(const QRect &roi)
{
    QJsonObject myRoi;
    myRoi.insert("OffsetX", roi.x());
    myRoi.insert("OffsetY", roi.y());
    myRoi.insert("Width", roi.width());
    myRoi.insert("Height", roi.height());

    return myRoi;
}

void BoundingBoxesProcessing::setTFLiteFiles(const QPair<QString, QString> &currentTFLiteFiles)
{
    if (currentTFLiteFiles != _tfLiteFiles) {
        qCDebug(lc) << "setTFLiteFiles" << currentTFLiteFiles << _tfLiteFiles;
        _box = nullptr;
        _decoder = nullptr;

        if (!currentTFLiteFiles.first.isEmpty() && !currentTFLiteFiles.second.isEmpty()) {
            _box = std::make_unique<TfLite>(currentTFLiteFiles.first.toStdString());
            _decoder = std::make_unique<TfLite>(currentTFLiteFiles.second.toStdString());
        }

        _tfLiteFiles = currentTFLiteFiles;
    }
}

QList<TfLite::overlayData> BoundingBoxesProcessing::processDetections(
        const std::unique_ptr<MultiBuffer> &buffer,
        QSize fullSize,
        QSize smallSize,
        const QStringList &classes,
        float threshold)
{
    if (_box && _decoder) {
        auto boxProposals = _box->doBoxRegression(buffer->allBuffers());
        auto detections = _decoder->doBoxDecoding(boxProposals, threshold);

        return TfLite::createResult(detections, fullSize, smallSize, classes);
    }

    return QList<TfLite::overlayData> {};
}
