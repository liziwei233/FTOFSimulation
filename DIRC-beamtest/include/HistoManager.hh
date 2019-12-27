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
// $Id: HistoManager.hh 68017 2013-03-13 13:29:53Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 

#ifndef HistoManager_h
#define HistoManager_h 1

#include "globals.hh"
#include <vector>
#include "g4root.hh"
#include <G4ThreeVector.hh>
#include <TTree.h>
#include <TFile.h>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
class ParticleInfo
{
  public:
      G4double fPrimaryEnergy;
    std::vector<G4double> fTrackDx;
    std::vector<G4double> fTrackDy;
    std::vector<G4double> fTrackDz;
    std::vector<G4double> fTrackPx;
    std::vector<G4double> fTrackPy;
    std::vector<G4double> fTrackPz;
    std::vector<G4double> fFlightLength;
    G4int    fPhotoNu;
    std::vector<G4int> fType;
    std::vector<G4int> fSectorID;
    std::vector<G4int> fQuartzID;
    std::vector<G4int> fChannelX;
    std::vector<G4int> fChannelY;
    std::vector<G4double> fPhotonKinetic;
    std::vector<G4double> fWaveLength;
    std::vector<G4double> fGlobalTime;
    std::vector<G4double> fLocalTime;
    std::vector<G4double> fTrackLength;
    std::vector<G4double> fThetaX;
    std::vector<G4double> fThetaZ;
    std::vector<G4double> fThetaC;
    std::vector<G4double> fPhotonDx;
    std::vector<G4double> fPhotonDy;
    std::vector<G4double> fPhotonDz;
    std::vector<G4double> fPhotonBPx;
    std::vector<G4double> fPhotonBPy;
    std::vector<G4double> fPhotonBPz;
    std::vector<G4double> fPhotonHPx;
    std::vector<G4double> fPhotonHPy;
    std::vector<G4double> fPhotonHPz;
   
    G4String     fDecayChain;                   

    void reset()
    {
      fPrimaryEnergy = 0;
      fTrackDx.clear();
      fTrackDy.clear();
      fTrackDz.clear();
      fTrackPx.clear();
      fTrackPy.clear();
      fTrackPz.clear();
      fFlightLength.clear();
      fPhotoNu = 0;
      fType.clear();
      fSectorID.clear();
      fQuartzID.clear();
      fChannelX.clear();
      fChannelY.clear();
      fPhotonKinetic.clear();
      fWaveLength.clear();
      fGlobalTime.clear();
      fLocalTime.clear();
      fTrackLength.clear();
      fThetaX.clear();
      fThetaZ.clear();
      fThetaC.clear();
      fPhotonDx.clear();
      fPhotonDy.clear();
      fPhotonDz.clear();
      fPhotonBPx.clear();
      fPhotonBPy.clear();
      fPhotonBPz.clear();
      fPhotonHPx.clear();
      fPhotonHPy.clear();
      fPhotonHPz.clear();

      fDecayChain = "";
      
    };

    ParticleInfo() {
      reset();
    };
    ~ParticleInfo(){};
};

class HistoManager
{
  public:
    HistoManager(G4String foutname="DIRC.root");
    ~HistoManager();
    static HistoManager* Instance();

  public:
    void save();
    void book();

    void SetParticleInfo(ParticleInfo);

  public:  
    ParticleInfo fParticleInfo;
    TFile* fRootFile;
    TTree* fNtuple;

  private:
    static HistoManager* fgInstance;
    G4String fOutName;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

