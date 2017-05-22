#ifndef ALIANACLUSTERSHAPECORRELSTUDIES_H
#define ALIANACLUSTERSHAPECORRELSTUDIES_H
/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice     */

//_________________________________________________________________________
/// \class AliAnaClusterShapeCorrelStudies
/// \ingroup CaloTrackCorrelationsAnalysis 
/// \brief Class for cluster shape, cell T-Card correlation and exoticity
///
/// Class for cluster shape, cell T-Card correlation and exoticity.
/// Extracted/cloned from AliAnaCalorimeterQA version AN20170521
///
/// \author Gustavo Conesa Balbastre <Gustavo.Conesa.Balbastre@cern.ch>, LPSC-IN2P3-CNRS
///_________________________________________________________________________

// --- Root system ---
class TH3F;
class TH2F;
class TH1F;
class TObjString;
class TObjArray;

// --- Analysis system --- 
class AliVCaloCells;
class AliVCaloCluster;
class AliVTrack;

#include "AliAnaCaloTrackCorrBaseClass.h"
 
class AliAnaClusterShapeCorrelStudies : public AliAnaCaloTrackCorrBaseClass {
  
public:
    
  AliAnaClusterShapeCorrelStudies() ;
    
  /// Virtual destructor. Not implemented.
  virtual ~AliAnaClusterShapeCorrelStudies() { ; }
    
  // General methods
  
  TObjString * GetAnalysisCuts();

  TList      * GetCreateOutputObjects();
  
  void         Init();
  
  void         InitParameters();
    
  void         MakeAnalysisFillHistograms() ;
  
  void         Print(const Option_t * opt) const;
    
  // Main methods
  
  void         BadClusterHistograms(AliVCluster* clus, const TObjArray *caloClusters,  AliVCaloCells * cells,
                                    Int_t absIdMax,    Double_t maxCellFraction, Float_t eCrossFrac, Double_t tmax);
          
  void         ClusterAsymmetryHistograms(AliVCluster* clus, Int_t absIdMax, Bool_t goodCluster );
  
  void         ClusterM02DependentHistograms(AliVCluster* cluster, AliVCaloCells * cells, 
                                             Int_t absIdMax, Double_t maxCellFraction, Float_t eCrossFrac, Double_t tmax);
  
  void         ClusterLoopHistograms(const TObjArray * clusters, AliVCaloCells * cells);
    
//  void         ExoticHistograms(Int_t absIdMax, Float_t ampMax,
//                                AliVCluster *clus, AliVCaloCells* cells);
  
  void         ChannelCorrelationInTCard(AliVCluster* clus, AliVCaloCells * cells, Bool_t matched, Int_t absIdMax, Float_t exoticity) ;
    
  Bool_t       IsGoodCluster(Int_t absIdMax, Float_t m02, Int_t nCellsPerCluster, AliVCaloCells *cells);

  void         WeightHistograms(AliVCluster *clus, AliVCaloCells* cells);

  
  // Setters and getters
    
  Float_t      GetM02Min()               const  { return fM02Min   ; }
  void         SetM02Min(Float_t m02)           { fM02Min = m02    ; }

  Float_t      GetDistToBadMin()         const  { return fMinDistToBad ; }
  void         SetDistToBadMin(Float_t di)      { fMinDistToBad = di   ; }

  Int_t        GetNCellsPerClusterMin()  const  { return fNCellMin ; }
  void         SetNCellsPerClusterMin(Int_t n)  { fNCellMin = n    ; }

  Float_t      GetInvMassMinECut()       const  { return fInvMassMinECut     ; }
  void         SetInvMassMinECut(Float_t cut)   { fInvMassMinECut = cut      ; }

  Float_t      GetInvMassMaxECut()       const  { return fInvMassMaxECut     ; }
  void         SetInvMassMaxECut(Float_t cut)   { fInvMassMaxECut = cut      ; }

  Float_t      GetInvMassMinM02Cut()     const  { return fInvMassMinM02Cut   ; }
  void         SetInvMassMinM02Cut(Float_t cut) { fInvMassMinM02Cut = cut    ; }
  
  Float_t      GetInvMassMaxM02Cut()     const  { return fInvMassMaxM02Cut   ; }
  void         SetInvMassMaxM02Cut(Float_t cut) { fInvMassMaxM02Cut = cut    ; }
  
  Float_t      GetInvMassMaxOpenAngle()  const  { return fInvMassMaxOpenAngle; }
  void         SetInvMassMaxOpenAngle(Float_t c){ fInvMassMaxOpenAngle = c   ; }
  
  Float_t      GetInvMassMaxTimeDifference()  const   { return fInvMassMaxTimeDifference; }
  void         SetInvMassMaxTimeDifference(Float_t c) { fInvMassMaxTimeDifference = c   ; }  
      
  void         SetNEBinCuts(Int_t nb)           { fNEBinCuts = nb            ; }
  void         SetEBinCutsAt(Int_t i, Float_t va) { if(i < 15) fEBinCuts[i] = va ; }

  // Analysis switchs
  
  void SwitchOnStudyClustersAsymmetry()         { fStudyClustersAsymmetry = kTRUE  ; }
  void SwitchOffStudyClustersAsymmetry()        { fStudyClustersAsymmetry = kFALSE ; }

  void SwitchOnStudyWeight()                    { fStudyWeight      = kTRUE  ; }
  void SwitchOffStudyWeight()                   { fStudyWeight      = kFALSE ; }
  
  void SwitchOnStudyTCardCorrelation()          { fStudyTCardCorrelation = kTRUE  ; }
  void SwitchOffStudyTCardCorrelation()         { fStudyTCardCorrelation = kFALSE ; }
  
  void SwitchOnStudyM02Dependence()             { fStudyM02Dependence = kTRUE  ; }
  void SwitchOffStudyM02Dependence()            { fStudyM02Dependence = kFALSE ; }
  
  void SwitchOnStudyExotic()                    { fStudyExotic      = kTRUE  ; }
  void SwitchOffStudyExotic()                   { fStudyExotic      = kFALSE ; }
  
