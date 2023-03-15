#pragma once

#include "libutils_cnnhelper.h"

#include <QObject>
#include <QRect>

class InferenceResultProcessing
{
public:
    struct overlayData {
        QList<QPair<QString, float>> inference;
        QRect cnnRoi;
        int color = 0;
    };

    static QList<InferenceResultProcessing::overlayData> createResult(const IDS::NXT::CnnHelper::InferenceResult &detections,
                                                                      QSize full,
                                                                      const QStringList &classes);
};
