/*
***********************************************************
  Implementation of the AliHistogramManager class
  Contact: iarsene@cern.ch
  2015/04/07
  *********************************************************
*/

#include "AliHistogramManager.h"

#include <iostream>
#include <fstream>
using namespace std;

#include <TObject.h>
#include <TString.h>
#include <TObjArray.h>
#include <TFile.h>
#include <TDirectory.h>
#include <THashList.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>
#include <TProfile.h>
#include <TProfile2D.h>
#include <TProfile3D.h>
#include <THn.h>
#include <TIterator.h>
#include <TKey.h>
#include <TAxis.h>
#include <TArrayD.h>
#include <TClass.h>

#include "AliReducedVarManager.h"

ClassImp(AliHistogramManager)


//_______________________________________________________________________________
AliHistogramManager::AliHistogramManager() :
  fMainList(),
  fName("histos"),
  fMainDirectory(0x0),
  fHistFile(0x0),
  fOutputList(),
  fUseDefaultVariableNames(kFALSE),
  fUsedVars(),
  fBinsAllocated(0),
  fVariableNames(),
  fVariableUnits(),
  fNVars(0)
{
  //
  // Constructor
  //
   fMainList.SetOwner(kTRUE);
   fMainList.SetName("HistogramList");
   fOutputList.SetName(fName);
}

//_______________________________________________________________________________
AliHistogramManager::AliHistogramManager(const Char_t* name, Int_t nvars) :
  fMainList(),
  fName(name),
  fMainDirectory(0x0),
  fHistFile(0x0),
  fOutputList(),
  fUseDefaultVariableNames(kFALSE),
  fUsedVars(),
  fBinsAllocated(0),
  fVariableNames(),
  fVariableUnits(),
  fNVars(nvars)
{
  //
  // Constructor
  //
//  fUsedVars = new Bool_t[nvars];
  fMainList.SetOwner(kTRUE);
  fMainList.SetName("HistogramList");
  //fOutputList = new THashList();
  fOutputList.SetName(fName);
  //fVariableNames = new TString[nvars];
  //fVariableUnits = new TString[nvars];
}

//_______________________________________________________________________________
AliHistogramManager::~AliHistogramManager()
{
  //
  // De-constructor
  //
  //if(fUsedVars) delete fUsedVars;
  //if(fMainList) {delete fMainList; fMainList=0x0;}
  if(fMainDirectory) {delete fMainDirectory; fMainDirectory=0x0;}
  if(fHistFile) {delete fHistFile; fHistFile=0x0;}
  //if(fOutputList) {delete fOutputList; fOutputList=0x0;}
}

//_______________________________________________________________________________
void AliHistogramManager::SetDefaultVarNames(TString* vars, TString* units) 
{
   //
   // Set default variable names
   //
   for(Int_t i=0;i<AliReducedVarManager::kNVars;++i) {
     fVariableNames[i] = vars[i]; 
     fVariableUnits[i] = units[i];
   }
};


//__________________________________________________________________
void AliHistogramManager::AddHistClass(const Char_t* histClass) {
  //
  // Add a new histogram list
  //
  /*if(!fMainList) {
    fMainList = new TObjArray();
    fMainList->SetOwner();
    fMainList->SetName(fName.Data());
  }*/
  
  if(fMainList.FindObject(histClass)) {
    cout << "Warning in AliHistogramManager::AddHistClass: Cannot add histogram class " << histClass
         << " because it already exists." << endl;
    return;
  }
  THashList* hList=new THashList;
  hList->SetOwner(kTRUE);
  hList->SetName(histClass);
  fMainList.Add(hList);
}

