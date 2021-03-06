//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// Author:
// Jacob E Bickus, 2021
// MIT, NSE
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////

#include "DetectorMessenger.hh"


DetectorMessenger::DetectorMessenger(DetectorConstruction* DetectorAction)
        : DetectorA(DetectorAction)
{
        myDir = new G4UIdirectory("/mydet/");
        myDir2 = new G4UIdirectory("/mytar/");
        myDir3 = new G4UIdirectory("/chopper/");
        myDir4 = new G4UIdirectory("/myvisualization/");
        myDir5 = new G4UIdirectory("/material/");
        myDir2->SetGuidance("Target Setup Commands");
        myDir->SetGuidance("Detector Setup Commands");
        myDir3->SetGuidance("Chopper Setup Commands");
        myDir4->SetGuidance("Customized Visualization Commands");
        myDir5->SetGuidance("Material Table Verbosity");
        Cmd = new G4UIcmdWithADouble("/mydet/PCrad",this);
        CmdX = new G4UIcmdWithADouble("/mydet/WaterX",this);
        CmdY = new G4UIcmdWithADouble("/mydet/WaterY",this);
        CmdZ = new G4UIcmdWithADouble("/mydet/WaterZ",this);
        Cmdtr = new G4UIcmdWithADouble("/mytar/IntObjRad",this);
        Cmdtrad = new G4UIcmdWithADouble("/mytar/abundance",this);
        Cmdtsel = new G4UIcmdWithAString("/mytar/target",this);
        CmdtXpos = new G4UIcmdWithADouble("/mytar/IntObjXPos",this);
        CmdtYpos = new G4UIcmdWithADouble("/mytar/IntObjYPos",this);
        CmdtZpos = new G4UIcmdWithADouble("/mytar/IntObjZPos",this);
        Cmdpcmat = new G4UIcmdWithAString("/mydet/PCmat",this);
        CmdnPMT = new G4UIcmdWithAnInteger("/mydet/nPMT",this);
        CmdAngle = new G4UIcmdWithADouble("/mydet/Angle",this);
        CmdChopMaterial = new G4UIcmdWithAString("/chopper/material",this);
        CmdChopthick = new G4UIcmdWithADouble("/chopper/thickness", this);
        CmdChopZ = new G4UIcmdWithADouble("/chopper/distance", this);
        CmdChopperOn = new G4UIcmdWithAString("/chopper/state", this);
        CmdChopperAbundance = new G4UIcmdWithADouble("/chopper/abundance",this);
        CmdAttenOn = new G4UIcmdWithAString("/mydet/attenuator", this);
        CmdAttenOn2 = new G4UIcmdWithAString("/mydet/attenuator2",this);
        CmdAttenThick = new G4UIcmdWithADouble("/mydet/attenuatorThickness",this);
        CmdAttenThick2 = new G4UIcmdWithADouble("/mydet/attenuatorThickness2",this);
        CmdAttenMat = new G4UIcmdWithAString("/mydet/attenuatorMaterial",this);
        CmdAttenMat2 = new G4UIcmdWithAString("/mydet/attenuatorMaterial2",this);
        CmdPlexi = new G4UIcmdWithADouble("/mydet/PlexiglassThickness",this);
        CmdTape = new G4UIcmdWithADouble("/mydet/OpticalTapeThickness",this);
        CmdVis = new G4UIcmdWithAString("/myvisualization/DetectorViewOnly", this);
        CmdVerbose = new G4UIcmdWithAString("/material/verbose",this);
        CmdCheckOverlaps = new G4UIcmdWithAString("/material/CheckOverlaps",this);


        Cmd->SetGuidance("Choose Desired PhotoCathode Radius");
        CmdX->SetGuidance("Choose Desired X Size of Water Tank");
        CmdY->SetGuidance("Choose Desired Y Size of Water Tank");
        CmdZ->SetGuidance("Choose Desired Z Size of Water Tank");
        Cmdtr->SetGuidance("Choose Desired radius Size of Interogation Target");
        Cmdtrad->SetGuidance("Choose Desired fission isotope abundance(enrichment) of Interrogation Target");
        Cmdtsel->SetGuidance("Choose Desired target");
        CmdtXpos->SetGuidance("Choose Desired X Position of Interogation Target");
        CmdtYpos->SetGuidance("Choose Desired Y Position of Interogation Target");
        CmdtZpos->SetGuidance("Choose Desired Z Position of Interogation Target");
        Cmdpcmat->SetGuidance("Choose desired photocathode material");
        CmdnPMT->SetGuidance("Choose desired number of PMTs");
        CmdChopMaterial->SetGuidance("Choose desired Chopper Material");
        CmdChopthick->SetGuidance("Choose desired chopper thickness");
        CmdChopZ->SetGuidance("Choose desired chopper distance from brem beam");
        CmdChopperOn->SetGuidance("Choose desired chopper wheel state");
        CmdChopperAbundance->SetGuidance("Choose desired chopper wheel material isotope abundance(enrichment)");
        CmdAngle->SetGuidance("Choose desired Detector BackScatter Angle in Degrees");
        CmdAttenOn->SetGuidance("Choose if Attenuator Present or not");
        CmdAttenThick->SetGuidance("Choose Desired attenuator thickness");
        CmdAttenMat->SetGuidance("Choose desired attenuator material from NIST materials");
        CmdAttenOn2->SetGuidance("Choose if Attenuator Second Layer Present or not");
        CmdAttenThick2->SetGuidance("Choose Desired attenuator thickness");
        CmdAttenMat2->SetGuidance("Choose desired attenuator material from NIST materials");
        CmdPlexi->SetGuidance("Choose desired plexiglass thickness in mm");
        CmdTape->SetGuidance("Choose desired optical tape wrap thickness in cm");
        CmdVis->SetGuidance("Choose if visualization will show Cherenkov Detector Only");
        CmdCheckOverlaps->SetGuidance("Choose to check for geometric overlaps");
        Cmd->SetParameterName("radius",false);
        CmdX->SetParameterName("waterx",false);
        CmdY->SetParameterName("watery",false);
        CmdZ->SetParameterName("waterz",false);
        Cmdtr->SetParameterName("targetradius",false);
        Cmdtrad->SetParameterName("targetabundance",false);
        Cmdtrad->SetRange("targetabundance > 0 && targetabundance < 100");
        Cmdtsel->SetParameterName("targetsel",false);
        CmdtXpos->SetParameterName("targetxpos",false);
        CmdtYpos->SetParameterName("targetypos",false);
        CmdtZpos->SetParameterName("targetzpos",false);
        Cmdpcmat->SetParameterName("photocathodeMat", false);
        CmdnPMT->SetParameterName("numberPMT", false);
        CmdChopMaterial->SetParameterName("ChopperMaterial",false);
        CmdChopZ->SetParameterName("chopperZ", false);
        CmdChopZ->SetRange("chopperZ > 0.5 && chopperZ < 6");
        CmdChopthick->SetParameterName("chopperthickness",false);
        CmdChopperOn->SetParameterName("chopperOn",false);
        CmdChopperAbundance->SetParameterName("chopperAbundance",false);
        CmdChopperAbundance->SetRange("chopperAbundance > 0 && chopperAbundance < 100");
        CmdAngle->SetParameterName("Angle",false);
        CmdAngle->SetRange("Angle > 90 && Angle < 135");
        CmdAttenOn->SetParameterName("attenuator",false);
        CmdAttenThick->SetParameterName("attenThickness",false);
        CmdAttenMat->SetParameterName("attenMaterial",false);
        CmdAttenOn2->SetParameterName("attenuator2",false);
        CmdAttenThick2->SetParameterName("attenThickness2",false);
        CmdAttenMat2->SetParameterName("attenMaterial2",false);
        CmdPlexi->SetParameterName("PlexiThickness",false);
        CmdTape->SetParameterName("TapeThickness",false);
        CmdVis->SetParameterName("visualization",false);
        CmdVerbose->SetParameterName("verbosity",false);
        CmdCheckOverlaps->SetParameterName("overlaps",false);

        Cmdtsel->SetCandidates("Uranium NaturalU Plutonium NaturalPu Lead Steel Plastic");
        Cmdpcmat->SetCandidates("GaAsP Bialkali");
        CmdChopperOn->SetCandidates("On on Off off");
        CmdChopMaterial->SetCandidates("Uranium Plutonium Lead Tungsten");
        CmdAttenOn->SetCandidates("On on Off off");
        CmdAttenMat->SetCandidates("G4_Pb G4_Cu G4_Zn G4_Ag G4_Cd G4_Th G4_U G4_Au G4_W G4_Fe");
        CmdAttenMat2->SetCandidates("G4_POLYETHYLENE G4_POLYPROPYLENE G4_POLYSTYRENE G4_POLYVINYL_CHLORIDE G4_POLYCARBONATE");
        CmdVis->SetCandidates("True true False false");
        CmdVerbose->SetCandidates("True true False false On on Off off");
        CmdCheckOverlaps->SetCandidates("True true False false");

}

