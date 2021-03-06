//=========Total multiplicity=========//
Double_t nTotalMultiplicityMean = 100.;
Double_t nTotalMultiplicitySigma = 10.;

//=========Net charge=========//
Double_t nNetChargeMean = 0.0;
Double_t nNetChargeSigma = 3.0;

//==============Particles and spectra==============//
//Double_t gAllChargesTemperature = 0.11; //in GeV
Double_t gAllChargesTemperature = 4.5; //not temperature==>modified hagedorn
Double_t gPionPercentage = 0.8;
Double_t gPionTemperature = 0.1; //in GeV
Double_t gKaonPercentage = 0.12;
Double_t gKaonTemperature = 0.12; //in GeV
Double_t gProtonPercentage = 0.08;
Double_t gProtonTemperature = 0.2; //in GeV
//==============Particles and spectra==============//

//==============Flow values==============//
Double_t gDirectedFlow = 0.0;
Double_t gEllipticFlow = 0.2;
Double_t gTriangularFlow = 0.0;
Double_t gQuandrangularFlow = 0.0;
Double_t gPentangularFlow = 0.0;
//==============Flow values==============//

//=========Acceptance definition=========//
Double_t gEtaMin = -1.0;
Double_t gEtaMax = 1.0;
Double_t gPtMin = 0.1;
Double_t gPtMax = 20.0;
//=========Acceptance definition=========//

//=========Acceptance filter=========//
Bool_t kUseAcceptanceFilter = kFALSE;
const char *gAcceptanceFilterFile = "efficiencyALICE.root";
//=========Acceptance filter=========//

//=========Detector effects=========//
Bool_t kSimulateDetectorEffects = kTRUE;
Int_t fNumberOfInefficientSectors = 5;//inefficient secotrs in phi
Double_t fInefficiencyFactorInPhi = 0.65;//efficiency factor < 1
Int_t fNumberOfDeadSectors = 3;//number of dead sectors
Bool_t fEfficiencyDropNearEtaEdges = kTRUE;//efficiency drop in eta edges
//=========Detector effects=========//

//=========Jets=========//
Bool_t kUseJets = kFALSE;
//=========Jets=========//

//=========Dynamical Correlations=========//
Bool_t kUseDynamicalCorrelations = kFALSE;
Double_t gDynamicalCorrelationsPercentage = 0.1;
//=========Dynamical Correlations=========//

//=========bf object configuration=========//
Bool_t kRunShuffling = kFALSE;
Bool_t kRunMixing = kTRUE;
Bool_t bResonancesCut = kFALSE;
Bool_t bHBTcut = kFALSE;
Bool_t bConversionCut = kFALSE;
Bool_t bMomentumDifferenceCut = kFALSE;
TString fArgEventClass = "EventPlane";
Double_t deltaEtaMax = TMath::Abs(gEtaMax-gEtaMin);
Bool_t bVertexBinning = kFALSE;
//=========bf object configuration=========//

//=========Debug option=========//
Bool_t kUseDebug = kFALSE;
//=========Debug option=========//

// Run macro used for the toy model analysis
// Author: Panos.Christakoglou@nikhef.nl

