//
// ************************************************************************************************ //
// ************************************************************************************************ //
// To Run File in ROOT CERN:
// root -b 'SNR.cc("testOn.root")'
// ************************************************************************************************ //
// ************************************************************************************************ //
// File Explanation:
//
// Requires 1 inputs
// 1. Filename. const char*
//
// This File Scans the File for the IntObjIn and IntObjOut TTrees and finds the SNR

void SNR(const char* inFile)
{
  // Check to make sure file exists
  if(gSystem->AccessPathName(inFile))
  {
    std::cerr << "ERROR Could not find " << inFile << "exiting..." << std::endl;
    exit(1);
  }
  TFile *f = new TFile(inFile);
  f->cd();
  TTree *aIntObjIn, *aIntObjOut;
  aIntObjIn = (TTree*) f->Get("IntObjIn");
  aIntObjOut = (TTree*) f->Get("IntObjOut");

  if(aIntObjIn !=0 && aIntObjOut !=0)
  {
    aIntObjIn->Print();
    aIntObjOut->Print();
    double inMax = aIntObjIn->GetMaximum("Energy");
    double outMax = aIntObjOut->GetMaximum("Energy");
    aIntObjIn->SetEstimate(-1);
    aIntObjOut->SetEstimate(-1);
    TH1D* e1 = new TH1D("e1","IntObjIn NRF Histogram",100,1.73350, 1.73360);
    TH1D* e2 = new TH1D("e2","IntObjOut NRF Histogram",100,1.73350,1.73360);
    TH1D* eT = new TH1D("eT","IntObjIn Histogram",100,0.0,inMax);
    TH1D* eT2 = new TH1D("eT2","IntObjOut Histogram",100,0.0,outMax);
    aIntObjIn->Draw("Energy>>e1","Weight","goff");
    aIntObjOut->Draw("Energy>>e2","Weight","goff");
    aIntObjIn->Draw("Energy>>eT","Weight","goff");
    aIntObjOut->Draw("Energy>>eT2","Weight","goff");
    e1->Print();
    eT->Print();
    e2->Print();
    eT2->Print();
    int binStart = e1->GetXaxis()->FindBin(1.73354);
    int binEnd = e1->GetXaxis()->FindBin(1.73355);
    double inSignal = e1->Integral(binStart, binEnd);
    std::cout << "IntObjIn Signal: \t" << inSignal << std::endl;
    double inNoise = eT->Integral();
    std::cout << "IntObjIn Noise: \t" << inNoise << std::endl;
    double outSignal = e2->Integral(binStart,binEnd);
    double outNoise = eT2->Integral();
    std::cout << "The Larger the SNR the Better the Interrogation Beam." << std::endl;
    std::cout << "IntObjIn SNR: \t" << abs(inSignal - inNoise)/sqrt(inNoise) << std::endl;
    std::cout << "IntObjOut SNR: \t" << abs(outSignal - outNoise)/sqrt(outNoise) << std::endl;
  }
}
