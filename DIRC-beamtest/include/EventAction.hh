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
/// \file DIRC/include/EventAction.hh
/// \brief Definition of the EventAction class
//
// $Id: EventAction.hh 68017 2013-03-13 13:29:53Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include "HistoManager.hh"
#include "PrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "DetectorConstruction.hh"
#include "RunAction.hh"
#include "G4Track.hh"
#include <map>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RunAction*);
    ~EventAction();

  public:
    virtual void   BeginOfEventAction(const G4Event*);
    virtual void   EndOfEventAction(const G4Event*);
    RunAction* GetRunAction(){return fRunAction;};
    HistoManager* GetHistoManager(){return fHistoManager;};

    ///set ParticleInfo
    void AddDecayChain(G4String val) {fParticleInfo.fDecayChain += val;};
    //void AddEdep(G4int,G4double);
    //void AddPos(G4int,G4int,G4int,G4double,G4double,G4double,G4double);
    void AddPhoton(const G4Track*,
        G4int,G4int,G4int,G4int,
        G4double,G4double,G4double,G4double,
        G4ThreeVector,G4ThreeVector);
    void AddTrack(double,G4ThreeVector,G4ThreeVector);
    void AddThetaC(const G4Track*,G4ThreeVector,G4ThreeVector,G4ThreeVector);
    std::map<const G4Track*,bool> flag;

  private:
    ParticleInfo fParticleInfo;
    RunAction* fRunAction;
    HistoManager* fHistoManager;
    std::map<const G4Track*,double> fThetaC;
    std::map<const G4Track*,G4ThreeVector> fPhotonBD;
    std::map<const G4Track*,G4ThreeVector> fPhotonBP;
    std::map<const G4Track*,G4ThreeVector> fPhotonTD;
    
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


