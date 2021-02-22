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
extern G4bool debug;

PrimaryGeneratorAction::PrimaryGeneratorAction()
        : G4VUserPrimaryGeneratorAction(),
        fParticleGun(0)
{
        fParticleGun = new G4ParticleGun(1);
        if(!bremTest)
          beam_size = 80.0*mm;
        else
          beam_size = 1.3*mm;

        G4cout << "PrimaryGeneratorAction::PrimaryGeneratorAction -> Beam Size set to: " << beam_size << " mm" << G4endl;

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

                if(gSystem->AccessPathName(inFile.c_str()))
                {
                  G4cerr << "PrimaryGeneratorAction::PrimaryActionGenerator FATAL ERROR -> " << inFile << " NOT FOUND!" << G4endl;
                  exit(1);
                }
                TFile *fin = TFile::Open(inFile.c_str());
                fin->cd();
                hBrems  = (TH1D*) fin->Get("hBrems");
                hBrems->Print();
                if(!hBrems)
                {
                  G4cerr << "PrimaryGeneratorAction::PrimaryActionGenerator FATAL ERROR -> hBrems Fail." << G4endl;
                  exit(1);
                }

                if(!inFile.compare(0,24,"brems_distributions.root"))
                {
                    file_check = false;
                    gBrems = (TGraph*) fin->Get("Graph_from_hBrems");
                    hSample = (TH1D*) fin->Get("hSample");
                    gSample = (TGraph*) fin->Get("Graph_from_hSample");
                    hSample->Print();
                    if(!hSample || !gSample || !gBrems)
                    {
                      G4cerr << "PrimaryGeneratorAction::PrimaryGeneratorAction() -> FATAL ERROR Failure to grab TGraphs from File: " << inFile << G4endl;
                      exit(1);
                    }
                }
                else
                  file_check = true;
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
  // ******************** Energy Sampling Options *************************** //
    if(debug)
        std::cout << "PrimaryGeneratorAction::GeneratePrimaries -> Beginning." << std::endl;

        G4double w = 1.;
        if(!resonanceTest && chosen_energy < 0)
        {
          // User IS USING importance sampling
          if(!file_check)
          {
            // Grab random from importance sampling distribution
            energy = hSample->GetRandom()*MeV;
            // Grab contents of bin closest
            // TGraph::Eval -> Description: linear interpolation between the
            // two points close to x is computed. If x is outside the graph
            // range, a linear extrapolation is computed. Eval here returns the
            // probability per 5 eV for each respective distribution
            G4double dNdE = gBrems->Eval(energy);
            G4double importanceSampling = gSample->Eval(energy);
            // Create importance weighting based on the two distributions probability
            w = dNdE/importanceSampling;
          }
          // User IS NOT USING importance sampling
          else
            energy = hBrems->GetRandom()*MeV;
        }
        // The user has selected a mono-energetic beam
        else if(chosen_energy > 0)
          energy = chosen_energy*MeV;
        // User wishes to sample from Uranium resonance energies. dNdE will not
        // be used here
        else
          energy = SampleUResonances();

        // Set the energy
        fParticleGun->SetParticleEnergy(energy);

      if(debug)
        std::cout << "Particle Energy: " << energy/(MeV) << " MeV" << std::endl;

        const float pi=acos(-1);

        // Set beam position
        G4double x_r = beam_size*acos(G4UniformRand())/pi*2.*cos(360.*G4UniformRand()*CLHEP::deg);
        G4double y_r = beam_size*acos(G4UniformRand())/pi*2.*sin(360.*G4UniformRand()*CLHEP::deg);
        fParticleGun->SetParticlePosition(G4ThreeVector(x_r,y_r,beamStart*cm)); // set the electron beam far enough back behind brem radiator

        // Set beam momentum
        fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0,0,1)); // along z axis

        fParticleGun->GeneratePrimaryVertex(anEvent);

// Pass the event information
        eventInformation *anInfo = new eventInformation(anEvent);
        anInfo->SetWeight(w);
        anInfo->SetBeamEnergy(energy);
        anEvent->SetUserInformation(anInfo);
      if(debug)
        std::cout << "PrimaryActionGenerator::GeneratePrimaries() -> End!" << std::endl;
}

G4double PrimaryGeneratorAction::SampleUResonances()
 {
        std::vector<double> er;
        er.push_back(1.65624253132*MeV);
        er.push_back(1.7335537285*MeV);
        er.push_back(1.86232584382*MeV);

        G4int idx = Random.Integer(er.size());
        G4double de = 25.0*eV;

        return Random.Uniform(er[idx]-de, er[idx]+de);
}