//_________________________________________________________________
void AliHistogramManager::AddHistogram(const Char_t* histClass,
		                       const Char_t* name, const Char_t* title, Bool_t isProfile,
                                       Int_t nXbins, Double_t xmin, Double_t xmax, Int_t varX,
		                       Int_t nYbins, Double_t ymin, Double_t ymax, Int_t varY,
		                       Int_t nZbins, Double_t zmin, Double_t zmax, Int_t varZ,
                                       const Char_t* xLabels, const Char_t* yLabels, const Char_t* zLabels,
                                       Int_t varT, Int_t varW) {
  //
  // add a histogram
  //
  THashList* hList = (THashList*)fMainList.FindObject(histClass);
  if(!hList) {
    cout << "Warning in AliHistogramManager::AddHistogram(): Histogram list " << histClass << " not found!" << endl;
    cout << "         Histogram not created" << endl;
    return;
  }
  if(hList->FindObject(name)) {
    cout << "Warning in AliHistogramManager::AddHistogram(): Histogram " << name << " already exists" << endl;
    return;
  }
  TString hname = name;
  
  Int_t dimension = 1;
  if(varY>AliReducedVarManager::kNothing) dimension = 2;
  if(varZ>AliReducedVarManager::kNothing) dimension = 3;
  
  TString titleStr(title);
  TObjArray* arr=titleStr.Tokenize(";");
  if(varT>AliReducedVarManager::kNothing) fUsedVars[varT] = kTRUE;
  if(varW>AliReducedVarManager::kNothing) fUsedVars[varW] = kTRUE;
  
  TH1* h=0x0;
  switch(dimension) {
    case 1:
      h=new TH1F(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xmin,xmax);
      fBinsAllocated+=nXbins+2;
      h->Sumw2();
      h->SetUniqueID(0);
      if(varW>=0) h->SetUniqueID(100*(varW+1)+0); 
      h->GetXaxis()->SetUniqueID(UInt_t(varX));
      if(fVariableNames[varX][0]) 
	h->GetXaxis()->SetTitle(Form("%s %s", fVariableNames[varX].Data(), 
				     (fVariableUnits[varX][0] ? Form("(%s)", fVariableUnits[varX].Data()) : "")));
      if(arr->At(1)) h->GetXaxis()->SetTitle(arr->At(1)->GetName());
      if(xLabels[0]!='\0') MakeAxisLabels(h->GetXaxis(), xLabels);
      fUsedVars[varX] = kTRUE;
      hList->Add(h);
      h->SetDirectory(0);
      break;
    case 2:
      if(isProfile) {
	h=new TProfile(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xmin,xmax);
        fBinsAllocated+=nXbins+2;
	h->Sumw2();
        h->SetUniqueID(1);
        if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(100*(varW+1)+1);
      }
      else {
	h=new TH2F(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xmin,xmax,nYbins,ymin,ymax);
        fBinsAllocated+=(nXbins+2)*(nYbins+2);
        h->Sumw2();
        h->SetUniqueID(0);
        if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(100*(varW+1)+0); 
      }
      h->GetXaxis()->SetUniqueID(UInt_t(varX));
      h->GetYaxis()->SetUniqueID(UInt_t(varY));
      if(fVariableNames[varX][0]) 
	h->GetXaxis()->SetTitle(Form("%s %s", fVariableNames[varX].Data(), 
				     (fVariableUnits[varX][0] ? Form("(%s)", fVariableUnits[varX].Data()) : "")));
      if(arr->At(1)) h->GetXaxis()->SetTitle(arr->At(1)->GetName());
      if(xLabels[0]!='\0') MakeAxisLabels(h->GetXaxis(), xLabels);
      if(fVariableNames[varY][0]) 
	h->GetYaxis()->SetTitle(Form("%s %s", fVariableNames[varY].Data(), 
				     (fVariableUnits[varY][0] ? Form("(%s)", fVariableUnits[varY].Data()) : "")));
      if(fVariableNames[varY][0] && isProfile) 
	h->GetYaxis()->SetTitle(Form("<%s> %s", fVariableNames[varY].Data(), 
				     (fVariableUnits[varY][0] ? Form("(%s)", fVariableUnits[varY].Data()) : "")));	
      if(arr->At(2)) h->GetYaxis()->SetTitle(arr->At(2)->GetName());
      if(yLabels[0]!='\0') MakeAxisLabels(h->GetYaxis(), yLabels);
      fUsedVars[varX] = kTRUE;
      fUsedVars[varY] = kTRUE;
      hList->Add(h);
      h->SetDirectory(0);
      break;
    case 3:
      if(isProfile) {
        if(varT>AliReducedVarManager::kNothing) {
          h=new TProfile3D(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xmin,xmax,nYbins,ymin,ymax,nZbins,zmin,zmax);
          fBinsAllocated+=(nXbins+2)*(nYbins+2)*(nZbins+2);
	  h->Sumw2();
          if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(((varW+1)+(fNVars+1)*(varT+1))*100+1);   // 4th variable "varT" is encoded in the UniqueId of the histogram
          else h->SetUniqueID((fNVars+1)*(varT+1)*100+1);
        }
        else {
	  h=new TProfile2D(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xmin,xmax,nYbins,ymin,ymax);
          fBinsAllocated+=(nXbins+2)*(nYbins+2);
	  h->Sumw2();
          h->SetUniqueID(1);
          if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(100*(varW+1)+1); 
        }
      }
      else {
	h=new TH3F(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xmin,xmax,nYbins,ymin,ymax,nZbins,zmin,zmax);
        fBinsAllocated+=(nXbins+2)*(nYbins+2)*(nZbins+2);
        h->Sumw2();
        h->SetUniqueID(0);
        if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(100*(varW+1)+0); 
      }
      h->GetXaxis()->SetUniqueID(UInt_t(varX));
      h->GetYaxis()->SetUniqueID(UInt_t(varY));
      h->GetZaxis()->SetUniqueID(UInt_t(varZ));
      if(fVariableNames[varX][0]) 
	h->GetXaxis()->SetTitle(Form("%s %s", fVariableNames[varX].Data(), 
				     (fVariableUnits[varX][0] ? Form("(%s)", fVariableUnits[varX].Data()) : "")));
      if(arr->At(1)) h->GetXaxis()->SetTitle(arr->At(1)->GetName());
      if(xLabels[0]!='\0') MakeAxisLabels(h->GetXaxis(), xLabels);
      if(fVariableNames[varY][0]) 
	h->GetYaxis()->SetTitle(Form("%s %s", fVariableNames[varY].Data(), 
                                     (fVariableUnits[varY][0] ? Form("(%s)", fVariableUnits[varY].Data()) : "")));
      if(arr->At(2)) h->GetYaxis()->SetTitle(arr->At(2)->GetName());
      if(yLabels[0]!='\0') MakeAxisLabels(h->GetYaxis(), yLabels);
      if(fVariableNames[varZ][0]) 
	h->GetZaxis()->SetTitle(Form("%s %s", fVariableNames[varZ].Data(), 
                                     (fVariableUnits[varZ][0] ? Form("(%s)", fVariableUnits[varZ].Data()) : "")));
      if(fVariableNames[varZ][0] && isProfile && varT<0)  // for TProfile2D 
	h->GetZaxis()->SetTitle(Form("<%s> %s", fVariableNames[varZ].Data(), 
                                     (fVariableUnits[varZ][0] ? Form("(%s)", fVariableUnits[varZ].Data()) : "")));	
      if(arr->At(3)) h->GetZaxis()->SetTitle(arr->At(3)->GetName());
      if(zLabels[0]!='\0') MakeAxisLabels(h->GetZaxis(), zLabels);
      fUsedVars[varX] = kTRUE;
      fUsedVars[varY] = kTRUE;
      fUsedVars[varZ] = kTRUE;
      h->SetDirectory(0);
      hList->Add(h);
      break;
  }
}

