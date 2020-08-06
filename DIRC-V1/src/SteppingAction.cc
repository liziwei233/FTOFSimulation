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
/// \file analysis/shared/src/SteppingAction.cc
/// \brief Implementation of the SteppingAction class
//
//
// $Id: SteppingAction.cc 68015 2013-03-13 13:27:27Z gcosmo $
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "SteppingAction.hh"

#include "DetectorConstruction.hh"
#include "HistoManager.hh"
#include "G4RunManager.hh"
#include "TrackingAction.hh"
#include "G4Step.hh"
#include "G4UnitsTable.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  SteppingAction::SteppingAction(TrackingAction* aTrackingAction) 
: G4UserSteppingAction(),fTrackingAction(aTrackingAction),fHistoManager()
{
  fHistoManager = HistoManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{
  G4StepPoint* preStepPoint = aStep->GetPreStepPoint();
  G4StepPoint* postStepPoint = aStep->GetPostStepPoint();

  G4VPhysicalVolume *prePhyVolume = preStepPoint->GetPhysicalVolume();
  G4VPhysicalVolume *postPhyVolume = postStepPoint->GetPhysicalVolume();
  if(!postPhyVolume) return;

  const G4DynamicParticle* particle = aStep->GetTrack()->GetDynamicParticle();
  G4String particleName = particle->GetDefinition()->GetParticleName();

  // collect energy and track length step by step
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(prePhyVolume->GetName() =="Quartz"&&edep>1e-3)
  {
    //G4int QuartzID = prePhyVolume->GetCopyNo();
    G4int QuartzID = preStepPoint->GetTouchable()->GetVolume(1)->GetCopyNo();
    fTrackingAction->GetEventAction()->AddEdep(QuartzID,edep);
  }
  // check if we are in scoring volume
  if(postStepPoint->GetStepStatus()==fGeomBoundary&&particleName=="opticalphoton")
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

    if(boundary&&postPhyVolume->GetName()=="Cathode")
    {
      G4OpBoundaryProcessStatus theStatus=boundary->GetStatus();
      switch(theStatus){
        case Absorption: 
          break;
        case Detection:
          {
            G4int tagNu;

            if((aStep->GetTrack()->GetCreatorProcess()->GetProcessName())=="Cerenkov")
              if(aStep->GetTrack()->GetParentID()==1)  tagNu=1;
              else  tagNu=2;
            else tagNu =0;

            G4int CathodeID = postStepPoint->GetTouchable()->GetVolume(0)->GetCopyNo();
            G4int PhotonDetID = postStepPoint->GetTouchable()->GetVolume(1)->GetCopyNo();
            G4int SectorID = postStepPoint->GetTouchable()->GetVolume(2)->GetCopyNo();
            G4int ChannelX = PhotonDetID*4+CathodeID/4;
            G4int ChannelY = CathodeID%4;
            G4double GlobalTime = aStep->GetTrack()->GetGlobalTime();
            G4double LocalTime = aStep->GetTrack()->GetLocalTime();
            G4double TrackLength = aStep->GetTrack()->GetTrackLength();
            G4double particleKinetic = particle->GetKineticEnergy();

            G4ThreeVector Bornposition = aStep->GetTrack()->GetVertexPosition();
            G4ThreeVector Hitposition = aStep->GetTrack()->GetPosition();

            if(GlobalTime > 20.) return;
            fTrackingAction->GetEventAction()->
              AddPos(tagNu,SectorID,ChannelX,ChannelY,Hitposition,Bornposition,particleKinetic,GlobalTime,LocalTime,TrackLength);

            break;
          }
        default:
          break;
      }
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
