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
/// \file radioactivedecay/rdecay01/src/DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
//
//
// $Id: DetectorConstruction.cc 78307 2013-12-11 10:55:57Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "DetectorConstruction.hh"

#include "SteppingAction.hh"

#include "G4AffineTransform.hh"
#include "G4Navigator.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Trd.hh"
#include "G4Para.hh"
#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "G4UnionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"
#include "G4GlobalMagFieldMessenger.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(), physiWorld(NULL)
{
  m_SigmaAlpha = 0.1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume *DetectorConstruction::Construct()
{

  // ********************* define a material *********************************

  // ------------ Air ------------
  G4Material *air =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");

  //// ------------ Vacuum ------------
  //G4Material* Vacuum =
  //  G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

  //// ------------ Water -------------
  //  G4Material* Water =
  //    G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");

  //// ------------ Crystal ------------
  //G4Material* CsI =
  //  G4NistManager::Instance()->FindOrBuildMaterial("G4_CESIUM_IODIDE");
  //G4Material* BGO =
  //  G4NistManager::Instance()->FindOrBuildMaterial("G4_BGO");

  // ------------- Al ----------------
  G4Material *Al =
      G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");
  // ------------- CFRP --------------
  G4Element *C = new G4Element("Carbon", "C", 6., 12 * g / mole);
  G4Element *N = new G4Element("Nitrogen", "N", 7., 14.00 * g / mole);
  G4Element *H = new G4Element("Hydrogen", "H", 1., 1.01 * g / mole);
  G4Material *CFRP = new G4Material("quartz", 1.7 * g / cm3, 3);
  CFRP->AddElement(C, 95 * perCent);
  CFRP->AddElement(N, 4 * perCent);
  CFRP->AddElement(H, 1 * perCent);
  //
  // -----------------MPT of CFRP-----------------
  //
  //G4MaterialPropertiesTable *MPT_CFRP = new G4MaterialPropertiesTable();

  //G4double ErefractiveIndex_CFRP[] =
  //    {
  //        1. * eV, 4. * eV}; //
  //G4int nEntries = sizeof(ErefractiveIndex_CFRP) / sizeof(G4double);
  //G4double refractiveIndex_CFRP[] =
  //    {
  //        2.4168, 2.4168};
  //assert(sizeof(refractiveIndex_CFRP) == sizeof(ErefractiveIndex_CFRP));
  //MPT_CFRP->AddProperty("RINDEX", ErefractiveIndex_CFRP, refractiveIndex_CFRP, nEntries);
  //CFRP->SetMaterialPropertiesTable(MPT_CFRP);

  // ------------ SiO2 ---------------
  G4Element *Si = new G4Element("Silicon", "Si", 14., 28.1 * g / mole);
  G4Element *O = new G4Element("Oxygen", "O", 8., 16.00 * g / mole);

  G4Material *SiO2 = new G4Material("quartz", 2.2 * g / cm3, 2);
  SiO2->AddElement(Si, 1);
  SiO2->AddElement(O, 2);
  G4Material *PhotonDet = new G4Material("PhotonDet", 2.2 * g / cm3, 1);
  PhotonDet->AddElement(Si, 1);
  G4Material *Cathode = new G4Material("Cathode", 2.2 * g / cm3, 1);
  Cathode->AddElement(Si, 1);

  //
  // ************************ Generate & Add Material Properties Table ***********************
  //
  //
  // ----------------- quartz ------------------
  //

  //
  // -----------------optical photon energy-----------------
  //
  G4MaterialPropertiesTable *MPT_Quartz = new G4MaterialPropertiesTable();

  G4double ErefractiveIndex_quartz[] =
      {
          1.909 * eV, 1.939 * eV, 1.969 * eV, 2.001 * eV, 2.034 * eV, 2.068 * eV, 2.103 * eV, 2.139 * eV, 2.177 * eV, 2.216 * eV,
          2.256 * eV, 2.298 * eV, 2.341 * eV, 2.386 * eV, 2.433 * eV, 2.481 * eV, 2.532 * eV, 2.585 * eV, 2.640 * eV, 2.697 * eV,
          2.757 * eV, 2.820 * eV, 2.885 * eV, 2.954 * eV, 3.026 * eV, 3.102 * eV, 3.181 * eV, 3.265 * eV, 3.353 * eV, 3.446 * eV,
          3.545 * eV, 3.649 * eV, 3.760 * eV, 3.877 * eV, 4.002 * eV, 4.136 * eV}; //
  G4int nEntries = sizeof(ErefractiveIndex_quartz) / sizeof(G4double);
  G4double refractiveIndex_quartz[] =
      {
          1.4565, 1.4568, 1.4571, 1.4574, 1.4577, 1.4580, 1.4584, 1.4587, 1.4591, 1.4595,
          1.4599, 1.4603, 1.4608, 1.4613, 1.4618, 1.4623, 1.4629, 1.4635, 1.4641, 1.4648,
          1.4656, 1.4663, 1.4672, 1.4681, 1.4691, 1.4701, 1.4713, 1.4725, 1.4738, 1.4753,
          1.4769, 1.4787, 1.4806, 1.4827, 1.4851, 1.4878};
  assert(sizeof(refractiveIndex_quartz) == sizeof(ErefractiveIndex_quartz));
  MPT_Quartz->AddProperty("RINDEX", ErefractiveIndex_quartz, refractiveIndex_quartz, nEntries)->SetSpline(true);

  G4double Eabsorption[] =
      {
          1.907 * eV, 1.921 * eV, 1.946 * eV, 1.978 * eV, 2.019 * eV, 2.071 * eV, 2.117 * eV, 2.162 * eV, 2.215 * eV, 2.261 * eV,
          2.311 * eV, 2.375 * eV, 2.442 * eV, 2.498 * eV, 2.566 * eV, 2.634 * eV, 2.706 * eV, 2.772 * eV, 2.852 * eV, 2.937 * eV,
          3.011 * eV, 3.088 * eV, 3.174 * eV, 3.255 * eV, 3.335 * eV, 3.435 * eV, 3.524 * eV, 3.630 * eV, 3.737 * eV, 3.863 * eV,
          3.991 * eV, 4.120 * eV, 4.216 * eV};
  nEntries = sizeof(Eabsorption) / sizeof(G4double);
  G4double absorption[] =
      {
          2349 * m, 2283 * m, 2163 * m, 2026 * m, 1866 * m, 1689 * m, 1543 * m, 1423 * m, 1290 * m, 1188 * m,
          1086 * m, 975 * m, 873 * m, 798 * m, 714 * m, 647 * m, 581 * m, 527 * m, 465 * m, 417 * m,
          377 * m, 341 * m, 306 * m, 275 * m, 253 * m, 222 * m, 199 * m, 182 * m, 160 * m, 137 * m,
          124 * m, 106 * m, 102 * m};
  assert(sizeof(absorption) == sizeof(Eabsorption));
  MPT_Quartz->AddProperty("ABSLENGTH", Eabsorption, absorption, nEntries)->SetSpline("true");

  //G4double photonEnergy2[2] = {0.4*eV, 6.0*eV};
  //G4double scintilFast[2] = {1.00,1.00};
  //G4double scintilSlow[2] = {1.00,1.00};

  //MPT_Quartz ->AddProperty("FASTCOMPONENT", photonEnergy2,scintilFast,2) ->SetSpline("true");
  //MPT_Quartz ->AddProperty("SLOWCOMPONENT", photonEnergy2,scintilSlow,2) ->SetSpline("true");
  //MPT_Quartz ->AddConstProperty("SCINTILLATIONYIELD", 0/MeV);
  //MPT_Quartz ->AddConstProperty("RESOLUTIONSCALE", 1.0);
  //MPT_Quartz ->AddConstProperty("FASTTIMECONSTANT", 1*ns);
  //MPT_Quartz ->AddConstProperty("SLOWTIMECONSTANT", 10*ns);
  //MPT_Quartz ->AddConstProperty("YIELDRATIO", 1.0);

  SiO2->SetMaterialPropertiesTable(MPT_Quartz);

  //
  // ----------------- Air --------------------
  //
  G4double ErefractiveIndex_air[2] = {0.4 * eV, 6.0 * eV};
  G4double refractiveIndex_air[2] = {1.00, 1.00};

  G4MaterialPropertiesTable *MPT_Air = new G4MaterialPropertiesTable();
  MPT_Air->AddProperty("RINDEX", ErefractiveIndex_air, refractiveIndex_air, 2);

  G4cout << "Air G4MaterialPropertiesTable" << G4endl;

  air->SetMaterialPropertiesTable(MPT_Air);

  // Option to switch checking of Volumes Overlaps
  //
  G4bool checkOverlaps = true;
  //G4bool checkOverlaps = false;

  G4double fWorldLength = 10 * m;

  const int SectorNu = 4;
  G4double SectorT = 200 * mm;
  G4double SectorR1 = 545 * mm / sin(75 * deg);
  G4double SectorR2 = 1050 * mm;
  //G4double SectorZ = 1300 * mm;
  G4double SectorZ = 1500 * mm;
  const G4double SectorTArray[2] = {-0.5 * SectorT, 0.5 * SectorT};
  //* Radius of G4Polyhedra actually means the height of Polyhedra.
  const G4double SectorR1Array[2] = {SectorR1 * sin(75 * deg), SectorR1 * sin(75 * deg)};
  const G4double SectorR2Array[2] = {SectorR2 * sin(75 * deg), SectorR2 * sin(75 * deg)};

  //
  // ----------------- Shield ---------------
  //
  G4double ShieldT = 5 * mm;
  G4double ShieldR1 = SectorR1;
  G4double ShieldR2 = SectorR2 - 2 * ShieldT;
  const G4double ShieldTArray[2] = {-0.5 * SectorT + ShieldT, 0.5 * SectorT - ShieldT};
  const G4double ShieldR1Array[2] = {ShieldR1 * sin(75 * deg), ShieldR1 * sin(75 * deg)};
  const G4double ShieldR2Array[2] = {ShieldR2 * sin(75 * deg), ShieldR2 * sin(75 * deg)};

  //
  //----------------- Quartz ---------------
  //
  G4double QuartzT = 1.5 * cm;
  //G4double QuartzT = 5 * cm;
  G4double interval = 10 * mm;
  G4double QuartzR1 = SectorR1;
  G4double QuartzR2 = SectorR2 - 2 * interval;
  G4double QuartzZ = -SectorT/2.+ShieldT+interval+QuartzT/2.;
  const G4double QuartzTArray[2] = {-0.5 * QuartzT, 0.5 * QuartzT};
  const G4double QuartzR1Array[2] = {QuartzR1 * sin(75 * deg), QuartzR1 * sin(75 * deg)};
  const G4double QuartzR2Array[2] = {QuartzR2 * sin(75 * deg), QuartzR2 * sin(75 * deg)};

  //----------------- Coating ---------------
  G4double CoatingT = 0.5 * mm;

  const G4double UpCoatingTArray[2] = {-0.5 * QuartzT, 0.5 * QuartzT};
  const G4double UpCoatingR1Array[2] = {QuartzR2 * sin(75 * deg), QuartzR2 * sin(75 * deg)};
  const G4double UpCoatingR2Array[2] = {QuartzR2 * sin(75 * deg) + CoatingT, QuartzR2 * sin(75 * deg) + CoatingT};

  const G4double DownCoatingTArray[2] = {-0.5 * QuartzT, 0.5 * QuartzT};
  const G4double DownCoatingR1Array[2] = {QuartzR1 * sin(75 * deg) - CoatingT, QuartzR1 * sin(75 * deg) - CoatingT};
  const G4double DownCoatingR2Array[2] = {QuartzR1 * sin(75 * deg), QuartzR1 * sin(75 * deg)};

  //----------------- Senser  ---------------
  const int CathodeNuR = 4;
  const int CathodeNuC = 4;
  G4double PhotonDetW = 27.5 * mm;
  G4double PhotonDetT = 16.6 * mm;
  G4double PhotonDetH = PhotonDetW;
  G4double PhotonDetR = QuartzR2 * sin(75 * deg) - PhotonDetH / 2.;

  G4double QuartzW2 = (PhotonDetR)*tan(360. * deg / 2 / SectorNu / 3.) * 2.;
  G4double PhotonDetMargin = PhotonDetH/2*tan(360. * deg / 2 / SectorNu / 3.);
  const int PhotonDetNu = int((QuartzW2-PhotonDetMargin*2) / PhotonDetW);
  G4double PhotonDetGap = (QuartzW2 - PhotonDetMargin * 2 - PhotonDetW * PhotonDetNu) / PhotonDetNu;
  PhotonDetMargin += PhotonDetGap/2.;
  G4double CathodeW = 5.5 * mm;
  G4double CathodeT = 1.0 * mm;
  G4double CathodeH = 5.5 * mm;

  //
  // ************************ Volumes ******************************
  //

  // ----------------- Full sphere shape --------------------
  G4Box *
      solidWorld = new G4Box("World", fWorldLength / 2, fWorldLength / 2, fWorldLength / 2);
  G4LogicalVolume *
      logicWorld = new G4LogicalVolume(solidWorld, air, "World");

  physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);

  // ----------------- DIRC --------------------

  G4Tubs *
      solidDIRCTubs1 = new G4Tubs("DIRCTubs1", SectorR1 * sin(75 * deg), SectorR2, SectorZ + SectorT / 2., 0., 360 * deg);
  G4Tubs *
      solidDIRCTubs2 = new G4Tubs("DIRCTubs2", SectorR1 * sin(75 * deg), SectorR2, SectorZ - SectorT / 2., 0., 360 * deg);
  G4SubtractionSolid *
      solidDIRC = new G4SubtractionSolid("DIRC", solidDIRCTubs1, solidDIRCTubs2);
  G4LogicalVolume *
      logicDIRC = new G4LogicalVolume(solidDIRC, air, "DIRC");
  new G4PVPlacement(0, G4ThreeVector(), logicDIRC, "DIRC", logicWorld, false, 0, checkOverlaps);

  //
  // ----------------- Sector  -------------------
  //
  //* Radius of G4Polyhedra actually means the height of Polyhedra.
  G4Polyhedra *solidSector = new G4Polyhedra("Sector", 0, 90 * deg, 3, 2, SectorTArray, SectorR1Array, SectorR2Array);
  
  G4LogicalVolume *logicSector = new G4LogicalVolume(solidSector, air, "Sector");
  G4VPhysicalVolume *
      physiSector[SectorNu * 2];

  //for (int i = 0; i < 1; i++) /// at +Z
  for (int i = 0; i < SectorNu; i++) /// at +Z
  {
    G4double theta = 360. * deg / SectorNu;
    G4double theta0 = 0 * deg;
    G4RotationMatrix *rotSector = new G4RotationMatrix();
    rotSector->rotateZ(theta * i + theta0);
    physiSector[i] = new G4PVPlacement(rotSector, G4ThreeVector(0, 0, SectorZ), logicSector, "Sector", logicDIRC, false, i, checkOverlaps);
  }
  
  for (int i = 0; i < SectorNu; i++) /// at -Z
  {
    G4double theta = 360. * deg / SectorNu;
    G4double theta0 = 0 * deg;
    G4RotationMatrix *rotSector = new G4RotationMatrix();
    rotSector->rotateX(-180*deg);
    rotSector->rotateZ(theta * i + theta0);

    physiSector[i+SectorNu] = new G4PVPlacement(rotSector,G4ThreeVector(0, 0, -1*SectorZ),logicSector, "Sector", logicDIRC, false, i+SectorNu, checkOverlaps);
  }

  //
  // ----------------- Shielding Box  -------------------
  //
  G4Polyhedra *solidShieldOut = new G4Polyhedra("ShieldOut", 0, 90 * deg, 3, 2, SectorTArray, SectorR1Array, SectorR2Array);

  G4Polyhedra *solidShieldIn = new G4Polyhedra("ShieldIn", 0, 90 * deg, 3, 2, ShieldTArray, ShieldR1Array, ShieldR2Array);

  G4SubtractionSolid *
      subShield = new G4SubtractionSolid("Shield", solidShieldOut, solidShieldIn, 0, G4ThreeVector(ShieldT, ShieldT, 0));

  G4LogicalVolume *logicShield = new G4LogicalVolume(subShield, CFRP, "Shield");

  new G4PVPlacement(0, G4ThreeVector(0, 0, 0), logicShield, "Shield", logicSector, false, 0, checkOverlaps);
  
  //
  // ----------------- Quartz  -------------------
  //
  G4Polyhedra *solidQuartz = new G4Polyhedra("Quartz", 0, 90 * deg, 3, 2, QuartzTArray, QuartzR1Array, QuartzR2Array);
  G4LogicalVolume *logicQuartz = new G4LogicalVolume(solidQuartz, SiO2, "Quartz");
  G4VPhysicalVolume *physiQuartz = new G4PVPlacement(0, G4ThreeVector(interval, interval, QuartzZ ), logicQuartz, "Quartz", logicSector, false, 0, checkOverlaps);

  
  //
  // ----------------- Coating  -------------------
  //
  G4Polyhedra *solidUpCoating = new G4Polyhedra("UpCoating", 0, 90 * deg, 3, 2, UpCoatingTArray, UpCoatingR1Array, UpCoatingR2Array);
  G4LogicalVolume *logicUpCoating = new G4LogicalVolume(solidUpCoating, Al, "UpCoating");
  G4VPhysicalVolume *physiUpCoating = new G4PVPlacement(0, G4ThreeVector(interval, interval, QuartzZ ), logicUpCoating, "UpCoating", logicSector, false, 0, checkOverlaps);

  G4Polyhedra *solidDownCoating = new G4Polyhedra("DownCoating", 0, 90 * deg, 3, 2, DownCoatingTArray, DownCoatingR1Array, DownCoatingR2Array);
  G4LogicalVolume *logicDownCoating = new G4LogicalVolume(solidDownCoating, Al, "DownCoating");
  G4VPhysicalVolume *physiDownCoating = new G4PVPlacement(0, G4ThreeVector(interval, interval, QuartzZ ), logicDownCoating, "DownCoating", logicSector, false, 0, checkOverlaps);

  G4Box* solidSideCoatingBox = new G4Box("SideCoatingBox",(QuartzR2-QuartzR1)/2.,CoatingT/2.,QuartzT/2.);
  G4UnionSolid* solidSideCoating = new G4UnionSolid("SideCoating",solidSideCoatingBox,solidSideCoatingBox,
      new G4RotationMatrix(90*deg, 0, 0),G4ThreeVector(-QuartzR1/2-QuartzR2/2-CoatingT/2, QuartzR1/2+QuartzR2/2+CoatingT/2, 0));
  G4LogicalVolume* logicSideCoating = new G4LogicalVolume(solidSideCoating, Al, "SideCoating");
  G4VPhysicalVolume *physiSideCoating = new G4PVPlacement(0, G4ThreeVector(interval+QuartzR1/2+QuartzR2/2, interval-CoatingT/2., QuartzZ ), logicSideCoating, "SideCoating", logicSector, false, 0, checkOverlaps);

  //
  // ----------------- PhotonDet ---------------------------------
  //

  G4Box *solidPhotonDet = new G4Box("PhotonDet", PhotonDetW / 2, PhotonDetT / 2, PhotonDetH / 2);
  G4LogicalVolume *logicPhotonDet = new G4LogicalVolume(solidPhotonDet, PhotonDet, "PhotonDet");
  G4double PDW;
  G4double theta0, posX0, posY0;
  for (int i = 0; i < PhotonDetNu * 3; i++)
  {
    if (i < PhotonDetNu * 1)
    {
      theta0 = 75 * deg;
      posY0 = interval + PhotonDetR / sin(75 * deg);
      posX0 = interval;
    }
    else if (i < PhotonDetNu * 2)
    {
      theta0 = 45 * deg;
      posY0 = interval + PhotonDetR / sin(75 * deg) - QuartzW2 * cos(75 * deg);
      posX0 = interval + QuartzW2 * sin(75 * deg);
    }
    else
    {
      theta0 = 15 * deg;
      posY0 = interval + PhotonDetR / sin(75 * deg) - QuartzW2 * cos(75 * deg) - QuartzW2 * cos(45 * deg);
      posX0 = interval + QuartzW2 * sin(75 * deg) + QuartzW2 * sin(45 * deg);
    }

    PDW = PhotonDetMargin + PhotonDetW / 2. + (PhotonDetW + PhotonDetGap) * (i % PhotonDetNu);
    

    G4double PhotonDet_posX = posX0 + PDW * sin(theta0);
    G4double PhotonDet_posY = posY0 - PDW * cos(theta0);
    G4double PhotonDet_posZ = QuartzZ + PhotonDetT / 2. + QuartzT / 2.;

    G4RotationMatrix *rotSector = new G4RotationMatrix();
    rotSector->rotateZ(90 * deg - theta0);
    rotSector->rotateX(-90 * deg);
    new G4PVPlacement(rotSector, G4ThreeVector(PhotonDet_posX, PhotonDet_posY, PhotonDet_posZ), logicPhotonDet, "PhotonDet", logicSector, false, i, checkOverlaps);
  }

  //
  // ----------------- Cathode ---------------------------------
  //
  G4Box *
      solidCathode = new G4Box("Cathode", CathodeW / 2, CathodeT / 2, CathodeH / 2);
  G4LogicalVolume *
      logicCathode = new G4LogicalVolume(solidCathode, Cathode, "Cathode");
  G4VPhysicalVolume *
      physiCathode[CathodeNuR * CathodeNuC];
  for (int i = 0; i < CathodeNuR; i++)
    for (int j = 0; j < CathodeNuC; j++)
    {
      G4double Cathode_posX = -CathodeW * (CathodeNuR - 1) / 2 + CathodeW * i;
      G4double Cathode_posY = -PhotonDetT / 2 + CathodeT / 2;
      G4double Cathode_posZ = CathodeH * (CathodeNuC - 1) / 2 - CathodeH * j;
      physiCathode[CathodeNuC * i + j] = new G4PVPlacement(0, G4ThreeVector(Cathode_posX, Cathode_posY, Cathode_posZ), logicCathode, "Cathode", logicPhotonDet, false, CathodeNuC * i + j, checkOverlaps);
    }
  //
  // ----------------- Surfaces ---------------------------------
  //
  //

  // Quartz-Air Surface
  G4OpticalSurface *opQuartzAirSurface = new G4OpticalSurface("QuartzAirSurface");
  opQuartzAirSurface->SetType(dielectric_dielectric);
  opQuartzAirSurface->SetFinish(ground);
  opQuartzAirSurface->SetModel(unified);
  opQuartzAirSurface->SetSigmaAlpha(m_SigmaAlpha * deg);

  for (int i = 0; i < SectorNu * 2; i++)
    new G4LogicalBorderSurface("QuartzAirSurface", physiQuartz, physiSector[i], opQuartzAirSurface);

  // Mirror Surface
  G4OpticalSurface *opMirrorSurface = new G4OpticalSurface("MirrorSurface");
  opMirrorSurface->SetType(dielectric_metal);
  opMirrorSurface->SetFinish(polished);
  opMirrorSurface->SetModel(unified);
  new G4LogicalBorderSurface("SideCoatingQuartzSurface", physiQuartz, physiSideCoating, opMirrorSurface);

  // Absorber Surface
  G4OpticalSurface *opAbsorberSurface = new G4OpticalSurface("AbsorberSurface");
  opAbsorberSurface->SetType(dielectric_metal);
  opAbsorberSurface->SetFinish(polished);
  opAbsorberSurface->SetModel(unified);

  new G4LogicalBorderSurface("UpCoatingQuartzSurface", physiQuartz, physiUpCoating, opAbsorberSurface);
  new G4LogicalBorderSurface("DownCoatingQuartzSurface", physiQuartz, physiDownCoating, opAbsorberSurface);
  new G4LogicalSkinSurface("ShieldSurface", logicShield, opAbsorberSurface);

  // Quartz-Cathode Surface
  G4OpticalSurface *opQuartzCathodeSurface = new G4OpticalSurface("QuartzCathodeSurface");
  opQuartzCathodeSurface->SetType(dielectric_metal);
  opQuartzCathodeSurface->SetFinish(polished);
  opQuartzCathodeSurface->SetModel(unified);

  for (int k = 0; k < CathodeNuR * CathodeNuC; k++)
    new G4LogicalBorderSurface("ExpandCathodeSurface", physiQuartz, physiCathode[k], opQuartzCathodeSurface);

  //
  // ************************ Generate & Add Material Properties Table attached to the optical surfaces ******************************
  //
  G4double ephoton[2] = {0.4 * eV, 6.02 * eV};

  // ------------------- Optical Quartz Air Surface ---------------------
  G4double EreflecQuartzAir[] =
      {
          1.904 * eV, 2.055 * eV, 2.264 * eV, 2.584 * eV, 3.034 * eV, 3.163 * eV, 3.237 * eV, 3.314 * eV, 3.395 * eV, 3.474 * eV,
          3.581 * eV, 3.663 * eV, 3.737 * eV, 3.821 * eV, 3.881 * eV, 3.943 * eV, 3.993 * eV, 4.044 * eV, 4.074 * eV, 4.127 * eV,
          4.150 * eV, 4.181 * eV};
  nEntries = sizeof(EreflecQuartzAir) / sizeof(G4double);
  G4double reflecQuartzAir[] =
      {
          0.982, 0.981, 0.980, 0.979, 0.978, 0.977, 0.976, 0.976, 0.974, 0.973,
          0.971, 0.969, 0.967, 0.965, 0.963, 0.961, 0.960, 0.958, 0.957, 0.955,
          0.954, 0.953};
  assert(sizeof(EreflecQuartzAir) == sizeof(reflecQuartzAir));

  G4double specularlobeQuartzAir[2] = {0.0, 0.0};
  G4double specularspikeQuartzAir[2] = {1.0, 1.0};
  G4double backscatterQuartzAir[2] = {0, 0};

  G4MaterialPropertiesTable *SMPT_QuartzAir = new G4MaterialPropertiesTable();

  SMPT_QuartzAir->AddProperty("REFLECTIVITY", EreflecQuartzAir, reflecQuartzAir, nEntries);
  SMPT_QuartzAir->AddProperty("SPECULARLOBECONSTANT", ephoton, specularlobeQuartzAir, 2);
  SMPT_QuartzAir->AddProperty("SPECULARSPIKECONSTANT", ephoton, specularspikeQuartzAir, 2);
  SMPT_QuartzAir->AddProperty("BACKSCATTERCONSTANT", ephoton, backscatterQuartzAir, 2);

  opQuartzAirSurface->SetMaterialPropertiesTable(SMPT_QuartzAir);

  // ------------------- Optical Mirror Surface ---------------------
  G4double EreflecMirror[] =
      {
          1.906 * eV, 1.914 * eV, 1.922 * eV, 1.931 * eV, 1.941 * eV, 1.958 * eV, 1.977 * eV, 1.997 * eV, 2.024 * eV, 2.055 * eV,
          2.089 * eV, 2.124 * eV, 2.150 * eV, 2.176 * eV, 2.200 * eV, 2.217 * eV, 2.235 * eV, 2.256 * eV, 2.279 * eV, 2.300 * eV,
          2.339 * eV, 2.361 * eV, 2.381 * eV, 2.400 * eV, 2.420 * eV, 2.439 * eV, 2.450 * eV, 2.466 * eV, 2.477 * eV, 2.522 * eV,
          2.554 * eV, 2.643 * eV, 2.671 * eV, 2.701 * eV, 2.821 * eV, 2.902 * eV, 3.016 * eV, 3.171 * eV, 3.259 * eV, 3.343 * eV,
          3.399 * eV, 3.431 * eV, 3.506 * eV, 3.546 * eV, 3.586 * eV, 3.668 * eV, 3.705 * eV, 3.749 * eV, 3.846 * eV, 3.893 * eV,
          3.935 * eV, 3.991 * eV, 4.094 * eV, 4.163 * eV};
  nEntries = sizeof(EreflecMirror) / sizeof(G4double);
  G4double reflecMirror[] =
      {
          0.869, 0.872, 0.875, 0.878, 0.881, 0.883, 0.886, 0.890, 0.894, 0.898,
          0.903, 0.908, 0.912, 0.915, 0.918, 0.920, 0.921, 0.923, 0.924, 0.925,
          0.926, 0.927, 0.928, 0.929, 0.930, 0.931, 0.932, 0.933, 0.935, 0.936,
          0.937, 0.938, 0.939, 0.940, 0.939, 0.938, 0.937, 0.936, 0.935, 0.934,
          0.933, 0.932, 0.931, 0.930, 0.929, 0.928, 0.927, 0.926, 0.925, 0.924,
          0.923, 0.922, 0.921, 0.920};
  assert(sizeof(EreflecMirror) == sizeof(reflecMirror));
  G4MaterialPropertiesTable *SMPT_Mirror = new G4MaterialPropertiesTable();
  SMPT_Mirror->AddProperty("REFLECTIVITY", EreflecMirror, reflecMirror, nEntries);

  opMirrorSurface->SetMaterialPropertiesTable(SMPT_Mirror);

  // ------------------- Optical Absorber Surface ---------------------
  G4double reflecAbsorber[2] = {0, 0};
  G4MaterialPropertiesTable *SMPT_Absorber = new G4MaterialPropertiesTable();
  SMPT_Absorber->AddProperty("REFLECTIVITY", ephoton, reflecAbsorber, 2);

  opAbsorberSurface->SetMaterialPropertiesTable(SMPT_Absorber);

  // ------------------- Optical Quartz PhotonDet-Cathode Surface ---------------------
  G4double reflecQuartzPhotonDet[2] = {0, 0};
  G4double EeffiQuartzPhotonDet[] =
      {
          1.900 * eV, 1.928 * eV, 1.961 * eV, 1.990 * eV, 2.048 * eV, 2.118 * eV, 2.178 * eV, 2.231 * eV, 2.314 * eV, 2.386 * eV,
          2.436 * eV, 2.509 * eV, 2.565 * eV, 2.646 * eV, 2.724 * eV, 2.789 * eV, 2.867 * eV, 2.931 * eV, 3.027 * eV, 3.108 * eV,
          3.239 * eV, 3.333 * eV, 3.458 * eV, 3.592 * eV, 3.753 * eV, 3.896 * eV, 4.122 * eV};
  nEntries = sizeof(EeffiQuartzPhotonDet) / sizeof(G4double);
  G4double effiQuartzPhotonDet[] =
      {
          0.0386, 0.0414, 0.0456, 0.0490, 0.0572, 0.0660, 0.0743, 0.0837, 0.0966, 0.1088,
          0.1197, 0.1317, 0.1466, 0.1632, 0.1795, 0.2023, 0.2173, 0.2362, 0.2537, 0.2725,
          0.2928, 0.2928, 0.2894, 0.2793, 0.2632, 0.2571, 0.2452};
  assert(sizeof(EeffiQuartzPhotonDet) == sizeof(effiQuartzPhotonDet));
  G4MaterialPropertiesTable *SMPT_Quartz_Cathode = new G4MaterialPropertiesTable();
  SMPT_Quartz_Cathode->AddProperty("REFLECTIVITY", ephoton, reflecQuartzPhotonDet, 2);
  SMPT_Quartz_Cathode->AddProperty("EFFICIENCY", EeffiQuartzPhotonDet, effiQuartzPhotonDet, nEntries);
  opQuartzCathodeSurface->SetMaterialPropertiesTable(SMPT_Quartz_Cathode);

  //
  // ************************ Draw Detector Volumes  ******************************
  //

  G4VisAttributes *visAttributes = new G4VisAttributes(G4Colour(1.0, 1.0, 1.0)); //white
  visAttributes->SetVisibility(false);
  logicWorld->SetVisAttributes(visAttributes);
  logicDIRC->SetVisAttributes(visAttributes);
  logicSector->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.2)); //grey
  logicShield->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 1.0)); //blue
  logicQuartz->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0, 0.5)); //green
  logicPhotonDet->SetVisAttributes(visAttributes);
  visAttributes = new G4VisAttributes(G4Colour(1.0, 0.0, 1.0, 1.0)); //megenta
  logicCathode->SetVisAttributes(visAttributes);

  visAttributes = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 1.0)); //grey
  logicUpCoating->SetVisAttributes(visAttributes);
  logicDownCoating->SetVisAttributes(visAttributes);
  visAttributes = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 1.0)); //red
  logicSideCoating->SetVisAttributes(visAttributes);
  
  //
  //always return the physical World
  //

  //G4GDMLParser fParser;
  //fParser.Read("DIRC.gdml",false);
  //G4VPhysicalVolume* physiWorld = fParser.GetWorldVolume();
  //physiWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
  //fParser.Write("DIRC.gdml",physiWorld);
  
  //DumpStructure();
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
