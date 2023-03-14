#include "inferenceresultprocessing.h"

QList<InferenceResultProcessing::overlayData> InferenceResultProcessing::createResult(const IDS::NXT::CnnHelper::InferenceResult &detections,
                                                                                      QSize full,
                                                                                      const QStringList &classes)
{
    QList<InferenceResultProcessing::overlayData> drawData;

    auto limitRect = [full](const QRect rect) {
        const auto fullRect = QRect(QPoint(0, 0), full);

        return fullRect.intersected(rect);
    };

    for (const auto &roiResult : detections.roiResults) {
        QPair<QString, float> currentResult;

        const auto classText = roiResult.inferenceProbabilities.first();
        const auto rect = roiResult.roi;

        currentResult = QPair<QString, float>(classText.inferenceClass,
                                              roiResult.inferenceProbabilities.first().probability);

        // QList to support multiple results per roi
        QList<QPair<QString, float>> drawResult;

        drawResult.append(currentResult);

        InferenceResultProcessing::overlayData dat;
        dat.cnnRoi = limitRect(rect);
        dat.inference = drawResult;
        // get indice of of class within resultlist. Used to choose color the rectangles in customresultimage
        dat.color = classes.indexOf(drawResult.at(0).first);

        if (!((dat.cnnRoi.x() == 0) && (dat.cnnRoi.y() == 0) && (dat.cnnRoi.width() == 0)
              && (dat.cnnRoi.height() == 0))) {
            drawData.append(dat);
        }
    }

    return drawData;
}
