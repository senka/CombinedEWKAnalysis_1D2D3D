/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooAbsData.h"
#include "CombinedEWKAnalysis/CommonTools/interface/RooACSemiAnalyticPdf_2D.h" 

#include <math.h> 
#include <string>
#include "TMath.h" 
#include "RooFormulaVar.h"
#include "RooRealVar.h"
#include "RooFit.h"

#include "TFile.h"

//using namespace RooFit ;

RooACSemiAnalyticPdf_2D::RooACSemiAnalyticPdf_2D() : 
  type_(notype), P_par1par2_histo(0), P_par1par2_TF(0)
{
  initializeProfiles();
}

RooACSemiAnalyticPdf_2D::RooACSemiAnalyticPdf_2D(const char *name, 
						     const char *title, 
						     RooAbsReal& _x,
						     RooAbsReal& _par1,
						     RooAbsReal& _par2,
						     RooAbsReal& _SM_shape,
						     const char * parFilename,
						     const unsigned& lt) :
  RooAbsPdf(name,title),
  x("observable","observable",this,_x),
  par1("par1","par1",this,_par1),
  par2("par2","par2",this,_par2),
  SM_shape("SM_shape","SM_shape",this,_SM_shape),
  type_((LimitType)lt),
  profileFilename(parFilename),
  P_par1par2_histo(0),
  P_par1par2_TF(0)
{ 
  initializeProfiles();
  initializeNormalization(std::string(""),_x,_SM_shape);
  const char* pwd = gDirectory->GetPath();
  TFile *f = TFile::Open(parFilename,"READ");  
  gDirectory->cd(pwd);

  std::vector<double> bins_;

  const RooRealVar& rdep = dynamic_cast<const RooRealVar&>(_x);
  int N_bins=rdep.getBinning("").numBins();
  
  for (int i=0;i<N_bins;i++){
    if (i==0){
      bins_.push_back(rdep.getBinning("").binLow(i));
    }
    bins_.push_back(rdep.getBinning("").binHigh(i));
  }
  
  readProfiles(bins_,*f);
  f->Close();
} 

RooACSemiAnalyticPdf_2D::RooACSemiAnalyticPdf_2D(const RooACSemiAnalyticPdf_2D& other, 
					       const char* name) :  
  RooAbsPdf(other,name),
  x("observable",this,other.x),
  par1("par1",this,other.par1),
  par2("par2",this,other.par2),
  SM_shape("SM_shape",this,other.SM_shape),
  type_(other.type_),
  integral_basis(other.integral_basis),
  bins(other.bins),
  profileFilename(other.profileFilename),
  P_par1par2_histo(0),
  P_par1par2_TF(0)
{ 
  initializeProfiles();
  initializeBins(x.arg());
  const char* pwd = gDirectory->GetPath();
  TFile *f = TFile::Open(profileFilename,"READ");  
  gDirectory->cd(pwd);  
  readProfiles(bins,*f);
  f->Close();
} 

void RooACSemiAnalyticPdf_2D::initializeProfiles() {
  int const N_bins=100;
  
  P_par1par2_histo = new TH2D*[N_bins]();
  P_par1par2_TF = new TF2*[N_bins]();
  
}


void RooACSemiAnalyticPdf_2D::initializeBins(const RooAbsReal& dep) const {

  std::vector<double> bins;
  const RooRealVar& rdep = dynamic_cast<const RooRealVar&>(dep);
  int N_bins=rdep.getBinning("").numBins();
  for(int i=0; i<N_bins; ++i) {
    if (i==0){
      bins.push_back(rdep.getBinning("").binLow(i));
    }
    bins.push_back(rdep.getBinning("").binHigh(i));

  }
}


void RooACSemiAnalyticPdf_2D::initializeNormalization(const std::string& rName,
						     const RooAbsReal& dep,
						     const RooAbsReal& shape) const {
  integral_basis[rName] = std::vector<double>();


  const RooRealVar& rdep = dynamic_cast<const RooRealVar&>(dep);
  int N_bins=rdep.getBinning("").numBins();
  int i;
  for(i=0; i<N_bins; ++i) {


    Double_t bin_low=rdep.getBinning("").binLow(i);
    Double_t bin_high=rdep.getBinning("").binHigh(i);

    if (i==0){
      bins.push_back(rdep.getBinning("").binLow(i));
    }
    bins.push_back(rdep.getBinning("").binHigh(i));

    RooRealVar& b = const_cast<RooRealVar&>(rdep);

    const std::string intRange="integRange";

    b.setRange((const char*)intRange.c_str(),bin_low,bin_high); 
    RooAbsReal* integral = shape.createIntegral(RooArgSet(rdep),RooArgSet(),(const char*)intRange.c_str());

    integral_basis[rName].push_back(integral->getVal());

    delete integral;
  }


}

