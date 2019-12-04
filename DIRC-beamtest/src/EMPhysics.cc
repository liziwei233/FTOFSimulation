//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id: EMPhysics.cc 68752 2013-04-05 10:23:47Z gcosmo $
//
/// \file optical//src/EMPhysics.cc
/// \brief Implementation of the EMPhysics class
//
//
#include "EMPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  EMPhysics::EMPhysics(const G4String& name)
:  G4VPhysicsConstructor(name)
{
  fPhotoEffect = NULL;
  fComptonEffect = NULL;
  fPairProduction = NULL;
  fElectronMultipleScattering = NULL;
  fElectronIonisation = NULL; 
  fElectronBremsStrahlung = NULL;
  fPositronMultipleScattering = NULL;
  fPositronIonisation = NULL; 
  fPositronBremsStrahlung = NULL;
  fAnnihilation = NULL;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EMPhysics::~EMPhysics() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4Gamma.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
//#include "G4Muon.hh"

#include "G4NeutrinoE.hh"
#include "G4AntiNeutrinoE.hh"

void EMPhysics::ConstructParticle()
{
  G4BosonConstructor bConstructor;
  bConstructor.ConstructParticle();

  G4LeptonConstructor lConstructor;
  lConstructor.ConstructParticle();

  G4MesonConstructor mConstructor;
  mConstructor.ConstructParticle();

  G4BaryonConstructor rConstructor;
  rConstructor.ConstructParticle();

  G4IonConstructor iConstructor;
  iConstructor.ConstructParticle(); 

  //    // gamma
  //    G4Gamma::GammaDefinition();
  //
  //    // electron
  //    G4Electron::ElectronDefinition();
  //    G4Positron::PositronDefinition();
  //    G4NeutrinoE::NeutrinoEDefinition();
  //    G4AntiNeutrinoE::AntiNeutrinoEDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#include "G4EmStandardPhysics.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"
#include "G4EmParameters.hh"
#include "G4LossTableManager.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4MuMultipleScattering.hh"
#include "G4hMultipleScattering.hh"

#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hIonisation.hh"

#include "G4PhysicsListHelper.hh"
#include "G4BuilderType.hh"
#include "G4EmModelActivator.hh"

#include "G4ProcessManager.hh"

// factory
#include "G4PhysicsConstructorFactory.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EMPhysics::ConstructProcess()
{
  //    G4ProcessManager* pmanager = 0;
  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();

    if (particleName == "gamma") {
      // gamma
      // Construct processes for gamma
      //    pmanager = G4Gamma::Gamma()->GetProcessManager();
      pmanager->AddDiscreteProcess(new G4GammaConversion());
      pmanager->AddDiscreteProcess(new G4ComptonScattering());
      pmanager->AddDiscreteProcess(new G4PhotoElectricEffect());
    } else if (particleName == "e-") {

      //electron
      // Construct processes for electron
      //   pmanager = G4Electron::Electron()->GetProcessManager();
      pmanager->AddProcess(new G4eMultipleScattering(),-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1, 3, 3);

    } else if (particleName == "e+") {
      //positron
      // Construct processes for positron
      //   pmanager = G4Positron::Positron()->GetProcessManager();
      pmanager->AddProcess(new G4eMultipleScattering(),-1, 1, 1);
      pmanager->AddProcess(new G4eIonisation(),       -1, 2, 2);
      pmanager->AddProcess(new G4eBremsstrahlung(),   -1, 3, 3);
      pmanager->AddProcess(new G4eplusAnnihilation(),  0,-1, 4);

    } else if (particleName == "mu+" ||
        particleName == "mu-"    ) {
      //muon
      // Construct processes for muon
      // pmanager = G4Muon::Muon()->GetProcessManager();
      pmanager->AddProcess(new G4MuMultipleScattering(),-1, 1, 1);
      pmanager->AddProcess(new G4MuIonisation(),      -1, 2, 2);
      pmanager->AddProcess(new G4MuBremsstrahlung(),  -1, 3, 3);
      pmanager->AddProcess(new G4MuPairProduction(),  -1, 4, 4);

    } else {
      if ((particle->GetPDGCharge()!=0.0) &&
          (particle->GetParticleName()!="chargedgeantino")) {
        // all others charged particles except geantino
        pmanager->AddProcess(new G4hMultipleScattering(),-1,1,1);
        pmanager->AddProcess(new G4hIonisation(),       -1,2,2);
      }
    }
  }  
}

