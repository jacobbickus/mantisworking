#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "globals.hh"
#include "G4UImanager.hh"
#include "G4PhysicalConstants.hh"

#include "G4GeometryManager.hh"
#include "G4SolidStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"

#include "G4OpticalSurface.hh"
#include "G4MaterialTable.hh"
#include "G4VisAttributes.hh"
#include "G4OpRayleigh.hh"

#include "G4Box.hh" //for box
//#include "G4Cons.hh" //for cone
//#include "G4Orb.hh" //for orb
#include "G4Sphere.hh" //for sphere
#include "G4Tubs.hh" // for tube/cylinder

// for color attributes
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "DetectorMessenger.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4RotationMatrix.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class DetectorMessenger;

/// Detector construction class to define materials and geometry.

class DetectorConstruction : public G4VUserDetectorConstruction
{

public:
DetectorConstruction(G4bool);
virtual ~DetectorConstruction();


virtual G4VPhysicalVolume* Construct();

void SetAttenuatorState(G4bool val)
{
  attenuatorState = val;
}
void SetAttenuatorThickness(G4double val)
{
   attenThickness = val;
}
void SetAttenuatorMaterial(G4String val)
{
  attenuatorMat = val;
}
void SetAttenuatorState2(G4bool val)
{
  attenuatorState2 = val;
}
void SetAttenuatorThickness2(G4double val)
{
   attenThickness2 = val;
}
void SetAttenuatorMaterial2(G4String val)
{
  attenuatorMat2 = val;
}
void SetWaterX(G4double val)
{
        water_size_x = val;
        water_size_x = water_size_x*cm;
}
void SetWaterY(G4double val)
{
        water_size_y = val;
        water_size_y = water_size_y*cm;
}
void SetWaterZ(G4double val)
{
        water_size_z = val;
        water_size_z = water_size_z*cm;
}
void SetPC_radius(G4double val)
{
        PMT_rmax = val;
        PMT_rmax = PMT_rmax*cm;
};
void SetIntObj_radius(G4double val)
{
        IntObj_rad = val;
        IntObj_rad = IntObj_rad*cm;
}
void SetIntObjX_pos(G4double val)
{
        intObj_x_pos = val;
        intObj_x_pos = intObj_x_pos*cm;
}
void SetIntObjY_pos(G4double val)
{
        intObj_y_pos = val;
        intObj_y_pos = intObj_y_pos*cm;
}
void SetIntObjZ_pos(G4double val)
{
        intObj_z_pos = val;
        intObj_z_pos = intObj_z_pos*cm;
}
void SetIntObjAbundance(G4double val)
{
        intObj_radio_abundance = val;
        intObj_radio_abundance = intObj_radio_abundance*perCent;
}
void SetChopperAbundance(G4double val)
{
  chopper_radio_abundance = val;
  chopper_radio_abundance = chopper_radioabundance*perCent;
}
void SetIntObj(G4String val)
{
        if(val == "Uranium")
        {
                IntObj_Selection = val;
                intObjDensity = 19.1*g/cm3;
        }
        else if(val == "Plutonium")
        {
                IntObj_Selection = val;
                intObjDensity = 19.6*g/cm3;
        }
        else if(val == "Lead")
        {
          IntObj_Selection = "Lead";
          intObjDensity = 11.4*g/cm3;
        }
        else if(val == "Steel")
        {
          IntObj_Selection = "Steel";
          intObjDensity = 8.05*g/cm3;
        }
        else if(val == "Plastic")
        {
          IntObj_Selection = "Plastic";
          intObjDensity = 0.94*g/cm3;
        }
        else{std::cerr << "ERROR: IntObj not correctly chosen"<<std::endl;}
}

void SetChopperMaterial(G4String val)
{
  if(val == "Uranium")
  {
    chopperDensity = 19.1*g/cm3;
  }
  else if(val == "Plutonium")
  {
    chopperDensity = 19.6*g/cm3;
  }
}

void setEndIntObj(G4double z_pos_con, G4double con_z_size)
{
        EndIntObj = z_pos_con + con_z_size/2;
}

G4double getEndIntObj()const
{
        return EndIntObj;
}
void SetPC_material(G4String val)
{
        pc_mat = val;
}
void SetnPMT(G4int val)
{
        nPMT = val;
}
void SetChopperOn(G4bool val)
{
        chopperOn = val;
}
G4bool GetChopperState()const
{
        return chopperOn;
}
void SetChopperThick(G4double val)
{
        chopper_thick = val;
        chopper_thick = chopper_thick*mm;
}
void SetChopper_z(G4double val)
{
        chopper_z = val;
        chopper_z = chopper_z*cm;
}
void SettheAngle(G4double val)
{
        theAngle = val;
}
void SetPlexiThickness(G4double val)
{
  plexiThickness = val;
  plexiThickness = plexiThickness*mm;
}
void SetTapeThickness(G4double val)
{
  tapeThick = val;
  tapeThick = tapeThick*cm;
}
void SetDetectorViewOnly(G4bool val)
{
  DetectorViewOnly = val;
}

private:
G4bool bremTest;
G4double container_x = 0.6096*m;     // 2ft cross section
G4double container_y = 2.5908*m;     // 8.5 ft in height
G4double container_z = 2.4384*m;     // 8 ft width
G4double EndIntObj;
G4double IntObj_rad;
G4double radio_abundance;
G4String IntObj_Selection;
G4double intObjDensity;
G4double chopperDensity;
G4double intObj_x_pos, intObj_y_pos, intObj_z_pos;
G4VPhysicalVolume* physIntObj;
G4Material* PC_mat;
G4LogicalVolume *logicalLinac;
G4LogicalVolume *logicalVacuum;
G4LogicalVolume *logicBremTarget;
G4LogicalVolume* logicPC;
G4LogicalVolume* logicPMT;
G4VPhysicalVolume* physPC;
G4LogicalVolume* logicChopper;
G4VPhysicalVolume* physWater;
G4VPhysicalVolume* physTape;
G4bool chopperOn;
G4double chopper_thick, chopper_z;
G4double theAngle;
G4double water_size_x, water_size_y, water_size_z, PMT_rmax;
G4int nPMT;
G4String pc_mat;
G4bool attenuatorState;
G4double attenThickness;
G4String attenuatorMat;
G4bool attenuatorState2;
G4double attenThickness2;
G4String attenuatorMat2;
G4double plexiThickness;
G4double tapeThick;
G4bool DetectorViewOnly;
DetectorMessenger* detectorM;

};


#endif