//_________________________________________________________________
void AliHistogramManager::AddHistogram(const Char_t* histClass,
		                       const Char_t* name, const Char_t* title, Bool_t isProfile,
                                       Int_t nXbins, Double_t* xbins, Int_t varX,
		                       Int_t nYbins, Double_t* ybins, Int_t varY,
		                       Int_t nZbins, Double_t* zbins, Int_t varZ,
		                       const Char_t* xLabels, const Char_t* yLabels, const Char_t* zLabels,
                                       Int_t varT, Int_t varW) {
  //
  // add a histogram
  //
  THashList* hList = (THashList*)fMainList.FindObject(histClass);
  if(!hList) {
    cout << "Warning in AliHistogramManager::AddHistogram(): Histogram list " << histClass << " not found!" << endl;
    cout << "         Histogram not created" << endl;
    return;
  }
  if(hList->FindObject(name)) {
    cout << "Warning in AliHistogramManager::AddHistogram(): Histogram " << name << " already exists" << endl;
    return;
  }
  TString hname = name;
  
  Int_t dimension = 1;
  if(varY>AliReducedVarManager::kNothing) dimension = 2;
  if(varZ>AliReducedVarManager::kNothing) dimension = 3;
  
  if(varT>AliReducedVarManager::kNothing) fUsedVars[varT] = kTRUE;
  if(varW>AliReducedVarManager::kNothing) fUsedVars[varW] = kTRUE;
  
  TString titleStr(title);
  TObjArray* arr=titleStr.Tokenize(";");
  
  TH1* h=0x0;
  switch(dimension) {
    case 1:
      h=new TH1F(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xbins);
      fBinsAllocated+=nXbins+2;
      h->Sumw2();
      h->SetUniqueID(0);
      if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(100*(varW+1)+0); 
      h->GetXaxis()->SetUniqueID(UInt_t(varX));
      if(fVariableNames[varX][0]) 
	h->GetXaxis()->SetTitle(Form("%s %s", fVariableNames[varX].Data(), 
                                     (fVariableUnits[varX][0] ? Form("(%s)", fVariableUnits[varX].Data()) : "")));
      if(arr->At(1)) h->GetXaxis()->SetTitle(arr->At(1)->GetName());
      if(xLabels[0]!='\0') MakeAxisLabels(h->GetXaxis(), xLabels);
      fUsedVars[varX] = kTRUE;
      h->SetDirectory(0);
      hList->Add(h);
      break;
    case 2:
      if(isProfile) {
	h=new TProfile(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xbins);
        fBinsAllocated+=nXbins+2;
	h->Sumw2();
        h->SetUniqueID(1);
        if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(100*(varW+1)+1); 
      }
      else {
	h=new TH2F(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xbins,nYbins,ybins);
        fBinsAllocated+=(nXbins+2)*(nYbins+2);
        h->Sumw2();
        h->SetUniqueID(0);
        if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(100*(varW+1)+0);
      }
      h->GetXaxis()->SetUniqueID(UInt_t(varX));
      h->GetYaxis()->SetUniqueID(UInt_t(varY));
      if(fVariableNames[varX][0]) 
	h->GetXaxis()->SetTitle(Form("%s (%s)", fVariableNames[varX].Data(), 
                                     (fVariableUnits[varX][0] ? Form("(%s)", fVariableUnits[varX].Data()) : "")));
      if(arr->At(1)) h->GetXaxis()->SetTitle(arr->At(1)->GetName());
      if(xLabels[0]!='\0') MakeAxisLabels(h->GetXaxis(), xLabels);
      if(fVariableNames[varY][0]) 
         h->GetYaxis()->SetTitle(Form("%s (%s)", fVariableNames[varY].Data(), 
                                      (fVariableUnits[varY][0] ? Form("(%s)", fVariableUnits[varY].Data()) : "")));
      if(fVariableNames[varY][0] && isProfile) 
         h->GetYaxis()->SetTitle(Form("<%s> (%s)", fVariableNames[varY].Data(), 
                                      (fVariableUnits[varY][0] ? Form("(%s)", fVariableUnits[varY].Data()) : "")));

      if(arr->At(2)) h->GetYaxis()->SetTitle(arr->At(2)->GetName());
      if(yLabels[0]!='\0') MakeAxisLabels(h->GetYaxis(), yLabels);
      fUsedVars[varX] = kTRUE;
      fUsedVars[varY] = kTRUE;
      h->SetDirectory(0);
      hList->Add(h);
      break;
    case 3:
      if(isProfile) {
         if(varT>AliReducedVarManager::kNothing) {
          h=new TProfile3D(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xbins,nYbins,ybins,nZbins,zbins);
          fBinsAllocated+=(nXbins+2)*(nYbins+2)*(nZbins+2);
	  h->Sumw2();
          if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(((varW+1)+(fNVars+1)*(varT+1))*100+1);   // 4th variable "varT" is encoded in the UniqueId of the histogram
          else h->SetUniqueID((fNVars+1)*(varT+1)*100+1);
        }
        else {
	  h=new TProfile2D(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xbins,nYbins,ybins);
          fBinsAllocated+=(nXbins+2)*(nYbins+2);
	  h->Sumw2();
          h->SetUniqueID(1);
          if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(100*(varW+1)+1);
        }
      }
      else {
	h=new TH3F(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nXbins,xbins,nYbins,ybins,nZbins,zbins);
        fBinsAllocated+=(nXbins+2)*(nYbins+2)*(nZbins+2);
        h->Sumw2();
        h->SetUniqueID(0);
        if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(100*(varW+1)+0);
      }
      h->GetXaxis()->SetUniqueID(UInt_t(varX));
      h->GetYaxis()->SetUniqueID(UInt_t(varY));
      h->GetZaxis()->SetUniqueID(UInt_t(varZ));
      if(fVariableNames[varX][0]) 
	h->GetXaxis()->SetTitle(Form("%s %s", fVariableNames[varX].Data(), 
                                     (fVariableUnits[varX][0] ? Form("(%s)", fVariableUnits[varX].Data()) : "")));
      if(arr->At(1)) h->GetXaxis()->SetTitle(arr->At(1)->GetName());
      if(xLabels[0]!='\0') MakeAxisLabels(h->GetXaxis(), xLabels);
      if(fVariableNames[varY][0]) 
	h->GetYaxis()->SetTitle(Form("%s %s", fVariableNames[varY].Data(), 
                                     (fVariableUnits[varY][0] ? Form("(%s)", fVariableUnits[varY].Data()) : "")));
      if(arr->At(2)) h->GetYaxis()->SetTitle(arr->At(2)->GetName());
      if(yLabels[0]!='\0') MakeAxisLabels(h->GetYaxis(), yLabels);
      if(fVariableNames[varZ][0]) 
	h->GetZaxis()->SetTitle(Form("%s %s", fVariableNames[varZ].Data(), 
                                     (fVariableUnits[varZ][0] ? Form("(%s)", fVariableUnits[varZ].Data()) : "")));
      if(fVariableNames[varZ][0] && isProfile && varT<0)  // TProfile2D 
	h->GetZaxis()->SetTitle(Form("<%s> %s", fVariableNames[varZ].Data(), 
                                     (fVariableUnits[varZ][0] ? Form("(%s)", fVariableUnits[varZ].Data()) : "")));
				     
      if(arr->At(3)) h->GetZaxis()->SetTitle(arr->At(3)->GetName());
      if(zLabels[0]!='\0') MakeAxisLabels(h->GetZaxis(), zLabels);
      fUsedVars[varX] = kTRUE;
      fUsedVars[varY] = kTRUE;
      fUsedVars[varZ] = kTRUE;
      hList->Add(h);
      break;
  }
}


