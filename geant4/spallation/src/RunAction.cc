#include "RunAction.hh"
#include "Analysis.hh"
#include "DetectorConstruction.hh"
// #include "PrimaryGeneratorAction.hh"
#include <G4GeneralParticleSource.hh>
#include <G4RunManager.hh>
#include <G4Gamma.hh>
#include <G4Electron.hh>
#include <G4AccumulableManager.hh>
#include <G4SystemOfUnits.hh>
#include <G4UnitsTable.hh>

RunAction::RunAction() :
G4UserRunAction()
{
  // Register created accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fAverageGammaEnergy);
  accumulableManager->RegisterAccumulable(fAverageElectronEnergy);
  accumulableManager->RegisterAccumulable(fTotalTrackLength);
  accumulableManager->RegisterAccumulable(fTotalEnergyDeposited);
  accumulableManager->RegisterAccumulable(fDirectDaughterNeutrons);


  // Uncomment the following 4 lines to enable analysis.
  // G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  // analysisManager->SetVerboseLevel(1);
  // analysisManager->SetFirstNtupleId(1);
  // analysisManager->SetFirstHistoId(1);

  // definition for SI prefixed greys
  //
  const G4double milligray = 1.e-3*gray;
  const G4double microgray = 1.e-6*gray;
  const G4double nanogray  = 1.e-9*gray;
  const G4double picogray  = 1.e-12*gray;

  new G4UnitDefinition("milligray", "mGy", "Dose", milligray);
  new G4UnitDefinition("microgray", "µGy", "Dose", microgray);
  new G4UnitDefinition("nanogray", "nGy", "Dose", nanogray);
  new G4UnitDefinition("picogray", "pGy", "Dose", picogray);
  new G4UnitDefinition("cm-2", "cm-2", "Fluence", 1/cm2);

  // Uncomment to write ROOT file
  // analysisManager->CreateH1("eDep", "Energy Deposited",  20, 50, 60);
  // analysisManager->OpenFile("task4");
}


void RunAction::BeginOfRunAction(const G4Run*)
{
  // Reset all accumulables to their initial values
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Reset();  // reset accumables
  fSecondaryNumbers.clear();    // clear secondaries taly
}

void RunAction::EndOfRunAction(const G4Run* run)
{
  // Retrieve the number of events produced in the run
  G4int nofEvents = run->GetNumberOfEvent();

  // Do nothing if no events were processed
  if (nofEvents == 0) return;

  // Merge accumulables
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->Merge();

  // Get target
  /*
  const DetectorConstruction* detectorConstruction
  = static_cast<const DetectorConstruction*>
  (G4RunManager::GetRunManager()->GetUserDetectorConstruction());

  // Get mass and volume of target

  G4double mass = detectorConstruction->GetScoringVolume()->GetMass();
  G4double volume = detectorConstruction->GetScoringVolume()->GetSolid()->GetCubicVolume();

  // Calculate dose & fluence

  G4double dose = fTotalEnergyDeposited.GetValue() / mass;
  G4double fluence = fTotalTrackLength.GetValue() / volume;
  */

  // Get current beam energy
  G4GeneralParticleSource* GPS;
  GPS = new G4GeneralParticleSource();
  G4String particle = GPS->GetCurrentSource()->GetParticleDefinition()->GetParticleName();
  G4double energy = GPS->GetCurrentSource()->GetEneDist()->GetMonoEnergy();

  if (IsMaster())
  {
    if (fDirectDaughterNeutrons.GetValue())
      {
        G4cout << "---------------------:: End of Run ::------------------------\n";
        G4cout << "There was " << nofEvents << " " << particle << "s with energy ";
        G4cout << energy / MeV << " MeV\n";
        G4cout << "There was " << fDirectDaughterNeutrons.GetValue() << " direct daughter";
        G4cout << " neutrons. This is a neutron multiplicity of ";
        G4cout <<  fDirectDaughterNeutrons.GetValue() / (G4double) nofEvents << "\n";
      }
  }
}

RunAction::~RunAction()
{
  // Uncomment to write ROOT file
  // G4AnalysisManager* man = G4AnalysisManager::Instance();
  // man->Write();
}

void RunAction::AddSecondary(const G4ParticleDefinition* particle,
  G4double energy)
{
  // adds particle to map if not already in (and starts at 1)
  // otherwise just increments secondary number
  fSecondaryNumbers[particle] += 1;

  if (particle == G4Gamma::Definition())
  {
    fAverageGammaEnergy += energy;
  }
  else if (particle == G4Electron::Definition())
  {
    fAverageElectronEnergy += energy;
  }
  return;
}

void RunAction::AddTrackLength(G4double trackLength)
{
  fTotalTrackLength += trackLength;
}

void RunAction::AddEnergyDeposited(G4double energyDeposited)
{
  fTotalEnergyDeposited += energyDeposited;
}

void RunAction::AddDaughterNeutron()
{
  fDirectDaughterNeutrons++;
}