  void SetConstantTimeShift(Float_t shift)      { fConstantTimeShift     = shift  ; }
  
 private:
  
  // Switches
  Bool_t   fStudyClustersAsymmetry;             ///<  Study asymmetry of clusters, not QA related
  Bool_t   fStudyExotic;                        ///<  Study the exotic cluster for different cuts, not QA related
  Bool_t   fStudyWeight;                        ///<  Study the energy weight used in different cluster calculations, not QA related
  Bool_t   fStudyTCardCorrelation;              ///<  Study TCard channels cross correlation
  Bool_t   fStudyM02Dependence;                 ///<  TH3 histograms where M02 and energy are 2 axes and 
    
  // Cuts
  Float_t  fM02Min;                             ///<  Minimum M02 on clusters
  Int_t    fNCellMin;                           ///<  Minimum number of cells on clusters
  Float_t  fMinDistToBad;                       ///<  Minimum distance to bad channel
  
  Float_t  fEBinCuts[15] ;                      ///<  Energy bins cut 
  Int_t    fNEBinCuts;                          ///<  Number of energy bin cuts

  // Invariant mass analysis
  
  Float_t  fInvMassMinECut;                     ///<  Minimum energy cut value for clusters entering the invariant mass calculation
  Float_t  fInvMassMaxECut;                     ///<  Maximum energy cut value for clusters entering the invariant mass calculation
  Float_t  fInvMassMinM02Cut;                   ///<  Minimum M02 shower shape cut value for clusters entering the invariant mass calculation
  Float_t  fInvMassMaxM02Cut;                   ///<  Maximum M02 shower shape cut value for clusters entering the invariant mass calculation
  Float_t  fInvMassMaxOpenAngle;                ///<  Combine clusters within with a maximum opening angle between them. In radians.
  Float_t  fInvMassMaxTimeDifference;           ///<  Maximum difference between the time of the 2 clusters to be considered in invariant mass. In ns.
  
  TLorentzVector fClusterMomentum;              //!<! Cluster momentum, temporary container
  TLorentzVector fClusterMomentum2;             //!<! Cluster momentum, inv mass loop, temporary container
  
  Float_t  fConstantTimeShift;                  ///<  Apply a 615 ns time shift in case of simulation, shift in ns.

  // Calorimeter Clusters
      
  // T-Card correlation
  
  TH2F *   fhEnergyTime1Cell[2];                            //!<! 1 cell cluster energy vs time
  TH2F *   fhEnergyTMEtaResidual1Cell;                      //!<! 1 cell cluster energy vs eta track-cluster residual
  TH2F *   fhEnergyTMPhiResidual1Cell;                      //!<! 1 cell cluster energy vs phi track-cluster residual
  TH2F *   fhColRowExoticHighE1CellPosTime;                 //!<! cluster col-row cluster cell max, E > 8 GeV, t > 5 ns 
  TH2F *   fhColRowExoticHighE1CellNegTime;                 //!<! cluster col-row cluster cell max, E > 8 GeV, t < -5 ns 
  TH2F *   fhColRowExoticHighE1CellNulTime;                 //!<! cluster col-row cluster cell max, E > 8 GeV, -5 < t < 5 ns 
  TH2F *   fhColRowExoticLowE1Cell [2];                     //!<! 1 cell cluster col-row cluster cell max, 5 < E < 8 GeV
  TH2F *   fhColRowExoticHighE1Cell[2];                     //!<! 1 cell cluster col-row cluster cell max, E > 8 GeV

  TH2F *   fhEnergyTimeExotic[2];                           //!<! cluster energy vs time, exo > 0.97, loose cuts
  TH2F *   fhEnergyTMEtaResidualExotic;                     //!<! cluster energy vs eta track-cluster residual, exo > 0.97, loose cuts
  TH2F *   fhEnergyTMPhiResidualExotic;                     //!<! cluster energy vs phi track-cluster residual, exo > 0.97, loose cuts
  TH2F *   fhColRowExoticHighEPosTime;                      //!<! cluster col-row cluster cell max, E > 8 GeV, t > 5 ns exo > 0.97
  TH2F *   fhColRowExoticHighENegTime;                      //!<! cluster col-row cluster cell max, E > 8 GeV, t < -5 ns exo > 0.97
  TH2F *   fhColRowExoticHighENulTime;                      //!<! cluster col-row cluster cell max, E > 8 GeV, -5 < t < 5 ns exo > 0.97
  TH2F *   fhColRowExoticLowE [2];                          //!<! col-row cluster cell max, 5 < E < 8 GeV, exo > 0.97, loose cuts
  TH2F *   fhColRowExoticHighE[2];                          //!<! col-row cluster cell max, E > 8 GeV, exo > 0.97, loose cuts  
  
  TH2F *   fhColRowExotic2ndCellDiffLowE [2];               //!<! secondary cell in diff TCard col vs row, 5 < E < 8 GeV, exo > 0.97, loose cuts
  TH2F *   fhColRowExotic2ndCellDiffHighE[2];               //!<! secondary cell in diff TCard col vs row, E > 8 GeV, exo > 0.97, loose cuts
  TH2F *   fhColRowExotic2ndCellSameLowE [2];               //!<! secondary cell in same TCard col vs row, 5 < E < 8 GeV, exo > 0.97, loose cuts
  TH2F *   fhColRowExotic2ndCellSameHighE[2];               //!<! secondary cell in same TCard col vs row, E > 8 GeV, exo > 0.97, loose cuts
  
  TH2F *   fhColRowHighEPosTime;                            //!<! cluster col-row cluster cell max, E > 8 GeV, t > 5 ns exo < 0.97, n cell > 1
  TH2F *   fhColRowHighENegTime;                            //!<! cluster col-row cluster cell max, E > 8 GeV, t < -5 ns exo < 0.97, n cell > 1
  TH2F *   fhColRowHighENulTime;                            //!<! cluster col-row cluster cell max, E > 8 GeV, -5 < t < 5 ns exo < 0.97, n cell > 1
  