//_________________________________________________________________
void AliHistogramManager::AddHistogram(const Char_t* histClass,
                                       const Char_t* name, const Char_t* title,
                                       Int_t nDimensions, Int_t* vars,
                                       Int_t* nBins, Double_t* xmin, Double_t* xmax,
                                       TString* axLabels,
                                       Int_t varW) {
  //
  // add a multi-dimensional histogram THnF
  //
  THashList* hList = (THashList*)fMainList.FindObject(histClass);
  if(!hList) {
    cout << "Warning in AliHistogramManager::AddHistogram(): Histogram list " << histClass << " not found!" << endl;
    cout << "         Histogram not created" << endl;
    return;
  }
  if(hList->FindObject(name)) {
    cout << "Warning in AliHistogramManager::AddHistogram(): Histogram " << name << " already exists" << endl;
    return;
  }
  TString hname = name;
  
  TString titleStr(title);
  TObjArray* arr=titleStr.Tokenize(";");
  
  if(varW>AliReducedVarManager::kNothing) fUsedVars[varW] = kTRUE;
  
  THnF* h=new THnF(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nDimensions,nBins,xmin,xmax);
  h->Sumw2();
  if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(10+nDimensions+100*(varW+1));
  else h->SetUniqueID(10+nDimensions);
  ULong_t bins = 1;
  for(Int_t idim=0;idim<nDimensions;++idim) {
    bins*=(nBins[idim]+2);
    TAxis* axis = h->GetAxis(idim);
    axis->SetUniqueID(vars[idim]);
    if(fVariableNames[vars[idim]][0]) 
      axis->SetTitle(Form("%s %s", fVariableNames[vars[idim]].Data(), 
                          (fVariableUnits[vars[idim]][0] ? Form("(%s)", fVariableUnits[vars[idim]].Data()) : "")));
    if(arr->At(1+idim)) axis->SetTitle(arr->At(1+idim)->GetName());
    if(axLabels && !axLabels[idim].IsNull()) 
      MakeAxisLabels(axis, axLabels[idim].Data());
    fUsedVars[vars[idim]] = kTRUE;
  }
  hList->Add(h);
  fBinsAllocated+=bins;
}