DetectorMessenger::~DetectorMessenger()
{
        delete Cmd;
        delete CmdX;
        delete CmdY;
        delete CmdZ;
        delete Cmdtr;
        delete Cmdtrad;
        delete Cmdtsel;
        delete CmdtXpos;
        delete CmdtYpos;
        delete CmdtZpos;
        delete Cmdpcmat;
        delete CmdnPMT;
        delete CmdChopthick;
        delete CmdChopZ;
        delete CmdChopperOn;
        delete CmdChopperAbundance;
        delete CmdAngle;
        delete CmdAttenOn;
        delete CmdAttenThick;
        delete CmdAttenMat;
        delete CmdAttenOn2;
        delete CmdAttenThick2;
        delete CmdAttenMat2;
        delete CmdPlexi;
        delete CmdTape;
        delete CmdVis;
        delete CmdChopMaterial;
        delete CmdVerbose;
        delete CmdCheckOverlaps;
}


void DetectorMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{ // this function only works if named SetNewValue
        if(command == Cmd)
        {
                G4double theCommand = Cmd->GetNewDoubleValue(newValue);
                DetectorA->SetPC_radius(theCommand);
                G4cout << "The Photocathode Radius manually set to: " << theCommand << " cm" << G4endl;
        }
        else if(command == CmdX)
        {
                G4double theCommandX = CmdX->GetNewDoubleValue(newValue);
                DetectorA->SetWaterX(theCommandX);
                G4cout << "The Water X-Dimension Size manually set to: " << theCommandX << " cm" << G4endl;
        }
        else if(command == CmdY)
        {
                G4double theCommandY = CmdY->GetNewDoubleValue(newValue);
                DetectorA->SetWaterY(theCommandY);
                G4cout << "The Water Y-Dimension Size manually set to: " << theCommandY << " cm" << G4endl;
        }
        else if(command == CmdZ)
        {
                G4double theCommandZ = CmdZ->GetNewDoubleValue(newValue);
                DetectorA->SetWaterZ(theCommandZ);
                G4cout << "The Water Z-Dimension Size manually set to: " << theCommandZ << " cm" << G4endl;
        }
        else if(command == Cmdtr)
        {
                G4double theCommandtX = Cmdtr->GetNewDoubleValue(newValue);
                DetectorA->SetIntObj_radius(theCommandtX);
                G4cout << "The Interrogation Object Radius manually set to: " << theCommandtX << " cm" << G4endl;
        }
        else if(command == Cmdtrad)
        {
                G4double theCommandtrad = Cmdtrad->GetNewDoubleValue(newValue);
                DetectorA->SetIntObjAbundance(theCommandtrad);
                G4cout << "The Interrogation Object fissile radioisotope abundance manually set to: " << theCommandtrad << " percent" << G4endl;
        }
        else if(command == Cmdtsel)
        {
                G4String theCommandtsel = newValue;
                DetectorA->SetIntObj(theCommandtsel);
                G4cout << "The Interrogation Object manually set to: " << theCommandtsel << " material!" << G4endl << G4endl;
        }
        else if(command == CmdtXpos)
        {
                G4double theCommandtXpos = CmdtXpos->GetNewDoubleValue(newValue);
                DetectorA->SetIntObjX_pos(theCommandtXpos);
                G4cout << "The Interrogation Object X Position manually set to: " << theCommandtXpos << " cm" << G4endl;
        }
        else if(command == CmdtYpos)
        {
                G4double theCommandtYpos = CmdtYpos->GetNewDoubleValue(newValue);
                DetectorA->SetIntObjY_pos(theCommandtYpos);
                G4cout << "The Interrogation Object Y Position manually set to: " << theCommandtYpos << " cm" << G4endl;
        }
        else if(command == CmdtZpos)
        {
                G4double theCommandtZpos = CmdtZpos->GetNewDoubleValue(newValue);
                DetectorA->SetIntObjZ_pos(theCommandtZpos);
                G4cout << "The Interrogation Object Z Position manually set to: " << theCommandtZpos << " cm" << G4endl << G4endl;
        }
        else if(command == Cmdpcmat)
        {
                G4String commandpcmat = newValue;
                G4cout << commandpcmat << " manually set as photocathode material." << G4endl;
                DetectorA->SetPC_material(commandpcmat);
        }
        else if(command == CmdnPMT)
        {
                G4int thecmdnPMT = CmdnPMT->GetNewIntValue(newValue);
                DetectorA->SetnPMT(thecmdnPMT);
                G4cout << "The number of photocathodes per cherenkov Detector manually set to: " << thecmdnPMT << G4endl;
        }
        else if(command == CmdChopZ)
        {
                G4double thecmdchopz = CmdChopZ->GetNewDoubleValue(newValue);
                DetectorA->SetChopper_z(thecmdchopz);
                G4cout << "The Chopper distance from the source manually set to: " << thecmdchopz << " cm" << G4endl;
        }
        else if(command == CmdChopthick)
        {
                G4double thecmdchopthick = CmdChopthick->GetNewDoubleValue(newValue);
                DetectorA->SetChopperThick(thecmdchopthick);
                G4cout << "The Chopper thickness manually set to: " << thecmdchopthick << " mm" <<G4endl;
        }
        else if(command == CmdChopperOn)
        {
                G4String thecmdchopperon = newValue;
                if(thecmdchopperon == "On" || thecmdchopperon == "on")
                {
                        DetectorA->SetChopperOn(true);
                        G4cout << "The Chopper state set to On!" << G4endl;
                }
                else
                {
                        DetectorA->SetChopperOn(false);
                        G4cout << "The Chopper state set to Off!" << G4endl;
                }
        }
        else if(command == CmdChopperAbundance)
        {
                G4double thechopperabundance = CmdChopperAbundance->GetNewDoubleValue(newValue);
                DetectorA->SetChopperAbundance(thechopperabundance);
                G4cout << "The Chopper isotope abundance manually set to: " << thechopperabundance << " percent" << G4endl;
        }
        else if(command == CmdAngle)
        {
                G4double thecmdAngle = CmdAngle->GetNewDoubleValue(newValue);
                DetectorA->SettheAngle(thecmdAngle);
                G4cout << "The Detector angle manually set to: " << thecmdAngle << " degrees" << G4endl;
        }
        else if(command == CmdAttenOn)
        {
                G4String theCmdAttenOn = newValue;
                if(theCmdAttenOn == "Off" || theCmdAttenOn == "off")
                {
                        DetectorA->SetAttenuatorState(false);
                        DetectorA->SetAttenuatorMaterial("G4_AIR");
                        check_atten_on = false;
                }
                else
                {
                        check_atten_on = true;
                        DetectorA->SetAttenuatorState(true);
                }
                G4cout << "The first attenuation layer manually set to: " << theCmdAttenOn << G4endl;
        }
        else if(command == CmdAttenThick)
        {
                if(check_atten_on)
                {
                        G4double theCmdAttendThickness = CmdAttenThick->GetNewDoubleValue(newValue);
                        DetectorA->SetAttenuatorThickness(theCmdAttendThickness);
                        G4cout << "The first attenuation layer thickness manually set to: " << theCmdAttendThickness << " cm" << G4endl;
                }
        }
        else if(command == CmdAttenMat)
        {
                if(check_atten_on)
                {
                        G4String theAttenMaterial = newValue;
                        DetectorA->SetAttenuatorMaterial(theAttenMaterial);
                        G4cout << "The first attenuation layer material manually set to: " << theAttenMaterial << G4endl;
                }
        }
        else if(command == CmdAttenOn2)
        {
                G4String theCmdAttenOn2 = newValue;
                if(theCmdAttenOn2 == "Off" || theCmdAttenOn2 == "off")
                {
                        DetectorA->SetAttenuatorState2(false);
                        DetectorA->SetAttenuatorMaterial2("G4_AIR");
                        check_atten2_on = false;
                }
                else
                {
                        check_atten2_on = true;
                        DetectorA->SetAttenuatorState2(true);
                }
                G4cout << "The second attenuation layer manually set to: " << theCmdAttenOn2 << G4endl;
        }
        else if(command == CmdAttenThick2)
        {
                if(check_atten2_on)
                {
                        G4double theCmdAttendThickness2 = CmdAttenThick2->GetNewDoubleValue(newValue);
                        DetectorA->SetAttenuatorThickness2(theCmdAttendThickness2);
                        G4cout << "The second attenuation layer thickness manually set to: " << theCmdAttendThickness2 << " cm" << G4endl;
                }
        }
        else if(command == CmdAttenMat2)
        {
                if(check_atten2_on)
                {
                        G4String theAttenMaterial2 = newValue;
                        DetectorA->SetAttenuatorMaterial2(theAttenMaterial2);
                        G4cout << "The second attenuation layer material manually set to: " << theAttenMaterial2 << G4endl;
                }
        }
        else if(command == CmdPlexi)
        {
                G4double plexiThickness = CmdPlexi->GetNewDoubleValue(newValue);
                DetectorA->SetPlexiThickness(plexiThickness);
                G4cout << "The Plexiglass thickness manually set to: " << plexiThickness << " cm" << G4endl;
        }
        else if(command == CmdTape)
        {
                G4double tapeThickness = CmdTape->GetNewDoubleValue(newValue);
                DetectorA->SetTapeThickness(tapeThickness);
                G4cout << "The tape thickness manually set to: " << tapeThickness << " mm" << G4endl;
        }
        else if(command == CmdVis)
        {
                G4String theCmdVis = newValue;
                if(theCmdVis == "True" || theCmdVis == "true")
                {
                        DetectorA->SetDetectorViewOnly(true);
                        G4cout << "Detector Visualization View Only set to True!" << G4endl;
                }
        }
        else if(command == CmdChopMaterial)
        {
                G4String theCmdChopMaterial = newValue;
                DetectorA->SetChopperMaterial(theCmdChopMaterial);
                G4cout << "The chopper material manually set to: " << theCmdChopMaterial << G4endl;
        }
        else if(command == CmdVerbose)
        {
                G4String theVerbosity = newValue;
                G4bool theVerbositybool = false;
                if(theVerbosity == "On" || theVerbosity == "on" || theVerbosity == "True" || theVerbosity == "true")
                {
                        G4cout << "The Material Verbosity Setting manually set to: " << theVerbosity << G4endl;
                        theVerbositybool = true;
                }
                DetectorA->SetMaterialVerbosity(theVerbositybool);
        }
        else if(command == CmdCheckOverlaps)
        {
                G4String theCheck = newValue;
                G4bool theCheckBool = true;
                if(theCheck == "false" || theCheck == "False")
                {
                        G4cout << "CheckOverlaps set to off!" << G4endl;
                        theCheckBool = false;
                }
                DetectorA->SetCheckOverlaps(theCheckBool);
        }
        else
        {
                G4cerr << "ERROR DetectorMessenger :: SetDetectorInputValue command != Cmd" << G4endl;
        }
}