  TH2F *   fhColRowTCardCorrNoSelectionExoticLowE [2];      //!<! col-row cluster cell max for those selected for TCard correlation studies, 5 < E < 8 GeV, exoticity > 0.97
  TH2F *   fhColRowTCardCorrNoSelectionExoticHighE[2];      //!<! col-row cluster cell max for those selected for TCard correlation studies, E > 8 GeV, exoticity > 0.97
  
  TH2F *   fhColRowTCardCorrNoSelectionExotic2ndCellDiffLowE [2];//!<! secondary cell in diff TCard col vs row, 5 < E < 8 GeV, exo > 0.97
  TH2F *   fhColRowTCardCorrNoSelectionExotic2ndCellDiffHighE[2];//!<! secondary cell in diff TCard col vs row, E > 8 GeV, exo > 0.97
  TH2F *   fhColRowTCardCorrNoSelectionExotic2ndCellSameLowE [2];//!<! secondary cell in same TCard col vs row, 5 < E < 8 GeV, exo > 0.97
  TH2F *   fhColRowTCardCorrNoSelectionExotic2ndCellSameHighE[2];//!<! secondary cell in same TCard col vs row, E > 8 GeV, exo > 0.97
  
  TH2F *   fhColRowTCardCorrNoSelectionExotic2ndCellDiffNoSameLowE [2];//!<! secondary cell in diff TCard col vs row, 5 < E < 8 GeV, exo > 0.97, 0 cells in same T-Card
  TH2F *   fhColRowTCardCorrNoSelectionExotic2ndCellDiffNoSameHighE[2];//!<! secondary cell in diff TCard col vs row, E > 8 GeV, exo > 0.97, 0 cells in same T-Card
  TH2F *   fhColRowTCardCorrNoSelectionExotic2ndCellSameNoDiffLowE [2];//!<! secondary cell in same TCard col vs row, 5 < E < 8 GeV, exo > 0.97, 0 cells in diff T-Card
  TH2F *   fhColRowTCardCorrNoSelectionExotic2ndCellSameNoDiffHighE[2];//!<! secondary cell in same TCard col vs row, E > 8 GeV, exo > 0.97, 0 cells in diff T-Card
  
  TH2F *   fhColRowTCardCorrNoSelectionLowE[2];             //!<! col-row cluster cell max for those selected for TCard correlation studies, 5 < E < 8 GeV
  TH2F *   fhColRowTCardCorrNoSelectionHighE[2];            //!<! col-row cluster cell max for those selected for TCard correlation studies, E > 8 GeV
 
  TH2F *   fhEnergyTimeTCardCorrNoSelection1Cell[2];        //!<! 1 cell cluster energy vs time, T-Card strict cuts
  TH2F *   fhEnergyTMEtaResidualTCardCorrNoSelection1Cell;  //!<! 1 cell cluster energy vs eta track-cluster residual, T-Card strict cuts
  TH2F *   fhEnergyTMPhiResidualTCardCorrNoSelection1Cell;  //!<! 1 cell cluster energy vs phi track-cluster residual, T-Card strict cuts
  TH2F *   fhEnergyTimeTCardCorrNoSelectionExotic[2];       //!<! cluster energy vs time, exo > 0.97, T-Card strict cuts
  TH2F *   fhEnergyTMEtaResidualTCardCorrNoSelectionExotic; //!<! cluster energy vs eta track-cluster residual, exo > 0.97, T-Card strict cuts 
  TH2F *   fhEnergyTMPhiResidualTCardCorrNoSelectionExotic; //!<! cluster energy vs phi track-cluster residual, exo > 0.97, T-Card strict cuts
  
  TH2F *   fhTimeTCardCorrNoSelection[2];                   //!<! Cluster time vs E for clusters selected for TCard correlation studies
  TH2F *   fhLambda0TCardCorrNoSelection[2];                //!<! Cluster m02 vs E for clusters selected for TCard correlation studies
  TH2F *   fhLambda1TCardCorrNoSelection[2];                //!<! Cluster m20 vs E for clusters selected for TCard correlation studies
  TH2F *   fhLambda0NLM1TCardCorrNoSelection[2];            //!<! Cluster m02 vs E for clusters selected for TCard correlation studies, nlm=1
  TH2F *   fhLambda1NLM1TCardCorrNoSelection[2];            //!<! Cluster m20 vs E for clusters selected for TCard correlation studies, nlm=1
  TH2F *   fhLambda0NLM2TCardCorrNoSelection[2];            //!<! Cluster m02 vs E for clusters selected for TCard correlation studies, nlm=2
  TH2F *   fhLambda1NLM2TCardCorrNoSelection[2];            //!<! Cluster m20 vs E for clusters selected for TCard correlation studies, nlm=2
  TH2F *   fhLambdaRTCardCorrNoSelection[2];                //!<! Cluster m20/m02 vs E for clusters selected for TCard correlation studies
  TH2F *   fhNLocMaxTCardCorrNoSelection[2];                //!<! Cluster Number of local Maxima vs E for clusters selected for TCard correlation studies
 
  TH2F *   fhEMaxRatNLM1TCardCorrNoSelection[2];            //!<! Cluster E cell max / E cluster for NLM=1 vs E for clusters selected for TCard correlation studies
  TH2F *   fhEMaxRatNLM2TCardCorrNoSelection[2];            //!<! Cluster E cell max / E cluster for NLM=2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhEMaxRatNLM3TCardCorrNoSelection[2];            //!<! Cluster E cell max / E cluster for NLM>2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhE2ndRatNLM1TCardCorrNoSelection[2];            //!<! Cluster E cell second max / E cluster for NLM=1 vs E for clusters selected for TCard correlation studies
  TH2F *   fhE2ndRatNLM2TCardCorrNoSelection[2];            //!<! Cluster E cell second loc max / E cluster for NLM=2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhE2ndRatNLM3TCardCorrNoSelection[2];            //!<! Cluster E cell second loc max / E cluster for NLM>2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhE2ndEMaxRatNLM1TCardCorrNoSelection[2];        //!<! Cluster E cell second loc max / E Max for NLM=1 vs E for clusters selected for TCard correlation studies
  TH2F *   fhE2ndEMaxRatNLM2TCardCorrNoSelection[2];        //!<! Cluster E cell second loc max / E Max for NLM=2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhE2ndEMaxRatNLM3TCardCorrNoSelection[2];        //!<! Cluster E cell second loc max / E Max for NLM>2 vs E for clusters selected for TCard correlation studies