void RooACSemiAnalyticPdf_2D::readProfiles(std::vector<double> bins,TDirectory& dir) const {

  int N_bins;
  int i;
  switch(type_) {
  case par1par2_TF2par:
    N_bins=1;
    break;
  case par1par2_TH2:
  case par1par2_TF2:
    N_bins=bins.size()-1;
    break;
  default:
    assert(NULL && "invalid limit type!");
    break;
  }
  for(i=0; i<N_bins; ++i) {
    if (P_par1par2_histo[i]) delete P_par1par2_histo[i];
    if (P_par1par2_TF[i]) delete P_par1par2_TF[i];
    TString par2name = TString::Format("bin_content_par1_par2_%i",i+1);

    switch(type_) {
    case par1par2_TF2par:
    case par1par2_TH2:
      P_par1par2_histo[i] = dynamic_cast<TH2D *>(dir.Get(par2name)->Clone(par2name+"new"));
      P_par1par2_histo[i]->SetDirectory(0);
      break;
    case par1par2_TF2:
      P_par1par2_TF[i] = dynamic_cast<TF2 *>(dir.Get(par2name)->Clone(par2name+"new"));
      break;
    default:
      assert(NULL && "invalid limit type!");
      break;
    }

  }

}

void RooACSemiAnalyticPdf_2D::readProfiles(RooACSemiAnalyticPdf_2D const& other) {


  int N_bins;
  switch(type_) {
  case par1par2_TF2par:
    N_bins=1;
    break;
  case par1par2_TH2:
  case par1par2_TF2:
    N_bins=bins.size()-1;
    break;
  default:
    assert(NULL && "invalid limit type!");
    break;
  }


  for (int i = 0; i<N_bins; ++i) {
    TString par2name = TString::Format("bin_content_par1_par2_%i",i+1);

    switch(type_) {
    case par1par2_TF2par:
    case par1par2_TH2:
      P_par1par2_histo[i] = dynamic_cast<TH2D *>(other.P_par1par2_histo[i]->Clone(par2name+"new"));
      P_par1par2_histo[i]->SetDirectory(0);
      break;
    case par1par2_TF2:
      P_par1par2_TF[i] = dynamic_cast<TF2 *>(other.P_par1par2_histo[i]->Clone(par2name+"new"));
      //      P_par1par2_TF[i]->SetDirectory(0);
      break;
    default:
      assert(NULL && "invalid limit type!");
      break;
    }

  }
}

RooACSemiAnalyticPdf_2D::~RooACSemiAnalyticPdf_2D() {
  int const N_bins=100;

  for(int i = 0; i<N_bins; ++i) {
    if (P_par1par2_histo[i])
      delete P_par1par2_histo[i];
    if (P_par1par2_TF[i])
      delete P_par1par2_TF[i];
  }
  delete[] P_par1par2_histo;
  delete[] P_par1par2_TF;
}

