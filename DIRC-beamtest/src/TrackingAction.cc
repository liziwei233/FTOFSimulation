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
/// \file DIRC/src/TrackingAction.cc
/// \brief Implementation of the TrackingAction class
//
//
// $Id: TrackingAction.cc 78307 2013-12-11 10:55:57Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "TrackingAction.hh"

#include "HistoManager.hh"
#include "EventAction.hh"

#include "G4Track.hh"
#include "G4ParticleTypes.hh"
#include "G4RunManager.hh"
#include "SteppingAction.hh"
#include "RunAction.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4DynamicParticle.hh"
#include "G4OpBoundaryProcess.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


  TrackingAction::TrackingAction(EventAction* aEventAction)
:G4UserTrackingAction(),fEventAction(aEventAction),fHistoManager()
{
  fHistoManager = HistoManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
  /*
  G4ParticleDefinition* particle = track->GetDefinition();
  fCharge = particle->GetPDGCharge();
  fMass   = particle->GetPDGMass();
  G4String name    = particle->GetParticleName();
  G4String typle   = particle->GetParticleType();
  G4int    ID      = track->GetTrackID();
  G4int    pID     = track->GetParentID();

  if (typle == "nucleus") {
    //build decay chain
    if(ID == 1) fEventAction->AddDecayChain(name+"("+std::to_string(pID)+")"+"("+std::to_string(ID)+")");
    else        fEventAction->AddDecayChain(" ---> " + name+"("+std::to_string(pID)+")"+"("+std::to_string(ID)+")");
  }
  */

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* track){
  G4String particleName = track->GetDefinition()->GetParticleName();
  G4StepPoint* postStepPoint = track->GetStep()->GetPostStepPoint();
  G4VPhysicalVolume *postPhyVolume = postStepPoint->GetPhysicalVolume();
  if(!postPhyVolume) return; 
   if(postStepPoint->GetStepStatus()==fGeomBoundary
      &&particleName=="opticalphoton"
      &&postPhyVolume->GetName()=="Cathode")
  {
    G4ProcessManager* pm=G4OpticalPhoton::OpticalPhoton()->GetProcessManager();
    G4int nprocesses=pm->GetProcessListLength();
    G4ProcessVector* pv=pm->GetProcessList();
    G4OpBoundaryProcess* boundary=NULL;
    for(int i=0;i<nprocesses;i++)
    {
      if((*pv)[i]->GetProcessName()=="OpBoundary")
      {
        boundary=(G4OpBoundaryProcess*)(*pv)[i];
        break;
      }
    }

    if(boundary)
    {
      G4OpBoundaryProcessStatus theStatus=boundary->GetStatus();
      switch(theStatus){
        case Absorption: 
          break;
        case Detection:
          {
            G4int Type;
            if(track->GetParentID()==1)  Type=1;
            else  Type=0;

            const G4int CathodeNu = 4;
            G4int CathodeID = postStepPoint->GetTouchable()->GetVolume(0)->GetCopyNo();
            G4int PhotonDetID = postStepPoint->GetTouchable()->GetVolume(1)->GetCopyNo();
            //G4int SectorID = postStepPoint->GetTouchable()->GetVolume(2)->GetCopyNo();
            G4int ChannelX = PhotonDetID*CathodeNu+CathodeID/CathodeNu;
            G4int ChannelY = CathodeID%CathodeNu;
            G4double GlobalTime = track->GetGlobalTime();
            G4double LocalTime = track->GetLocalTime();
            G4double TrackLength = track->GetTrackLength();
            G4double PhotonKinetic = track->GetDynamicParticle()->GetKineticEnergy();
            G4ThreeVector direction = track->GetVertexMomentumDirection();
            G4ThreeVector position = track->GetVertexPosition();
          

            if(GlobalTime >20.) return;
            fEventAction->AddPhoton(track,Type,0,ChannelX,ChannelY,
                PhotonKinetic,GlobalTime,LocalTime,TrackLength,
                direction,position);

            break;
          }
        default:
          break;
      }
    }
  } 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

