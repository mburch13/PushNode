//pushNode.h

#ifndef pushNodep_H
#define pushNodep_H

#include <maya/MMatrix.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MEventMessage.h>
#include <maya/MNodeMessage.h>
#include <maya/MIntArray.h>
#include <maya/MPointArray.h>
#include <maya/MPxDeformerNode.h>

#include <vector>
#include <set>

class pushNode : public MPxDeformerNode{
 public:
  pushNode() {};
  static void* creator() { return new pushNode(); };
  static MStatus initialize();
  virtual MStatus deform(MDataBlock& data, MItGeometry& iter, const MMatrix& mat, unsigned int mIndex);

 public:
  static MTypeId typeId;
  static MObject stressMap;
  static MObject useStress;
  static MObject amount;
};


#endif
