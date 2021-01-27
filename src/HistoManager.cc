// User histogram manager action class for the Cargo Container NRF simulations
// Jacob E Bickus, MIT, 2020
// jbickus@mit.edu

#include "HistoManager.hh"

extern G4String gOutName;
HistoManager::HistoManager(): fFactoryOn(false), chosen_energy(-1)
{
}

HistoManager::~HistoManager()
{
}

void HistoManager::Book(G4bool bremTest)
{
  G4AnalysisManager* manager = G4AnalysisManager::Instance();
  G4int nbins = 100000;
  manager->SetVerboseLevel(0);
  manager->SetNtupleMerging(true);
  xmax = chosen_energy;
  #if defined (G4ANALYSIS_USE_ROOT)
      if(!bremTest && chosen_energy < 0)
      {
          TFile *fin = TFile::Open("brems_distributions.root");
          hBrems  = (TH1D*) fin->Get("hBrems");
          if (hBrems)
          {
              G4cout << "Imported brems from " << fin->GetName() << G4endl << G4endl;
              xmax = hBrems->GetXaxis()->GetXmax();
              std::cout << "Found Input Max Energy: " << xmax << " MeV" << std::endl;
              G4cout << "Found Input Max Energy: " << xmax << " MeV" << G4endl;
              fin->Close();
          }
          else
          {
                  std::cerr << "Error reading from file " << fin->GetName() << std::endl;
                  exit(1);
          }
      }

  #endif

  // open output file
  G4bool fileOpen = manager->OpenFile(gOutName);

  if(! fileOpen){
    G4cerr << "HistoManager::Book(): Cannot Open " <<manager->GetFileName()<<G4endl;
    return;
  }
  else
  {
    G4cout << "HistoManager::Book() Opened!" << G4endl;
  }
  
  // Create ID 0 Ntuple for Incident Chopper Data 
  manager->CreateNtuple("ChopIn", "Chopper Wheel Incident Data");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("Weight");
  manager->CreateNtupleIColumn("EventID");
  manager->FinishNtuple();
  // Create ID 1 Ntuple for Exiting Chopper Data
  manager->CreateNtuple("ChopOut", "Chopper Wheel Exiting Radiation Data");
  manager->CreateNtupleDColumn("Energy");
  manager->CreateNtupleDColumn("Weight");
  manager->CreateNtupleIColumn("EventID");
  manager->CreateNtupleIColumn("isNRF");
  manager->FinishNtuple();
  
  if(!bremTest)
  {
      // Create ID 0,1,2,3 1D Histogram for Interogation Object Data
      manager->CreateH1("IntObjIn","Interrogation Object Incident Weighted Energy Spectrum", nbins, 0., xmax, "MeV");
      manager->CreateH1("NRFIntObjIn", "Interrogation Object NRF Photons Incident Weighted Energy Spectrum",nbins,0.,xmax, "MeV");
      manager->CreateH1("IntObjOut", "Interrogation Object Exiting Weighted Energy Spectrum", nbins, 0., xmax, "MeV");
      manager->CreateH1("NRFIntObjOut", "Interrogation Object NRF Photons Exiting Weighted Energy Spectrum", nbins, 0., xmax, "MeV");
      // Create ID 4,5 1D Histogram for incident water data
      manager->CreateH1("WaterIn", "Water Tank Incident Weighted Energy Spectrum", nbins,0., xmax, "MeV");
      manager->CreateH1("NRFWaterIn","Water Tank NRF Photons Incident Weighted Energy Spectrum", nbins, 0., xmax, "MeV");
      
      // Create ID 2 Ntuple for NRF Materials 
      manager->CreateNtuple("NRFMatData","NRF Material vs Energy");
      manager->CreateNtupleIColumn("EventID");
      manager->CreateNtupleIColumn("TrackID");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleDColumn("Weight");
      manager->CreateNtupleSColumn("Material");
      manager->CreateNtupleDColumn("ZPos");
      manager->FinishNtuple();
    
      // Create ID 3 Ntuple for cherenkov in water 
      manager->CreateNtuple("Cherenkov","Cherenkov in Water Data");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleDColumn("Weight");
      manager->CreateNtupleIColumn("EventID");
      manager->CreateNtupleIColumn("TrackID");
      manager->CreateNtupleIColumn("NumSecondaries");
      manager->CreateNtupleDColumn("XPos");
      manager->CreateNtupleDColumn("YPos");
      manager->CreateNtupleDColumn("ZPos");
      manager->CreateNtupleDColumn("Time");
      manager->FinishNtuple();

      // Create 6 Histogram for Energy if detected
      manager->CreateH1("Detected","Photons Detected by Photocathode Weighted Energy Spectrum", nbins, 0., 2.1, "MeV");
    
      // Create ID 4 Ntuple for Reactions within detector
      manager->CreateNtuple("DetInfo","Detected Information");
      manager->CreateNtupleIColumn("EventID");
      manager->CreateNtupleIColumn("TrackID");
      manager->CreateNtupleDColumn("Energy");
      manager->CreateNtupleDColumn("Weight");
      manager->CreateNtupleSColumn("CreatorProcess");
      manager->CreateNtupleSColumn("DetectionProcess");
      manager->CreateNtupleDColumn("Time");
      manager->FinishNtuple();
  }

  fFactoryOn = true;

}

void HistoManager::finish()
{
    if(! fFactoryOn){
        G4cout << "ERROR HistoManager::finish: Failed to write to file" << G4endl;
        return;
    }
    G4AnalysisManager* manager = G4AnalysisManager::Instance();
    manager->Write();
    manager->CloseFile();
    G4cout << "Ntuples are saved. " << G4endl;
    delete manager;
    fFactoryOn = false;
}

