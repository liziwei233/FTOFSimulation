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
: G4VUserDetectorConstruction()
{
  m_SigmaAlpha = 0.1;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{

  // ********************* define a material *********************************

  // ------------ Air ------------  
  G4Material* air =
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
  G4Material* Al =
    G4NistManager::Instance()->FindOrBuildMaterial("G4_Al"); 

  // ------------ SiO2 ---------------
  G4Element* Si = new G4Element("Silicon","Si",14., 28.1*g/mole);
  G4Element* O = new G4Element("Oxygen","O",8.,16.00*g/mole);

  G4Material* SiO2 = new G4Material("quartz",2.2*g/cm3,2);
  SiO2->AddElement(Si,1);
  SiO2->AddElement(O,2);
  G4Material* PhotonDet = new G4Material("PhotonDet",2.2*g/cm3,1);
  PhotonDet->AddElement(Si,1);
  G4Material* Cathode = new G4Material("Cathode",2.2*g/cm3,1);
  Cathode->AddElement(Si,1);

  //
  // ************************ Generate & Add Material Properties Table ***********************
  //
  //
  // ----------------- quartz ------------------
  //

  //
  // -----------------optical photon energy-----------------
  //
  G4MaterialPropertiesTable* MPT_Quartz = new G4MaterialPropertiesTable();

  G4double ErefractiveIndex_quartz[] = 
  { 
    1.909*eV,	1.939*eV,	1.969*eV,	2.001*eV,	2.034*eV,	2.068*eV,	2.103*eV,	2.139*eV,	2.177*eV,	2.216*eV,	
    2.256*eV,	2.298*eV,	2.341*eV,	2.386*eV,	2.433*eV,	2.481*eV,	2.532*eV,	2.585*eV,	2.640*eV,	2.697*eV,	
    2.757*eV,	2.820*eV,	2.885*eV,	2.954*eV,	3.026*eV,	3.102*eV,	3.181*eV,	3.265*eV,	3.353*eV,	3.446*eV,	
    3.545*eV,	3.649*eV,	3.760*eV,	3.877*eV,	4.002*eV,	4.136*eV
  }; //  
  G4int nEntries = sizeof(ErefractiveIndex_quartz)/sizeof(G4double);
  G4double refractiveIndex_quartz[] = 
  { 
    1.4565,	1.4568,	1.4571,	1.4574,	1.4577,	1.4580,	1.4584,	1.4587,	1.4591,	1.4595,	
    1.4599,	1.4603,	1.4608,	1.4613,	1.4618,	1.4623,	1.4629,	1.4635,	1.4641,	1.4648,	
    1.4656,	1.4663,	1.4672,	1.4681,	1.4691,	1.4701,	1.4713,	1.4725,	1.4738,	1.4753,	
    1.4769,	1.4787,	1.4806,	1.4827,	1.4851,	1.4878
  };
  assert(sizeof(refractiveIndex_quartz) == sizeof(ErefractiveIndex_quartz));
  MPT_Quartz ->AddProperty("RINDEX", ErefractiveIndex_quartz, refractiveIndex_quartz, nEntries) ->SetSpline(true);

  G4double Eabsorption[] = 
  {
    1.907*eV,	1.921*eV,	1.946*eV,	1.978*eV,	2.019*eV,	2.071*eV,	2.117*eV,	2.162*eV,	2.215*eV,	2.261*eV,	
    2.311*eV,	2.375*eV,	2.442*eV,	2.498*eV,	2.566*eV,	2.634*eV,	2.706*eV,	2.772*eV,	2.852*eV,	2.937*eV,	
    3.011*eV,	3.088*eV,	3.174*eV,	3.255*eV,	3.335*eV,	3.435*eV,	3.524*eV,	3.630*eV,	3.737*eV,	3.863*eV,	
    3.991*eV,	4.120*eV,	4.216*eV
  };   
  nEntries = sizeof(Eabsorption)/sizeof(G4double);
  G4double absorption[] = 
  {
    2349*m,	2283*m,	2163*m,	2026*m,	1866*m,	1689*m,	1543*m,	1423*m,	1290*m,	1188*m,	
    1086*m,	975*m,  873*m,  798*m,  714*m,  647*m,  581*m,  527*m,  465*m,  417*m,  
    377*m,  341*m,  306*m,  275*m,  253*m,  222*m,  199*m,  182*m,  160*m,  137*m,  
    124*m,  106*m, 102*m
  };   
  assert(sizeof(absorption) == sizeof(Eabsorption));
  MPT_Quartz ->AddProperty("ABSLENGTH", Eabsorption,absorption,nEntries) ->SetSpline("true");

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

  SiO2 ->SetMaterialPropertiesTable(MPT_Quartz);

  //
  // ----------------- Air --------------------
  //
  G4double ErefractiveIndex_air[2] = {0.4*eV, 6.0*eV};   
  G4double refractiveIndex_air[2] = {1.00, 1.00};  

  G4MaterialPropertiesTable* MPT_Air = new G4MaterialPropertiesTable();
  MPT_Air ->AddProperty("RINDEX",ErefractiveIndex_air, refractiveIndex_air, 2);

  G4cout << "Air G4MaterialPropertiesTable" << G4endl;

  air ->SetMaterialPropertiesTable(MPT_Air);


  // Option to switch checking of Volumes Overlaps
  //
  G4bool checkOverlaps = true;

  G4double fWorldLength = 5*m;

  const int CathodeNu = 4;
  G4double  PhotonDetW   = 27.5*mm;
  G4double  PhotonDetT   = 16.6*mm;
  G4double  PhotonDetL   = PhotonDetW;
  G4double  CathodeW  = 5.5*mm;
  G4double  CathodeT  = 1.0*mm;
  G4double  CathodeL  = 22.0*mm;

  G4double  QuartzR = CathodeL;
  G4double  QuartzL = 50.*cm;
  G4double  QuartzW = 12.*cm;
  G4double  QuartzH = 1.5*cm;

  G4double  AlT     = 0.5*mm; 

  const int PhotonDetNu  = 4;
  G4double  PhotonDetGap = 3.33*mm;


  G4double  ShieldL = 54.*cm;
  G4double  ShieldW = 13.*cm;
  G4double  ShieldH = 10.*cm;
  G4double  ShieldT = 1.*mm;
  //
  // ************************ Volumes ******************************
  //

  // ----------------- Full sphere shape --------------------
  G4Box*
    solidWorld = new G4Box("World", fWorldLength/2, fWorldLength/2, fWorldLength/2);
  G4LogicalVolume*                         
    logicWorld = new G4LogicalVolume(solidWorld, air, "World");
  G4VPhysicalVolume*                                   
    physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0);
  // ----------------- Shielding Box  -------------------
  // 
  G4Box*  
    solidShieldP1 = new G4Box("ShieldP1", 0.5*ShieldL+ShieldT, 0.5*ShieldW+ShieldT, 0.5*ShieldH+ShieldT);
  G4Box*  
    solidShieldP2 = new G4Box("ShieldP2", 0.5*ShieldL, 0.5*ShieldW, 0.5*ShieldH);
  G4SubtractionSolid*
    solidShield = new G4SubtractionSolid("Shield",solidShieldP1,solidShieldP2);
  G4LogicalVolume*
    logicShield = new G4LogicalVolume(solidShield, Al, "Shield"); 
  new G4PVPlacement(0, G4ThreeVector(0,0,0),
      logicShield, "Shield", logicWorld, false, 0, checkOverlaps);    
  //
  // ----------------- Quartz  -------------------
  // 
  G4Box*  
    solidQuartz = new G4Box("Quartz", 0.5*QuartzL, 0.5*QuartzW, 0.5*QuartzH);
  G4LogicalVolume*
    logicQuartz = new G4LogicalVolume(solidQuartz, SiO2, "Quartz"); 
  G4VPhysicalVolume *physiQuartz = new G4PVPlacement(0, G4ThreeVector(-QuartzR/2.,0,0),
      logicQuartz, "Quartz", logicWorld, false, 0, checkOverlaps);

  G4Tubs*
    solidFocus = new G4Tubs("Focus",0.,QuartzR,0.5*QuartzW,-90.*deg,90.*deg);
  G4RotationMatrix* rotFocus = new G4RotationMatrix();
  rotFocus->rotateX(90.*deg);
  G4LogicalVolume*
    logicFocus = new G4LogicalVolume(solidFocus, SiO2, "Focus"); 
  G4VPhysicalVolume *physiFocus = new G4PVPlacement(rotFocus, 
      G4ThreeVector(-QuartzR/2.+QuartzL/2.,0,QuartzH/2.-QuartzR),
      logicFocus, "Focus", logicWorld, false, 0, checkOverlaps);    
  // ----------------- Al  -------------------
  //
  G4Box*
    solidAlSide1 = new G4Box("AlSide1", 0.5*QuartzL, 0.5*AlT, 0.5*QuartzH);
  G4LogicalVolume*
    logicAlSide1 = new G4LogicalVolume(solidAlSide1, Al, "AlSide1"); 
  G4VPhysicalVolume *physiAlLeft = new G4PVPlacement(0, G4ThreeVector(-QuartzR/2.,-QuartzW/2.-AlT/2.,0),
      logicAlSide1, "AlLeft", logicWorld, false, 0, checkOverlaps);
  G4VPhysicalVolume *physiAlRight = new G4PVPlacement(0, G4ThreeVector(-QuartzR/2.,+QuartzW/2.+AlT/2.,0),
      logicAlSide1, "AlRight", logicWorld, false, 0, checkOverlaps);

  G4Tubs*
    solidAlSideP2 = new G4Tubs("AlSideP2",0.,QuartzR,0.5*QuartzW+AlT,-90.*deg,90.*deg);
  G4SubtractionSolid*
    solidAlSide2 = new G4SubtractionSolid("AlSide2", solidAlSideP2, solidFocus);
  G4LogicalVolume*
    logicAlSide2 = new G4LogicalVolume(solidAlSide2, SiO2, "AlSide2"); 
  G4VPhysicalVolume *physiAlSide = new G4PVPlacement(rotFocus, 
      G4ThreeVector(-QuartzR/2.+QuartzL/2.,0,QuartzH/2.-QuartzR),
      logicAlSide2, "AlSide", logicWorld, false, 0, checkOverlaps);

  G4Tubs*
    solidAlFocusP1 = new G4Tubs("AlFocusP1",0.,QuartzR+AlT,0.5*QuartzW,-90.*deg,90.*deg);
  G4SubtractionSolid*
    solidAlFocus = new G4SubtractionSolid("AlFocus", solidAlFocusP1, solidFocus);
  G4LogicalVolume*
    logicAlFocus = new G4LogicalVolume(solidAlFocus, SiO2, "AlFocus"); 
  G4VPhysicalVolume *physiAlFocus = new G4PVPlacement(rotFocus, 
      G4ThreeVector(-QuartzR/2.+QuartzL/2.,0,QuartzH/2.-QuartzR),
      logicAlFocus, "AlFocus", logicWorld, false, 0, checkOverlaps);

  G4Box*  
    solidAlTop = new G4Box("AlTop", 0.5*AlT, 0.5*QuartzW, 0.5*QuartzH);
  G4LogicalVolume* 
    logicAlTop = new G4LogicalVolume(solidAlTop, Al, "AlTop"); 
  G4VPhysicalVolume *physiAlTop = new G4PVPlacement(0,G4ThreeVector(-QuartzR/2.-QuartzL/2.-AlT/2., 0, 0),
      logicAlTop, "AlTop", logicWorld, false, 0, checkOverlaps);

  G4Box*  
    solidAlWrapper = new G4Box("AlWrapper", 0.5*AlT, 0.5*QuartzW, 0.5*QuartzR-0.5*QuartzH);
  G4LogicalVolume* 
    logicAlWrapper = new G4LogicalVolume(solidAlWrapper, Al, "AlWrapper"); 
  G4VPhysicalVolume *physiAlWrapper = new G4PVPlacement(0,G4ThreeVector(QuartzL/2.-QuartzR/2.-AlT/2., 0, -QuartzR/2.),
      logicAlWrapper, "AlWrapper", logicWorld, false, 0, checkOverlaps);    

  // ----------------- PhotonDet ---------------------------------
  //

  G4double PhotonDet_posX = QuartzL/2.;
  G4double PhotonDet_posY = 0;
  G4double PhotonDet_posZ = QuartzH/2-PhotonDetT/2.-QuartzR;
  G4Box*  
    solidPhotonDet = new G4Box("PhotonDet", PhotonDetL/2, PhotonDetW/2, PhotonDetT/2);
  G4LogicalVolume* 
    logicPhotonDet = new G4LogicalVolume(solidPhotonDet, PhotonDet, "PhotonDet");
  G4VPhysicalVolume*
    physiPhotonDet[PhotonDetNu];
  for(int j=0; j<PhotonDetNu;j++) 
  {
    PhotonDet_posY = -(PhotonDetW+PhotonDetGap)*(PhotonDetNu-1)/2+(PhotonDetW+PhotonDetGap)*j;
    physiPhotonDet[j] = new G4PVPlacement(0, G4ThreeVector(PhotonDet_posX, PhotonDet_posY, PhotonDet_posZ),
        logicPhotonDet, "PhotonDet", logicWorld, false, j, checkOverlaps);   
  }

  // ----------------- Cathode ---------------------------------
  //
  G4double Cathode_posX = 0;
  G4double Cathode_posY = 0;
  G4double Cathode_posZ = PhotonDetT/2-CathodeT/2;
  G4Box*  
    solidCathode = new G4Box("Cathode", CathodeW/2, CathodeW/2, CathodeT/2);
  G4LogicalVolume*  
    logicCathode = new G4LogicalVolume(solidCathode, Cathode, "Cathode");
  G4VPhysicalVolume*
    physiCathode[CathodeNu*CathodeNu];
  for(int k=0;k<CathodeNu;k++)
    for(int j=0; j<CathodeNu;j++) 
    {
      Cathode_posX = -CathodeW*(CathodeNu-1)/2+CathodeW*j; 
      Cathode_posY = -CathodeW*(CathodeNu-1)/2+CathodeW*k; 
      physiCathode[k*4+j] =new G4PVPlacement(0, G4ThreeVector(Cathode_posX, Cathode_posY, Cathode_posZ),
          logicCathode, "Cathode", logicPhotonDet, false, k*4+j, checkOverlaps);   
    }  


  //
  // ----------------- Surfaces ---------------------------------
  //
  //

  // Quartz-Air Surface
  G4OpticalSurface* opQuartzAirSurface = new G4OpticalSurface("QuartzAirSurface");
  opQuartzAirSurface ->SetType(dielectric_dielectric);
  opQuartzAirSurface ->SetFinish(ground);
  opQuartzAirSurface ->SetModel(unified);
  opQuartzAirSurface ->SetSigmaAlpha(m_SigmaAlpha*deg);

    new G4LogicalBorderSurface("QuartzAirSurface", physiQuartz, physiWorld, opQuartzAirSurface);
    new G4LogicalBorderSurface("QuartzAirSurface", physiFocus, physiWorld, opQuartzAirSurface);

  // Quartz-AlFocus Surface
  G4OpticalSurface* opQuartzAlMirror = new G4OpticalSurface("QuartzAlFocusSurface");
  opQuartzAlMirror ->SetType(dielectric_metal);
  opQuartzAlMirror ->SetFinish(polished);
  opQuartzAlMirror ->SetModel(unified);

  new G4LogicalBorderSurface("QuartzAlFocusSurface", physiFocus, physiAlFocus, opQuartzAlMirror);

  // Quartz-AlTop Surface
  G4OpticalSurface* opQuartzAlTopSurface = new G4OpticalSurface("QuartzAlTopSurface");
  opQuartzAlTopSurface ->SetType(dielectric_metal);
  opQuartzAlTopSurface ->SetFinish(polished);
  opQuartzAlTopSurface ->SetModel(unified);

  new G4LogicalBorderSurface("QuartzAlTopSurface", physiQuartz, physiAlTop, opQuartzAlTopSurface);


  // Quartz-AlSide Surface
  G4OpticalSurface* opQuartzAlSideSurface = new G4OpticalSurface("QuartzAlSideSurface");
  opQuartzAlSideSurface ->SetType(dielectric_metal);
  opQuartzAlSideSurface ->SetFinish(polished);
  opQuartzAlSideSurface ->SetModel(unified);

  new G4LogicalBorderSurface("QuartzAlLeftSurface", physiQuartz, physiAlLeft, opQuartzAlMirror);
  new G4LogicalBorderSurface("QuartzAlRightSurface", physiQuartz, physiAlRight, opQuartzAlMirror);
  new G4LogicalBorderSurface("QuartzAlSideSurface", physiFocus, physiAlSide, opQuartzAlMirror);
  new G4LogicalBorderSurface("QuartzAlWrapperSurface", physiFocus, physiAlWrapper, opQuartzAlSideSurface);

  // Quartz-PhotonDet Surface
  G4OpticalSurface* opQuartzPhotonDetSurface = new G4OpticalSurface("QuartzPhotonDetSurface");
  opQuartzPhotonDetSurface ->SetType(dielectric_metal);
  opQuartzPhotonDetSurface ->SetFinish(polished);
  opQuartzPhotonDetSurface ->SetModel(unified);

  for(int j=0; j<PhotonDetNu; j++)
  {
    new G4LogicalBorderSurface("ExpandPhotonDetSurface", physiFocus, physiPhotonDet[j], opQuartzPhotonDetSurface);
  }

  // Quartz-Cathode Surface
  G4OpticalSurface* opQuartzCathodeSurface = new G4OpticalSurface("QuartzCathodeSurface");
  opQuartzCathodeSurface ->SetType(dielectric_metal);
  opQuartzCathodeSurface ->SetFinish(polished);
  opQuartzCathodeSurface ->SetModel(unified);

  for(int k=0; k<CathodeNu*CathodeNu; k++)
  {
    new G4LogicalBorderSurface("ExpandCathodeSurface", physiFocus, physiCathode[k], opQuartzCathodeSurface);
  }

  // PhotonDet Surface
  G4OpticalSurface* opPhotonDetSurface = new G4OpticalSurface("PhotonDetSurface");
  opPhotonDetSurface ->SetType(dielectric_metal);
  opPhotonDetSurface ->SetFinish(polished);
  opPhotonDetSurface ->SetModel(glisur);

  new G4LogicalSkinSurface("PhotonDetSurface", logicPhotonDet, opPhotonDetSurface);

  // Al Surface
  G4OpticalSurface* opAlSurface = new G4OpticalSurface("AlSurface");
  opAlSurface ->SetType(dielectric_metal);
  opAlSurface ->SetFinish(polished);
  opAlSurface ->SetModel(glisur);

  new G4LogicalSkinSurface("AlFocusSurface", logicAlFocus, opAlSurface);
  new G4LogicalSkinSurface("AlWrapperSurface", logicAlWrapper, opAlSurface);
  new G4LogicalSkinSurface("AlTopSurface", logicAlTop, opAlSurface);
  new G4LogicalSkinSurface("AlSideSurface", logicAlSide1, opAlSurface);
  new G4LogicalSkinSurface("AlSideSurface", logicAlSide2, opAlSurface);
  new G4LogicalSkinSurface("ShieldSurface", logicShield, opAlSurface);

  //
  // ************************ Generate & Add Material Properties Table attached to the optical surfaces ******************************
  //
  const G4int num = 2;
  G4double ephoton[num] = {0.4*eV, 6.02*eV};

  // ------------------- Optical Quartz Air Surface ---------------------
  G4double EreflecQuartzAir[] = 
  {
    1.904*eV,	2.055*eV,	2.264*eV,	2.584*eV,	3.034*eV,	3.163*eV,	3.237*eV,	3.314*eV,	3.395*eV,	3.474*eV,	
    3.581*eV,	3.663*eV,	3.737*eV,	3.821*eV,	3.881*eV,	3.943*eV,	3.993*eV,	4.044*eV,	4.074*eV,	4.127*eV,	
    4.150*eV,	4.181*eV
  };
  nEntries = sizeof(EreflecQuartzAir)/sizeof(G4double);
  G4double reflecQuartzAir[] = 
  {
    0.982,	0.981,	0.980,	0.979,	0.978,	0.977,	0.976,	0.976,	0.974,	0.973,	
    0.971,	0.969,	0.967,	0.965,	0.963,	0.961,	0.960,	0.958,	0.957,	0.955,	
    0.954,	0.953
  };
  assert(sizeof(EreflecQuartzAir)==sizeof(reflecQuartzAir));

  G4double specularlobeQuartzAir[num] = {1.0,1.0};
  G4double specularspikeQuartzAir[num] = {0,0};
  G4double backscatterQuartzAir[num] = {0,0};

  G4MaterialPropertiesTable* SMPT_QuartzAir = new G4MaterialPropertiesTable();

  SMPT_QuartzAir ->AddProperty("REFLECTIVITY", EreflecQuartzAir, reflecQuartzAir, nEntries);
  SMPT_QuartzAir ->AddProperty("SPECULARLOBECONSTANT", ephoton, specularlobeQuartzAir, 2);
  SMPT_QuartzAir ->AddProperty("SPECULARSPIKECONSTANT", ephoton, specularspikeQuartzAir, 2);
  SMPT_QuartzAir ->AddProperty("BACKSCATTERCONSTANT", ephoton, backscatterQuartzAir, 2);

  opQuartzAirSurface ->SetMaterialPropertiesTable(SMPT_QuartzAir);

  // ------------------- Optical Quartz Al Surface ---------------------
  G4double EreflecMirror[] = 
  {
    1.906*eV,	1.914*eV,	1.922*eV,	1.931*eV,	1.941*eV,	1.958*eV,	1.977*eV,	1.997*eV,	2.024*eV,	2.055*eV,	
    2.089*eV,	2.124*eV,	2.150*eV,	2.176*eV,	2.200*eV,	2.217*eV,	2.235*eV,	2.256*eV,	2.279*eV,	2.300*eV,	
    2.339*eV,	2.361*eV,	2.381*eV,	2.400*eV,	2.420*eV,	2.439*eV,	2.450*eV,	2.466*eV,	2.477*eV,	2.522*eV,	
    2.554*eV,	2.643*eV,	2.671*eV,	2.701*eV,	2.821*eV,	2.902*eV,	3.016*eV,	3.171*eV,	3.259*eV,	3.343*eV,	
    3.399*eV,	3.431*eV,	3.506*eV,	3.546*eV,	3.586*eV,	3.668*eV,	3.705*eV,	3.749*eV,	3.846*eV,	3.893*eV,	
    3.935*eV,	3.991*eV,	4.094*eV,	4.163*eV
  };
  nEntries = sizeof(EreflecMirror)/sizeof(G4double);
  G4double reflecMirror[] = 
  {
    0.869,	0.872,	0.875,	0.878,	0.881,	0.883,	0.886,	0.890,	0.894,	0.898,	
    0.903,	0.908,	0.912,	0.915,	0.918,	0.920,	0.921,	0.923,	0.924,	0.925,	
    0.926,	0.927,	0.928,	0.929,	0.930,	0.931,	0.932,	0.933,	0.935,	0.936,	
    0.937,	0.938,	0.939,	0.940,	0.939,	0.938,	0.937,	0.936,	0.935,	0.934,	
    0.933,	0.932,	0.931,	0.930,	0.929,	0.928,	0.927,	0.926,	0.925,	0.924,	
    0.923,	0.922,	0.921,	0.920
  };
  assert(sizeof(EreflecMirror)==sizeof(reflecMirror));
  G4MaterialPropertiesTable* SMPT_Mirror = new G4MaterialPropertiesTable();
  SMPT_Mirror ->AddProperty("REFLECTIVITY", EreflecMirror, reflecMirror, nEntries);

  G4double reflecAbsorbtion[2] ={0, 0}; 
  G4MaterialPropertiesTable* SMPT_Absorbtion = new G4MaterialPropertiesTable();
  SMPT_Absorbtion ->AddProperty("REFLECTIVITY", ephoton, reflecAbsorbtion, num);

  opQuartzAlMirror      -> SetMaterialPropertiesTable(SMPT_Mirror);
  opQuartzAlTopSurface   -> SetMaterialPropertiesTable(SMPT_Absorbtion);
  opQuartzAlSideSurface -> SetMaterialPropertiesTable(SMPT_Absorbtion);

  // ------------------- Optical Quartz PhotonDet-Cathode Surface ---------------------
  G4double reflecQuartzPhotonDet[num] = {0, 0}; 
  G4double effiQuartzPhotonDetX[] =
  {
    1.900*eV, 1.928*eV, 1.961*eV, 1.990*eV, 2.048*eV, 2.118*eV, 2.178*eV, 2.231*eV, 2.314*eV, 2.386*eV,
    2.436*eV, 2.509*eV, 2.565*eV, 2.646*eV, 2.724*eV, 2.789*eV, 2.867*eV, 2.931*eV, 3.027*eV, 3.108*eV,
    3.239*eV, 3.333*eV, 3.458*eV, 3.592*eV, 3.753*eV, 3.896*eV, 4.122*eV
  };
  nEntries = sizeof(effiQuartzPhotonDetX)/sizeof(G4double);
  G4double effiQuartzPhotonDetY[] =
  {
    0.0386, 0.0414, 0.0456, 0.0490, 0.0572, 0.0660, 0.0743, 0.0837, 0.0966, 0.1088,
    0.1197, 0.1317, 0.1466, 0.1632, 0.1795, 0.2023, 0.2173, 0.2362, 0.2537, 0.2725,
    0.2928, 0.2928, 0.2894, 0.2793, 0.2632, 0.2571, 0.2452
  };
  assert(sizeof(effiQuartzPhotonDetX)==sizeof(effiQuartzPhotonDetY));
  G4MaterialPropertiesTable* SMPT_Quartz_Cathode = new G4MaterialPropertiesTable();
  SMPT_Quartz_Cathode ->AddProperty("REFLECTIVITY", ephoton, reflecQuartzPhotonDet, num);
  SMPT_Quartz_Cathode ->AddProperty("EFFICIENCY", effiQuartzPhotonDetX, effiQuartzPhotonDetY, nEntries);
  opQuartzCathodeSurface ->SetMaterialPropertiesTable(SMPT_Quartz_Cathode);

  // ------------------- Optical PhotonDet Surface ---------------------
  G4double reflecPhotonDet[num] = {0, 0};
  G4MaterialPropertiesTable* SMPT_PhotonDet = new G4MaterialPropertiesTable();
  SMPT_PhotonDet ->AddProperty("REFLECTIVITY", ephoton, reflecPhotonDet, num);
  opQuartzPhotonDetSurface ->SetMaterialPropertiesTable(SMPT_PhotonDet);
  opPhotonDetSurface ->SetMaterialPropertiesTable(SMPT_PhotonDet);

  // ------------------- Optical Al Surface ---------------------
  G4double reflecAl[num] = {0, 0};
  G4MaterialPropertiesTable* SMPT_Al = new G4MaterialPropertiesTable();
  SMPT_Al ->AddProperty("REFLECTIVITY", ephoton, reflecAl, num);
  opAlSurface ->SetMaterialPropertiesTable(SMPT_Al);

  //
  //

  // ************************ Draw Detector Volumes  ******************************
  G4VisAttributes* visAttributes = new G4VisAttributes(G4Colour(1.0,1.0,1.0)); //white
  visAttributes -> SetVisibility(false);
  logicWorld ->SetVisAttributes(visAttributes);
  logicShield ->SetVisAttributes(visAttributes);
  visAttributes = new G4VisAttributes(G4Colour(0.0,0.0,1.0));//blue
  logicQuartz -> SetVisAttributes(visAttributes);
  logicFocus  -> SetVisAttributes(visAttributes);
  visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,0.0));//green
  logicPhotonDet ->SetVisAttributes(visAttributes);
  visAttributes = new G4VisAttributes(G4Colour(0.0,1.0,1.0));//cyen
  logicCathode ->SetVisAttributes(visAttributes);
  visAttributes = new G4VisAttributes(G4Colour(0.5,0.5,0.5));//gray
  logicAlSide1  -> SetVisAttributes(visAttributes);
  logicAlSide2  -> SetVisAttributes(visAttributes);
  logicAlFocus  -> SetVisAttributes(visAttributes);
  logicAlWrapper  -> SetVisAttributes(visAttributes);
  logicAlTop     -> SetVisAttributes(visAttributes);
  //
  //always return the physical World
  //

  G4GDMLParser fParser; 
  //fParser.Read("DIRC.gdml",false);
  //G4VPhysicalVolume* physiWorld = fParser.GetWorldVolume();
  //physiWorld->GetLogicalVolume()->SetVisAttributes(G4VisAttributes::Invisible);
  //fParser.Write("DIRC.gdml",physiWorld);
  return physiWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
