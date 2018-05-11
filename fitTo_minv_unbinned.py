import ROOT
import math

ROOT.gROOT.ProcessLineSync(".x CB2Pdf.cxx+")

input_file = ROOT.TFile("../subsample_matrix_tree_file.root","READ") # on LOCAL
#input_file = ROOT.TFile("/afs/cern.ch/work/l/lmichele/private/subsample_matrix_tree_file.root","READ") # on LXPLUS

DimuMass_unb = ROOT.RooRealVar("DimuMass_unb_subsample","#it{m}_{#mu#mu}",2,5)
dataset = ROOT.RooDataSet("dataset","dataset",ROOT.RooArgSet(DimuMass_unb),ROOT.RooFit.Import(input_file.Get("subsample_tree_0_0")))

# signal = CB2
mean_CB2 = ROOT.RooRealVar("#it{m}_{J/#psi}","mean CB2",3.096,2.9,3.3)
width_CB2 = ROOT.RooRealVar("#it{#sigma}_{J/#psi}","width CB2",0.07,0.05,0.09)
alpha1_CB2 = ROOT.RooRealVar("#alpha1_{CB2}","alpha1 CB2",1.06)
n1_CB2 = ROOT.RooRealVar("n1_{CB2}","n1 CB2",3.23)
alpha2_CB2 = ROOT.RooRealVar("#alpha2_{CB2}","alpha2 CB2",2.55)
n2_CB2 = ROOT.RooRealVar("n2_{CB2}","n2 CB2",1.56)
sig_CB2 = ROOT.CB2Pdf("sig_CB2","sig_CB2",DimuMass_unb,mean_CB2,width_CB2,alpha1_CB2,n1_CB2,alpha2_CB2,n2_CB2)
sig = ROOT.RooRealVar("signal","signal yield",1.70000e+03,500,3000)

# background = VWG
mean_VWG = ROOT.RooRealVar("mean_{VWG}","mean VWG",2,-10,10);
alpha_VWG = ROOT.RooRealVar("#alpha_{VWG}","alpha VWG",0.5,-10,10);
beta_VWG = ROOT.RooRealVar("#beta_{VWG}","beta VWG",0.2,-10,10);
bck_VWG = ROOT.RooGenericPdf("bck_VWG","bck_VWG","exp(-(@0 - @1)*(@0 - @1)/(2*(@2 + @3*((@0 - @1)/@1))*(@2 + @3*((@0 - @1)/@1))))",ROOT.RooArgList(DimuMass_unb,mean_VWG,alpha_VWG,beta_VWG))
bck = ROOT.RooRealVar("background","background yield",3.45165e+04,10000,500000)

# total PDF
model = ROOT.RooAddPdf("model","CB2 + VWG",ROOT.RooArgList(sig_CB2,bck_VWG),ROOT.RooArgList(sig,bck))
model.fitTo(dataset)

DimuMass_unb_frame = DimuMass_unb.frame(ROOT.RooFit.Title("Dimuon mass distribution"))
dataset.plotOn(DimuMass_unb_frame)
model.plotOn(DimuMass_unb_frame,ROOT.RooFit.Components("bck_VWG"),ROOT.RooFit.LineStyle(ROOT.kDashed),ROOT.RooFit.LineColor(ROOT.kGray+1))
model.plotOn(DimuMass_unb_frame,ROOT.RooFit.Components("sig_CB2"),ROOT.RooFit.LineStyle(ROOT.kDashed),ROOT.RooFit.LineColor(ROOT.kRed+1),ROOT.RooFit.DrawOption("F"),ROOT.RooFit.FillColor(ROOT.kRed+1),ROOT.RooFit.MoveToBack())
model.plotOn(DimuMass_unb_frame)
model.paramOn(DimuMass_unb_frame)
DimuMass_unb_frame.Draw()

outpute_file = ROOT.TFile("DimuMass_unb_frame.root","RECREATE")
outpute_file.cd()
DimuMass_unb_frame.Write()
outpute_file.Close()

raw_input()
