// ----------------------------------------------------------------------------
// petalosim | PetAnalysisRunAction.cc
//
// This is the run action used for optical photon analyses.
// The G4 analysis manager is opened and closed and the histograms and n-tuples
// to be saved are declared.
// A message at the beginning and at the end of the simulation is also printed.
//
// The PETALO Collaboration
// ----------------------------------------------------------------------------

#include "PetAnalysisRunAction.h"

#include "nexus/FactoryBase.h"

#include <G4Run.hh>
#include <G4AnalysisManager.hh>
#include <G4GenericMessenger.hh>

REGISTER_CLASS(PetAnalysisRunAction, G4UserRunAction)

PetAnalysisRunAction::PetAnalysisRunAction(): G4UserRunAction(),
                                              fname_("OptTrackInfo"),
                                              ntuple_fname_("OpticalEvtInfo")
{
  msg_ = new G4GenericMessenger(this, "/Actions/PetAnalysisRunAction/");
  msg_->DeclareProperty("histo_file", fname_,
                        "Name of the file for the analysis histograms");
  msg_->DeclareProperty("ntuple_file", ntuple_fname_,
                        "Name of the file for the analysis histograms");
  
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetDefaultFileType("csv");

  // Book 1D histograms
  analysisManager->CreateH1("CherLambda","Wavelength of Cherenkov photons (nm)", 1000, 0, 1500.); // histo ID = 0
  analysisManager->CreateH1("ScintLambda","Wavelength of scintillation photons (nm)", 1000, 0, 800.); // histo ID = 1
  analysisManager->CreateH1("ScintillationTime", "Scintillation time (ps)", 8000, 0, 40000.); // histo ID = 2
  analysisManager->CreateH1("PhVelocity", "Velocity of scintillation photons (mm/ps)", 1000, 0, 0.4); // histo ID = 3
  analysisManager->CreateH1("PhLambdaDet", "Wavelength of detected photons (nm)", 1000, 0, 1500.); // histo ID = 4

  /*
  // Example of how to book 2D histograms
  //  analysisManager->CreateH2("PhLambdaV", "Wavelength (nm) vs velocity (mm/ps) of detected photons", 1000., 0, 0.4, 1000, 0, 1500.); // histo ID = 0
  //  analysisManager->CreateH2("PhVelTime", "Velocity (mm/ps) vs arrival time of detected photons (ps)", 1000, 0, 1000, 1000., 0, 0.4); // histo ID = 1
  */

  // Book event observables to save (Object ID = 0)
  analysisManager->CreateNtuple("OptEvt", "Number of produced optical photon");
  analysisManager->CreateNtupleDColumn("NPhotons"); // column ID = 0
  analysisManager->FinishNtuple();

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
  // The name of the histogram and ntuple files need to be set.
  // If no histogram files are booked, you still need to provide a name here,
  // but it's not used anywhere.
  analysisManager->SetFileName(fname_);
  analysisManager->SetNtupleFileName(0, ntuple_fname_);
  analysisManager->OpenFile();
}


void PetAnalysisRunAction::EndOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " end." << G4endl;

  auto analysisManager = G4AnalysisManager::Instance();

  analysisManager->Write();
  analysisManager->CloseFile();
}
