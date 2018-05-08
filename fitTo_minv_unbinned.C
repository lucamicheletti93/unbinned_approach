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

#ifndef __CINT__
#include "VWGPdf.h"
#include "CB2Pdf.h"
#endif

using namespace RooFit;

void fitTo_minv_unbinned(){

  #ifdef __CINT__
    gROOT->ProcessLineSync(".x VWGPdf.cxx+") ;
    gROOT->ProcessLineSync(".x CB2Pdf.cxx+") ;
  #endif

  double PI = TMath::Pi();

  //============================================================================
  // Importing data ...
  //============================================================================

  //TFile *input_file = new TFile("../Unbinned_tree_full_statistics.root","READ");
  TFile *input_file = new TFile("../subsample_file.root","READ");
  //TTree* tree = (TTree*) input_file -> Get("output_tree");
  TTree* tree = (TTree*) input_file -> Get("subsample_tree");

  RooRealVar DimuMass_unb("DimuMass_unb_subsample","DimuMass_unb_subsample",2,5);
  DimuMass_unb.setRange("sig",2.9,3.3);
  DimuMass_unb.setRange("bck_left",2,2.9);
  DimuMass_unb.setRange("bck_right",3.3,5);
  RooRealVar CosthHE_unb("CosthHE_unb","CosthHE_unb",-1,1);
  RooRealVar PhiHE_unb("PhiHE_unb","PhiHE_unb",-PI,PI);
  RooDataSet minv_dataset("minv_dataset","minv_dataset",RooArgSet(DimuMass_unb),Import(*tree));

  //============================================================================
  // Fitting functions
  //============================================================================

  // CB2Pdf signal
  RooRealVar N("N","N",1,0,2);
  RooRealVar MEAN("MEAN","MEAN",3.096,2.9,3.3);
  RooRealVar SIGMA("SIGMA","SIGMA",0.07,0.05,0.09);
  RooRealVar ALPHA1("ALPHA1","ALPHA1",1.06);
  RooRealVar A1("A1","A1",3.23);
  RooRealVar ALPHA2("ALPHA2","ALPHA2",2.55);
  RooRealVar A2("A2","A2",1.56);
  CB2Pdf CB2_sig("CB2_sig","CB2_sig",DimuMass_unb,N,MEAN,SIGMA,ALPHA1,A1,ALPHA2,A2);

  // VWGPdf background
  RooRealVar N("N","N",1,0,2);
  RooRealVar M("M","M",1,-50,50);
  RooRealVar A("A","A",0.5,-10,10);
  RooRealVar B("B","B",0.2,-10,10);
  VWGPdf VWG_bck("VWG_bck","VWG_bck",DimuMass_unb,N,M,A,B);

  // model = CB2 + VWG
  //RooRealVar sig("signal","signal yield",1.70000e+05,150000,300000);
  RooRealVar sig("signal","signal yield",1.70000e+03,500,3000);
  //RooRealVar bck("background","background yield",3.45165e+06,1000000,5000000);
  RooRealVar bck("background","background yield",3.45165e+04,10000,500000);
  RooAddPdf model("model","gaussian plus exponential PDF",RooArgList(CB2_sig,VWG_bck),RooArgList(sig,bck));
  model.fitTo(minv_dataset);

  //============================================================================
  // Drawing the plot ...
  //============================================================================

  RooPlot* plot = DimuMass_unb.frame();
  minv_dataset.plotOn(plot);
  model.plotOn(plot,Components(VWG_bck),LineStyle(kDashed),LineColor(kGray));
  model.plotOn(plot,Components(CB2_sig),LineStyle(kDashed),LineColor(kRed+1));
  model.plotOn(plot);
  plot -> Draw();
}
