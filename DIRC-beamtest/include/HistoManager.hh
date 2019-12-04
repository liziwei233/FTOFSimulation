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
    G4int    fPhotoNu;
    std::vector<G4int> fQuartzID;
    std::vector<G4double> fEnergyDep;
    std::vector<G4int> fTagNO;
    std::vector<G4int> fChannelY;
    std::vector<G4int> fChannelX;
    std::vector<G4double> fPhotonKinetic;
    std::vector<G4double> fWaveLength;
    std::vector<G4double> fGlobalTime;
    std::vector<G4double> fLocalTime;
    std::vector<G4double> fTrackLength;
    G4String     fDecayChain;                   

    void reset()
    {
      fPrimaryEnergy = 0;
      fPhotoNu = 0;
      fDecayChain = "";
      fQuartzID.clear();
      fEnergyDep.clear();
      fTagNO.clear();
      fChannelY.clear();
      fChannelX.clear();
      fPhotonKinetic.clear();
      fWaveLength.clear();
      fGlobalTime.clear();
      fLocalTime.clear();
      fTrackLength.clear();
    };

    ParticleInfo() {
      fPrimaryEnergy = 0;
      fPhotoNu = 0;
      fDecayChain = "";
      fQuartzID.clear();
      fEnergyDep.clear();
      fTagNO.clear();
      fChannelY.clear();
      fChannelX.clear();
      fPhotonKinetic.clear();
      fWaveLength.clear();
      fGlobalTime.clear();
      fLocalTime.clear();
      fTrackLength.clear();
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

