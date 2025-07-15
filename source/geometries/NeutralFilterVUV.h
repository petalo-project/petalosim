// ----------------------------------------------------------------------------
// petalosim | NeutralFilterVUV.h
//
// Neutral filter VUV.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef NEUTRAL_FILTER_H
#define NEUTRAL_FILTER_H


#include "nexus/GeometryBase.h"

using namespace nexus;

class NeutralFilterVUV : public GeometryBase
{
 public:
  /// Constructor
  NeutralFilterVUV();
  /// Destructor
  ~NeutralFilterVUV();
  
  void Construct();
  
  G4double GetFilterDepth() const;
  
 private:
 
  G4double filter_depth_;

};

inline G4double NeutralFilterVUV::GetFilterDepth() const {return filter_depth_;}

#endif
