/*****************************************************************************
 * Project: RooFit                                                           *
 *                                                                           *
  * This code was autogenerated by RooClassFactory                            *
 *****************************************************************************/

#ifndef VWGPDF
#define VWGPDF

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooCategoryProxy.h"
#include "RooAbsReal.h"
#include "RooAbsCategory.h"

class VWGPdf : public RooAbsPdf {
public:
  VWGPdf() {} ;
  VWGPdf(const char *name, const char *title,
	      RooAbsReal& _x,
        RooAbsReal& _N,
        RooAbsReal& _mean,
	      RooAbsReal& _alpha,
	      RooAbsReal& _beta);
  VWGPdf(const VWGPdf& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new VWGPdf(*this,newname); }
  inline virtual ~VWGPdf() { }

  //Int_t getAnalyticalIntegral(RooArgSet& allVars, RooArgSet& analVars, const char* rangeName=0) const ;
  //Double_t analyticalIntegral(Int_t code, const char* rangeName=0) const ;

protected:

  RooRealProxy x ;
  RooRealProxy N ;
  RooRealProxy mean ;
  RooRealProxy alpha ;
  RooRealProxy beta ;

  Double_t evaluate() const ;

private:

  ClassDef(VWGPdf,1) // Your description goes here...
};

#endif