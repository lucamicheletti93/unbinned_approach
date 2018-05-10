import ROOT
import math

ROOT.gROOT.ProcessLineSync(".x VWGPdf.cxx+")
ROOT.gROOT.ProcessLineSync(".x CB2Pdf.cxx+")

input_file = ROOT.TFile("../subsample_matrix_tree_file.root","READ")

DimuMass_unb = ROOT.RooRealVar("DimuMass_unb_subsample","#it{m}_{#mu#mu}",2,5)
CosthHE_unb = ROOT.RooRealVar("CosthHE_unb","cos#it{#theta}^{HX}",-1,1)
PhiHE_unb = ROOT.RooRealVar("PhiHE_unb","#it{#varphi}^{HX}",0,math.pi);

dataset = ROOT.RooDataSet("dataset","dataset",ROOT.RooArgSet(DimuMass_unb),ROOT.RooFit.Import(input_file.Get("subsample_tree_17_9")))

N_CB2 = ROOT.RooRealVar("N_CB2","N_CB2",1)
MEAN = ROOT.RooRealVar("MEAN","MEAN",3.096,2.9,3.3)
SIGMA = ROOT.RooRealVar("SIGMA","SIGMA",0.07,0.05,0.09)
ALPHA1 = ROOT.RooRealVar("ALPHA1","ALPHA1",1.06)
A1 = ROOT.RooRealVar("A1","A1",3.23)
ALPHA2 = ROOT.RooRealVar("ALPHA2","ALPHA2",2.55)
A2 = ROOT.RooRealVar("A2","A2",1.56)
CB2_sig = ROOT.CB2Pdf("CB2_sig","CB2_sig",DimuMass_unb,N_CB2,MEAN,SIGMA,ALPHA1,A1,ALPHA2,A2)
sig = ROOT.RooRealVar("signal","signal yield",1.70000e+03,500,3000)

N_VWG = ROOT.RooRealVar("N_VWG","N_VWG",1);
M = ROOT.RooRealVar("M","M",1,-50,50);
A = ROOT.RooRealVar("A","A",0.5,-10,10);
B = ROOT.RooRealVar("B","B",0.2,-10,10);
VWG_bck = ROOT.VWGPdf("VWG_bck","VWG_bck",DimuMass_unb,N_VWG,M,A,B);
bck = ROOT.RooRealVar("background","background yield",3.45165e+04,10000,500000)

model = ROOT.RooAddPdf("model","CB2 + VWG",ROOT.RooArgList(CB2_sig,VWG_bck),ROOT.RooArgList(sig,bck))
model.fitTo(dataset)

DimuMass_unb_frame = DimuMass_unb.frame(ROOT.RooFit.Title("Dimuon mass distribution"))
dataset.plotOn(DimuMass_unb_frame)
model.plotOn(DimuMass_unb_frame,ROOT.RooFit.Components("VWG_bck"),ROOT.RooFit.LineStyle(ROOT.kDashed),ROOT.RooFit.LineColor(ROOT.kGray))
model.plotOn(DimuMass_unb_frame,ROOT.RooFit.Components("CB2_sig"),ROOT.RooFit.LineStyle(ROOT.kDashed),ROOT.RooFit.LineColor(ROOT.kRed+1))
model.plotOn(DimuMass_unb_frame)
DimuMass_unb_frame.Draw()

raw_input()
