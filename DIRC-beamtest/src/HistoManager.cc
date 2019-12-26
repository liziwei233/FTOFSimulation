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
//
// $Id: HistoManager.cc 72249 2013-07-12 08:57:49Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#include "HistoManager.hh"
#include "G4UnitsTable.hh"
#include "TTree.h"
#include "TFile.h"
#include <pthread.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

pthread_mutex_t mutex_instance;
pthread_mutex_t mutex_fill;

HistoManager* HistoManager::fgInstance = 0;
HistoManager* HistoManager::Instance()
{
  if(fgInstance == NULL) 
  {
    pthread_mutex_lock(&mutex_instance);
    if(fgInstance == NULL)
      fgInstance = new HistoManager();
    pthread_mutex_unlock(&mutex_instance);
  }  
  return fgInstance;
}

  HistoManager::HistoManager(G4String foutname)
: fRootFile(0),fNtuple(0),fOutName(foutname)
{
  pthread_mutex_init(&mutex_instance,NULL);
  pthread_mutex_init(&mutex_fill,NULL);
  fgInstance = this;
  book();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HistoManager::~HistoManager()
{
  save();
  fgInstance = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HistoManager::book()
{
  G4cout<<"------>create rootfile: "<<fOutName<<G4endl;
  fRootFile = new TFile(fOutName.c_str(),"RECREATE");
  fNtuple = new TTree("DIRC","data of DIRC");
  fNtuple->Branch("PrimaryEnergy",    &fParticleInfo.fPrimaryEnergy);
  fNtuple->Branch("QuartzID",         &fParticleInfo.fQuartzID);
  fNtuple->Branch("TagNO",            &fParticleInfo.fTagNO);
  fNtuple->Branch("PhotoNu",          &fParticleInfo.fPhotoNu);
  fNtuple->Branch("ChannelX",         &fParticleInfo.fChannelX);
  fNtuple->Branch("ChannelY",         &fParticleInfo.fChannelY);
  fNtuple->Branch("PhotonKinetic",    &fParticleInfo.fPhotonKinetic);
  fNtuple->Branch("WaveLength",       &fParticleInfo.fWaveLength);
  
   fNtuple->Branch("FlightLength",    &fParticleInfo.fFlightLength);
  fNtuple->Branch("GlobalTime",       &fParticleInfo.fGlobalTime);
  fNtuple->Branch("LocalTime",        &fParticleInfo.fLocalTime);

  fNtuple->Branch("TrackLength",      &fParticleInfo.fTrackLength);

  fNtuple->Branch("TrackDx",         &fParticleInfo.fTrackDx);
  fNtuple->Branch("TrackDy",         &fParticleInfo.fTrackDy);
  fNtuple->Branch("TrackDz",         &fParticleInfo.fTrackDz);
  fNtuple->Branch("TrackPx",         &fParticleInfo.fTrackPx);
  fNtuple->Branch("TrackPy",         &fParticleInfo.fTrackPy);
  fNtuple->Branch("TrackPz",         &fParticleInfo.fTrackPz);
  fNtuple->Branch("PhotonDx",        &fParticleInfo.fPhotonDx);
  fNtuple->Branch("PhotonDy",        &fParticleInfo.fPhotonDy);
  fNtuple->Branch("PhotonDz",        &fParticleInfo.fPhotonDz);
  fNtuple->Branch("PhotonPx",        &fParticleInfo.fPhotonPx);
  fNtuple->Branch("PhotonPy",        &fParticleInfo.fPhotonPy);
  fNtuple->Branch("PhotonPz",        &fParticleInfo.fPhotonPz);

    fNtuple->Branch("ThetaX",          &fParticleInfo.fThetaX);
  fNtuple->Branch("ThetaZ",          &fParticleInfo.fThetaZ);
  fNtuple->Branch("ThetaC",          &fParticleInfo.fThetaC);


}

void HistoManager::save()
{
  fNtuple->Write();
  fRootFile->Close();
  G4cout<<"------> close rootfile: "<<fOutName<<G4endl;
}

void HistoManager::SetParticleInfo(ParticleInfo a)
{
  pthread_mutex_lock(&mutex_fill);
  fParticleInfo=a;
  fNtuple->Fill();
  pthread_mutex_unlock(&mutex_fill);
}
