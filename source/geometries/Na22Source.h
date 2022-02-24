// ----------------------------------------------------------------------------
// petalosim | Na22Source.h
//
// Na-22 calibration specific source with plastic support used in PETit.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#ifndef NA22_SOURCE_H
#define NA22_SOURCE_H

#include "DiskSource.h"

namespace nexus {

  class Na22Source: public DiskSource {
  public:
    /// Constructor
    Na22Source();

    /// Destructor
    ~Na22Source();

    void Construct();

  };
}
#endif