//_________________________________________________________________
void AliHistogramManager::AddHistogram(const Char_t* histClass,
                                       const Char_t* name, const Char_t* title,
                                       Int_t nDimensions, Int_t* vars,
                                       TArrayD* binLimits,
                                       TString* axLabels,
                                       Int_t varW) {
  //
  // add a multi-dimensional histogram THnF with equal or variable bin widths
  //
  THashList* hList = (THashList*)fMainList.FindObject(histClass);
  if(!hList) {
    cout << "Warning in AliHistogramManager::AddHistogram(): Histogram list " << histClass << " not found!" << endl;
    cout << "         Histogram not created" << endl;
    return;
  }
  if(hList->FindObject(name)) {
    cout << "Warning in AliHistogramManager::AddHistogram(): Histogram " << name << " already exists" << endl;
    return;
  }
  TString hname = name;
  
  TString titleStr(title);
  TObjArray* arr=titleStr.Tokenize(";");
  
  if(varW>AliReducedVarManager::kNothing) fUsedVars[varW] = kTRUE;
  
  Double_t* xmin = new Double_t[nDimensions];
  Double_t* xmax = new Double_t[nDimensions];
  Int_t* nBins = new Int_t[nDimensions];
  for(Int_t idim=0;idim<nDimensions;++idim) {
    nBins[idim] = binLimits[idim].GetSize()-1;
    xmin[idim] = binLimits[idim][0];
    xmax[idim] = binLimits[idim][nBins[idim]];
  }
  
  THnF* h=new THnF(hname.Data(),(arr->At(0) ? arr->At(0)->GetName() : ""),nDimensions,nBins,xmin,xmax);
  for(Int_t idim=0;idim<nDimensions;++idim) {
    TAxis* axis=h->GetAxis(idim);
    axis->Set(nBins[idim], binLimits[idim].GetArray());
  }
  
  h->Sumw2();
  if(varW>AliReducedVarManager::kNothing) h->SetUniqueID(10+nDimensions+100*(varW+1));
  else h->SetUniqueID(10+nDimensions);
  ULong_t bins = 1;
  for(Int_t idim=0;idim<nDimensions;++idim) {
    bins*=(nBins[idim]+2);
    TAxis* axis = h->GetAxis(idim);
    axis->SetUniqueID(vars[idim]);
    if(fVariableNames[vars[idim]][0]) 
      axis->SetTitle(Form("%s %s", fVariableNames[vars[idim]].Data(), 
                          (fVariableUnits[vars[idim]][0] ? Form("(%s)", fVariableUnits[vars[idim]].Data()) : "")));
    if(arr->At(1+idim)) axis->SetTitle(arr->At(1+idim)->GetName());
    if(axLabels && !axLabels[idim].IsNull()) 
      MakeAxisLabels(axis, axLabels[idim].Data());
    fUsedVars[vars[idim]] = kTRUE;
  }
  hList->Add(h);
  fBinsAllocated+=bins;
}