Double_t RooACSemiAnalyticPdf_2D::evaluate() const 
{ 


  TH2D ** P_histo = NULL;
  TF2 ** P_TF = NULL;
  double v1(0.0), v2(0.0);
  
  P_histo = P_par1par2_histo;
  P_TF = P_par1par2_TF;
  v1 = par1;
  v2 = par2;
  
  if (not P_histo[0] and not P_TF[0]) {
    TFile f(profileFilename);
    readProfiles(bins,f); 
    f.Close();
  }

  switch(type_) {
  case par1par2_TF2par:
    break;
  case par1par2_TH2:
    if (v1 < P_histo[0]->GetXaxis()->GetXmin())
      v1 = P_histo[0]->GetXaxis()->GetXmin();
    if (v1 > P_histo[0]->GetXaxis()->GetXmax())
      v1 = P_histo[0]->GetXaxis()->GetXmax();
    if (v2 < P_histo[0]->GetYaxis()->GetXmin())
      v2 = P_histo[0]->GetYaxis()->GetXmin();
    if (v2 > P_histo[0]->GetYaxis()->GetXmax())
      v2 = P_histo[0]->GetYaxis()->GetXmax();
    break;
  case par1par2_TF2:   
/* 
    if (v1 < P_TF[0]->GetXmin())
      v1 = P_TF[0]->GetXmin();
    if (v1 > P_TF[0]->GetXmax())
      v1 = P_TF[0]->GetXmax();
    if (v2 < P_TF[0]->GetYmin())
      v2 = P_TF[0]->GetYmin();
    if (v2 > P_TF[0]->GetYmax())
      v2 = P_TF[0]->GetYmax();
*/
    break;
  default:
    assert(NULL && "invalid limit type!");
    break;

  }  

  double ret(0.);

  int N_bins=bins.size()-1;

    int bin_with_x=1;
    for (int j=0;j<N_bins;j++)
      if (x<bins[j+1] && x>bins[j]) bin_with_x=j+1;
    
    switch(type_) {
    case par1par2_TF2par:
      ret = (P_histo[0]->GetBinContent(1,bin_with_x)+P_histo[0]->GetBinContent(2,bin_with_x)*v1+P_histo[0]->GetBinContent(3,bin_with_x)*v2+P_histo[0]->GetBinContent(4,bin_with_x)*v1*v2+P_histo[0]->GetBinContent(5,bin_with_x)*v1*v1+P_histo[0]->GetBinContent(6,bin_with_x)*v2*v2)*SM_shape;
      break;
    case par1par2_TH2:
      ret = P_histo[bin_with_x-1]->Interpolate(v1, v2)*SM_shape;
      break;
    case par1par2_TF2:
      ret = P_TF[bin_with_x-1]->Eval(v1, v2)*SM_shape;
      break;
    case notype:
      assert(NULL && "invalid limit type!");
      break;
    }

  
  if (ret < 0.) ret = 0.;
  return ret; 
}

Int_t RooACSemiAnalyticPdf_2D::
getAnalyticalIntegral(RooArgSet& allVars,RooArgSet& analVars, 
		      const char* /*rangeName*/) const {  
  if (matchArgs(allVars,analVars,x)) return 1 ;
  return 0 ;
}

Double_t RooACSemiAnalyticPdf_2D::
analyticalIntegral(Int_t code, const char* rangeName) const {  

  assert(code==1 && "invalid analytic integration code!");

  std::string rName = ( rangeName == 0 ? std::string("") : std::string(rangeName) );
  if( integral_basis.find(rName) == integral_basis.end() ) {
    initializeNormalization(rName,x.arg(),SM_shape.arg());
  }

  TH2D ** P_histo = NULL;
  TF2 ** P_TF = NULL;
  double v1(0.0), v2(0.0);
  
  P_histo = P_par1par2_histo;
  P_TF = P_par1par2_TF;
  v1 = par1;
  v2 = par2;
  

  if (not P_histo[0] and not P_TF[0]) {
    TFile f(profileFilename);
    readProfiles(bins,f);
    f.Close();
  }

  double ret(0.);

  int N_bins=bins.size()-1;
  for(Int_t i=0 ; i<N_bins ; i++) {
    switch(type_) {
    case par1par2_TF2par:
      ret += (P_histo[0]->GetBinContent(1,i+1)+P_histo[0]->GetBinContent(2,i+1)*v1+P_histo[0]->GetBinContent(3,i+1)*v2+P_histo[0]->GetBinContent(4,i+1)*v1*v2+P_histo[0]->GetBinContent(5,i+1)*v1*v1+P_histo[0]->GetBinContent(6,i+1)*v2*v2)*integral_basis[rName][i];
      break;
    case par1par2_TH2:
      ret += P_histo[i]->Interpolate(v1, v2)*integral_basis[rName][i];
      break;
    case par1par2_TF2:
      ret += P_TF[i]->Eval(v1, v2)*integral_basis[rName][i];
      break;
    default:
      assert(code==1 && "invalid limit type!");
      break;
    }

  }
  std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@--> par1(v1): "<<v1<<" par2(v2): "<<v2 <<" ret: "<<ret<< std::endl;
  
  std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@---> integral: "<<ret<< std::endl;

  return ret;
}
