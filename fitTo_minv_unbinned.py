import ROOT
import math
from array import array

ROOT.gROOT.ProcessLineSync(".x CB2Pdf.cxx+")

#input_file = ROOT.TFile("../subsample_matrix_tree_file.root","READ") # on LOCAL
input_file = ROOT.TFile("/afs/cern.ch/work/l/lmichele/private/subsample_matrix_tree_file.root","READ") # on LXPLUS do not load "source iniroot.sh"

CostValues = [-1.00,-0.80,-0.60,-0.50,-0.40,-0.30,-0.20,-0.12,-0.04,0.04,0.12,0.20,0.30,0.40,0.50,0.60,0.80,1.00]
PhiValues = [0.000000,0.502655,1.005310,1.256637,1.445133,1.570796,1.696460,1.884956,2.136283,2.638938,3.141593]

histo_Jpsi = ROOT.TH2D("histo_Jpsi","histo_Jpsi",17,array('d',CostValues),10,array('d',PhiValues))

for i in range(1,16):
    for j in range(1,9):
        sample_name = "subsample_tree_%i_%i" % (i,j)
        DimuMass_unb = ROOT.RooRealVar("DimuMass_unb_subsample","#it{m}_{#mu#mu}",2,5)
        dataset = ROOT.RooDataSet("dataset","dataset",ROOT.RooArgSet(DimuMass_unb),ROOT.RooFit.Import(input_file.Get(sample_name)))

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
        bck = ROOT.RooRealVar("background","background yield",3.45165e+04,1000,500000)

        # total PDF
        model = ROOT.RooAddPdf("model","CB2 + VWG",ROOT.RooArgList(sig_CB2,bck_VWG),ROOT.RooArgList(sig,bck))
        #model.fitTo(dataset)
        result = model.fitTo(dataset,ROOT.RooFit.Extended(),ROOT.RooFit.Save());
        #result.Print()

        histo_Jpsi.SetBinContent(i+1,j+1,sig.getVal())

        DimuMass_unb_frame = DimuMass_unb.frame(ROOT.RooFit.Title("Dimuon mass distribution"))
        dataset.plotOn(DimuMass_unb_frame)
        model.plotOn(DimuMass_unb_frame,ROOT.RooFit.Components("bck_VWG"),ROOT.RooFit.LineStyle(ROOT.kDashed),ROOT.RooFit.LineColor(ROOT.kGray+1))
        #model.plotOn(DimuMass_unb_frame,ROOT.RooFit.Components("sig_CB2"),ROOT.RooFit.DrawOption("F"),ROOT.RooFit.FillColor(ROOT.kRed+1),ROOT.RooFit.FillStyle(3005),ROOT.RooFit.LineStyle(ROOT.kDashed),ROOT.RooFit.LineColor(ROOT.kRed+1),ROOT.RooFit.MoveToBack())
        model.plotOn(DimuMass_unb_frame,ROOT.RooFit.Components("sig_CB2"),ROOT.RooFit.DrawOption("F"),ROOT.RooFit.FillColor(ROOT.kRed+1),ROOT.RooFit.FillStyle(3344))
        model.plotOn(DimuMass_unb_frame,ROOT.RooFit.Components("sig_CB2"),ROOT.RooFit.DrawOption("L"),ROOT.RooFit.LineColor(ROOT.kRed+1))
        model.plotOn(DimuMass_unb_frame)
        model.paramOn(DimuMass_unb_frame)

        #canvas_name = "range_%i_%i.eps" % (i,j)
        #canvas = ROOT.TCanvas(sample_name,sample_name,20,20,600,600)
        #canvas.cd()
        #DimuMass_unb_frame.Draw()
        #canvas.SaveAs(canvas_name)

histo_Jpsi.Draw("COLZ")


#DimuMass_unb.setBins(100)
#histsig = sig_CB2.generate(ROOT.RooArgSet(DimuMass_unb),500)
#histsigbck = model.generate(ROOT.RooArgSet(DimuMass_unb),1.61964e+04)

#mass_frame = DimuMass_unb.frame(ROOT.RooFit.Title("mass_frame"))
#histsig.plotOn(mass_frame)
#histsigbck.plotOn(mass_frame)
#model.plotOn(mass_frame,ROOT.RooFit.Components("bck_VWG"),ROOT.RooFit.LineStyle(ROOT.kDashed),ROOT.RooFit.LineColor(ROOT.kGray+1))
#model.plotOn(mass_frame,ROOT.RooFit.Components("sig_CB2"),ROOT.RooFit.LineStyle(ROOT.kDashed),ROOT.RooFit.LineColor(ROOT.kRed+1),ROOT.RooFit.DrawOption("F"),ROOT.RooFit.FillColor(ROOT.kRed+1),ROOT.RooFit.MoveToBack())
#model.plotOn(mass_frame)
#mass_frame.Draw()

#outpute_file = ROOT.TFile("DimuMass_unb_frame.root","RECREATE")
#outpute_file.cd()
#DimuMass_unb_frame.Write()
#outpute_file.Close()

raw_input()
