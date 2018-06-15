#if !defined(__CINT__) || defined(__MAKECINT__)
#include <stdio.h>

#include <TCanvas.h>
#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TProfile.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TLine.h>
#include <TList.h>
#include <TSystem.h>
#include <TGrid.h>
#include <TString.h>
#include <TStopwatch.h>
#include <TGraphErrors.h>
#include <TMath.h>
#include <TStopwatch.h>
#endif

void create_sub_tree(){

  double value_cost[19] = {-1,-0.5,-0.4,-0.3,-0.2,-0.16,-0.12,-0.08,-0.04,0,0.04,0.08,0.12,0.16,0.2,0.3,0.4,0.5,1};
  double value_phi[11] = {0,0.942478,1.13097,1.25664,1.3823,1.5708,1.75929,1.88496,2.01062,2.19911,3.14159};
  value_phi[10] = TMath::Pi();

  TFile *input_file = new TFile("../Unbinned_tree_full_statistics.root","READ");

  Double_t DimuMass_unb;
  Double_t CostHE_unb, PhiHE_unb;

  TTree *input_tree = (TTree*) input_file -> Get("output_tree");
  input_tree -> SetBranchAddress("DimuMass_unb",&DimuMass_unb);
  input_tree -> SetBranchAddress("CostHE_unb",&CostHE_unb);
  input_tree -> SetBranchAddress("PhiHE_unb",&PhiHE_unb);

  Double_t DimuMass_unb_subsample;
  TFile *subsample_file = new TFile("../subsample_matrix_tree_file.root","RECREATE");
  TTree *subsample_tree[18][10];
  char TREE_NAME[100];

  for(int i = 0;i < 18;i++){
    for(int j = 0;j < 10;j++){
      sprintf(TREE_NAME,"subsample_tree_%i_%i",i,j);
      subsample_tree[i][j] = new TTree(TREE_NAME,TREE_NAME);
      subsample_tree[i][j] -> Branch("DimuMass_unb_subsample",&DimuMass_unb_subsample,"DimuMass_unb_subsample/D");
    }
  }
  //subsample_tree -> Branch("DimuMass_unb_subsample",&DimuMass_unb_subsample,"DimuMass_unb_subsample/D");


  int index_cost = 0, index_phi = 0;
  for(int i = 0;i < input_tree -> GetEntries();i++){
    input_tree -> GetEntry(i);
    //if(CostHE_unb > 0. && CostHE_unb < 0.1){
      //if(TMath::Abs(PhiHE_unb) > 1. && TMath::Abs(PhiHE_unb) < 1.2){
        //DimuMass_unb_subsample = DimuMass_unb;
        //subsample_tree -> Fill();
      //}
    //}

    while(CostHE_unb < value_cost[index_cost] || CostHE_unb >= value_cost[index_cost+1]) index_cost++;
    //printf("[%f - %f] -> %f \n",value_cost[index_cost],value_cost[index_cost+1],CostHE_unb);
    while(TMath::Abs(PhiHE_unb) < value_phi[index_phi] || TMath::Abs(PhiHE_unb) >= value_phi[index_phi+1]) index_phi++;
    //printf("[%f - %f] -> %f \n",value_phi[index_phi],value_phi[index_phi+1],PhiHE_unb);
    printf("%f %i - %f %i \n",CostHE_unb,index_cost,TMath::Abs(PhiHE_unb),index_phi);

    DimuMass_unb_subsample = DimuMass_unb;
    subsample_tree[index_cost][index_phi] -> Fill();

    index_cost = 0;
    index_phi = 0;
  }

  subsample_file -> cd();
  for(int i = 0;i < 18;i++){
    for(int j = 0;j < 10;j++){
      subsample_tree[i][j] -> Write();
    }
  }

}