  TH2F *   fhE2ndSameRatNLM1TCardCorrNoSelection[2];        //!<! Cluster E cell second max / E cluster for NLM=1 vs E for clusters selected for TCard correlation studies
  TH2F *   fhE2ndSameRatNLM2TCardCorrNoSelection[2];        //!<! Cluster E cell second loc max / E cluster for NLM=2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhE2ndSameRatNLM3TCardCorrNoSelection[2];        //!<! Cluster E cell second loc max / E cluster for NLM>2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhE2ndSameEMaxRatNLM1TCardCorrNoSelection[2];    //!<! Cluster E cell second loc max / E Max for NLM=1 vs E for clusters selected for TCard correlation studies
  TH2F *   fhE2ndSameEMaxRatNLM2TCardCorrNoSelection[2];    //!<! Cluster E cell second loc max / E Max for NLM=2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhE2ndSameEMaxRatNLM3TCardCorrNoSelection[2];    //!<! Cluster E cell second loc max / E Max for NLM>2 vs E for clusters selected for TCard correlation studies

  TH2F *   fhECellClusRatNLM1TCardCorrNoSelection[2];       //!<! Cluster E cell / E cluster for NLM=1 vs E for clusters selected for TCard correlation studies
  TH2F *   fhECellClusRatNLM2TCardCorrNoSelection[2];       //!<! Cluster E cell / E cluster for NLM=2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhECellClusRatNLM3TCardCorrNoSelection[2];       //!<! Cluster E cell / E cluster for NLM>2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhECellWeightNLM1TCardCorrNoSelection[2];        //!<! Cluster E cell weight for NLM=1 vs E for clusters selected for TCard correlation studies
  TH2F *   fhECellWeightNLM2TCardCorrNoSelection[2];        //!<! Cluster E cell weight for NLM=2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhECellWeightNLM3TCardCorrNoSelection[2];        //!<! Cluster E cell weight for NLM>2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhLogECellNLM1TCardCorrNoSelection[2];           //!<! Cluster Log E cell for NLM=1 vs E for clusters selected for TCard correlation studies
  TH2F *   fhLogECellNLM2TCardCorrNoSelection[2];           //!<! Cluster Log E cell for NLM=2 vs E for clusters selected for TCard correlation studies
  TH2F *   fhLogECellNLM3TCardCorrNoSelection[2];           //!<! Cluster Log E cell for NLM>2 vs E for clusters selected for TCard correlation studies

