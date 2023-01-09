// ----------------------------------------------------------------------------
// nexus | PetAnalysisRunAction.cc
//
// This is the default run action of the PETALO simulations.
// The G4 analysis manager is opened and closed.
// A message at the beginning and at the end of the simulation is also printed.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetAnalysisRunAction.h"

#include "nexus/FactoryBase.h"

#include <G4Run.hh>
#include <G4AnalysisManager.hh>

using namespace nexus;

REGISTER_CLASS(PetAnalysisRunAction, G4UserRunAction)

PetAnalysisRunAction::PetAnalysisRunAction(): G4UserRunAction()
{
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("csv");
  // Set the base name of the histogram files.
  // If no histogram files are booked, you still need to provide a name here,
  // but it's not used anywhere.
  analysisManager->SetFileName("OptTrackInfo");

  // Book histograms
  analysisManager->CreateH1("CherLambda","Wavelengths of Cherenkov photons (nm)", 1000, 0, 1500.); // histo ID = 0
  analysisManager->CreateH1("ScintLambda","Wavelengths of scintillation photons (nm)", 1000, 0, 800.); // histo ID = 1
  analysisManager->CreateH1("ScintillationTime", "Scintillation time (ps)", 8000, 0, 40000.); // histo ID = 2

  // Book event observables to save (Object ID = 0)
  analysisManager->CreateNtuple("OptEvt", "Number of produced optical photon");
  analysisManager->CreateNtupleDColumn("NPhotons"); // column ID = 0
  analysisManager->FinishNtuple();
  analysisManager->SetNtupleFileName(0, "OpticalEvtInfo");

  /*
  // Example of how to create a different file with different observables
  // Book track observables to save (Object ID = 1)
  analysisManager->CreateNtuple("OptTrack", "Photon properties");
  analysisManager->CreateNtupleDColumn("CherLambda"); // column ID = 0
  analysisManager->CreateNtupleDColumn("ScintLambda"); // column ID = 1
  analysisManager->CreateNtupleDColumn("ScintillationTime"); // column ID = 2
  analysisManager->FinishNtuple();
  analysisManager->SetNtupleFileName(1, "OpticalTrackingInfo");
  */
}



PetAnalysisRunAction::~PetAnalysisRunAction()
{
}



void PetAnalysisRunAction::BeginOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Reset();
  analysisManager->OpenFile();
}


void PetAnalysisRunAction::EndOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " end." << G4endl;

  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}