//_________________________________________________________________
THnF* AliHistogramManager::CreateHistogram( const Char_t* name, const Char_t* title,
                                   Int_t nDimensions,
                                   TArrayD* binLimits){
  //
  // create a multi-dimensional histogram THnF with equal or variable bin widths
  //
  TString hname = name;

  TString titleStr(title);
  TObjArray* arr=titleStr.Tokenize(";");

  Double_t* xmin = new Double_t[nDimensions];
  Double_t* xmax = new Double_t[nDimensions];
  Int_t* nBins = new Int_t[nDimensions];
  for(Int_t idim=0;idim<nDimensions;++idim) {
    nBins[idim] = binLimits[idim].GetSize()-1;
    xmin[idim] = binLimits[idim][0];
    xmax[idim] = binLimits[idim][nBins[idim]];
  }

  THnF* h=new THnF(hname.Data(),arr->At(0)->GetName(),nDimensions,nBins,xmin,xmax);
  for(Int_t idim=0;idim<nDimensions;++idim) {
    TAxis* axis=h->GetAxis(idim);
    axis->Set(nBins[idim], binLimits[idim].GetArray());
  }

  h->Sumw2();

  delete [] xmin;
  delete [] xmax;
  delete [] nBins;
  //delete [] binLimits;

  return h;
}



//_________________________________________________________________
THnF* AliHistogramManager::CreateHistogram( const Char_t* name, const Char_t* title,
                                   Int_t nDimensions,
                                   TAxis* axes){
  //
  // create a multi-dimensional histogram THnF with equal or variable bin widths
  //
  TString hname = name;

  TString titleStr(title);
  TObjArray* arr=titleStr.Tokenize(";");

  Double_t* xmin = new Double_t[nDimensions];
  Double_t* xmax = new Double_t[nDimensions];
  Int_t* nBins = new Int_t[nDimensions];
  for(Int_t idim=0;idim<nDimensions;++idim) {
    nBins[idim] = axes[idim].GetNbins();
    xmin[idim]  = axes[idim].GetBinLowEdge(1);
    xmax[idim]  = axes[idim].GetBinUpEdge(nBins[idim]);
  }

  THnF* h=new THnF(hname.Data(),arr->At(0)->GetName(),nDimensions,nBins,xmin,xmax);
  for(Int_t idim=0;idim<nDimensions;++idim) {
    TAxis* axis=h->GetAxis(idim);
    *axis=TAxis(axes[idim]);
    //axis->SetTitle(arr->At(idim+1)->GetName());
  }

  h->Sumw2();

  delete [] xmin;
  delete [] xmax;
  delete [] nBins;

  return h;
}



