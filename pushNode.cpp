//pushNode.cpp

#include "pushNode.h"
#include <maya/MPxDeformerNode.h>
#include <maya/MItGeometry.h>
#include <maya/MItMeshVertex.h>
#include <maya/MFnNumericAttribute.h>  //numeric attribute function set
#include <maya/MFnTypedAttribute.h>  //static class provviding common API global functions
#include <maya/MPoint.h>
#include <maya/MVector.h>
#include <maya/MGlobal.h>  //static class provviding common API global functions
#include <maya/MFnMesh.h>
#include <maya/MPointArray.h>
#include <maya/MFnDoubleArrayData.h>
#include <maya/MItMeshPolygon.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>

#include <set>

#define SMALL (float)1e-6
#define BIG_DIST 99999

MTypeId pushNode::typeId(0x00108b13);  //define value for typeId

MObject pushNode::stressMap;
MObject pushNode::useStress;
MObject pushNode::amount;

MStatus pushNode::initialize(){
  MFnNumericAttribute numAttr;
  MFnTypedAttribute typedFn;

  useStress = numAttr.create("useStress", "ust", MFnNumericData::kBoolean, 0);
  numAttr.setKeyable(true);
  numAttr.setStorable(true);
  addAttribute(useStress);

  amount = numAttr.create("amount", "am", MFnNumericData::kDouble, 0);
  numAttr.setKeyable(true);
  numAttr.setStorable(true);
  addAttribute(amount);

  stressMap = typedFn.create("stressMap", "str", MFnData::kDoubleArray);
  typedFn.setKeyable(true);
  typedFn.setStorable(true);
  typedFn.setWritable(true);
  addAttribute(stressMap);

  attributeAffects(stressMap, outputGeom);
  attributeAffects(amount, outputGeom);
  attributeAffects(useStress, outputGeom);

  MGlobal::executeCommand("makePaintable -attrType multiFloat -sm deformer pushNode weights");

  return MS::kSuccess;
}

MStatus pushNode::deform(MDataBlock& data, MItGeometry& iter, const MMatrix& mat, unsigned int mIndex){

  //gather data
  double envelopeV = data.inputValue(envelope).asFloat();
  bool useStressV = data.inputValue(useStress).asBool();
  double amountV = data.inputValue(amount).asDouble();

  //if envelope is 0 do not compute
  if(envelopeV < SMALL){
    return MS::kSuccess;
  }

  //if useStress is turned on lets pull out the data of the stressMap
  MDoubleArray stressV;
  if(useStressV == true){
    //pull out raw data as an MObject
    MObject stressData = data.inputValue(stressMap).data();
    //convert raw data to double array
    MFnDoubleArrayData stressDataFn(stressData);
    stressV = stressDataFn.array();
  }

  //put out all the points
  MPointArray pos;
  iter.allPositions(pos, MSpace::kWorld);

  MArrayDataHandle meshH = data.inputArrayValue(input);
  meshH.jumpToArrayElement(0);
  MObject mesh = meshH.inputValue().child(inputGeom).asMesh();
  MFnMesh meshFn(mesh);

  //pull out all normals
  MFloatVectorArray normals;
  meshFn.getNormals(normals, MSpace::kWorld);

  MPoint temp;
  for(int i=0; i<iter.exactCount(); i++){
    if(useStressV == true){
      pos[i] += (MVector(normals[i])*envelopeV*amountV*stressV[i]);
    }
    else{
      pos[i] += (MVector(normals[i]*envelopeV*amountV));
    }
  }

  iter.setAllPositions(pos);  //set all positions
  return MS::kSuccess;
}
