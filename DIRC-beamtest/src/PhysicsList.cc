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
// $Id: PhysicsList.cc 68752 2013-04-05 10:23:47Z gcosmo $
//
/// \file optical//src/PhysicsList.cc
/// \brief Implementation of the PhysicsList class
//
//
#include "PhysicsList.hh"
/*
#include "DecayPhysics.hh"
#include "OpticalPhysics.hh"
#include "EMPhysics.hh"
#include "MyHadronInelasticPhysics.hh"
#include "MyHadronElasticPhysics.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "QGSP_BERT.hh"
*/
#include "G4VUserPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4OpticalProcessIndex.hh"

#include "G4SystemOfUnits.hh"

#include "G4BosonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::PhysicsList() : G4VModularPhysicsList()
{
  // default cut value  (1.0mm)
  defaultCutValue = 1.0*mm;
  /*
  // General Physics
  //RegisterPhysics( new DecayPhysics("decay") );

  // EM Physics
  RegisterPhysics( new EMPhysics("standard EM"));

  // Hadron Physics
  //RegisterPhysics(new MyHadronInelasticPhysics("hInelastic QGSP_BERT",1));
  //RegisterPhysics(new MyHadronElasticPhysics(0));

  // Optical Physics
  RegisterPhysics(new OpticalPhysics("optical"));

*/

  RegisterPhysics(new G4EmStandardPhysics);
	RegisterPhysics(new G4OpticalPhysics);
	RegisterPhysics(new G4DecayPhysics);

	RegisterPhysics(new G4StepLimiterPhysics);
    
    SetDefaultCutValue(defaultCutValue);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PhysicsList::~PhysicsList() {}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::SetCuts(){
  //  " G4VUserPhysicsList::SetCutsWithDefault" method sets
  //   the default cut value for all particle types
  SetCutsWithDefault();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PhysicsList::ConstructParticle(){
  G4VModularPhysicsList::ConstructParticle();

  G4BosonConstructor pBosonConstructor;
  pBosonConstructor.ConstructParticle();

  G4LeptonConstructor pLeptonConstructor;
  pLeptonConstructor.ConstructParticle();

  G4MesonConstructor pMesonConstructor;
  pMesonConstructor.ConstructParticle();

  G4BaryonConstructor pBaryonConstructor;
  pBaryonConstructor.ConstructParticle();

  G4IonConstructor pIonConstructor;
  pIonConstructor.ConstructParticle();

  G4ShortLivedConstructor pShortLivedConstructor;
  pShortLivedConstructor.ConstructParticle();

}
