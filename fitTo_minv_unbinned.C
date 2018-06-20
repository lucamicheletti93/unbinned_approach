#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooGenericPdf.h"
#include "RooConstVar.h"
#include "RooCBShape.h"
#include "RooAbsReal.h"
#include "RooAddPdf.h"
#include "RooClassFactory.h"
#include <TROOT.h>
#include <TMinuit.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TF1.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLine.h>
#include <TMath.h>
#include <TPad.h>
#include <TSystem.h>
#include <TGraphErrors.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TFitResult.h>
#include <TMatrixDSym.h>
#include <TPaveText.h>
#include <TCollection.h>
#include <TKey.h>
#include <TGaxis.h>


#ifndef __CINT__
#include "CB2Pdf.h"
#endif

using namespace RooFit;

////////////////////////////////////////////////////////////////////////////////
// To run in compiled mode :
//      - .L CB2Pdf.cxx+
//      - .x fitTo_minv_unbinned.C+
////////////////////////////////////////////////////////////////////////////////

void fitTo_minv_unbinned(){

  #ifdef __CINT__
    gROOT->ProcessLineSync(".x CB2Pdf.cxx+") ;
  #endif

  double PI = TMath::Pi();

  //============================================================================
  // Importing data ...
  //============================================================================

  //TFile *input_file = new TFile("../Unbinned_tree_full_statistics.root","READ");
  TFile *input_file = new TFile("../subsample_matrix_tree_file.root","READ");
  //TTree* tree = (TTree*) input_file -> Get("output_tree");

  for(int i = 1;i < 17;i++){
    for(int j = 1;j < 10;j++){
      TTree* tree = (TTree*) input_file -> Get(Form("subsample_tree_%i_%i",i,j));

      RooRealVar DimuMass_unb("DimuMass_unb_subsample","DimuMass_unb_subsample",2,5);
      DimuMass_unb.setRange("sig",2.9,3.3);
      DimuMass_unb.setRange("bck_left",2,2.9);
      DimuMass_unb.setRange("bck_right",3.3,5);
      RooRealVar CosthHE_unb("CosthHE_unb","CosthHE_unb",-1,1);
      RooRealVar PhiHE_unb("PhiHE_unb","PhiHE_unb",-PI,PI);
      RooDataSet dataset("dataset","dataset",RooArgSet(DimuMass_unb),Import(*tree));

      //============================================================================
      // Fitting functions
      //============================================================================


      RooRealVar mean_CB2("#it{m}_{J/#psi}","mean CB2",3.096,2.9,3.3);
      RooRealVar width_CB2("#it{#sigma}_{J/#psi}","width CB2",0.07,0.05,0.09);
      RooRealVar alpha1_CB2("#alpha1_{CB2}","alpha1 CB2",1.06);
      RooRealVar n1_CB2("n1_{CB2}","n1 CB2",3.23);
      RooRealVar alpha2_CB2("#alpha2_{CB2}","alpha2 CB2",2.55);
      RooRealVar n2_CB2("n2_{CB2}","n2 CB2",1.56);
      CB2Pdf sig_CB2("sig_CB2","sig_CB2",DimuMass_unb,mean_CB2,width_CB2,alpha1_CB2,n1_CB2,alpha2_CB2,n2_CB2);
      RooRealVar sig("signal","signal yield",1.70000e+03,500,3000);

      RooRealVar mean_VWG("mean_{VWG}","mean VWG",2,-10,10);
      RooRealVar alpha_VWG("#alpha_{VWG}","alpha VWG",0.5,-10,10);
      RooRealVar beta_VWG("#beta_{VWG}","beta VWG",0.2,-10,10);
      RooGenericPdf bck_VWG("bck_VWG","bck_VWG","exp(-(@0 - @1)*(@0 - @1)/(2*(@2 + @3*((@0 - @1)/@1))*(@2 + @3*((@0 - @1)/@1))))",RooArgList(DimuMass_unb,mean_VWG,alpha_VWG,beta_VWG));
      RooRealVar bck("background","background yield",3.45165e+04,1000,500000);


      RooAddPdf model("model","CB2 + VWG",RooArgList(sig_CB2,bck_VWG),RooArgList(sig,bck));
      model.fitTo(dataset,Extended(),Save());


      //model.fitTo(minv_dataset);

      //============================================================================
      // Drawing the plot ...
      //============================================================================

      //RooPlot* plot = DimuMass_unb.frame();
      //minv_dataset.plotOn(plot);
      //model.plotOn(plot,Components(VWG_bck),LineStyle(kDashed),LineColor(kGray));
      //model.plotOn(plot,Components(CB2_sig),LineStyle(kDashed),LineColor(kRed+1));
      //model.plotOn(plot);
      //plot -> Draw();
    }
  }

}
