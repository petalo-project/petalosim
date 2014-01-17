
//
// $Id$
//

#ifndef NexusStepLimiterBuilder_h
#define NexusStepLimiterBuilder_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"


class G4StepLimiter;
class G4UserSpecialCuts;

class NexusStepLimiterBuilder : public G4VPhysicsConstructor
{
public:

  NexusStepLimiterBuilder(const G4String& name = "stepLimiter");
  virtual ~NexusStepLimiterBuilder();

public:

  // This method is dummy for physics
  virtual void ConstructParticle();

  // This method will be invoked in the Construct() method.
  // each physics process will be instantiated and
  // registered to the process manager of each particle type
  virtual void ConstructProcess();

private:

   // hide assignment operator
  NexusStepLimiterBuilder & operator=(const NexusStepLimiterBuilder &right);
  NexusStepLimiterBuilder(const NexusStepLimiterBuilder&);

  G4StepLimiter* fStepLimiter;
  G4UserSpecialCuts* fUserSpecialCuts;
};



#endif
