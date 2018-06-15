#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooGaussian.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TAxis.h"
#include "RooPlot.h"
#include "RooFitResult.h"
#include "RooGenericPdf.h"
#include "RooConstVar.h"
#include "RooCBShape.h"
#include "RooAbsReal.h"

#ifndef __CINT__
#include "PolarizationPdf.h"
#endif

using namespace RooFit;

void fitTo_CosTheta_Phi_unbinned(){

  #ifdef __CINT__
    gROOT->ProcessLineSync(".x PolarizationPdf.cxx+") ;
  #endif

  double PI = TMath::Pi();

  //============================================================================
  // Importing data ...
  //============================================================================

  TFile *input_file = new TFile("/home/luca/cernbox/JPSI/JPSI_POLARIZATION/ANALYSIS/UNBINNED_APPROACH/File_for_UnbinnedFit_FullStat.root","READ");
  TTree* tree = (TTree*) input_file -> Get("output_tree");

  //RooRealVar DimuMass_unb("DimuMass_unb","DimuMass_unb",2,5);
  //DimuMass_unb.setRange("sig",2.9,3.3);
  //DimuMass_unb.setRange("bck_left",2,2.9);
  //DimuMass_unb.setRange("bck_right",3.3,5);
  RooRealVar CostHE_unb("CostHE_unb","cos#it{#theta}^{HX}",-1,1);
  RooRealVar PhiHE_unb("PhiHE_unb","#it{#varphi}^{HX}",0,PI);
  RooDataSet minv_dataset("minv_dataset","minv_dataset",RooArgSet(CostHE_unb,PhiHE_unb),Import(*tree));

  RooRealVar N("N","N",1);
  RooRealVar lambdaTheta("lambdaTheta","lambdaTheta",0,-1,1);
  RooRealVar lambdaPhi("lambdaPhi","lambdaPhi",0,-1,1);
  RooRealVar lambdaThetaPhi("lambdaThetaPhi","lambdaThetaPhi",0,-1,1);
  PolarizationPdf Polarization_func("Polarization_func","VWG_bck",CostHE_unb,PhiHE_unb,N,lambdaTheta,lambdaPhi,lambdaThetaPhi);

  RooRealVar sig("signal","signal yield",1.70000e+06,500,3.70000e+06);
  RooAddPdf model("model","polarization PDF",RooArgList(Polarization_func),RooArgList(sig));

  RooRealVar N_Axe("N_Axe","N_Axe",1);
  RooRealVar lambdaTheta_Axe("lambdaTheta_Axe","lambdaTheta_Axe",-1.22039);
  RooRealVar lambdaPhi_Axe("lambdaPhi_Axe","lambdaPhi_Axe",-1.21131e-01);
  RooRealVar lambdaThetaPhi_Axe("lambdaThetaPhi_Axe","lambdaThetaPhi_Axe",-4.89281e-02);
  PolarizationPdf Polarization_func_Axe("Polarization_func_Axe","Polarization_func_Axe",CostHE_unb,PhiHE_unb,N_Axe,lambdaTheta_Axe,lambdaPhi_Axe,lambdaThetaPhi_Axe);

  RooEffProd modelAxe("modelAxe","model with Axe",model,Polarization_func_Axe) ;

  //modelAxe.fitTo(minv_dataset);

  TH1 *hist_minv_dataset = minv_dataset.createHistogram("CostHE_unb,PhiHE_unb",100,100);

  TCanvas* c2 = new TCanvas("rf309_3dimplot","rf309_3dimplot",800,400) ;
  //gPad->SetLeftMargin(0.15) ; hist_minv_dataset->GetZaxis()->SetTitleOffset(1.4) ; hist_minv_dataset->Draw("box");
  model.Draw("SURF");
  //modelAxe.Draw("SURF");

  return;

  //============================================================================
  // Fitting functions
  //============================================================================

  // CB2Pdf signal
  RooRealVar N("N","N",1);
  RooRealVar MEAN("MEAN","MEAN",3.096,2.9,3.3);
  RooRealVar SIGMA("SIGMA","SIGMA",0.07,0.05,0.09);
  RooRealVar ALPHA1("ALPHA1","ALPHA1",1.06);
  RooRealVar A1("A1","A1",3.23);
  RooRealVar ALPHA2("ALPHA2","ALPHA2",2.55);
  RooRealVar A2("A2","A2",1.56);
  CB2Pdf CB2_sig("CB2_sig","CB2_sig",DimuMass_unb,N,MEAN,SIGMA,ALPHA1,A1,ALPHA2,A2);

  // CBPdf signal
  RooCBShape CB_sig("CB_sig","CB_sig",DimuMass_unb,MEAN,SIGMA,ALPHA1,A1);


  // VWGPdf background
  RooRealVar N("N","N",1);
  RooRealVar M("M","M",1,-50,50);
  RooRealVar A("A","A",0.5,-10,10);
  RooRealVar B("B","B",0.2,-10,10);
  VWGPdf VWG_bck("VWG_bck","VWG_bck",DimuMass_unb,N,M,A,B);

  // model = CB2 + VWG
  RooRealVar sig("signal","signal yield",1.70000e+03,500,3000);
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
