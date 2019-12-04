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
/// \file DIRC/src/EventAction.cc
/// \brief Implementation of the EventAction class
//
// $Id: EventAction.cc 68030 2015-03-13 13:51:27Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "EventAction.hh"

#include "G4Event.hh"
#include <iomanip>
#include <HistoManager.hh>
#include <TTree.h>
#include "RunAction.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

  EventAction::EventAction(RunAction* aRunAction)
:G4UserEventAction(),fRunAction(aRunAction),fHistoManager()
{
  fHistoManager = HistoManager::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* aEvent)
{
  fParticleInfo.reset();

  G4PrimaryParticle* particle= aEvent->GetPrimaryVertex()->GetPrimary(); 
  G4double primaryEkin= particle->GetTotalEnergy(); 
  G4String partName = particle->GetParticleDefinition()->GetParticleName();
  fRunAction->SetpartName(partName);
  fRunAction->Seteprimary(primaryEkin);
  fParticleInfo.fPrimaryEnergy=primaryEkin;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* aEvent)
{
  G4int evtNb = aEvent->GetEventID(); 
  //
  if (evtNb%100 ==0||fParticleInfo.fDecayChain.length()>1) 
    G4cout << " end of event "<< evtNb 
      << " :" + fParticleInfo.fDecayChain << G4endl;

  fHistoManager->SetParticleInfo(fParticleInfo);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
void EventAction::AddEdep(G4int QuartzID,G4double edep)
{
  for(size_t i_copyNo=0; i_copyNo!=(fParticleInfo.fQuartzID.size()); ++i_copyNo){
    if(fParticleInfo.fQuartzID.at(i_copyNo)==QuartzID){
      fParticleInfo.fEnergyDep.at(i_copyNo) =  fParticleInfo.fEnergyDep.at(i_copyNo) + edep;
      return;
    }
  }
  fParticleInfo.fQuartzID.push_back(QuartzID);
  fParticleInfo.fEnergyDep.push_back(edep);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
void EventAction::AddPos(G4int tagNO,G4int SectorID,G4int ChannelX,G4int ChannelY,G4double particleKinetic,G4double GlobalTime,G4double LocalTime,G4double TrackLength)
{
  fParticleInfo.fPhotoNu += 1;
  fParticleInfo.fTagNO.push_back(tagNO);
  fParticleInfo.fSectorID.push_back(SectorID);
  fParticleInfo.fChannelX.push_back(ChannelX);
  fParticleInfo.fChannelY.push_back(ChannelY);
  fParticleInfo.fPhotonKinetic.push_back(particleKinetic);
  fParticleInfo.fWaveLength.push_back(1240.7/particleKinetic/1e6);
  fParticleInfo.fGlobalTime.push_back(GlobalTime);
  fParticleInfo.fLocalTime.push_back(LocalTime);
  fParticleInfo.fTrackLength.push_back(TrackLength);
}