  TH2F *   fhECellSameClusRatNLM1TCardCorrNoSelection[2];   //!<! Cluster E cell / E cluster for NLM=1 vs E for clusters selected for TCard correlation studies, same Tcard as leading
  TH2F *   fhECellSameClusRatNLM2TCardCorrNoSelection[2];   //!<! Cluster E cell / E cluster for NLM=2 vs E for clusters selected for TCard correlation studies, same Tcard as leading
  TH2F *   fhECellSameClusRatNLM3TCardCorrNoSelection[2];   //!<! Cluster E cell / E cluster for NLM>2 vs E for clusters selected for TCard correlation studies, same Tcard as leading
  TH2F *   fhECellSameWeightNLM1TCardCorrNoSelection[2];    //!<! Cluster E cell weight for NLM=1 vs E for clusters selected for TCard correlation studies, same Tcard as leading
  TH2F *   fhECellSameWeightNLM2TCardCorrNoSelection[2];    //!<! Cluster E cell weight for NLM=2 vs E for clusters selected for TCard correlation studies, same Tcard as leading
  TH2F *   fhECellSameWeightNLM3TCardCorrNoSelection[2];    //!<! Cluster E cell weight for NLM>2 vs E for clusters selected for TCard correlation studies, same Tcard as leading
  TH2F *   fhLogECellSameNLM1TCardCorrNoSelection[2];       //!<! Cluster Log E cell for NLM=1 vs E for clusters selected for TCard correlation studies, same Tcard as leading
  TH2F *   fhLogECellSameNLM2TCardCorrNoSelection[2];       //!<! Cluster Log E cell for NLM=2 vs E for clusters selected for TCard correlation studies, same Tcard as leading
  TH2F *   fhLogECellSameNLM3TCardCorrNoSelection[2];       //!<! Cluster Log E cell for NLM>2 vs E for clusters selected for TCard correlation studies, same Tcard as leading

  
  TH2F *   fhNCellsTCardCorrNoSelection[2];                 //!<! Ncells per cluster vs cluster energy, clusters selected for TCard correlation studies
  TH2F *   fhNCellsTCardCorrWithWeightNoSelection[2];       //!<! Ncells per cluster vs cluster energy, select cells with w>0.01, clusters selected for TCard correlation studies
  TH2F *   fhNCellsTCardCorrRatioWithWeightNoSelection[2];  //!<! Ncells per cluster/Ncells per cluster with w>0.01 vs cl. energy, clusters selected for TCard correlation studies
  TH2F *   fhExoticTCardCorrNoSelection[2];                 //!<! exoticity per cluster vs cluster energy, clusters selected for TCard correlation studies

//  TH2F *   fhLambda0TCardCorrelN[8][2];                     //!<! Cluster m02 vs E, max cell correlated with 0 to >6 cells in TCard                
//  TH2F *   fhNCellsTCardCorrelN [8][2];                     //!<! Cluster Ncells vs E, select cells with w > 0.01, max cell correlated with 0 to >6 cells in TCard
//  TH2F *   fhExoticTCardCorrelN [8][2];                     //!<! Cluster exoticity vs E, select cells with w > 0.01, max cell correlated with 0 to >6 cells in TCard
//  TH2F *   fhColRowTCardCorrelNLowE [8][2];                 //!<! Cluster max cell col vs row, E > 2 GeV select cells with w > 0.01, max cell correlated with 0 to >6 cells in TCard
//  TH2F *   fhColRowTCardCorrelNHighE[8][2];                 //!<! Cluster max cell col vs row, E > 8 GeV select cells with w > 0.01, max cell correlated with 0 to >6 cells in TCard
//  
//  TH2F *   fhLambda0TCardCorrelNAllSameTCard[8][2];         //!<! Cluster m02 vs E, max cell correlated with 0 to >6 cells in TCard                
//  TH2F *   fhNCellsTCardCorrelNAllSameTCard [8][2];         //!<! Cluster Ncells vs E, select cells with w > 0.01, max cell correlated with 0 to >6 cells in TCard
//  TH2F *   fhExoticTCardCorrelNAllSameTCard [8][2];         //!<! Cluster exoticity vs E, select cells with w > 0.01, max cell correlated with 0 to >6 cells in TCard
//  TH2F *   fhColRowTCardCorrelNAllSameTCardLowE [8][2];     //!<! Cluster max cell col vs row, E > 2 GeV select cells with w > 0.01, max cell correlated with 0 to >6 cells in TCard
//  TH2F *   fhColRowTCardCorrelNAllSameTCardHighE[8][2];     //!<! Cluster max cell col vs row, E > 8 GeV select cells with w > 0.01, max cell correlated with 0 to >6 cells in TCard
//  
//  TH2F *   fhLambda0TCardCorrelNExotic[8][2];               //!<! Cluster m02 vs E, max cell correlated with 0 to >6 cells in TCard, exoticity > 0.97                
//  TH2F *   fhNCellsTCardCorrelNExotic [8][2];               //!<! Cluster Ncells vs E, select cells with w > 0.01, max cell correlated with >6 cells in TCard, exoticity > 0.97
//  TH2F *   fhColRowTCardCorrelNLowEExotic [8][2];           //!<! Cluster max cell col vs row, E > 2 GeV select cells with w > 0.01, max cell correlated with 0 to >6 cells in TCard
//  TH2F *   fhColRowTCardCorrelNHighEExotic[8][2];           //!<! Cluster max cell col vs row, E > 8 GeV select cells with w > 0.01, max cell correlated with 0 to >6 cells in TCard
//
//  TH2F *   fhLambda0TCardCorrelNAllSameTCardExotic[8][2];   //!<! Cluster m02 vs E, max cell correlated with 0 to >6 cells in TCard, exoticity > 0.97                
//  TH2F *   fhNCellsTCardCorrelNAllSameTCardExotic [8][2];   //!<! Cluster Ncells vs E, select cells with w > 0.01, max cell correlated with 0 to >6 cells in TCard, exoticity > 0.97
// 
//  
//  TH2F *   fhLambda0TCardCorrel[7][2];                      //!<! Cluster m02 vs E, max cell correlated with different combinations of cells in TCard               
//  TH2F *   fhNCellsTCardCorrel [7][2];                      //!<! Cluster Ncells vs E, select cells with w > 0.01, max cell correlated with different combinations of cells in TCard, exoticity > 0.97  
//  TH2F *   fhExoticTCardCorrel [7][2];                      //!<! Cluster exoticity vs E, select cells with w > 0.01, max cell correlated with different combinations of cells in TCard, exoticity > 0.97  
//
//  TH2F *   fhLambda0TCardCorrelExotic[4][2];                //!<! Cluster m02 vs E, max cell correlated with different combinations of cells in TCard, exoticity > 0.97                
//  TH2F *   fhNCellsTCardCorrelExotic [4][2];                //!<! Cluster Ncells vs E, select cells with w > 0.01, max cell correlated with different combinations of cells in TCard, exoticity > 0.97 

  TH2F *   fhLambda0Exoticity[14][2];                       //!<! Cluster m02 vs exoticy, for different cluster energy bins               
  TH2F *   fhLambda1Exoticity[14][2];                       //!<! Cluster m02 vs exoticy, for different cluster energy bins               
  TH2F *   fhLambdaRExoticity[14][2];                       //!<! Cluster m02 vs exoticy, for different cluster energy bins               
  TH2F *   fhNCellsExoticity [14][2];                       //!<! Cluster NCells vs exoticy, for different cluster energy bins
  TH2F *   fhTimeExoticity   [14][2];                       //!<! Cluster time vs exoticy, for different cluster energy bins
  TH2F *   fhLambda0Lambda1  [14][2];                       //!<! Cluster m02 vs m20,for different cluster energy bins               

//  TH2F *   fhLambda0ExoticityAllSameTCard[14][2];           //!<! Cluster m02 vs exoticy, for different cluster energy bins, all cells in same TCard as leading               
//  TH2F *   fhLambda1ExoticityAllSameTCard[14][2];           //!<! Cluster m02 vs exoticy, for different cluster energy bins, all cells in same TCard as leading               
//  TH2F *   fhLambdaRExoticityAllSameTCard[14][2];           //!<! Cluster m02 vs exoticy, for different cluster energy bins, all cells in same TCard as leading               
//  TH2F *   fhNCellsExoticityAllSameTCard [14][2];           //!<! Cluster NCells vs exoticy, for different cluster energy bins, all cells in same TCard as leading 
//  TH2F *   fhLambda0Lambda1AllSameTCard  [14][2];           //!<! Cluster m02 vs m20,for different cluster energy bins, all cells in same TCard as leading               
//  
  TH2F *   fhNCellsTCardSameAndDiff      [14][2];           //!<! Cluster NCells in same TCard as leading vs NCells on different TCard               
  TH2F *   fhNCellsTCardSameAndDiffExotic[14][2];           //!<! Cluster NCells in same TCard as leading vs NCells on different TCard, exoticity > 0.97              

