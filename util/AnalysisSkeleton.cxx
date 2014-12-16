#include "IFAEFramework/NtupleData.h"
#include "IFAEFramework/NtupleReader.h"
#include "IFAEFramework/OptionsBase.h"
#include "IFAEFramework/HistManager.h"

#include <iostream>
int main(int argc, char** argv){

  //Read in the options and send to an optionsbase
  //declare a ntupledata
  //pass the ntupldata and options base to the ntuplereader
  //set branch addresses
  //declare a histmanager and book the output histograms
  //loop over the tree and fill the histograms
  //finalize the histograms 

  OptionsBase* optBase = new OptionsBase();
  optBase->parseUserOpts(argc, argv);

  NtupleData* nD = new NtupleData();
  NtupleReader* ntupReader = new NtupleReader(nD, optBase);
  ntupReader->Init();
  ntupReader->setEventBranchAddresses();
  ntupReader->setJetBranchAddresses("jet_AntiKt4LCTopo");
  int nentries = ntupReader->chainNEntries();

  HistManager* hstMngr = new HistManager();
  hstMngr->BookTH1D("jet_pt", "jet_pt", 20., 0., 1000.);
  hstMngr->BookTH2D("nvtx_vs_mu", "nvtx_vs_mu", 1.,0.,20.,1.,0.,30.);

  //------------------------------------
  //Loop
  std::cout<<"nentries = "<<nentries<<endl;
  nentries = 100;
  for(int n = 0; n < nentries; n++){
    if(n%50 == 0){std::cout<<"n = "<<n<<std::endl;}
    ntupReader->getChainEntry(n);
    for(int j = 0; j < nD->jet_n; j++){
      hstMngr->FillTH1D("jet_pt", nD->jet_pt->at(j)/1000.);
    }
    hstMngr->FillTH2D("nvtx_vs_mu", nD->vxp_n, nD->mu);
  }
  std::cout<<"Finished event loop"<<std::endl;

  TFile* outfile = TFile::Open("testOutputFile", "RECREATE");


  vector<string> h1list = hstMngr->GetTH1KeyList(); 
  std::cout<<"Histogram Wrap-up"<<std::endl;
  for(vector<string>::iterator hit = h1list.begin(); hit != h1list.end(); hit++){
    hstMngr->FinaliseTH1Bins(*hit);
    outfile->cd();
    hstMngr->GetTH1D(*hit)->Write();    
    hstMngr->ClearTH1(*hit);
  }

  vector<string> h2list = hstMngr->GetTH2KeyList(); 
  std::cout<<"Histogram Wrap-up"<<std::endl;
  for(vector<string>::iterator hit = h2list.begin(); hit != h2list.end(); hit++){
    outfile->cd();
    hstMngr->GetTH2D(*hit)->Write();    
    hstMngr->ClearTH2(*hit);
  }



  std::cout<<"Ntuple Reader Finalise"<<endl;
  ntupReader->Finalise();
  std::cout<<"Delete NtupleReader"<<std::endl;
  delete ntupReader;
  std::cout<<"Delete NtupleData"<<std::endl;
  //delete nD;
  std::cout<<"Delete HistManager"<<std::endl;
  delete hstMngr;
  std::cout<<"Delete OptionsBase"<<std::endl;
  //delete optBase;
  std::cout<<"THE END"<<std::endl;

  //-------------------------------------




  return 0; 
}
