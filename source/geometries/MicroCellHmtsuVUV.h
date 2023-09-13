// ----------------------------------------------------------------------------
// petalosim | MicroCellHmtsuVUV.h
//
// Each one of the cells of a Hamamatsu VUV SiPM.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef MICROCELL_HMTSU_H
#define MICROCELL_HMTSU_H

#include "MicroCellGeometryBase.h"

#include "nexus/GeometryBase.h"

using namespace nexus;

class MicroCellHmtsuVUV : public MicroCellGeometryBase
{
 public:
  /// Constructor
  MicroCellHmtsuVUV();
  /// Destructor
  ~MicroCellHmtsuVUV();
  
  void Construct();

};

#endif