//____________________________________________________________________
void runBalanceFunctionToyModel(Int_t nEvents = 10,
				Bool_t kUseAllCharges = kTRUE) {
  TStopwatch timer;
  timer.Start();

  // load libraries
  gSystem->Load("libCore");
  gSystem->Load("libGeom");
  gSystem->Load("libVMC");
  gSystem->Load("libPhysics");
  gSystem->Load("libTree");

  gSystem->Load("libSTEERBase");
  gSystem->Load("libESD");
  gSystem->Load("libAOD");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  gSystem->Load("libEventMixing");
  gSystem->Load("libCORRFW");
  gSystem->Load("libPWGTools");
  gSystem->Load("libPWGCFebye");

  //configure the bf objects
  gROOT->LoadMacro("$ALICE_PHYSICS/PWGCF/EBYE/macros/configBalanceFunctionPsiAnalysis.C");
  AliBalancePsi *bf  = GetBalanceFunctionObject("MC","",0,100,kRunShuffling,bResonancesCut,bHBTcut,bConversionCut,bMomentumDifferenceCut,fArgEventClass,deltaEtaMax,bVertexBinning);
  AliBalancePsi *bfm = 0x0;
  if(kRunMixing) 
    bfm = GetBalanceFunctionObject("MC","",0,100,kRunShuffling,bResonancesCut,bHBTcut,bConversionCut,bMomentumDifferenceCut,fArgEventClass,deltaEtaMax,bVertexBinning);
  
  //Configure the toy model object
  AliAnalysisTaskToyModel *toyModelAnalysis = new AliAnalysisTaskToyModel();
  if(kUseDebug) toyModelAnalysis->SetDebugFlag();
  toyModelAnalysis->SetAnalysisObject(bf);
  if(kRunMixing)
    toyModelAnalysis->SetMixingObject(bfm);
  toyModelAnalysis->SetTotalMultiplicity(nTotalMultiplicityMean,nTotalMultiplicitySigma);
  toyModelAnalysis->SetNetCharge(nNetChargeMean,nNetChargeSigma);
  toyModelAnalysis->SetKinematicsCutsMC(gPtMin,gPtMax,gEtaMin,gEtaMax);

  if(kUseAllCharges) {
    toyModelAnalysis->SetSpectraTemperatureForAllCharges(gAllChargesTemperature);
    toyModelAnalysis->SetDirectedFlowForAllCharges(gDirectedFlow);
    toyModelAnalysis->SetEllipticFlowForAllCharges(gEllipticFlow);
    toyModelAnalysis->SetTriangularFlowForAllCharges(gTriangularFlow);
    toyModelAnalysis->SetQuandrangularFlowForAllCharges(gQuandrangularFlow);
    toyModelAnalysis->SetPentangularFlowForAllCharges(gPentangularFlow);
  }
  else {
    //Pions
    toyModelAnalysis->SetPionPercentage(gPionPercentage);
    toyModelAnalysis->SetSpectraTemperatureForPions(gPionTemperature);
    toyModelAnalysis->SetDirectedFlowForPions(gDirectedFlow);
    toyModelAnalysis->SetEllipticFlowForPions(gEllipticFlow);
    toyModelAnalysis->SetTriangularFlowForPions(gTriangularFlow);
    toyModelAnalysis->SetQuandrangularFlowForPions(gQuandrangularFlow);
    toyModelAnalysis->SetPentangularFlowForPions(gPentangularFlow);

    //Kaons
    toyModelAnalysis->SetKaonPercentage(gKaonPercentage);
    toyModelAnalysis->SetSpectraTemperatureForKaons(gKaonTemperature);
    toyModelAnalysis->SetDirectedFlowForKaons(gDirectedFlow);
    toyModelAnalysis->SetEllipticFlowForKaons(gEllipticFlow);
    toyModelAnalysis->SetTriangularFlowForKaons(gTriangularFlow);
    toyModelAnalysis->SetQuandrangularFlowForKaons(gQuandrangularFlow);
    toyModelAnalysis->SetPentangularFlowForKaons(gPentangularFlow);

    //Protons
    toyModelAnalysis->SetProtonPercentage(gProtonPercentage);
    toyModelAnalysis->SetSpectraTemperatureForProtons(gProtonTemperature);
    toyModelAnalysis->SetDirectedFlowForProtons(gDirectedFlow);
    toyModelAnalysis->SetEllipticFlowForProtons(gEllipticFlow);
    toyModelAnalysis->SetTriangularFlowForProtons(gTriangularFlow);
    toyModelAnalysis->SetQuandrangularFlowForProtons(gQuandrangularFlow);
    toyModelAnalysis->SetPentangularFlowForProtons(gPentangularFlow);
  }

  //Jets
  if(kUseJets)
    toyModelAnalysis->SetUseJets();

  //Dynamical correlations
  if(kUseDynamicalCorrelations) 
    toyModelAnalysis->SetCorrelationPercentage(gDynamicalCorrelationsPercentage);

  //Acceptance filter
  if(kUseAcceptanceFilter) {
    TFile *gParamFile = TFile::Open(gAcceptanceFilterFile);
    if((!gParamFile) || (!gParamFile->IsOpen())) {
      Printf("File %s not found!!!",acceptanceFilename);
      return;
    }

    TString gParamName;
    for(Int_t iCentrality = 0; iCentrality < numberOfCentralityBins; iCentrality++) {
      gParamName = "gParamCentrality0";//centrality 0-5%
      TF1 *gParameterization = dynamic_cast<TF1 *>(gParamFile->Get(gParamName.Data()));
    }
    toyModelAnalysis->SetAcceptanceParameterization(gParameterization);
  }

  //Detector effects
  if(kSimulateDetectorEffects) 
    toyModelAnalysis->SimulateDetectorEffects();
  if(fNumberOfInefficientSectors) {
    toyModelAnalysis->SetNumberOfInefficientSectorsInPhi(fNumberOfInefficientSectors);
    toyModelAnalysis->SetInefficiencyFactor(fInefficiencyFactorInPhi);
  }
  if(fNumberOfDeadSectors)
    toyModelAnalysis->SetNumberOfDeadSectorsInPhi(fNumberOfDeadSectors);
  if(fEfficiencyDropNearEtaEdges)
    toyModelAnalysis->EnableEfficiencyDropNearEtaEdges();

  //Initialize and run
  toyModelAnalysis->Init();
  toyModelAnalysis->CreateOutputObjects();
  toyModelAnalysis->Run(nEvents);
  toyModelAnalysis->FinishOutput();

  // Print real and CPU time used for analysis:  
  timer.Stop();
  timer.Print();
}