  TH2F *   fhTMPhiResidualExoticity[14];                    //!<! Cluster-track matching residual in phi vs exoticity              
  TH2F *   fhTMEtaResidualExoticity[14];                    //!<! Cluster-track matching residual in phi vs exoticity             
  TH2F *   fhTMPhiResidualExoticityLooseCut[14];            //!<! Cluster-track matching residual in phi vs exoticity, loose acceptance cut              
  TH2F *   fhTMEtaResidualExoticityLooseCut[14];            //!<! Cluster-track matching residual in phi vs exoticity, loose acceptance cut             
//  TH2F *   fhTMPhiResidualExoticityAllSameTCard[14];        //!<! Cluster-track matching residual in phi vs exoticity, all cells in same TCard as leading                
//  TH2F *   fhTMEtaResidualExoticityAllSameTCard[14];        //!<! Cluster-track matching residual in phi vs exoticity, all cells in same TCard as leading                

  TH2F *   fhLambda0TCardCorrelNCell[6][6][2];              //!<! Cluster m02 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhLambda1TCardCorrelNCell[6][6][2];              //!<! Cluster m20 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard          
  TH2F *   fhLambda0NLM1TCardCorrelNCell[6][6][2];          //!<! Cluster m02 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard, nlm=1            
  TH2F *   fhLambda1NLM1TCardCorrelNCell[6][6][2];          //!<! Cluster m20 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard, nlm=1           
  TH2F *   fhLambda0NLM2TCardCorrelNCell[6][6][2];          //!<! Cluster m02 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard, nlm=2            
  TH2F *   fhLambda1NLM2TCardCorrelNCell[6][6][2];          //!<! Cluster m20 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard, nlm=2            
//TH2F *   fhLambdaRTCardCorrelNCell[6][6][2];              //!<! Cluster m20/m02 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhNLocMaxTCardCorrelNCell[6][6][2];              //!<! Cluster nlocmax vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  
  TH2F *   fhEMaxRatNLM1TCardCorrelNCell[6][6][2];          //!<! Cluster E cell max / E cluster for NLM=1 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhEMaxRatNLM2TCardCorrelNCell[6][6][2];          //!<! Cluster E cell max / E cluster for NLM=2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhEMaxRatNLM3TCardCorrelNCell[6][6][2];          //!<! Cluster E cell max / E cluster for NLM>2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhE2ndRatNLM1TCardCorrelNCell[6][6][2];          //!<! Cluster E cell second max / E cluster for NLM=1 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhE2ndRatNLM2TCardCorrelNCell[6][6][2];          //!<! Cluster E cell second loc max / E cluster for NLM=2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhE2ndRatNLM3TCardCorrelNCell[6][6][2];          //!<! Cluster E cell second loc max / E cluster for NLM>2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard    
  TH2F *   fhE2ndEMaxRatNLM1TCardCorrelNCell[6][6][2];      //!<! Cluster E cell second max / E cell max for NLM=1 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhE2ndEMaxRatNLM2TCardCorrelNCell[6][6][2];      //!<! Cluster E cell second loc max / E cell max for NLM=2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhE2ndEMaxRatNLM3TCardCorrelNCell[6][6][2];      //!<! Cluster E cell second loc max / E cell max for NLM>2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            

  TH2F *   fhECellClusRatNLM1TCardCorrelNCell[6][6][2];     //!<! Cluster E cell / E cluster vs E for NLM=1, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard    
  TH2F *   fhECellClusRatNLM2TCardCorrelNCell[6][6][2];     //!<! Cluster E cell / E cluster for NLM=2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhECellClusRatNLM3TCardCorrelNCell[6][6][2];     //!<! Cluster E cell / E cluster for NLM>2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard      
  TH2F *   fhLogECellNLM1TCardCorrelNCell    [6][6][2];     //!<! Cluster log E cell vs E cluster for NLM=1, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard    
  TH2F *   fhLogECellNLM2TCardCorrelNCell    [6][6][2];     //!<! Cluster log E cell vs E cluster for NLM=2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhLogECellNLM3TCardCorrelNCell    [6][6][2];     //!<! Cluster log E cell vs E cluster for NLM>2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard           
  TH2F *   fhECellWeightNLM1TCardCorrelNCell [6][6][2];     //!<! Cluster E cell weight for NLM=1 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard    
  TH2F *   fhECellWeightNLM2TCardCorrelNCell [6][6][2];     //!<! Cluster E cell weight for NLM=2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard            
  TH2F *   fhECellWeightNLM3TCardCorrelNCell [6][6][2];     //!<! Cluster E cell weight for NLM>2 vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard           
  
  TH2F *   fhMassEClusTCardCorrelNCell[6][6][2];            //!<! Cluster invariant mass vs E cluster, one of clusters  0.1<m02<0.4 contains 0 to more than 6 cells with w > 0.01, in same TCard or diff TCard            
//TH2F *   fhMassEPairTCardCorrelNCell[6][6][2];            //!<! Cluster invariant mass vs E pair, one of clusters  0.1<m02<0.4 contains 0 to more than 6 cells with w > 0.01, in same TCard or diff TCard            
  TH2F *   fhExoticTCardCorrelNCell   [6][6][2];            //!<! Cluster exoticity vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard  
  TH2F *   fhTimeDiffTCardCorrelNCell   [6][6][2];          //!<! Cluster time-secondary cell time vs E, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard  
  TH2F *   fhTimeDiffExoTCardCorrelNCell[6][6][2];          //!<! Cluster time-secondary cell time vs E, for exotic luster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard  
  TH2F *   fhColRowTCardCorrelNCellLowE [6][6][2];          //!<! Cluster max cell col vs row, E > 2 GeV, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard
  TH2F *   fhColRowTCardCorrelNCellHighE[6][6][2];          //!<! Cluster max cell col vs row, E > 8 GeV, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard  
  TH2F *   fhColRowTCardCorrelNCellExoticLowE [6][6][2];    //!<! Cluster max cell col vs row, E > 2 GeV, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard, exoticity > 0.97
  TH2F *   fhColRowTCardCorrelNCellExoticHighE[6][6][2];    //!<! Cluster max cell col vs row, E > 8 GeV, cluster contains 0 to more than 6 cells with w > 0.01 in same TCard or diff TCard, exoticity < 0.97

