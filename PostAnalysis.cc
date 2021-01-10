// To Run in Root: root -b -q 'PostAnalysis.cc("ChopOn.root",ChopOff.root")'
void PostAnalysis(const char *ChopOn, const char *ChopOff)
{
    std::cout << "Setting Chop On File: " << ChopOn << std::endl;
    std::cout << "Setting Chop Off File: " << ChopOff << std::endl;
    string response = "Processing an older file structure...";
    
    TFile *chopOn = new TFile(ChopOn);
    TFile *chopOff = new TFile(ChopOff);
    bool confirmation = chopOn->cd();
    double chopOn_entries, intObjOn_entries, chopOn_sum, intObjOn_sum;
    double chopOn_inc_sum = 0;
    
    double chopOff_entries, intObjOff_entries, chopOff_sum, intObjOff_sum;
    double chopOff_inc_sum = 0;
    
    // Variables Set, Complete Calculations 
    
    if(confirmation)
    {
        TH1D *Detected;
        TH1D *IntObj;
        TH1D *NRFIncWater;
        TH1D *IncWater; // for processing earlier runs 
        chopOn->GetObject("Detected",Detected);
        chopOn->GetObject("IncObj", IntObj);
        try
        {
            chopOn->GetObject("NRFIncWater",NRFIncWater);
            if(NRFIncWater == NULL)
            {
                throw response;
            }
            else chopOn_inc_sum = NRFIncWater->Integral();
        }
        catch(string response)
        {
            std::cout << response << std::endl;
            chopOn->GetObject("IncWater",IncWater);
            chopOn_inc_sum = IncWater->Integral();
            std::cout << "Older Chopper On File Processed." << std::endl;
        }
            
        chopOn_entries = Detected->GetEntries();
        intObjOn_entries = IntObj->GetEntries();
        chopOn_sum = Detected->Integral();
        intObjOn_sum = IntObj->Integral();
    }
    else
    {
        std::cout << "ERROR: Chopper On File with name: " << ChopOn << " not opened!!!"<< std::endl;
        return;
    }
    
    bool confirmation2 = chopOff->cd();
    if(confirmation2)
    {
        TH1D *Detected2;
        TH1D *NRFIncWater2;
        TH1D *IntObj2;
        TH1D *IncWater2; // for processing earlier runs 
        chopOff->GetObject("Detected",Detected2);
        chopOff->GetObject("IncObj", IntObj2);
        try
        {
            chopOff->GetObject("NRFIncWater",NRFIncWater2);
            if(NRFIncWater2 == NULL)
            {
                throw response;
            }
            else chopOff_inc_sum = NRFIncWater2->Integral();
        }
        catch(string response)
        {
            std::cout << response << std::endl;
            chopOff->GetObject("IncWater",IncWater2);
            chopOff_inc_sum = IncWater2->Integral();
            std::cout << "Older Chopper Off File Processed." << std::endl;
        }
            
        chopOff_entries = Detected2->GetEntries();
        intObjOff_entries = IntObj2->GetEntries();
        chopOff_sum = Detected2->Integral();
        intObjOff_sum = IntObj2->Integral();
        
    }
    else
    {
        std::cout << "ERROR: Chopper Off File with name: " << ChopOff << " not opened!!!"<<std::endl;
        return;
    }
    
    std::cout << "Files Read. Printing Results..." << std::endl;
    
    double intObj_z = abs(intObjOn_sum - intObjOff_sum)/(sqrt(pow(sqrt(intObjOn_sum),2) + pow(sqrt(intObjOff_sum),2)));
    double detected_z = abs(chopOn_sum - chopOff_sum)/(sqrt(pow(sqrt(chopOn_sum),2) + pow(sqrt(chopOff_sum),2)));
    
    std::cout << "Chopper On Interrogation Object Entries: " << intObjOn_entries << " On Sum: " << intObjOn_sum << std::endl;
    std::cout << "Chopper Off Interrogation Object Entries: " << intObjOff_entries << " Off Sum: " << intObjOff_sum << std::endl;
    std::cout << "Interrogation Object Z-test result: " << intObj_z << std::endl;
    std::cout << "Chopper On Detected Entries: " << chopOn_entries << " On Sum: " << chopOn_sum << " On NRF: " << chopOn_inc_sum << std::endl;
    std::cout << "Chopper Off Detected Entries: " << chopOff_entries << " Off Sum: " << chopOff_sum << " Off NRF: " << chopOff_inc_sum << std::endl;
    std::cout << "Detected Z-test result: " << detected_z << std::endl;
}