//__________________________________________________________________
void AliHistogramManager::FillHistClass(const Char_t* className, Float_t* values) {
  //
  //  fill a class of histograms
  //
  THashList* hList = (THashList*)fMainList.FindObject(className);
  if(!hList) {
    /*cout << "Warning in AliHistogramManager::FillHistClass(): Histogram list " << className << " not found!" << endl;
    cout << "         Histogram list not filled" << endl; */
    return;
  }
  if(!hList) {
    return;
  }
    
  TIter next(hList);
  TObject* h=0x0;
  Bool_t isProfile;
  Bool_t isTHn;
  Int_t thnDim=0;
  Double_t fillValues[20]={0.0};
  Int_t uid = 0;
  Int_t varX=-1, varY=-1, varZ=-1, varT=-1, varW=-1;
  Int_t dimension=0;
  while((h=next())) {
    Bool_t allVarsGood = kTRUE;
    uid = h->GetUniqueID();
    isProfile = (uid%10==1 ? kTRUE : kFALSE);   // units digit encodes the isProfile
    isTHn = ((uid%100)>10 ? kTRUE : kFALSE);      
    if(isTHn) thnDim = (uid%100)-10;        // the excess over 10 from the last 2 digits give the dimension of the THn
    dimension = 0;
    if(!isTHn) dimension = ((TH1*)h)->GetDimension();
        
    uid = (uid-(uid%100))/100;
    varX = -1;
    varY = -1;
    varZ = -1;
    varT = -1;
    varW = -1;
    if(uid>0) {
      varW = uid%(fNVars+1)-1;
      if(varW==0) varW=AliReducedVarManager::kNothing;
      uid = (uid-(uid%(fNVars+1)))/(fNVars+1);
      if(uid>0) varT = uid - 1;
      //cout << "Filling " << h->GetName() << " with varT " << varT << endl;
    }
        
    if(!isTHn) {
      varX = ((TH1*)h)->GetXaxis()->GetUniqueID();
      if(fUsedVars[varX]) {
        switch(dimension) {
          case 1:
            if(isProfile) {
              varY = ((TH1*)h)->GetYaxis()->GetUniqueID();
              if(!fUsedVars[varY]) break;
              if(varW>AliReducedVarManager::kNothing) { 
                if(!fUsedVars[varW]) break;
                ((TProfile*)h)->Fill(values[varX],values[varY],values[varW]);
              }
              else 
                ((TProfile*)h)->Fill(values[varX],values[varY]);
            }
            else {
              if(varW>AliReducedVarManager::kNothing) {
                if(!fUsedVars[varW]) break;
                ((TH1F*)h)->Fill(values[varX],values[varW]);
              }
              else
                ((TH1F*)h)->Fill(values[varX]);
            }
          break;
          case 2:
            varY = ((TH1*)h)->GetYaxis()->GetUniqueID();
            if(!fUsedVars[varY]) break;
            if(isProfile) {
              varZ = ((TH1*)h)->GetZaxis()->GetUniqueID();
              if(!fUsedVars[varZ]) break;
              if(varW>AliReducedVarManager::kNothing) {
                if(!fUsedVars[varW]) break;
                ((TProfile2D*)h)->Fill(values[varX],values[varY],values[varZ],values[varW]);
              }
              else
                ((TProfile2D*)h)->Fill(values[varX],values[varY],values[varZ]);        
            }
            else {
              if(varW>AliReducedVarManager::kNothing) {
                if(!fUsedVars[varW]) break;
                ((TH2F*)h)->Fill(values[varX],values[varY], values[varW]);
              }
              else
                ((TH2F*)h)->Fill(values[varX],values[varY]);
            }
          break;
          case 3:
            varY = ((TH1*)h)->GetYaxis()->GetUniqueID();
            if(!fUsedVars[varY]) break;
            varZ = ((TH1*)h)->GetZaxis()->GetUniqueID();
            if(!fUsedVars[varZ]) break;
            if(isProfile) {
              if(!fUsedVars[varT]) break;
              if(varW>AliReducedVarManager::kNothing) {
                if(!fUsedVars[varW]) break;
                ((TProfile3D*)h)->Fill(values[varX],values[varY],values[varZ],values[varT],values[varW]);
              }
              else
                ((TProfile3D*)h)->Fill(values[varX],values[varY],values[varZ],values[varT]);
            }
            else {
              if(varW>AliReducedVarManager::kNothing) {
                if(!fUsedVars[varW]) break;
                ((TH3F*)h)->Fill(values[varX],values[varY],values[varZ],values[varW]);
              }
              else
                ((TH3F*)h)->Fill(values[varX],values[varY],values[varZ]);
            }
          break;
          default:
          break;
        }  // end switch
      }
    }  // end if(!isTHn)
    else {
      for(Int_t idim=0;idim<thnDim;++idim) {
        allVarsGood &= fUsedVars[((THnF*)h)->GetAxis(idim)->GetUniqueID()];
        fillValues[idim] = values[((THnF*)h)->GetAxis(idim)->GetUniqueID()];
      }
      if(allVarsGood) {
        if(varW>AliReducedVarManager::kNothing) {
          if(fUsedVars[varW])
            ((THnF*)h)->Fill(fillValues,values[varW]);
        }
        else
          ((THnF*)h)->Fill(fillValues);
      }
    }
  }
}

//__________________________________________________________________
void AliHistogramManager::WriteOutput(TFile* save) {
  //
  // Write the histogram lists in the output file
  //
  cout << "Writing the output to " << save->GetName() << " ... " << flush;
  TDirectory* mainDir = save->mkdir(fMainList.GetName());
  mainDir->cd();
  for(Int_t i=0; i<fMainList.GetEntries(); ++i) {
    THashList* list = (THashList*)fMainList.At(i);
    TDirectory* dir = mainDir->mkdir(list->GetName());
    dir->cd();
    list->Write();
    mainDir->cd();
  }
  save->Close();
  cout << "done" << endl;
}


//__________________________________________________________________
THashList* AliHistogramManager::AddHistogramsToOutputList() {
  //
  // Write the histogram lists in a list
  //
  for(Int_t i=0; i<fMainList.GetEntries(); ++i) {
    //THashList* hlist = new THashList();
    THashList* list = (THashList*)fMainList.At(i);
    //hlist->SetName(list->GetName());
    //hlist->Add(list);
    //hlist->SetOwner(kTRUE);
    fOutputList.Add(list);
  }
  fOutputList.SetOwner(kTRUE);
  return &fOutputList;
}