  TH2F *   fhLambda0ExoticityPerNCell[6][6][2];             //!<! Cluster m02 vs exoticy,for E > 8 and n cell bins with w>0.01, in same TCard or diff TCard              
  TH2F *   fhLambda1ExoticityPerNCell[6][6][2];             //!<! Cluster m20 vs exoticy,for E > 8 and n cell bins with w>0.01, in same TCard or diff TCard              
//TH2F *   fhLambdaRExoticityPerNCell[6][6][2];             //!<! Cluster m20/m02 vs exoticy,for E > 8 and n cell bins with w>0.01, in same TCard or diff TCard              

  TH2F *   fhNCellsTCardSameAndDiffFraction      [2];       //!<! Cluster fraction of NCells in same TCard as leading vs energy               
  TH2F *   fhNCellsTCardSameAndDiffFractionExotic[2];       //!<! Cluster fraction of NCells in same TCard as leading vs energy, exoticity > 0.97      
  
//TH2F *   fhLambda0TCardCorrelNearRow[6][2];               //!<! Cluster m02 vs E, max cell correlated with different combinations of cells in TCard, one correl. cell is 1 row away               
//TH2F *   fhNCellsTCardCorrelNearRow [6][2];               //!<! Cluster Ncells vs E, select cells with w > 0.01, max cell correlated with different combinations of cells in TCard, one correl. cell is 1 row away 
//  
//TH2F *   fhLambda0TCardCorrel2ndMax[4][2];                //!<! Cluster m02 vs E, max cell correlated with different combinations of cells in TCard, 2nd max also in TCard               
//TH2F *   fhNCellsTCardCorrel2ndMax [4][2];                //!<! Cluster Ncells vs E, select cells with w > 0.01, max cell correlated with different combinations of cells in TCard, 2nd Max in TCard
//
//  TH2F *   fhLambda0TCardCorrelOtherTCard[7][2];            //!<! Cluster m02 vs E, max cell correlated with different combinations of cells in TCard               
//  TH2F *   fhNCellsTCardCorrelOtherTCard [7][2];            //!<! Cluster Ncells vs E, select cells with w > 0.01, max cell correlated with different combinations of cells in TCard
//  TH2F *   fhExoticTCardCorrelOtherTCard [7][2];            //!<! Cluster exoticity vs E, select cells with w > 0.01, max cell correlated with different combinations of cells in TCard
//  TH2F *   fhColRowTCardCorrelOtherTCardLowE [7][2];        //!<! Cluster max cell col vs row, E > 2 GeV select cells with w > 0.01, max cell correlated with different combinations of cells in TCard
//  TH2F *   fhColRowTCardCorrelOtherTCardHighE[7][2];        //!<! Cluster max cell col vs row, E > 8 GeV select cells with w > 0.01, max cell correlated with different combinations of cells in TCard

  TH2F *   fhTCardCorrECellMaxDiff[12][2];                  //!<! Cell max energy - secondary cell energy in cluster vs cluster energy, different secondary cell selections depending on TCard
  TH2F *   fhTCardCorrEClusterDiff[12][2];                  //!<! Cluster energy - secondary cell energy in cluster vs cluster energy, different secondary cell selections depending on TCard
//TH2F *   fhTCardCorrECellMaxRat [12][2];                  //!<! Secondary cell energy in cluster / cell max energy vs cluster energy, different secondary cell selections depending on TCard
//TH2F *   fhTCardCorrEClusterRat [12][2];                  //!<! Secondary cell energy in cluster / cluster energy - vs cluster energy, different secondary cell selections depending on TCard
  TH2F *   fhTCardCorrTCellMaxDiff[12][2];                  //!<! Cell max energy - secondary cell time in cell vs cluster energy, different secondary cell selections depending on TCard

  TH2F *   fhTCardCorrECellMaxDiffExo[12][2];               //!<! Cell max energy - secondary cell energy in cluster vs cluster energy, different secondary cell selections depending on TCard
  TH2F *   fhTCardCorrEClusterDiffExo[12][2];               //!<! Cluster energy - secondary cell energy in cluster vs cluster energy, different secondary cell selections depending on TCard
//TH2F *   fhTCardCorrECellMaxRatExo [12][2];               //!<! Secondary cell energy in cluster / cell max energy vs cluster energy, different secondary cell selections depending on TCard
//TH2F *   fhTCardCorrEClusterRatExo [12][2];               //!<! Secondary cell energy in cluster / cluster energy - vs cluster energy, different secondary cell selections depending on TCard
  TH2F *   fhTCardCorrTCellMaxDiffExo[12][2];               //!<! Cell max energy - secondary cell time in cell vs cluster energy, different secondary cell selections depending on TCard
  
  TH2F *   fhSameRowDiffColAndTCardCellsEnergyDiffClusterE   [2]; //!<! Secondary cell energy difference vs cluster energy, one in same TCard as cell max, the other not, both in same row and 1 column
  TH2F *   fhSameRowDiffColAndTCardCellsTimeDiffClusterE     [2]; //!<! Secondary cell energy difference vs cluster energy, one in same TCard as cell max, the other not, both in same row and 1 column
  TH2F *   fhSameRowDiffColAndTCardCellsEnergyDiffCellMaxE   [2]; //!<! Secondary cell energy difference vs leading cell energy, one in same TCard as cell max, the other not, both in same row and 1 column
  TH2F *   fhSameRowDiffColAndTCardCellsTimeDiffCellMaxE     [2]; //!<! Secondary cell energy difference vs leading cell energy, one in same TCard as cell max, the other not, both in same row and 1 column
  TH2F *   fhSameRowDiffColAndTCardCellsEnergyDiffClusterEExo[2]; //!<! Secondary cell energy difference vs cluster energy, one in same TCard as cell max, the other not, both in same row and 1 column, exo > 0.97
  TH2F *   fhSameRowDiffColAndTCardCellsTimeDiffClusterEExo  [2]; //!<! Secondary cell energy difference vs cluster energy, one in same TCard as cell max, the other not, both in same row and 1 column, exo > 0.97
  TH2F *   fhSameRowDiffColAndTCardCellsEnergyDiffCellMaxEExo[2]; //!<! Secondary cell energy difference vs leading cell energy, one in same TCard as cell max, the other not, both in same row and 1 column, exo > 0.97
  TH2F *   fhSameRowDiffColAndTCardCellsTimeDiffCellMaxEExo  [2]; //!<! Secondary cell energy difference vs leading cell energy, one in same TCard as cell max, the other not, both in same row and 1 column, exo > 0.97
  
