//McKenzie Burch
//Rigging Dojo Maya API
//mainPlugin.cpp
//standard setup for event node

#include "pushNode.h"  //change include header for each node
#include <maya/MGlobal.h>
#include <maya/MFnPlugin.h> //Maya class that redisters and deregisters plug-ins with Maya

MStatus initializePlugin(MObject obj){
  MStatus status;
  MFnPlugin fnplugin(obj, "McKenzie Burch", "1.0", "Any");

  status = fnplugin.registerNode("pushNode", pushNode::typeId, pushNode::creator, pushNode::initialize, pushNode::kDeformerNode);

  if(status != MS::kSuccess)
    status.perror("Could not regiser the pushNode node");

  return status;
}

MStatus uninitializePlugin(MObject obj){
  MFnPlugin pluginFn;
  //deregister the given user defined node type Maya
  pluginFn.deregisterNode(pushNode::typeId);

  return MS::kSuccess;
}
