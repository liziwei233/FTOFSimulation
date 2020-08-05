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
#include "G4SystemOfUnits.hh"
#include <iomanip>
#include <HistoManager.hh>
#include <TTree.h>
#include "RunAction.hh"
#include "TMath.h"

using namespace TMath;
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
  //G4double primaryEkin= particle->GetKineticEnergy(); 
  G4double primaryEkin= particle->GetTotalMomentum(); 
  G4String partName = particle->GetParticleDefinition()->GetParticleName();
  fRunAction->SetpartName(partName);
  fRunAction->Seteprimary(primaryEkin);
  fParticleInfo.fPrimaryEnergy=primaryEkin;
  fThetaC.clear();
  fPhotonBD.clear();
  fPhotonBP.clear();
  fPhotonTD.clear();
 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* aEvent)
{
  G4int evtNb = aEvent->GetEventID(); 
  //
  /*
  if (evtNb%100 ==0||fParticleInfo.fDecayChain.length()>1) 
    G4cout << " end of event "<< evtNb 
      << " :" + fParticleInfo.fDecayChain << G4endl;
      */
if (evtNb%100 ==0) 
    G4cout << " end of event "<< evtNb << G4endl;
  fHistoManager->SetParticleInfo(fParticleInfo);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
/*
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
void EventAction::AddPos(G4int tagNO,G4int ChannelX,G4int ChannelY,G4double particleKinetic,G4double GlobalTime,G4double LocalTime,G4double TrackLength)
{
  fParticleInfo.fPhotoNu += 1;
  fParticleInfo.fTagNO.push_back(tagNO);
  fParticleInfo.fChannelY.push_back(ChannelY);
  fParticleInfo.fChannelX.push_back(ChannelX);
  fParticleInfo.fPhotonKinetic.push_back(particleKinetic);
  fParticleInfo.fWaveLength.push_back(1240.7/particleKinetic/1e6);
  fParticleInfo.fGlobalTime.push_back(GlobalTime);
  fParticleInfo.fLocalTime.push_back(LocalTime);
  fParticleInfo.fTrackLength.push_back(TrackLength);
}
*/

    

void EventAction::AddPhoton(const G4Track* track,G4int Type,G4int SectorID,G4int ChannelX,G4int ChannelY,
    G4double PhotonKinetic,G4double GlobalTime,G4double LocalTime,G4double TrackLength,
    G4ThreeVector direction, G4ThreeVector hitposition)
{
  fParticleInfo.fPhotoNu += 1;
  fParticleInfo.fType.push_back(Type);
  fParticleInfo.fSectorID.push_back(SectorID);
  fParticleInfo.fChannelX.push_back(ChannelX);
  fParticleInfo.fChannelY.push_back(ChannelY);
  fParticleInfo.fPhotonKinetic.push_back(PhotonKinetic);
  fParticleInfo.fWaveLength.push_back(1240.7/PhotonKinetic/1e6);
  fParticleInfo.fGlobalTime.push_back(GlobalTime);
  fParticleInfo.fLocalTime.push_back(LocalTime);
  fParticleInfo.fTrackLength.push_back(TrackLength);

  G4double ThetaX = ATan(direction.y()/direction.x());
  G4double ThetaZ = Abs(ATan(direction.z()/direction.x()));
  G4double ThetaC = fThetaC[track]; 

  fParticleInfo.fThetaX.push_back(ThetaX);
  fParticleInfo.fThetaZ.push_back(ThetaZ);
  fParticleInfo.fThetaC.push_back(ThetaC);
  fParticleInfo.fPhotonHDx.push_back(direction.x());
  fParticleInfo.fPhotonHDy.push_back(direction.y());
  fParticleInfo.fPhotonHDz.push_back(direction.z());
  
  fParticleInfo.fPhotonHPx.push_back(hitposition.x());
  fParticleInfo.fPhotonHPy.push_back(hitposition.y());
  fParticleInfo.fPhotonHPz.push_back(hitposition.z());

  
  G4ThreeVector PhotonTD = fPhotonTD[track];
  G4ThreeVector PhotonBD = fPhotonBD[track];
  G4ThreeVector PhotonBP = fPhotonBP[track];

  fParticleInfo.fPhotonTDx.push_back(PhotonTD.x());
  fParticleInfo.fPhotonTDy.push_back(PhotonTD.y());
  fParticleInfo.fPhotonTDz.push_back(PhotonTD.z());

  fParticleInfo.fPhotonBDx.push_back(PhotonBD.x());
  fParticleInfo.fPhotonBDy.push_back(PhotonBD.y());
  fParticleInfo.fPhotonBDz.push_back(PhotonBD.z());
  
  fParticleInfo.fPhotonBPx.push_back(PhotonBP.x());
  fParticleInfo.fPhotonBPy.push_back(PhotonBP.y());
  fParticleInfo.fPhotonBPz.push_back(PhotonBP.z());
  
}

void EventAction::AddTrack(double FlightLength,G4ThreeVector direction, G4ThreeVector position)
{
  fParticleInfo.fFlightLength.push_back(FlightLength);
  fParticleInfo.fTrackDx.push_back(direction.x());
  fParticleInfo.fTrackDy.push_back(direction.y());
  fParticleInfo.fTrackDz.push_back(direction.z());
  
  // convert to center positon of quartz
  fParticleInfo.fTrackPx.push_back(position.x()+7.5*mm*direction.x()/direction.z());
  fParticleInfo.fTrackPy.push_back(position.y()+7.5*mm*direction.y()/direction.z());
  fParticleInfo.fTrackPz.push_back(position.z()+7.5*mm);
}

void EventAction::AddThetaC(const G4Track* track, G4ThreeVector direction,G4ThreeVector Sdirection,G4ThreeVector birthposition)
{
  //Is the photon repeat?
  if(fThetaC[track]!=0) return;
  G4double x=direction.x(),y=direction.y(),z=direction.z();
  G4double Sx=Sdirection.x(),Sy=Sdirection.y(),Sz=Sdirection.z();

  fThetaC[track]=ACos((x*Sx+y*Sy+z*Sz)/Sqrt((x*x+y*y+z*z)*(Sx*Sx+Sy*Sy+Sz*Sz)));

  

  fPhotonTD[track] = direction;
  fPhotonBD[track] = Sdirection;
  fPhotonBP[track] = birthposition;

}
