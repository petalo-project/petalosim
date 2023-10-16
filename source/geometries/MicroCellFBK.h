// ----------------------------------------------------------------------------
// petalosim | MicroCellFBKVUV.h
//
// Each one of the cells of an FBK VUV SiPM.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef MICROCELL_FBK_H
#define MICROCELL_FBK_H

#include "MicroCellGeometryBase.h"

#include "nexus/GeometryBase.h"

using namespace nexus;

class MicroCellFBK : public MicroCellGeometryBase
{
public:
  /// Constructor
  MicroCellFBK();
  /// Destructor
  ~MicroCellFBK();
  
  void Construct();
 

};

#endif
