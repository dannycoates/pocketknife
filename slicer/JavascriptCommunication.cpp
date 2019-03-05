#include "JavascriptCommunication.h"

namespace cura
{

JavascriptCommunication::JavascriptCommunication(){}
void JavascriptCommunication::beginGCode() { }
void JavascriptCommunication::flushGCode() { }
void JavascriptCommunication::sendCurrentPosition(const Point&) { }
void JavascriptCommunication::sendFinishedSlicing() const { }
void JavascriptCommunication::sendLayerComplete(const LayerIndex&, const coord_t&, const coord_t&) { }
void JavascriptCommunication::sendLineTo(const PrintFeatureType&, const Point&, const coord_t&, const coord_t&, const Velocity&) { }
void JavascriptCommunication::sendOptimizedLayerData() { }
void JavascriptCommunication::sendPolygon(const PrintFeatureType&, const ConstPolygonRef&, const coord_t&, const coord_t&, const Velocity&) { }
void JavascriptCommunication::sendPolygons(const PrintFeatureType&, const Polygons&, const coord_t&, const coord_t&, const Velocity&) { }
void JavascriptCommunication::setExtruderForSend(const ExtruderTrain&) { }
void JavascriptCommunication::setLayerForSend(const LayerIndex&) { }
bool JavascriptCommunication::isSequential() const {
  return true;
}
bool JavascriptCommunication::hasSlice() const {
  // TODO
  return true;
}
void JavascriptCommunication::sendGCodePrefix(const std::string&) const {}
void JavascriptCommunication::sendPrintTimeMaterialEstimates() const {}
void JavascriptCommunication::sendProgress(const float& progress) const {}
void JavascriptCommunication::sliceNext() {}

}