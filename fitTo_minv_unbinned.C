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

using namespace RooFit;

void fitTo_minv_unbinned(){

  double PI = TMath::Pi();

  //============================================================================
  // Importing data ...
  //============================================================================

  TFile *input_file = new TFile("../Unbinned_tree_full_statistics.root","READ");
  TTree* tree = (TTree*) input_file -> Get("output_tree");

  RooRealVar DimuMass_unb("DimuMass_unb","DimuMass_unb",2,5);
  DimuMass_unb.setRange("sig",2.9,3.3);
  DimuMass_unb.setRange("bck_left",2,2.9);
  DimuMass_unb.setRange("bck_right",3.3,5);
  RooRealVar CosthHE_unb("CosthHE_unb","CosthHE_unb",-1,1);
  RooRealVar PhiHE_unb("PhiHE_unb","PhiHE_unb",-PI,PI);
  RooDataSet minv_dataset("minv_dataset","minv_dataset",RooArgSet(DimuMass_unb),Import(*tree));

  //============================================================================
  // Fitting functions
  //============================================================================

  // signal = gaussian
  RooRealVar mean("mean","mean",3.1,2.9,3.3);
  RooRealVar sigma("sigma","sigma",0.07,0.05,0.1);
  RooGaussian gauss("gauss","gaussian PDF",DimuMass_unb,mean,sigma);

  // background = exponential
  RooRealVar lambda("lambda","lambda",-1.12058e+00,-3.,0.);
  RooExponential expo("expo","exponential PDF",DimuMass_unb,lambda);

  // model = gaussian + exponential
  RooRealVar sig("signal","signal yield",1.70000e+05,150000,300000);
  RooRealVar bck("background","background yield",3.45165e+06,1000000,5000000);
  RooAddPdf model("model","gaussian plus exponential PDF",RooArgList(gauss,expo),RooArgList(sig,bck));
  model.fitTo(minv_dataset);

  //============================================================================
  // Drawing the plot ...
  //============================================================================

  RooPlot* plot = DimuMass_unb.frame();
  minv_dataset.plotOn(plot);
  model.plotOn(plot,Components(expo),LineStyle(kDashed),LineColor(kRed));
  model.plotOn(plot,Components(gauss),LineStyle(kDashed),LineColor(kRed+2));
  model.plotOn(plot);
  plot -> Draw();
}
