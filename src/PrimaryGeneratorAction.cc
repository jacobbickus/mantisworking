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
// jbickus@mit.edu
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////////////////////////////////

#include "PrimaryGeneratorAction.hh"

extern G4long seed;
extern G4String inFile;
extern G4double chosen_energy;
extern G4bool resonanceTest;
extern G4bool bremTest;

PrimaryGeneratorAction::PrimaryGeneratorAction()
        : G4VUserPrimaryGeneratorAction(),
        fParticleGun(0)
{
        fParticleGun = new G4ParticleGun(1);
        if(chosen_energy > 0)
                G4cout << "PrimaryGeneratorAction::Beam Energy > 0" << G4endl;
        
        G4cout << "PrimaryGeneratorAction::Beam Position Set to: (0,0," << beamStart << ")cm" << G4endl;
        if(bremTest)
        {
                fParticleGun->SetParticleDefinition(G4Electron::Definition());
                G4cout << "Particle Type set to Electron!" << G4endl;
                file_check = false;
        }
        else
        {
                fParticleGun->SetParticleDefinition(G4Gamma::Definition());
                G4cout << "Particle Type set to Gamma!" << G4endl;
        }

        // Default Kinematics
        fParticleGun->SetParticleTime(0.0*ns);

        if(chosen_energy < 0)
        {
                gRandom->SetSeed(seed);
                if(gSystem->AccessPathName(inFile.c_str()) == 0)
                {
                        TFile *fin = TFile::Open(inFile.c_str());
                        G4String fileName = (G4String)fin->GetName();
                        if(fileName.compare(0,24,"brems_distributions.root") == 0)
                        {
                                file_check = false;
                                hBrems  = (TH1D*) fin->Get("hBrems");
                                hSample = (TH1D*) fin->Get("hSample");

                                if (hBrems && hSample)
                                {
                                        G4cout << "PrimaryGeneratorAction::Imported brems and sampling distributions from " << fin->GetName() << G4endl << G4endl;
                                }

                                else
                                {
                                        G4cerr << "Error reading from file " << fin->GetName() << G4endl;
                                        exit(1);
                                }
                        }
                        else
                        {

                                hBrems = (TH1D*) fin->Get("ChopperIn_Weighted"); // the purpose of this functionality is to sample from a bremsstrahlung beam without importance sampling

                                if(hBrems)
                                {
                                        G4cout << "PrimaryGeneratorAction::Imported brems distribution from " << fin->GetName() << G4endl;
                                        file_check = true;
                                }
                                else
                                {
                                        G4cerr << "PrimaryGeneratorAction::Error reading from file " << fin->GetName() << G4endl;
                                        exit(1);
                                }
                        }
                }
                else
                {
                        G4cerr << "FATAL ERROR: PrimaryGeneratorAction:: " << inFile << " NOT FOUND!" << G4endl;
                        exit(1);
                }
        }
        else
        {
                file_check = false;
                G4cout << "PrimaryGeneratorAction::PrimaryGeneratorAction Chosen Energy set to: " << chosen_energy << " MeV" << G4endl;
        }
        G4cout << G4endl << "User Macro Inputs" << G4endl;
        G4cout << "----------------------------------------------------------------------" << G4endl;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
        delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

// Set Particle Energy (Must be in generate primaries)
        //std::cout << "PrimaryGeneratorAction::GeneratePrimaries -> Begin!" << std::endl;
        if(file_check)
        {
                energy = hBrems->GetRandom()*MeV;
        }
        else if(chosen_energy < 0 && !file_check)
        {
                energy = hSample->GetRandom()*MeV; // sample the resonances specified by hSample
        }

        else if(chosen_energy > 0 && !file_check)
        {
                energy = chosen_energy*MeV;
        }
        else if(resonanceTest && !file_check)
        {
                energy = SampleUResonances();
        }

        fParticleGun->SetParticleEnergy(energy);


        const float pi=acos(-1);
        G4double beam_size = 1.3*mm;
        // Set beam position
        G4double x_r = beam_size*acos(G4UniformRand())/pi*2.*cos(360.*G4UniformRand()*CLHEP::deg);
        G4double y_r = beam_size*acos(G4UniformRand())/pi*2.*sin(360.*G4UniformRand()*CLHEP::deg);
        fParticleGun->SetParticlePosition(G4ThreeVector(x_r,y_r,beamStart*cm)); // set the electron beam far enough back behind brem radiator


        // Set beam momentum
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1)); // along z axis

        fParticleGun->GeneratePrimaryVertex(anEvent);
        G4double w = 1.0;

        if(chosen_energy < 0 && !file_check)
        {
                G4double theSampling = hSample->GetBinContent(hSample->GetXaxis()->FindBin(energy));
                G4double dNdE = hBrems->GetBinContent(hBrems->GetXaxis()->FindBin(energy));
                w = dNdE/theSampling;
        }

// Pass the event information
        eventInformation *anInfo = new eventInformation(anEvent);
        anInfo->SetWeight(w);
        anInfo->SetBeamEnergy(energy);
        anEvent->SetUserInformation(anInfo);
        //std::cout << "PrimaryActionGenerator::GeneratePrimaries() -> End!" << std::endl;
}

G4double PrimaryGeneratorAction::SampleUResonances() {
        std::vector<double> er;
        er.push_back(1.65624253132*MeV);
        er.push_back(1.7335537285*MeV);
        er.push_back(1.86232584382*MeV);

        G4int idx = Random.Integer(er.size());
        G4double de = 25.0*eV;

        return Random.Uniform(er[idx]-de, er[idx]+de);
}