  // Cluster cell size
    
  TH2F *   fhDeltaIEtaDeltaIPhiE0[2];           //!<! Difference between max cell index and farthest cell, eta vs phi, E < 2 GeV, with and without matching;
  TH2F *   fhDeltaIEtaDeltaIPhiE2[2];           //!<! Difference between max cell index and farthest cell, eta vs phi, 2 < E < 6 GeV, with and without matching;
  TH2F *   fhDeltaIEtaDeltaIPhiE6[2];           //!<! Difference between max cell index and farthest cell, eta vs phi, E > 6 GeV, with and without matching;
  TH2F *   fhDeltaIA[2];                        //!<! Cluster "asymmetry" in cell terms vs E, with and without matching
  TH2F *   fhDeltaIAL0[2];                      //!<! Cluster "asymmetry" in cell units vs Lambda0    for E > 0.5 GeV, n cells in cluster > 3, with and without matching
  TH2F *   fhDeltaIAL1[2];                      //!<! Cluster "asymmetry" in cell units vs Lambda1    for E > 0.5 GeV, n cells in cluster > 3, with and without matching
  TH2F *   fhDeltaIANCells[2] ;                 //!<! Cluster "asymmetry" in cell units vs number of cells in cluster for E > 0.5, with and without matching
  TH2F *   fhDeltaIAMC[4];                      //!<! Cluster "asymmetry" in cell terms vs E, from MC photon, electron, conversion or hadron.
  TH2F *   fhBadClusterDeltaIEtaDeltaIPhiE0;    //!<! Difference between max cell index and farthest cell, eta vs phi, E < 2 GeV, with and without matching; bad clusters.
  TH2F *   fhBadClusterDeltaIEtaDeltaIPhiE2;    //!<! Difference between max cell index and farthest cell, eta vs phi, 2 < E < 6 GeV, with and without matching; bad clusters.
  TH2F *   fhBadClusterDeltaIEtaDeltaIPhiE6;    //!<! Difference between max cell index and farthest cell, eta vs phi, E > 6 GeV, with and without matching; bad clusters.
  TH2F *   fhBadClusterDeltaIA;                 //!<! Cluster "asymmetry" in cell terms vs E, with and without matching; bad clusters.
  	
  // Weight studies
  
  TH2F *   fhECellClusterRatio;                 //!<! e cell / e cluster vs e cluster
  TH2F *   fhECellClusterLogRatio;              //!<! log (e cell / e cluster)  vs e cluster
  TH2F *   fhEMaxCellClusterRatio;              //!<! e max cell / e cluster vs e cluster
  TH2F *   fhEMaxCellClusterLogRatio;           //!<! log (e max cell / e cluster) vs e cluster
  
  TH2F *   fhLambda0ForW0AndCellCuts    [12][4][3]; //!<! L0 for different w0 and cell cuts
  TH2F *   fhLambda0ForW0AndCellCutsEta0[12][4][3]; //!<! L0 for different w0 and cell cuts, |eta| < 0.15
//TH2F *   fhLambda1ForW0AndCellCuts    [12][4][3]; //!<! L1 for different w0 and cell cuts

  TH2F *   fhLambda0ForW0MC[12][5];             //!<! L0 for different w0, depending on the particle of origin
//TH2F *   fhLambda1ForW0MC[12][5];             //!<! L1 for different w0, depending on the particle of origin
  
  TH2F *   fhECellTotalRatio;                   //!<! e cell / e total vs e total
  TH2F *   fhECellTotalLogRatio;                //!<! log (e cell / e total)  vs e total
  TH2F **  fhECellTotalRatioMod;                //!<! e cell / e total vs e total, per SM
  TH2F **  fhECellTotalLogRatioMod;             //!<! log (e cell / e total)  vs e total, per SM

  TH3F *   fhClusterTimeEnergyM02;                 //!<! Cluster Time vs Energy
//TH3F *   fhCellTimeSpreadRespectToCellMaxM02;    //!<! Difference of the time of cell with maximum dep energy and the rest of cells
  TH3F *   fhClusterMaxCellCloseCellRatioM02;      //!<! Ratio between max cell energy and cell energy of the same cluster
//TH3F *   fhClusterMaxCellCloseCellDiffM02;       //!<! Difference between max cell energy and cell energy of the same cluster
  TH3F *   fhClusterMaxCellDiffM02;                //!<! Difference between cluster energy and energy of cell with more energy, good clusters onl
  TH3F *   fhClusterMaxCellECrossM02;              //!<! 1 - Energy in cross around max energy cell / max energy cell vs cluster energy, good clusters
  TH3F *   fhNCellsPerClusterM02;                  //!<! N cells per cluster vs cluster energy vs eta of cluster
  
  /// Copy constructor not implemented.
  AliAnaClusterShapeCorrelStudies & operator = (const AliAnaClusterShapeCorrelStudies & qa) ;
    
  /// Assignment operator not implemented.
  AliAnaClusterShapeCorrelStudies(              const AliAnaClusterShapeCorrelStudies & qa) ;
  
  /// \cond CLASSIMP
  ClassDef(AliAnaClusterShapeCorrelStudies,1) ;
  /// \endcond

} ;

#endif //ALIANACLUSTERSHAPECORRELSTUDIES_H



