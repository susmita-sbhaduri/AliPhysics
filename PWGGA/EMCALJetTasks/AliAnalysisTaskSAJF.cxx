// $Id$
//
// Jet finder analysis task (S.Aiola).
//
//

#include <TObject.h>
#include <TChain.h>
#include <TClonesArray.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TList.h>
#include <TLorentzVector.h>
#include <TParticle.h>
#include <TRandom3.h>
#include <TParameter.h>

#include "AliAnalysisManager.h"
#include "AliCentrality.h"
#include "AliVCluster.h"
#include "AliVTrack.h"
#include "AliEmcalJet.h"
#include "AliVEventHandler.h"
#include "AliLog.h"

#include "AliAnalysisTaskSAJF.h"

ClassImp(AliAnalysisTaskSAJF)

//________________________________________________________________________
AliAnalysisTaskSAJF::AliAnalysisTaskSAJF() : 
  AliAnalysisTaskSE("AliAnalysisTaskSAJF"),
  fAnaType(kTPC),
  fMinEta(-0.9),
  fMaxEta(0.9),
  fMinPhi(-10),
  fMaxPhi(10),
  fJetRadius(0.4),
  fTracksName("Tracks"),
  fCaloName("CaloClusters"),
  fJetsName("Jets"),
  fKtJetsName("KtJets"),
  fEmbJetsName("EmbJets"),
  fRhoName("Rho"),
  fNbins(500),
  fMinPt(0),
  fMaxPt(250),
  fPtCut(0.15),
  fPtCutJetPart(10),
  fTracks(0),
  fCaloClusters(0),
  fJets(0),
  fKtJets(0),
  fEmbJets(0),
  fCent(0),
  fCentBin(-1),
  fRho(-1),
  fOutput(0),
  fHistCentrality(0),
  fHistJetPhiEta(0),
  fHistEmbJetPhiEta(0),
  fHistEmbPartPhiEta(0),
  fHistRhoPartVSleadJetPt(0),
  fHistMedKtVSRhoPart(0),
  fHistRCPtVSRhoPart(0),
  fHistMedKtVSEmbBkg(0),
  fHistMedKtVSRCPt(0),
  fHistRCPtExLJVSDPhiLJ(0),
  fHistRCPhiEta(0)

{
  // Default constructor.

  for (Int_t i = 0; i < 4; i++) {
    fHistJetsPt[i] = 0;
    fHistCorrJetsPt[i] = 0;
    fHistJetsPtCutPart[i] = 0;
    fHistCorrJetsPtCutPart[i] = 0;
    fHistJetsNEF[i] = 0;
    fHistJetsZ[i] = 0;
    fHistLeadingJetPt[i] = 0;
    fHistCorrLeadingJetPt[i] = 0;
    fHist2LeadingJetPt[i] = 0;
    fHistTracksPtLJ[i] = 0;
    fHistClusEtLJ[i] = 0;
    fHistTracksPtBkg[i] = 0;
    fHistClusEtBkg[i] = 0;
    fHistBkgClusMeanRho[i] = 0;
    fHistBkgTracksMeanRho[i] = 0;
    fHistMedianPtKtJet[i] = 0;
    fHistDeltaPtRC[i] = 0;
    fHistDeltaPtRCExLJ[i] = 0;
    fHistRCPt[i] = 0;
    fHistRCPtExLJ[i] = 0;
    fHistEmbJets[i] = 0;
    fHistEmbPart[i] = 0;
    fHistDeltaPtMedKtEmb[i] = 0;
  }

  // Output slot #1 writes into a TH1 container
  DefineOutput(1, TList::Class()); 
}

//________________________________________________________________________
AliAnalysisTaskSAJF::AliAnalysisTaskSAJF(const char *name) : 
  AliAnalysisTaskSE(name),
  fAnaType(kTPC),
  fMinEta(-0.9),
  fMaxEta(0.9),
  fMinPhi(-10),
  fMaxPhi(10),
  fJetRadius(0.4),
  fTracksName("Tracks"),
  fCaloName("CaloClusters"),
  fJetsName("Jets"),
  fKtJetsName("KtJets"),
  fEmbJetsName("EmbJets"),
  fRhoName("Rho"),
  fNbins(500),
  fMinPt(0),
  fMaxPt(250),
  fPtCut(0.15),
  fPtCutJetPart(10),
  fTracks(0),
  fCaloClusters(0),
  fJets(0),
  fKtJets(0),
  fEmbJets(0),
  fCent(0),
  fCentBin(-1),
  fRho(-1),
  fOutput(0),
  fHistCentrality(0),
  fHistJetPhiEta(0),
  fHistEmbJetPhiEta(0),
  fHistEmbPartPhiEta(0),
  fHistRhoPartVSleadJetPt(0),
  fHistMedKtVSRhoPart(0),
  fHistRCPtVSRhoPart(0),
  fHistMedKtVSEmbBkg(0),
  fHistMedKtVSRCPt(0),
  fHistRCPtExLJVSDPhiLJ(0),
  fHistRCPhiEta(0)
{
  // Standard constructor.

  for (Int_t i = 0; i < 4; i++) {
    fHistJetsPt[i] = 0;
    fHistCorrJetsPt[i] = 0;
    fHistJetsPtCutPart[i] = 0;
    fHistCorrJetsPtCutPart[i] = 0;
    fHistJetsNEF[i] = 0;
    fHistJetsZ[i] = 0;
    fHistLeadingJetPt[i] = 0;
    fHistCorrLeadingJetPt[i] = 0;
    fHist2LeadingJetPt[i] = 0;
    fHistTracksPtLJ[i] = 0;
    fHistClusEtLJ[i] = 0;
    fHistTracksPtBkg[i] = 0;
    fHistClusEtBkg[i] = 0;
    fHistBkgClusMeanRho[i] = 0;
    fHistBkgTracksMeanRho[i] = 0;
    fHistMedianPtKtJet[i] = 0;
    fHistDeltaPtRC[i] = 0;
    fHistDeltaPtRCExLJ[i] = 0;
    fHistRCPt[i] = 0;
    fHistRCPtExLJ[i] = 0;
    fHistEmbJets[i] = 0;
    fHistEmbPart[i] = 0;
    fHistDeltaPtMedKtEmb[i] = 0;
  }

  // Output slot #1 writes into a TH1 container
  DefineOutput(1, TList::Class()); 
}

