#ifndef JAVASCRIPTCOMMUNICATION_H
#define JAVASCRIPTCOMMUNICATION_H

#include <string>
#include "../CuraEngine/src/communication/Communication.h"

namespace cura
{
class JavascriptCommunication : public Communication
{
public:
    JavascriptCommunication();
    void beginGCode() override;
    void flushGCode() override;
    bool isSequential() const override;
    bool hasSlice() const override;
    void sendCurrentPosition(const Point&) override;
    void sendFinishedSlicing() const override;
    void sendGCodePrefix(const std::string&) const override;
    void sendLayerComplete(const LayerIndex&, const coord_t&, const coord_t&) override;
    void sendLineTo(const PrintFeatureType&, const Point&, const coord_t&, const coord_t&, const Velocity&) override;
    void sendOptimizedLayerData() override;
    void sendPolygon(const PrintFeatureType&, const ConstPolygonRef&, const coord_t&, const coord_t&, const Velocity&) override;
    void sendPolygons(const PrintFeatureType&, const Polygons&, const coord_t&, const coord_t&, const Velocity&) override;
    void sendPrintTimeMaterialEstimates() const override;
    void sendProgress(const float& progress) const override;
    void setExtruderForSend(const ExtruderTrain&) override;
    void setLayerForSend(const LayerIndex&) override;
    void sliceNext() override;
};

} //namespace cura

#endif //JAVASCRIPTCOMMUNICATION_H