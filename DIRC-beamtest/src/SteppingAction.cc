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
#include "G4OpBoundaryProcess.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  SteppingAction::SteppingAction(TrackingAction* aTrackingAction) 
: G4UserSteppingAction(),fTrackingAction(aTrackingAction),fHistoManager(),FlightTime(0)
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
  G4Track* aTrack = aStep->GetTrack();
  G4VPhysicalVolume *prePhyVolume = preStepPoint->GetPhysicalVolume();
  G4VPhysicalVolume *postPhyVolume = postStepPoint->GetPhysicalVolume();
  if(!postPhyVolume) return;

  const G4TrackVector* secondary = aStep->GetSecondary();
  if(aTrack->GetTrackID()==1&&secondary&&secondary->size()!=0)
  {
    int Nsecondary = secondary->size();
    G4ThreeVector direction = aStep->GetDeltaPosition();
    for(int i=0;i<Nsecondary;i++)
    {
      const G4Track* track = secondary->at(i);
      if(track->GetDefinition()->GetParticleName()!="opticalphoton") continue;
      G4ThreeVector Sdirection = track->GetMomentumDirection();
      //G4ThreeVector birthposition = track->GetVertexPosition();
      G4ThreeVector birthposition = track->GetPosition();
      fTrackingAction->GetEventAction()->AddThetaC(track,direction,Sdirection,birthposition);
    }
  }

  if(aStep->GetTrack()->GetTrackID()==1
      //&&prePhyVolume->GetName()=="Sector"
      &&prePhyVolume->GetName()=="World"
      &&postPhyVolume->GetName()=="Quartz")
  {
    double FlightLength = aStep->GetTrack()->GetTrackLength();
    G4ThreeVector direction = postStepPoint->GetMomentumDirection();
    G4ThreeVector position = postStepPoint->GetPosition();
    fTrackingAction->GetEventAction()->AddTrack(FlightLength,direction,position);
  }

/*
  const G4DynamicParticle* particle = aStep->GetTrack()->GetDynamicParticle();
  G4String particleName = particle->GetDefinition()->GetParticleName();

  // collect energy and track length step by step
  //if(prePhyVolume->GetName() =="Quartz"&&edep>1e-3)
  if(aStep->GetTrack()->GetTrackID()==1 &&
      prePhyVolume->GetName()=="World" &&
      postPhyVolume->GetName()=="Quartz")
  {
    //G4double edep = aStep->GetTotalEnergyDeposit();
    G4int QuartzID = postStepPoint->GetTouchable()->GetVolume(1)->GetCopyNo();
    FlightTime = aStep->GetTrack()->GetGlobalTime();
    fTrackingAction->GetEventAction()->AddEdep(QuartzID,FlightTime);
  }
  // check if we are in scoring volume
  //if(postPhyVolume->GetName()=="Cathode"&&aStep->GetTrack()->GetTrackStatus()==fStopAndKill&&particleName=="opticalphoton")
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
              else  tagNu=0;
            //else tagNu =0;

            const G4int CathodeNu = 4;
            G4int CathodeID = postStepPoint->GetTouchable()->GetVolume(0)->GetCopyNo();
            G4int PhotonDetID = postStepPoint->GetTouchable()->GetVolume(1)->GetCopyNo();
            G4int ChannelX = PhotonDetID*CathodeNu+CathodeID/CathodeNu;
            G4int ChannelY = CathodeID%CathodeNu;
            G4double GlobalTime = aStep->GetTrack()->GetGlobalTime();
            //G4double LocalTime = aStep->GetTrack()->GetLocalTime();
            G4double LocalTime = GlobalTime-FlightTime;
            G4double TrackLength = aStep->GetTrack()->GetTrackLength();
            G4double particleKinetic = particle->GetKineticEnergy();

            if(GlobalTime > 20.) return;
            fTrackingAction->GetEventAction()->
              AddPos(tagNu,ChannelX,ChannelY,particleKinetic,GlobalTime,LocalTime,TrackLength);
            break;
          }
        default:
          break;
      }
    }
  }
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