//________________________________________________________________________
AliAnalysisTaskSAJF::~AliAnalysisTaskSAJF()
{
  // Destructor
}

//________________________________________________________________________
void AliAnalysisTaskSAJF::UserCreateOutputObjects()
{
  // Create histograms
  
  OpenFile(1);
  fOutput = new TList();
  fOutput->SetOwner(); 
  
  fHistCentrality = new TH1F("fHistCentrality","Event centrality distribution", fNbins, 0, 100);
  fHistCentrality->GetXaxis()->SetTitle("Centrality (%)");
  fHistCentrality->GetYaxis()->SetTitle("counts");
  fOutput->Add(fHistCentrality);

  fHistJetPhiEta = new TH2F("fHistJetPhiEta","Phi-Eta distribution of jets", 20, -2, 2, 32, 0, 6.4);
  fHistJetPhiEta->GetXaxis()->SetTitle("Eta");
  fHistJetPhiEta->GetYaxis()->SetTitle("Phi");
  fOutput->Add(fHistJetPhiEta);

  fHistEmbJetPhiEta = new TH2F("fHistEmbJetPhiEta","Phi-Eta distribution of embedded jets", 20, -2, 2, 32, 0, 6.4);
  fHistEmbJetPhiEta->GetXaxis()->SetTitle("Eta");
  fHistEmbJetPhiEta->GetYaxis()->SetTitle("Phi");
  fOutput->Add(fHistEmbJetPhiEta);

  fHistEmbPartPhiEta = new TH2F("fHistEmbPartPhiEta","Phi-Eta distribution of embedded particles", 20, -2, 2, 32, 0, 6.4);
  fHistEmbPartPhiEta->GetXaxis()->SetTitle("Eta");
  fHistEmbPartPhiEta->GetYaxis()->SetTitle("Phi");
  fOutput->Add(fHistEmbPartPhiEta);

  fHistRhoPartVSleadJetPt = new TH2F("fHistRhoPartVSleadJetPt","fHistRhoPartVSleadJetPt", fNbins, fMinPt, fMaxPt, fNbins, fMinPt, fMaxPt);
  fHistRhoPartVSleadJetPt->GetXaxis()->SetTitle("#rho [GeV/c]");
  fHistRhoPartVSleadJetPt->GetYaxis()->SetTitle("Leading jet P_{T} [GeV/c]");
  fOutput->Add(fHistRhoPartVSleadJetPt);

  fHistMedKtVSRhoPart = new TH2F("fHistMedKtVSRhoPart","fHistMedKtVSRhoPart", fNbins, fMinPt, fMaxPt, fNbins, fMinPt, fMaxPt);
  fHistMedKtVSRhoPart->GetXaxis()->SetTitle("median kt P_{T} [GeV/c]");
  fHistMedKtVSRhoPart->GetYaxis()->SetTitle("#rho [GeV/c]");
  fOutput->Add(fHistMedKtVSRhoPart);
  
  fHistRCPtVSRhoPart = new TH2F("fHistRCPtVSRhoPart","fHistRCPtVSRhoPart", fNbins, fMinPt, fMaxPt, fNbins, fMinPt, fMaxPt);
  fHistRCPtVSRhoPart->GetXaxis()->SetTitle("rigid cone P_{T} [GeV/c]");
  fHistRCPtVSRhoPart->GetYaxis()->SetTitle("#rho [GeV/c]");
  fOutput->Add(fHistRCPtVSRhoPart);

  fHistMedKtVSEmbBkg = new TH2F("fHistMedKtVSEmbBkg","fHistMedKtVSEmbBkg", fNbins, fMinPt, fMaxPt, fNbins, fMinPt, fMaxPt);
  fHistMedKtVSEmbBkg->GetXaxis()->SetTitle("median kt P_{T} [GeV/c]");
  fHistMedKtVSEmbBkg->GetYaxis()->SetTitle("background of embedded track P_{T} [GeV]");
  fOutput->Add(fHistMedKtVSEmbBkg);

  fHistMedKtVSRCPt = new TH2F("fHistMedKtVSRCPt","fHistMedKtVSRCPt", fNbins, fMinPt, fMaxPt, fNbins, fMinPt, fMaxPt);
  fHistMedKtVSRCPt->GetXaxis()->SetTitle("median kt P_{T} [GeV/c]");
  fHistMedKtVSRCPt->GetYaxis()->SetTitle("rigid cone P_{T} [GeV/c]");
  fOutput->Add(fHistMedKtVSRCPt);

  fHistRCPtExLJVSDPhiLJ = new TH2F("fHistRCPtExLJVSDPhiLJ","fHistRCPtExLJVSDPhiLJ", fNbins, fMinPt, fMaxPt, 128, -1.6, 4.8);
  fHistRCPtExLJVSDPhiLJ->GetXaxis()->SetTitle("rigid cone P_{T} [GeV/c]");
  fHistRCPtExLJVSDPhiLJ->GetYaxis()->SetTitle("#Delta#phi");
  fOutput->Add(fHistRCPtExLJVSDPhiLJ);

  fHistRCPhiEta = new TH2F("fHistRCPhiEta","Phi-Eta distribution of rigid cones", 20, -2, 2, 32, 0, 6.4);
  fHistRCPhiEta->GetXaxis()->SetTitle("Eta");
  fHistRCPhiEta->GetYaxis()->SetTitle("Phi");
  fOutput->Add(fHistRCPhiEta);

  TString histname;

  for (Int_t i = 0; i < 4; i++) {
    histname = "fHistJetsPt_";
    histname += i;
    fHistJetsPt[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistJetsPt[i]->GetXaxis()->SetTitle("E [GeV]");
    fHistJetsPt[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistJetsPt[i]);

    histname = "fHistCorrJetsPt_";
    histname += i;
    fHistCorrJetsPt[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistCorrJetsPt[i]->GetXaxis()->SetTitle("E [GeV]");
    fHistCorrJetsPt[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistCorrJetsPt[i]);

    histname = "fHistJetsPtCutPart_";
    histname += i;
    fHistJetsPtCutPart[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistJetsPtCutPart[i]->GetXaxis()->SetTitle("E [GeV]");
    fHistJetsPtCutPart[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistJetsPtCutPart[i]);

    histname = "fHistCorrJetsPtCutPart_";
    histname += i;
    fHistCorrJetsPtCutPart[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistCorrJetsPtCutPart[i]->GetXaxis()->SetTitle("E [GeV]");
    fHistCorrJetsPtCutPart[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistCorrJetsPtCutPart[i]);
    
    histname = "fHistJetsNEF_";
    histname += i;
    fHistJetsNEF[i] = new TH1F(histname.Data(), histname.Data(), fNbins, 0, 1.2);
    fHistJetsNEF[i]->GetXaxis()->SetTitle("NEF");
    fHistJetsNEF[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistJetsNEF[i]);

    histname = "fHistJetsZ_";
    histname += i;
    fHistJetsZ[i] = new TH1F(histname.Data(), histname.Data(), fNbins, 0, 1.2);
    fHistJetsZ[i]->GetXaxis()->SetTitle("Z");
    fHistJetsZ[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistJetsZ[i]);

    histname = "fHistLeadingJetPt_";
    histname += i;
    fHistLeadingJetPt[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistLeadingJetPt[i]->GetXaxis()->SetTitle("P_{T} [GeV]");
    fHistLeadingJetPt[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistLeadingJetPt[i]);

    histname = "fHist2LeadingJetPt_";
    histname += i;
    fHist2LeadingJetPt[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHist2LeadingJetPt[i]->GetXaxis()->SetTitle("P_{T} [GeV]");
    fHist2LeadingJetPt[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHist2LeadingJetPt[i]);

    histname = "fHistCorrLeadingJetPt_";
    histname += i;
    fHistCorrLeadingJetPt[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistCorrLeadingJetPt[i]->GetXaxis()->SetTitle("P_{T} [GeV]");
    fHistCorrLeadingJetPt[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistCorrLeadingJetPt[i]);
    
    histname = "fHistTracksPtLJ_";
    histname += i;
    fHistTracksPtLJ[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt / 5);
    fHistTracksPtLJ[i]->GetXaxis()->SetTitle("P_{T} [GeV/c]");
    fHistTracksPtLJ[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistTracksPtLJ[i]);
    
    histname = "fHistClusEtLJ_";
    histname += i;
    fHistClusEtLJ[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt / 5);
    fHistClusEtLJ[i]->GetXaxis()->SetTitle("E_{T} [GeV]");
    fHistClusEtLJ[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistClusEtLJ[i]);

    histname = "fHistTracksPtBkg_";
    histname += i;
    fHistTracksPtBkg[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt / 5);
    fHistTracksPtBkg[i]->GetXaxis()->SetTitle("P_{T} [GeV/c]");
    fHistTracksPtBkg[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistTracksPtBkg[i]);
    
    histname = "fHistClusEtBkg_";
    histname += i;
    fHistClusEtBkg[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt / 5);
    fHistClusEtBkg[i]->GetXaxis()->SetTitle("E_{T} [GeV]");
    fHistClusEtBkg[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistClusEtBkg[i]);

    histname = "fHistBkgClusMeanRho_";
    histname += i;
    fHistBkgClusMeanRho[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistBkgClusMeanRho[i]->GetXaxis()->SetTitle("#rho [GeV]");
    fHistBkgClusMeanRho[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistBkgClusMeanRho[i]);

    histname = "fHistBkgTracksMeanRho_";
    histname += i;
    fHistBkgTracksMeanRho[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistBkgTracksMeanRho[i]->GetXaxis()->SetTitle("#rho [GeV/c]");
    fHistBkgTracksMeanRho[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistBkgTracksMeanRho[i]);

    histname = "fHistMedianPtKtJet_";
    histname += i;
    fHistMedianPtKtJet[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistMedianPtKtJet[i]->GetXaxis()->SetTitle("P_{T} [GeV/c]");
    fHistMedianPtKtJet[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistMedianPtKtJet[i]);

    histname = "fHistDeltaPtRC_";
    histname += i;
    fHistDeltaPtRC[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt - fMaxPt / 2, fMaxPt / 2);
    fHistDeltaPtRC[i]->GetXaxis()->SetTitle("#deltaP_{T} [GeV/c]");
    fHistDeltaPtRC[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistDeltaPtRC[i]);

    histname = "fHistDeltaPtRCExLJ_";
    histname += i;
    fHistDeltaPtRCExLJ[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt - fMaxPt / 2, fMaxPt / 2);
    fHistDeltaPtRCExLJ[i]->GetXaxis()->SetTitle("#deltaP_{T} [GeV/c]");
    fHistDeltaPtRCExLJ[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistDeltaPtRCExLJ[i]);

    histname = "fHistRCPt_";
    histname += i;
    fHistRCPt[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistRCPt[i]->GetXaxis()->SetTitle("rigid cone P_{T} [GeV/c]");
    fHistRCPt[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistRCPt[i]);

    histname = "fHistRCPtExLJ_";
    histname += i;
    fHistRCPtExLJ[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistRCPtExLJ[i]->GetXaxis()->SetTitle("rigid cone P_{T} [GeV/c]");
    fHistRCPtExLJ[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistRCPtExLJ[i]);

    histname = "fHistEmbJets_";
    histname += i;
    fHistEmbJets[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistEmbJets[i]->GetXaxis()->SetTitle("embedded jet P_{T} [GeV/c]");
    fHistEmbJets[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistEmbJets[i]);

    histname = "fHistEmbPart_";
    histname += i;
    fHistEmbPart[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt, fMaxPt);
    fHistEmbPart[i]->GetXaxis()->SetTitle("embedded particle P_{T} [GeV/c]");
    fHistEmbPart[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistEmbPart[i]);

    histname = "fHistDeltaPtMedKtEmb_";
    histname += i;
    fHistDeltaPtMedKtEmb[i] = new TH1F(histname.Data(), histname.Data(), fNbins, fMinPt - fMaxPt / 2, fMaxPt / 2);
    fHistDeltaPtMedKtEmb[i]->GetXaxis()->SetTitle("#deltaP_{T} [GeV/c]");
    fHistDeltaPtMedKtEmb[i]->GetYaxis()->SetTitle("counts");
    fOutput->Add(fHistDeltaPtMedKtEmb[i]);
  }

  PostData(1, fOutput); // Post data for ALL output slots >0 here, to get at least an empty histogram
}

//________________________________________________________________________
void AliAnalysisTaskSAJF::RetrieveEventObjects()
{
  if (strcmp(fKtJetsName,"") && fAnaType == kEMCAL) {
    fCaloClusters =  dynamic_cast<TClonesArray*>(InputEvent()->FindListObject(fCaloName));
    if (!fCaloClusters) {
      AliWarning(Form("Could not retrieve clusters!")); 
    }
  }

  fTracks = dynamic_cast<TClonesArray*>(InputEvent()->FindListObject(fTracksName));
  if (!fTracks) {
    AliWarning(Form("Could not retrieve tracks!")); 
  }

  fJets = dynamic_cast<TClonesArray*>(InputEvent()->FindListObject(fJetsName));
  if (!fJets) {
    AliWarning(Form("Could not retrieve jets!")); 
  }

  if (strcmp(fKtJetsName,"")) {
    fKtJets = dynamic_cast<TClonesArray*>(InputEvent()->FindListObject(fKtJetsName));
    if (!fKtJets) {
      AliWarning(Form("Could not retrieve Kt jets!")); 
    }
  }

  if (strcmp(fEmbJetsName,"")) {
    fEmbJets = dynamic_cast<TClonesArray*>(InputEvent()->FindListObject(fEmbJetsName));
    if (!fEmbJets) {
      AliWarning(Form("Could not retrieve emb jets!")); 
    }
  }

  fCent = InputEvent()->GetCentrality();
  if (fCent) {
    Float_t cent = fCent->GetCentralityPercentile("V0M");
    if      (cent >=  0 && cent <   10) fCentBin = 0;
    else if (cent >= 10 && cent <   30) fCentBin = 1;
    else if (cent >= 30 && cent <   50) fCentBin = 2;
    else if (cent >= 50 && cent <= 100) fCentBin = 3; 
    else {
      AliWarning(Form("Negative centrality: %f. Assuming 99", cent));
      fCentBin = 3;
    }
  }
  else {
    AliWarning(Form("Could not retrieve centrality information! Assuming 99"));
    fCentBin = 3;
  }

  TParameter<Double_t> *rho = dynamic_cast<TParameter<Double_t>*>(InputEvent()->FindListObject(fRhoName));
  
  if (rho) {
    fRho = rho->GetVal();
  }
  else {
    AliWarning("No rho information found in the event. Will recalculate it by myself...");
  }
}

//________________________________________________________________________
AliVTrack* AliAnalysisTaskSAJF::GetTrack(const Int_t i) const
{
  if (fTracks)
    return dynamic_cast<AliVTrack*>(fTracks->At(i));
  else
    return 0;
}

//________________________________________________________________________
Int_t AliAnalysisTaskSAJF::GetNumberOfTracks() const
{
  if (fTracks)
    return fTracks->GetEntriesFast();
  else
    return 0;
}

//________________________________________________________________________
AliVCluster* AliAnalysisTaskSAJF::GetCaloCluster(const Int_t i) const
{ 
  if (fCaloClusters)
    return dynamic_cast<AliVCluster*>(fCaloClusters->At(i));
  else
    return 0;
}

//________________________________________________________________________
Int_t AliAnalysisTaskSAJF::GetNumberOfCaloClusters() const
{ 
  if (fCaloClusters)
    return fCaloClusters->GetEntriesFast();
  else
    return 0;
}

//________________________________________________________________________
AliEmcalJet* AliAnalysisTaskSAJF::GetJet(const Int_t i) const
{
  if (fJets)
    return dynamic_cast<AliEmcalJet*>(fJets->At(i));
  else
    return 0;
}

//________________________________________________________________________
Int_t AliAnalysisTaskSAJF::GetNumberOfJets() const
{
  if (fJets)
    return fJets->GetEntriesFast();
  else
    return 0;
}

//________________________________________________________________________
AliEmcalJet* AliAnalysisTaskSAJF::GetKtJet(const Int_t i) const
{
  if (fKtJets)
    return dynamic_cast<AliEmcalJet*>(fKtJets->At(i));
  else
    return 0;
}

//________________________________________________________________________
Int_t AliAnalysisTaskSAJF::GetNumberOfKtJets() const
{
  if (fKtJets)
    return fKtJets->GetEntriesFast();
  else
    return 0;
}

//________________________________________________________________________
AliEmcalJet* AliAnalysisTaskSAJF::GetEmbJet(const Int_t i) const
{
  if (fEmbJets)
    return dynamic_cast<AliEmcalJet*>(fEmbJets->At(i));
  else
    return 0;
}

//________________________________________________________________________
Int_t AliAnalysisTaskSAJF::GetNumberOfEmbJets() const
{
  if (fEmbJets)
    return fEmbJets->GetEntriesFast();
  else
    return 0;
}

//________________________________________________________________________
void AliAnalysisTaskSAJF::FillHistograms()
{
  Float_t A = fJetRadius * fJetRadius * TMath::Pi();

  Float_t cent = 100;

  // as a rule will use rho provided by a previous RhoTask (see RetrieveEventObjects)
  if (fRho < 0) {
    Float_t rhoKt = 0;
    DoKtJetLoop(rhoKt);
    
    if (rhoKt == 0) {
      AliWarning("Skipping event with less than 2 reconstructed kt jets...");
      return;
    }

    fRho = rhoKt;
  }

  if (fCent)
    cent = fCent->GetCentralityPercentile("V0M");

  fHistCentrality->Fill(cent);

  Int_t maxJetIndex  = -1;
  Int_t max2JetIndex = -1;

  DoJetLoop(maxJetIndex, max2JetIndex);
  
  if (maxJetIndex < 0) 
    return;

  AliEmcalJet* jet = GetJet(maxJetIndex);
  if (!jet) 
    return;

  fHistLeadingJetPt[fCentBin]->Fill(jet->Pt());
  jet->SortConstituents();
  
  AliEmcalJet* jet2 = 0;
  if (max2JetIndex >= 0)
    jet2 = GetJet(max2JetIndex);

  if (jet2) {
    fHist2LeadingJetPt[fCentBin]->Fill(jet2->Pt());
    jet2->SortConstituents();
  }

  fHistMedianPtKtJet[fCentBin]->Fill(fRho);

  Float_t maxJetCorrPt = jet->Pt() - fRho * jet->Area();
  if (maxJetCorrPt > 0)
    fHistCorrLeadingJetPt[fCentBin]->Fill(maxJetCorrPt);
  
  Float_t rhoTracksLJex = 0;
  Float_t rhoTracks = 0;
  DoTrackLoop(rhoTracksLJex, rhoTracks, maxJetIndex, max2JetIndex);
  fHistBkgTracksMeanRho[fCentBin]->Fill(rhoTracksLJex);

  Float_t rhoClusLJex = 0;
  Float_t rhoClus = 0;
  if (fAnaType == kEMCAL)
    DoClusterLoop(rhoClusLJex, rhoClus, maxJetIndex, max2JetIndex);

  Float_t rhoPartLJex = rhoTracksLJex + rhoClusLJex;
 
  fHistBkgClusMeanRho[fCentBin]->Fill(rhoClusLJex);

  fHistRhoPartVSleadJetPt->Fill(jet->Area() * rhoPartLJex, jet->Pt());

  fHistMedKtVSRhoPart->Fill(fRho, rhoPartLJex);
  
  Int_t nRCs = 1; //(Int_t)(GetArea() / A - 1);

  for (Int_t i = 0; i < nRCs; i++) {
    Float_t RCpt = 0;
    Float_t RCeta = 0;
    Float_t RCphi = 0;
    GetRigidConePt(RCpt, RCeta, RCphi, 0);

    Float_t RCptExLJ = 0;
    Float_t RCetaExLJ = 0;
    Float_t RCphiExLJ = 0;
    GetRigidConePt(RCptExLJ, RCetaExLJ, RCphiExLJ, jet);
    
    fHistDeltaPtRC[fCentBin]->Fill(RCpt - A * fRho);
    fHistDeltaPtRCExLJ[fCentBin]->Fill(RCptExLJ - A * fRho);

    fHistRCPt[fCentBin]->Fill(RCpt / A);
    fHistRCPtExLJ[fCentBin]->Fill(RCptExLJ / A);
    fHistRCPtVSRhoPart->Fill(RCptExLJ / A, rhoPartLJex);

    fHistMedKtVSRCPt->Fill(A * fRho, RCptExLJ);

    fHistRCPhiEta->Fill(RCeta, RCphiExLJ);

    Float_t dphi = RCphiExLJ - jet->Phi();
    if (dphi > 4.8) dphi -= TMath::Pi() * 2;
    if (dphi < -1.6) dphi += TMath::Pi() * 2; 
    fHistRCPtExLJVSDPhiLJ->Fill(RCptExLJ, dphi);
  }

  AliEmcalJet *maxJet  = 0;
  TObject     *maxPart = 0;

  Bool_t embOK = DoEmbJetLoop(maxJet, maxPart);

  if (embOK) {
    AliVCluster *cluster = dynamic_cast<AliVCluster*>(maxPart);
    AliVTrack *track = dynamic_cast<AliVTrack*>(maxPart);
    Float_t maxEmbPartPt = 0;
    Float_t maxEmbPartEta = 0;
    Float_t maxEmbPartPhi = 0;
    if (cluster) {
      Double_t vertex[3] = {0, 0, 0};
      InputEvent()->GetPrimaryVertex()->GetXYZ(vertex);
      TLorentzVector nPart;
      cluster->GetMomentum(nPart, vertex);
      Float_t pos[3];
      cluster->GetPosition(pos);
      TVector3 clusVec(pos);
      
      maxEmbPartPt = nPart.Et();
      maxEmbPartEta = clusVec.Eta();
      maxEmbPartPhi = clusVec.Phi();
    }
    else if (track) {
      maxEmbPartPt = track->Pt();
      maxEmbPartEta = track->Eta();
      maxEmbPartPhi = track->Phi();
    }
    else {
      AliWarning("Embedded particle type not recognized (neither AliVCluster nor AliVTrack)!");
      return;
    }
    fHistEmbJets[fCentBin]->Fill(maxJet->Pt());
    fHistEmbPart[fCentBin]->Fill(maxEmbPartPt);
    fHistDeltaPtMedKtEmb[fCentBin]->Fill(maxJet->Pt() - maxJet->Area() * fRho - maxEmbPartPt);
    fHistMedKtVSEmbBkg->Fill(maxJet->Area() * fRho, maxJet->Pt() - maxEmbPartPt);
    fHistEmbJetPhiEta->Fill(maxJet->Eta(), maxJet->Phi());
    fHistEmbPartPhiEta->Fill(maxEmbPartEta, maxEmbPartPhi);
  }
  else {
    if (maxPart != 0)
      AliWarning("Embedded particle is not the leading particle of the leading jet!");
  }
}

//________________________________________________________________________
void AliAnalysisTaskSAJF::DoJetLoop(Int_t &maxJetIndex, Int_t &max2JetIndex)
{
  Double_t vertex[3] = {0, 0, 0};
  InputEvent()->GetPrimaryVertex()->GetXYZ(vertex);

  Int_t njets = GetNumberOfJets();

  Float_t maxJetPt = 0;
  Float_t max2JetPt = 0;
  for (Int_t ij = 0; ij < njets; ij++) {

    AliEmcalJet* jet = GetJet(ij);

    if (!jet) {
      AliError(Form("Could not receive jet %d", ij));
      continue;
    }  
    
    if (jet->Pt() <= 0)
      continue;

    if (!AcceptJet(jet))
      continue;

    Float_t corrPt = jet->Pt() - fRho * jet->Area();

    fHistJetsPt[fCentBin]->Fill(jet->Pt());

    if (corrPt > 0)
      fHistCorrJetsPt[fCentBin]->Fill(corrPt);

    fHistJetPhiEta->Fill(jet->Eta(), jet->Phi());
    fHistJetsNEF[fCentBin]->Fill(jet->NEF());

    Float_t maxPt = 0;

    for (Int_t it = 0; it < jet->GetNumberOfTracks(); it++) {
      AliVTrack *track = jet->TrackAt(it, fTracks);
      if (track)
	fHistJetsZ[fCentBin]->Fill(track->Pt() / jet->Pt());
      if (track->Pt() > maxPt)
	maxPt = track->Pt();
    }

    for (Int_t ic = 0; ic < jet->GetNumberOfClusters(); ic++) {
      AliVCluster *cluster = jet->ClusterAt(ic, fCaloClusters);

      if (cluster) {
	TLorentzVector nPart;
	cluster->GetMomentum(nPart, vertex);
	fHistJetsZ[fCentBin]->Fill(nPart.Et() / jet->Pt());

	if (nPart.Et() > maxPt)
	  maxPt = nPart.Et();
      }
    }

    if (maxPt >= fPtCutJetPart) {
      fHistJetsPtCutPart[fCentBin]->Fill(jet->Pt());
      if (corrPt > 0)
	fHistCorrJetsPtCutPart[fCentBin]->Fill(corrPt);
    }

    if (maxJetPt < jet->Pt()) {
      max2JetPt = maxJetPt;
      max2JetIndex = maxJetIndex;
      maxJetPt = jet->Pt();
      maxJetIndex = ij;
    }
    else if (max2JetPt < jet->Pt()) {
      max2JetPt = jet->Pt();
      max2JetIndex = ij;
    }
  } //jet loop 
}

//________________________________________________________________________
void AliAnalysisTaskSAJF::DoKtJetLoop(Float_t &rhoKt, Int_t nLJs)
{
  rhoKt = 0;

  Int_t nktjets =  GetNumberOfKtJets();

  Int_t NoOfZeroJets = 0;
  if (nktjets > 0) {
    
    TArrayI ktJets(nktjets);
    ktJets.Reset(-1);
    for (Int_t ij = 0; ij < nktjets; ij++) {
      
      AliEmcalJet* jet = GetKtJet(ij);
      
      if (!jet) {
	AliError(Form("Could not receive jet %d", ij));
	continue;
      } 
      
      if (jet->Pt() <= 0 || jet->Area() <= 0) {
	NoOfZeroJets++;
	continue;
      }
      
      if (!AcceptJet(jet)) {
	NoOfZeroJets++;
	continue;
      }
    
      Float_t rho = jet->Pt() / jet->Area();
      Int_t i = nktjets;
      AliEmcalJet *cmpjet = 0;
      do {
	i--;
	if (ktJets[i] >= 0)
	  cmpjet = GetKtJet(ktJets[i]);
	else
	  cmpjet = 0;
      } while (cmpjet && rho < cmpjet->Pt() / cmpjet->Area() && i > 0);
      
      memmove(ktJets.GetArray() + nktjets - ij - 1, ktJets.GetArray() + nktjets - ij, (ij + i - nktjets + 1) * sizeof(Int_t));
      ktJets[i] = ij;
    } //kt jet loop 

    nktjets -= NoOfZeroJets;
    if (nktjets < 1) return;

    memmove(ktJets.GetArray(), ktJets.GetArray() + NoOfZeroJets, nktjets * sizeof(Int_t));

    nktjets -= nLJs;
    if (nktjets < 1) return;

    const Float_t maxEta = fMaxEta - fJetRadius;
    const Float_t minEta = fMinEta + fJetRadius;
    const Float_t maxPhi = fMaxPhi - fJetRadius;
    const Float_t minPhi = fMinPhi + fJetRadius;

    for (Int_t i = 0; i < nktjets; i++) {
      AliEmcalJet *cmpjet = GetKtJet(ktJets[i]);
      if (cmpjet->Eta() > maxEta || cmpjet->Eta() < minEta
	  || cmpjet->Phi() > maxPhi || cmpjet->Phi() < minPhi) {
	nktjets--;
	memmove(ktJets.GetArray() + i, ktJets.GetArray() + i + 1, (nktjets - i) * sizeof(Int_t));
	i--;
      }
    }

    if (nktjets % 2 != 0) {   // odd
      AliEmcalJet *medJet = GetKtJet(ktJets[(nktjets - 1) / 2]);
      rhoKt = medJet->Pt() / medJet->Area();
    }
    else {   // even
      AliEmcalJet *medJet1 = GetKtJet(ktJets[nktjets / 2]);
      AliEmcalJet *medJet2 = GetKtJet(ktJets[nktjets / 2 - 1]);
      rhoKt = (medJet1->Pt() / medJet1->Area() + medJet2->Pt() / medJet2->Area()) / 2;
    }
  }
}

//________________________________________________________________________
Bool_t AliAnalysisTaskSAJF::DoEmbJetLoop(AliEmcalJet* &maxJet, TObject* &maxPart)
{
  Double_t vertex[3] = {0, 0, 0};
  InputEvent()->GetPrimaryVertex()->GetXYZ(vertex);

  Int_t nembjets = GetNumberOfEmbJets();

  Int_t maxJetIdx = -1;
  Int_t maxTrackIdx = -1;
  Int_t maxClusIdx = -1;

  Float_t maxTrackPt = 0;
  Float_t maxClusEt = 0;
  Float_t maxJetPt = 0;

  for (Int_t ij = 0; ij < nembjets; ij++) {
      
    AliEmcalJet* jet = GetEmbJet(ij);
      
    if (!jet) {
      AliError(Form("Could not receive jet %d", ij));
      continue;
    } 
      
    if (jet->Pt() <= 0)
	continue;
 
    if (!AcceptJet(jet))
      continue;
    
    if (jet->Pt() > maxJetPt) {
      maxJetPt = jet->Pt();
      maxJetIdx = ij;
    }
  }

  if (maxJetPt <= 0)
    return kFALSE;

  maxJet = GetEmbJet(maxJetIdx);

  for (Int_t it = 0; it < maxJet->GetNumberOfTracks(); it++) {
    AliVTrack *track = maxJet->TrackAt(it, fTracks);

    if (!track) continue;
     
    if (track->Pt() > maxTrackPt) {
      maxTrackPt = track->Pt();
      maxTrackIdx = it;
    }
  }

  for (Int_t ic = 0; ic < maxJet->GetNumberOfClusters(); ic++) {
    AliVCluster *cluster = maxJet->ClusterAt(ic, fCaloClusters);
    
    if (!cluster) continue;
    
    TLorentzVector nPart;
    cluster->GetMomentum(nPart, vertex);

    if (nPart.Et() > maxClusEt) {
      maxClusEt = nPart.Et();
      maxClusIdx = ic;
    } 
  }

  if (maxClusEt > maxTrackPt) {
    AliVCluster *cluster = maxJet->ClusterAt(maxClusIdx, fCaloClusters);
    maxPart = cluster;
    return (Bool_t)(cluster->Chi2() == 100);
  }
  else {
    AliVTrack *track = maxJet->TrackAt(maxTrackIdx, fTracks);
    maxPart = track;
    return (Bool_t)(track->GetLabel() == 100);
  }
}

//________________________________________________________________________
void AliAnalysisTaskSAJF::DoTrackLoop(Float_t &rhoTracksLJex, Float_t &rhoTracks, Int_t maxJetIndex, Int_t max2JetIndex)
{ 
  AliEmcalJet* jet = 0;
  if (max2JetIndex >= 0)
    jet = GetJet(maxJetIndex);

  AliEmcalJet* jet2 = 0;
  if (max2JetIndex >= 0)
    jet2 = GetJet(max2JetIndex);

  Float_t area = GetArea();
  if (jet) area -= jet->Area();
  if (jet2) area -= jet2->Area();

  Int_t ntracks = GetNumberOfTracks();

  rhoTracksLJex = 0;
  rhoTracks = 0;

  for(Int_t iTracks = 0; iTracks < ntracks; iTracks++) {
    AliVTrack* track = GetTrack(iTracks);         
    if(!track) {
      AliError(Form("Could not retrieve track %d",iTracks)); 
      continue; 
    }
    
    if (!AcceptTrack(track)) continue;
    
    rhoTracks += track->Pt();

    if (jet && IsJetTrack(jet, iTracks)) {
      fHistTracksPtLJ[fCentBin]->Fill(track->Pt());
    }
    else if (!jet2 || !IsJetTrack(jet2, iTracks)) {
      fHistTracksPtBkg[fCentBin]->Fill(track->Pt());
      rhoTracksLJex += track->Pt();
    }
  }
  rhoTracksLJex /= area;
  rhoTracks /= area;
}

//________________________________________________________________________
void AliAnalysisTaskSAJF::DoClusterLoop(Float_t &rhoClusLJex, Float_t &rhoClus, Int_t maxJetIndex, Int_t max2JetIndex)
{
  Double_t vertex[3] = {0, 0, 0};
  InputEvent()->GetPrimaryVertex()->GetXYZ(vertex);

  AliEmcalJet* jet = 0;
  if (max2JetIndex >= 0)
    jet = GetJet(maxJetIndex);

  AliEmcalJet* jet2 = 0;
  if (max2JetIndex >= 0)
    jet2 = GetJet(max2JetIndex);

  Float_t area = GetArea();
  if (jet) area -= jet->Area();
  if (jet2) area -= jet2->Area();

  rhoClusLJex = 0;
  rhoClus = 0;

  Int_t nclusters =  GetNumberOfCaloClusters();
  for (Int_t iClusters = 0; iClusters < nclusters; iClusters++) {
    AliVCluster* cluster = GetCaloCluster(iClusters);
    if (!cluster) {
      AliError(Form("Could not receive cluster %d", iClusters));
      continue;
    }  
    
    if (!(cluster->IsEMCAL())) continue;
    
    if (!AcceptCluster(cluster)) continue;

    TLorentzVector nPart;
    cluster->GetMomentum(nPart, vertex);

    rhoClus += nPart.Et();

    if (jet && IsJetCluster(jet, iClusters)) {
      fHistClusEtLJ[fCentBin]->Fill(nPart.Et());
    }
    else if (!jet2 || !IsJetCluster(jet2, iClusters)) {
      fHistClusEtBkg[fCentBin]->Fill(nPart.Et());
      rhoClusLJex += nPart.Et();
    }
  } //cluster loop 
  rhoClusLJex /= area;
  rhoClus /= area;
}

//________________________________________________________________________
void AliAnalysisTaskSAJF::Init()
{
  if (fAnaType == kTPC) {
    fMinEta = -0.9;
    fMaxEta = 0.9;
    fMinPhi = -10;
    fMaxPhi = 10;
  }
  else if (fAnaType == kEMCAL) {
    fMinEta = -0.7;
    fMaxEta = 0.7;
    fMinPhi = 80 * TMath::DegToRad();
    fMaxPhi = 180 * TMath::DegToRad();
  }
  else {
    AliWarning("Analysis type not recognized! Assuming kTPC...");
    fAnaType = kTPC;
    Init();
  }
}

//________________________________________________________________________
Bool_t AliAnalysisTaskSAJF::GetRigidConePt(Float_t &pt, Float_t &eta, Float_t &phi, AliEmcalJet *jet, Float_t minD)
{
  static TRandom3 random;

  Double_t vertex[3] = {0, 0, 0};
  InputEvent()->GetPrimaryVertex()->GetXYZ(vertex);

  eta = 0;
  phi = 0;

  Float_t LJeta = 999;
  Float_t LJphi = 999;

  if (jet) {
    LJeta = jet->Eta();
    LJphi = jet->Phi();
  }

  Float_t maxEta = fMaxEta - fJetRadius;
  Float_t minEta = fMinEta + fJetRadius;
  Float_t maxPhi = fMaxPhi - fJetRadius;
  Float_t minPhi = fMinPhi + fJetRadius;

  if (maxPhi > TMath::Pi() * 2) maxPhi = TMath::Pi() * 2;
  if (minPhi < 0) minPhi = 0;
  
  Float_t dLJ = 0;
  do {
    eta = random.Rndm() * (maxEta - minEta) + minEta;
    phi = random.Rndm() * (maxPhi - minPhi) + minPhi;
    dLJ = TMath::Sqrt((LJeta - eta) * (LJeta - eta) + (LJphi - phi) * (LJphi - phi));
  } while (dLJ < minD && !AcceptJet(eta, phi));
  
  pt = 0;

  if (fAnaType == kEMCAL) {
    Int_t nclusters =  GetNumberOfCaloClusters();
    for (Int_t iClusters = 0; iClusters < nclusters; iClusters++) {
      AliVCluster* cluster = GetCaloCluster(iClusters);
      if (!cluster) {
	AliError(Form("Could not receive cluster %d", iClusters));
	continue;
      }  
      
      if (!(cluster->IsEMCAL())) continue;
      
      if (!AcceptCluster(cluster)) continue;
      
      TLorentzVector nPart;
      cluster->GetMomentum(nPart, vertex);
      
      Float_t pos[3];
      cluster->GetPosition(pos);
      TVector3 clusVec(pos);
      
      Float_t d = TMath::Sqrt((clusVec.Eta() - eta) * (clusVec.Eta() - eta) + (clusVec.Phi() - phi) * (clusVec.Phi() - phi));
      
      if (d <= fJetRadius)
	pt += nPart.Pt();
    }
  }

  Int_t ntracks = GetNumberOfTracks();
  for(Int_t iTracks = 0; iTracks < ntracks; iTracks++) {
    AliVTrack* track = GetTrack(iTracks);         
    if(!track) {
      AliError(Form("Could not retrieve track %d",iTracks)); 
      continue; 
    }
    
    if (!AcceptTrack(track)) continue;

    Float_t tracketa = track->Eta();
    Float_t trackphi = track->Phi();
    
    if (TMath::Abs(trackphi - phi) > TMath::Abs(trackphi - phi + 2 * TMath::Pi()))
      trackphi += 2 * TMath::Pi();
    if (TMath::Abs(trackphi - phi) > TMath::Abs(trackphi - phi - 2 * TMath::Pi()))
      trackphi -= 2 * TMath::Pi();

    Float_t d = TMath::Sqrt((tracketa - eta) * (tracketa - eta) + (trackphi - phi) * (trackphi - phi));

    if (d <= fJetRadius)
      pt += track->Pt();
  }

  return kTRUE;
}

//________________________________________________________________________
Float_t AliAnalysisTaskSAJF::GetArea() const
{
  Float_t dphi = fMaxPhi - fMinPhi;
  if (dphi > TMath::Pi() * 2) dphi = TMath::Pi() * 2;
  Float_t deta = fMaxEta - fMinEta;
  return deta * dphi;
}

//________________________________________________________________________
Bool_t AliAnalysisTaskSAJF::IsJetTrack(AliEmcalJet* jet, Int_t itrack, Bool_t sorted) const
{
  for (Int_t i = 0; i < jet->GetNumberOfTracks(); i++) {
    Int_t ijettrack = jet->TrackAt(i);
    if (sorted && ijettrack > itrack)
      return kFALSE;
    if (ijettrack == itrack)
      return kTRUE;
  }
  return kFALSE;
}

//________________________________________________________________________
Bool_t AliAnalysisTaskSAJF::IsJetCluster(AliEmcalJet* jet, Int_t iclus, Bool_t sorted) const
{
  for (Int_t i = 0; i < jet->GetNumberOfClusters(); i++) {
    Int_t ijetclus = jet->ClusterAt(i);
    if (sorted && ijetclus > iclus)
      return kFALSE;
    if (ijetclus == iclus)
      return kTRUE;
  }
  return kFALSE;
}

//________________________________________________________________________
Bool_t AliAnalysisTaskSAJF::AcceptJet(Float_t eta, Float_t phi) const
{
  return (Bool_t)(eta > fMinEta + fJetRadius && eta < fMaxEta - fJetRadius && phi > fMinPhi + fJetRadius && phi < fMaxPhi - fJetRadius);
}

//________________________________________________________________________
Bool_t AliAnalysisTaskSAJF::AcceptJet(AliEmcalJet* jet) const
{
  return AcceptJet(jet->Eta(), jet->Phi());
}

//________________________________________________________________________
Bool_t AliAnalysisTaskSAJF::AcceptCluster(AliVCluster* clus, Bool_t acceptMC)
{
  if (!acceptMC && clus->Chi2() == 100)
    return kFALSE;

  Double_t vertex[3] = {0, 0, 0};
  InputEvent()->GetPrimaryVertex()->GetXYZ(vertex);
  TLorentzVector nPart;
  clus->GetMomentum(nPart, vertex);

  if (nPart.Et() < fPtCut)
    return kFALSE;

  return kTRUE;
}

//________________________________________________________________________
Bool_t AliAnalysisTaskSAJF::AcceptTrack(AliVTrack* track, Bool_t acceptMC) const
{
  if (!acceptMC && track->GetLabel() == 100)
    return kFALSE;

  if (track->Pt() < fPtCut)
    return kFALSE;
  
  return (Bool_t)(track->Eta() > fMinEta && track->Eta() < fMaxEta && track->Phi() > fMinPhi && track->Phi() < fMaxPhi);
}

//________________________________________________________________________
void AliAnalysisTaskSAJF::UserExec(Option_t *) 
{
  Init();

  RetrieveEventObjects();

  FillHistograms();
    
  // information for this iteration of the UserExec in the container
  PostData(1, fOutput);
}

//________________________________________________________________________
void AliAnalysisTaskSAJF::Terminate(Option_t *) 
{
  // Called once at the end of the analysis.
}