//____________________________________________________________________________________
void AliHistogramManager::InitFile(const Char_t* filename, const Char_t* mainListName /*=""*/) {
  //
  // Open an existing ROOT file containing lists of histograms and initialize the global list pointer
  //
  TString histfilename="";
  if(fHistFile) histfilename = fHistFile->GetName();
  if(!histfilename.Contains(filename)) {
    fHistFile = new TFile(filename);    // open file only if not already open
  
    if(!fHistFile) {
      cout << "AliHistogramManager::InitFile() : File " << filename << " not opened!!" << endl;
      return;
    }
    if(fHistFile->IsZombie()) {
      cout << "AliHistogramManager::InitFile() : File " << filename << " not opened!!" << endl;
      return;
    }
    TList* list1 = fHistFile->GetListOfKeys();
    TKey* key1 = 0x0; 
    if(mainListName[0]) key1 = (TKey*)list1->FindObject(mainListName);
    else key1 = (TKey*)list1->At(0);
    fMainDirectory = (THashList*)key1->ReadObj();
  }
}

//____________________________________________________________________________________
void AliHistogramManager::CloseFile() {
  //
  // Close the opened file
  //
  delete fMainDirectory; fMainDirectory = 0x0;
  if(fHistFile && fHistFile->IsOpen()) fHistFile->Close();
}

//____________________________________________________________________________________
THashList* AliHistogramManager::GetHistogramList(const Char_t* listname) const {
  //
  // Retrieve a histogram list
  //
  //if(!fMainDirectory && !fMainList) {
   if(!fMainDirectory && fMainList.GetEntries()==0) {
    cout << "AliHistogramManager::GetHistogramList() : " << endl;
    cout << "                   A ROOT file must be opened first with InitFile() or the main " << endl;
    cout << "                     list must be initialized by creating at least one histogram list !!" << endl;
    return 0x0;
  }
  //if(fMainList) {
  if(fMainList.GetEntries()>0) {
     cout << "fMainList entries :: " << fMainList.GetEntries() << endl;
    THashList* hList = (THashList*)fMainList.FindObject(listname);
    cout << "hList" << hList << endl;
    return hList;
  }
  THashList* listHist = (THashList*)fMainDirectory->FindObject(listname);
  cout << "fMainDirectory " << fMainDirectory << endl;
  cout << "listHist " << listHist << endl;
  //TDirectoryFile* hdir = (TDirectoryFile*)listKey->ReadObj();
  //return hdir->GetListOfKeys();
  return listHist;
}

//____________________________________________________________________________________
TObject* AliHistogramManager::GetHistogram(const Char_t* listname, const Char_t* hname) const {
  //
  // Retrieve a histogram from the list hlist
  //
  //if(!fMainDirectory && !fMainList) {
   if(!fMainDirectory && fMainList.GetEntries()==0) {
    cout << "AliHistogramManager::GetHistogramList() : " << endl;
    cout << "                   A ROOT file must be opened first with InitFile() or the main " << endl;
    cout << "                     list must be initialized by creating at least one histogram list !!" << endl;
    return 0x0;
  }
  //if(fMainList) {
  /*if(fMainList.GetEntries()==0) {
    THashList* hList = (THashList*)fMainList.FindObject(listname);
    if(!hList) {
      cout << "Warning in AliHistogramManager::GetHistogram(): Histogram list " << listname << " not found!" << endl;
      return 0x0;
    }
    return hList->FindObject(hname);
  }*/
  THashList* hList = (THashList*)fMainDirectory->FindObject(listname);
  //TDirectoryFile* hlist = (TDirectoryFile*)listKey->ReadObj();
  //TKey* key = hlist->FindKey(hname);
  //return key->ReadObj();
  return hList->FindObject(hname);
}

//____________________________________________________________________________________
void AliHistogramManager::MakeAxisLabels(TAxis* ax, const Char_t* labels) {
  //
  // add bin labels to an axis
  //
  TString labelsStr(labels);
  TObjArray* arr=labelsStr.Tokenize(";");
  for(Int_t ib=1; ib<=ax->GetNbins(); ++ib) {
    if(ib>=arr->GetEntries()+1) break;
    ax->SetBinLabel(ib, arr->At(ib-1)->GetName());
  }
}

//____________________________________________________________________________________
void AliHistogramManager::Print(Option_t*) const {
  //
  // Print the defined histograms
  //
  cout << "###################################################################" << endl;
  cout << "AliHistogramManager:: " << fName.Data() << endl;
  for(Int_t i=0; i<fMainList.GetEntries(); ++i) {
    THashList* list = (THashList*)fMainList.At(i);
    cout << "************** List " << list->GetName() << endl;
    for(Int_t j=0; j<list->GetEntries(); ++j) {
      TObject* obj = list->At(j);
      cout << obj->GetName() << ": " << obj->IsA()->GetName() << endl;
    }
  }
}
