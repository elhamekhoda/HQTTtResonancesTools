#ifndef TTRESEVENTSAVERFLATNTUPLE_H
#define TTRESEVENTSAVERFLATNTUPLE_H

#include "TopAnalysis/EventSaverFlatNtuple.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "TopEvent/Event.h"
#include "AsgTools/AsgTool.h"
// #include "AsgTools/ToolHandleArray.h"
#include "JetCPInterfaces/ICPJetUncertaintiesTool.h"
#include "TopEventReconstructionTools/TtresNeutrinoBuilder.h"
#include "TopPartons/PartonHistory.h"
#include "TtResonancesTools/Chi2Selector.h"
#include "xAODBTaggingEfficiency/BTaggingEfficiencyTool.h"
#include "xAODBTaggingEfficiency/BTaggingSelectionTool.h"
#include "FTagAnalysisInterfaces/IBTaggingSelectionTool.h"
#include "FTagAnalysisInterfaces/IBTaggingEfficiencyTool.h"
#include <set>

// HEPTopTagger headers
#include "JetRec/JetRecTool.h"
#include "BoostedJetTaggers/HEPTopTagger.h"

#include "JetAnalysisInterfaces/IJetSelectorTool.h"
#include "BoostedJetTaggers/SmoothedTopTagger.h"
#include "BoostedJetTaggers/JSSWTopTaggerBDT.h"
#include "BoostedJetTaggers/JSSWTopTaggerDNN.h"
#include "BoostedJetTaggers/TopoclusterTopTagger.h"

// EW corrections
#include "TtResonancesTools/WeakCorrScaleFactorParam.h"

// Sherpa W-filter
#include "HQTTtResonancesTools/WFilter.h"
#include "HQTTtResonancesTools/AngularCutsSL.h"
using namespace std;

// SSM Z' -> HVT Z' Signal Reweighting
#ifdef ENABLE_ZPRIMERWGT
#include "ZprimeRWGT/ZprimeRWGT.h"
#endif

namespace top {

typedef std::map<TString, vector<int>> TMapTSiarr;

struct Tagger {
    std::string TaggerDecorationName;
    std::string TaggerBranchName;
    std::vector<int> TaggerAccept;
    std::string TaggerScoreName;
    std::string TaggerScoreBranchName;
    std::vector<double> TaggerScore;
    std::string TaggerSFName = "";
    std::string TaggerSFBranchName = "";
    std::vector<double> TaggerSF = {};
    std::vector<CP::SystematicSet> SystematicSets1Up = {};
    std::vector<CP::SystematicSet> SystematicSets1Down = {};
    std::string TaggerSFVarsBranchName1Up = "";
    std::string TaggerSFVarsBranchName1Down = "";
    std::vector<std::vector<double>> TaggerSFVars1Up = {};
    std::vector<std::vector<double>> TaggerSFVars1Down = {};
};

class TtresEventSaverFlatNtuple : public top::EventSaverFlatNtuple {
  public:

    TtresEventSaverFlatNtuple();

    virtual ~TtresEventSaverFlatNtuple();

    virtual void initialize(std::shared_ptr<top::TopConfig> config, TFile* file, const std::vector<std::string>& extraBranches);
    virtual StatusCode initialize() {return StatusCode::SUCCESS;};
    virtual void finalize();

    void saveEvent(const top::Event& event);
    void saveParticleLevelEvent(const top::ParticleLevelEvent& event);

    void DeltaR_min(TLorentzVector p1, TLorentzVector p2, int i, float & tmp_dr, int & truth_idx);

    void MA_hadParton_truthjet(TLorentzVector p1, int & truth_idx, float & truth_dr, float criteria, bool useFatJet = false);

    void MA_truthjet_recojet(const top::Event& event, const xAOD::Jet* trueJet, float & reco_dr, int & reco_idx, float & pt, float & eta, float & phi, float & mass, float criteriav, bool useFatJet = false);

    void MA_eParton_reco(const top::Event& event, TLorentzVector parton_e, int & reco_idx, float & reco_dr, float criteria);

    void MA_muParton_reco(const top::Event& event, TLorentzVector parton_mu, int & reco_idx, float & reco_dr, float criteria);

    TLorentzVector * MA_nuParton_met(TLorentzVector * met, TLorentzVector parton_Nu,  TLorentzVector reco_chLep, float & dphi);

    int AnalyzeRecoIndex_forTop(TLorentzVector & t_p4, TLorentzVector w_p4, TLorentzVector b_p4, TLorentzVector Wdecay1_p4, TLorentzVector Wdecay2_p4, const int reco_b_idx, const int reco_Wdecay1_idx, const int reco_Wdecay2_idx);

    int AnalyzeRecoIndex_forW(TLorentzVector & w_p4, TLorentzVector Wdecay1_p4, const int reco_Wdecay1_idx, TLorentzVector Wdecay2_p4, const int reco_Wdecay2_idx);

    void WbosonDecayMode(int Wdecay1_pdgId, int Wdecay2_pdgId, std::string & W_DecayMode, int & t_type);

    void Reconstruction_w_top_ttbar(std::string W_t_DecayMode, int t_type, std::string W_tbar_DecayMode, int tbar_type);

    void Fill_MA_semileptonic(std::string W_t_DecayMode, std::string W_tbar_DecayMode);

    void TLorentzFill(const TLorentzVector& p1, float & mass, float & pt, float & eta, float & phi);

    void IniVariables();

    template <typename T> int  FindInVector(vector<T>& v, T x);
    void PrintME(const xAOD::TruthParticle* mcPtr, int depth, int maxDepth = 10);
    void FillME(const xAOD::TruthParticleContainer* truthparticles, const xAOD::TruthEventContainer* truthevents);
    void SetTopTaggingWPs(const std::vector<std::string> WPs);

  private:

    void calculateWjets(const top::Event &event);
    virtual void dumpToolConfig(std::string fname);
    const std::string& configValueDefault(const std::string& key, const std::string& default_value = "False") const;
    std::string m_dumpToolConfigTo;

    bool m_isMC;
    bool m_isTOPQ;
    bool m_isSherpaW;

    int n_b;
    int n_c;
    int n_l;

    top::Chi2Selector* m_chi2Sel;


    BTaggingSelectionTool* tempSelTool;
    std::string m_ghostjetcollection;
    std::string m_trackjetcollection;
    std::string m_akt4truthjetcollection;
    std::string m_akt4truthjetcollectionWjets;
    std::string m_akt10truthjetcollection;
    bool m_runHtt;
    bool m_saveTruthJets;
    bool m_savePartons;
    bool m_trackjetBtaggingExtra;
    bool m_runEWK;
    bool m_saveFullTruthRecord;
    float m_trackjetPtCut;
    float m_trackjetMv2c10Cut;
    WeakCorr::WeakCorrScaleFactorParam* weak;

    bool m_savePdfWeight;
#ifdef ENABLE_WJETS_FILTER
    int m_Wfilter;
    int m_Wfilter_2;
    int m_Wfilter_3;
    int m_Wfilter_4;
    int m_Wfilter_5;
    int m_Wfilter_6;
    float m_Sherpa22_weight;
#endif
    std::vector<std::string> m_LHAPDFSets;

    //Store output PDF weights from LHAPDF
    std::unordered_map<std::string, std::vector<float> > m_PDF_eventWeights;

#ifdef ENABLE_TOPTAG_DEBUG
    //std::vector<std::string> m_TopTaggingWP = {"DNNTOPTAG_CONTAINED80", "DNNTOPTAG_INCLUSIVE80", "DNNTOPTAG_CONTAINED50", "DNNTOPTAG_INCLUSIVE50"}; // We always store variables of DNNContained/DNNInclusive FC 80% and 50%;
    std::vector<std::string> m_TopTaggingWP = {"DNNTOPTAG_CONTAINED80", "DNNTOPTAG_INCLUSIVE80"};
    std::unordered_map<std::string, Tagger> m_toptagging;
    std::unordered_map<std::string, ToolHandle<ICPJetUncertaintiesTool>> m_toptaggingUncTools;
    std::unique_ptr<JSSWTopTaggerDNN> m_dnnTopTaggerContained80;//DNN contained top tagger 80%
    std::unique_ptr<JSSWTopTaggerDNN> m_dnnTopTaggerInclusive80;//DNN inclusive top tagger 80%
#endif
    std::vector<int>   m_ljet_good;
    std::vector<int>   m_ljet_notgood;// for WCR
    std::vector<float> m_ljet_tau32_wta;
    std::vector<float> m_ljet_tau21_wta;
    std::vector<int>   m_ljet_label;
#ifdef ENABLE_LJETSUBSTRUCTURE_DEBUG
    std::vector<float> m_ljet_D2;
    std::vector<float> m_ljet_C2;
    std::vector<float> m_ljet_ECF1;
    std::vector<float> m_ljet_ECF2;
    std::vector<float> m_ljet_ECF3;
    std::vector<int> m_ljet_MClike;
#endif
    std::vector<float> m_ljet_tau1_wta;
    std::vector<float> m_ljet_tau2_wta;
    std::vector<float> m_ljet_tau3_wta;
    std::vector<int>   m_jet_closeToLepton;
    std::vector<int>   m_ljet_angular_cuts;

#ifdef ENABLE_ZPRIMERWGT
    ZprimeRWGTTool &m_zprimerwgt_tool = ZprimeRWGTTool::getInstance();
#endif
    std::string m_ZprimeRWGTParams;
    bool m_doZprimeRWGT = false;
    double m_weight_rwgt = -999;
    std::vector<float> m_part_ljet_tau32_wta;

    int m_NB_hadside_calojet;
    int m_NB_lepside_calojet;
    //int m_NB_hadside_tjet;
    //int m_NB_lepside_tjet;
    int m_ljtmatch;
    int m_Btagcat_calojet;
    int m_Btagcat_tjet;
    unsigned int m_lumiblock;
    unsigned int m_npv;
    float m_vtxz;

    std::vector<char>  m_el_isTight;
    std::vector<char>  m_mu_isTight;

#ifdef ENABLE_EXTRA_LEP_VARS
    // extra electron variables
    std::vector<float>  m_el_d0;
    std::vector<float>  m_el_z0;
    std::vector<float>  m_el_d0sig;
    std::vector<float>  m_el_z0sig;
    std::vector<float> m_el_ptvarcone30_TightTTVALooseCone_pt1000;
    std::vector<float> m_el_ptcone20_TightTTVALooseCone_pt1000;
    // extra muon variables
    std::vector<float>  m_mu_d0;
    std::vector<float>  m_mu_z0;
    std::vector<float>  m_mu_d0sig;
    std::vector<float> m_mu_z0sig;
    std::vector<float> m_mu_ptvarcone30_TightTTVA_pt1000;
    std::vector<float> m_mu_ptcone20_TightTTVA_pt1000;
#endif

    std::vector<int>  m_truthparticle_type;
    std::vector<int>  m_truthparticle_origin;
    std::vector<float>  m_truthparticle_pt;
    std::vector<float>  m_truthparticle_eta;
    std::vector<float>  m_truthparticle_phi;
    std::vector<float>  m_truthparticle_m;

    // Declare HTT tool and branches

    std::shared_ptr<top::TopConfig> m_config;

#ifdef USE_HTT
    JetRecTool* httTool;
    std::map<std::string, JetRecTool*> m_HTT;
    int HTT_masswindow_n;
    int HTT_n;
    std::vector<TLorentzVector> TLorentzHTT;
    std::vector<float> HTT_pt;
    std::vector<float> HTT_m;
    std::vector<float> HTT_eta;
    std::vector<float> HTT_phi;
    std::vector<float> HTT_m23m123;
    std::vector<float> HTT_atan1312;
    std::vector<float> HTT_tt_m;
    std::vector<float> HTT_tt_pt;

    int CA15_n;
    std::vector<float> CA15_pt;
    std::vector<float> CA15_eta;
    std::vector<float> CA15_phi;
    std::vector<float> CA15_m;
    std::vector<TLorentzVector> TLorentzCA15;
    std::vector<int> CA15_trk_bjets_n;
#endif

    // initial state inofrmation for EWK corr

    int m_initial_type;
    double m_weight_EW;

    //const std::vector<std::pair<std::string, std::string> > TaggerBtagWP;
    std::string btaggingAlgWP = "";
    std::string btag_outputVar = "";

    // small-R calo jet trackjet b-tagging information
    std::vector<std::vector<int> > m_jet_ghosttrackjet_idx;
    std::vector<int> m_jet_nghosttrackjet;
    std::vector<int> m_jet_nghosttrackjetb;
    std::vector<int> m_jet_nghosttrackjetbb;
    std::vector<int> m_jet_nghosttrackjetc;
    std::vector<int> m_jet_nghosttrackjetcc;
    std::vector<int> m_jet_nghosttrackjetl;

    // large-R calo jet ghosttrackjet b-tagging information
    std::vector<std::vector<int> > m_ljet_ghosttrackjet_idx;
    std::vector<int> m_ljet_nghosttrackjet;
    std::vector<int> m_ljet_nghosttrackjetb;
    std::vector<int> m_ljet_nghosttrackjetbb;
    std::vector<int> m_ljet_nghosttrackjetc;
    std::vector<int> m_ljet_nghosttrackjetcc;
    std::vector<int> m_ljet_nghosttrackjetl;
    std::vector<int> m_jet_NumTrkPt500;

    std::vector<int> m_jet_trueflav;

    // book track jet info
    std::vector<int> m_tjet_numConstituents;
    std::vector<int> m_tjet_label;
    std::vector<int> m_tjet_ghostlabel;
    std::vector<std::vector<float> > m_tjet_BHadron_eta; //b-hadrons within dR=0.3 wrt track jet
    std::vector<std::vector<float> > m_tjet_BHadron_phi;
    std::vector<std::vector<float> > m_tjet_BHadron_pt;
    std::vector<std::vector<float> > m_tjet_BHadron_e;



    std::vector<float> m_tjet_bTagSF_70;
    std::vector<std::vector<float> > m_tjet_bTagSF_70_eigen_B_up;
    std::vector<std::vector<float> > m_tjet_bTagSF_70_eigen_B_down;
    std::vector<std::vector<float> > m_tjet_bTagSF_70_eigen_C_up;
    std::vector<std::vector<float> > m_tjet_bTagSF_70_eigen_C_down;
    std::vector<std::vector<float> > m_tjet_bTagSF_70_eigen_Light_up;
    std::vector<std::vector<float> > m_tjet_bTagSF_70_eigen_Light_down;

    std::vector<float> m_tjet_bTagSF_70_syst_extrapolation_up;
    std::vector<float> m_tjet_bTagSF_70_syst_extrapolation_down;
    std::vector<float> m_tjet_bTagSF_70_syst_extrapolation_from_charm_up;
    std::vector<float> m_tjet_bTagSF_70_syst_extrapolation_from_charm_down;

    std::vector<float> m_jet_bTagSF_70;

#ifdef ENABLE_BTAG_DEBUG
    std::vector<float> m_tjet_bTagSF_70_syst_B_up;
    std::vector<float> m_tjet_bTagSF_70_syst_B_down;
    std::vector<float> m_tjet_bTagSF_70_syst_C_up;
    std::vector<float> m_tjet_bTagSF_70_syst_C_down;
    std::vector<float> m_tjet_bTagSF_70_syst_Light_up;
    std::vector<float> m_tjet_bTagSF_70_syst_Light_down;
    float m_weight_trackjet_bTagSF_70_env_nom;
    float m_weight_trackjet_bTagSF_70_env_B_up;
    float m_weight_trackjet_bTagSF_70_env_B_down;
    float m_weight_trackjet_bTagSF_70_env_C_up;
    float m_weight_trackjet_bTagSF_70_env_C_down;
    float m_weight_trackjet_bTagSF_70_env_Light_up;
    float m_weight_trackjet_bTagSF_70_env_Light_down;
    float m_weight_trackjet_bTagSF_70_env_extrapolation_up;
    float m_weight_trackjet_bTagSF_70_env_extrapolation_down;
    float m_weight_trackjet_bTagSF_70_env_extrapolation_from_charm_up;
    float m_weight_trackjet_bTagSF_70_env_extrapolation_from_charm_down;

    std::vector<float> m_jet_bTagSF_70;
    std::vector<float> m_jet_bTagSF_70_syst_B_up;
    std::vector<float> m_jet_bTagSF_70_syst_B_down;
    std::vector<float> m_jet_bTagSF_70_syst_C_up;
    std::vector<float> m_jet_bTagSF_70_syst_C_down;
    std::vector<float> m_jet_bTagSF_70_syst_Light_up;
    std::vector<float> m_jet_bTagSF_70_syst_Light_down;
    std::vector<float> m_jet_bTagSF_70_syst_extrapolation_up;
    std::vector<float> m_jet_bTagSF_70_syst_extrapolation_down;
    std::vector<float> m_jet_bTagSF_70_syst_extrapolation_from_charm_up;
    std::vector<float> m_jet_bTagSF_70_syst_extrapolation_from_charm_down;
    float m_weight_bTagSF_70_env_nom;
    float m_weight_bTagSF_70_env_B_up;
    float m_weight_bTagSF_70_env_B_down;
    float m_weight_bTagSF_70_env_C_up;
    float m_weight_bTagSF_70_env_C_down;
    float m_weight_bTagSF_70_env_Light_up;
    float m_weight_bTagSF_70_env_Light_down;
    float m_weight_bTagSF_70_env_extrapolation_up;
    float m_weight_bTagSF_70_env_extrapolation_down;
    float m_weight_bTagSF_70_env_extrapolation_from_charm_up;
    float m_weight_bTagSF_70_env_extrapolation_from_charm_down;
#endif

    // truth jet info
    std::vector<float> m_akt4truthjet_pt;
    std::vector<float> m_akt4truthjet_eta;
    std::vector<float> m_akt4truthjet_phi;
    std::vector<float> m_akt4truthjet_m;

    std::vector<float> m_akt10truthjet_pt;
    std::vector<float> m_akt10truthjet_eta;
    std::vector<float> m_akt10truthjet_phi;
    std::vector<float> m_akt10truthjet_m;
    std::vector<float> m_akt10truthjet_tau32_wta;

    // ME
    std::vector<float> m_MC_px_me;
    std::vector<float> m_MC_py_me;
    std::vector<float> m_MC_pz_me;
    std::vector<float> m_MC_e_me;
    std::vector<float> m_MC_m_me;
    std::vector<int>   m_MC_id_me;
    float              m_MC_Q_me;
    std::vector<float> m_MC_QxAOD_me;
    std::vector<float> m_MC_aSxAOD_me;

    // Partons
    float m_MC_b_from_t_pt;
    float m_MC_b_from_t_eta;
    float m_MC_b_from_t_phi;
    float m_MC_b_from_t_m;

    float m_MC_b_from_tbar_pt;
    float m_MC_b_from_tbar_eta;
    float m_MC_b_from_tbar_phi;
    float m_MC_b_from_tbar_m;

    float m_MC_Wdecay1_from_t_pt;
    float m_MC_Wdecay1_from_t_eta;
    float m_MC_Wdecay1_from_t_phi;
    float m_MC_Wdecay1_from_t_m;
    int   m_MC_Wdecay1_from_t_pdgId;

    float m_MC_Wdecay2_from_t_pt;
    float m_MC_Wdecay2_from_t_eta;
    float m_MC_Wdecay2_from_t_phi;
    float m_MC_Wdecay2_from_t_m;
    int   m_MC_Wdecay2_from_t_pdgId;

    float m_MC_W_from_t_pt;
    float m_MC_W_from_t_eta;
    float m_MC_W_from_t_phi;
    float m_MC_W_from_t_m;

    float m_MC_Wdecay1_from_tbar_pt;
    float m_MC_Wdecay1_from_tbar_eta;
    float m_MC_Wdecay1_from_tbar_phi;
    float m_MC_Wdecay1_from_tbar_m;
    int   m_MC_Wdecay1_from_tbar_pdgId;

    float m_MC_Wdecay2_from_tbar_pt;
    float m_MC_Wdecay2_from_tbar_eta;
    float m_MC_Wdecay2_from_tbar_phi;
    float m_MC_Wdecay2_from_tbar_m;
    int   m_MC_Wdecay2_from_tbar_pdgId;

    float m_MC_W_from_tbar_pt;
    float m_MC_W_from_tbar_eta;
    float m_MC_W_from_tbar_phi;
    float m_MC_W_from_tbar_m;

    float m_MC_i1_px;
    float m_MC_i1_py;
    float m_MC_i1_pz;
    float m_MC_i1_m;
    int   m_MC_i1_pid;

    float m_MC_i2_px;
    float m_MC_i2_py;
    float m_MC_i2_pz;
    float m_MC_i2_m;
    int   m_MC_i2_pid;

    float m_MC_t_pt;
    float m_MC_t_eta;
    float m_MC_t_phi;
    float m_MC_t_m;

    int m_MC_ttbar_decay; // -1: undef; 1: fullhad; 11: e+jets; 13: mu+jets; 15:tau+jets; 1111: eebb; 1112:emubb; 1113: etaubb; 1313:muubb; 1315:mutaubb; 1515tautaubb

    float m_MC_tbar_pt;
    float m_MC_tbar_eta;
    float m_MC_tbar_phi;
    float m_MC_tbar_m;

    float m_MC_t_afterFSR_pt;
    float m_MC_t_afterFSR_eta;
    float m_MC_t_afterFSR_phi;
    float m_MC_t_afterFSR_m;

    float m_MC_tbar_afterFSR_pt;
    float m_MC_tbar_afterFSR_eta;
    float m_MC_tbar_afterFSR_phi;
    float m_MC_tbar_afterFSR_m;

    float m_MC_ttbar_beforeFSR_pt;
    float m_MC_ttbar_beforeFSR_eta;
    float m_MC_ttbar_beforeFSR_phi;
    float m_MC_ttbar_beforeFSR_m;

    float m_MC_ttbar_afterFSR_pt;
    float m_MC_ttbar_afterFSR_eta;
    float m_MC_ttbar_afterFSR_phi;
    float m_MC_ttbar_afterFSR_m;

    // post-FSR top or anti-top found using last top pair before decay // only store ttbar mass now
    float m_MC_ttbar_afterFSR_beforeDecay_m;

    // post-FSR top or anti-top found using statusCodes
    float m_MC_t_afterFSR_SC_pt;
    float m_MC_t_afterFSR_SC_eta;
    float m_MC_t_afterFSR_SC_phi;
    float m_MC_t_afterFSR_SC_m;

    float m_MC_tbar_afterFSR_SC_pt;
    float m_MC_tbar_afterFSR_SC_eta;
    float m_MC_tbar_afterFSR_SC_phi;
    float m_MC_tbar_afterFSR_SC_m;

    float m_MC_ttbar_afterFSR_SC_pt;
    float m_MC_ttbar_afterFSR_SC_eta;
    float m_MC_ttbar_afterFSR_SC_phi;
    float m_MC_ttbar_afterFSR_SC_m;

    // Matched jets
    float m_MA_b_from_t_pt;
    float m_MA_b_from_t_eta;
    float m_MA_b_from_t_phi;
    float m_MA_b_from_t_m;
    int   m_MA_b_from_t_recoidx;
    float m_MA_b_from_t_recodr;
    float m_MA_b_from_t_truthdr;

    float m_MA_b_from_tbar_pt;
    float m_MA_b_from_tbar_eta;
    float m_MA_b_from_tbar_phi;
    float m_MA_b_from_tbar_m;
    int   m_MA_b_from_tbar_recoidx;
    float m_MA_b_from_tbar_recodr;
    float m_MA_b_from_tbar_truthdr;

    float m_MA_Wdecay1_from_t_pt;
    float m_MA_Wdecay1_from_t_eta;
    float m_MA_Wdecay1_from_t_phi;
    float m_MA_Wdecay1_from_t_m;
    int   m_MA_Wdecay1_from_t_recoidx;
    float m_MA_Wdecay1_from_t_recodr;
    float m_MA_Wdecay1_from_t_truthdr;
    float m_MA_Wdecay1_from_t_NuDphi;

    float m_MA_Wdecay2_from_t_pt;
    float m_MA_Wdecay2_from_t_eta;
    float m_MA_Wdecay2_from_t_phi;
    float m_MA_Wdecay2_from_t_m;
    int   m_MA_Wdecay2_from_t_recoidx;
    float m_MA_Wdecay2_from_t_recodr;
    float m_MA_Wdecay2_from_t_truthdr;

    float m_MA_Wdecay1_from_tbar_pt;
    float m_MA_Wdecay1_from_tbar_eta;
    float m_MA_Wdecay1_from_tbar_phi;
    float m_MA_Wdecay1_from_tbar_m;
    int   m_MA_Wdecay1_from_tbar_recoidx;
    float m_MA_Wdecay1_from_tbar_recodr;
    float m_MA_Wdecay1_from_tbar_truthdr;

    float m_MA_Wdecay2_from_tbar_pt;
    float m_MA_Wdecay2_from_tbar_eta;
    float m_MA_Wdecay2_from_tbar_phi;
    float m_MA_Wdecay2_from_tbar_m;
    int   m_MA_Wdecay2_from_tbar_recoidx;
    float m_MA_Wdecay2_from_tbar_recodr;
    float m_MA_Wdecay2_from_tbar_truthdr;
    float m_MA_Wdecay2_from_tbar_NuDphi;

    // Reconstructed particles from the matched jets
    float m_MA_W_from_t_pt;
    float m_MA_W_from_t_eta;
    float m_MA_W_from_t_phi;
    float m_MA_W_from_t_m;
    float m_MA_W_from_t_Njets;

    float m_MA_W_from_tbar_pt;
    float m_MA_W_from_tbar_eta;
    float m_MA_W_from_tbar_phi;
    float m_MA_W_from_tbar_m;
    float m_MA_W_from_tbar_Njets;

    float m_MA_t_pt;
    float m_MA_t_eta;
    float m_MA_t_phi;
    float m_MA_t_m;
    float m_MA_t_Njets;

    float m_MA_tFJ_pt;
    float m_MA_tFJ_eta;
    float m_MA_tFJ_phi;
    float m_MA_tFJ_m;
    int   m_MA_tFJ_nPartons;

    float m_MA_tbar_pt;
    float m_MA_tbar_eta;
    float m_MA_tbar_phi;
    float m_MA_tbar_m;
    float m_MA_tbar_Njets;

    float m_MA_tbarFJ_pt;
    float m_MA_tbarFJ_eta;
    float m_MA_tbarFJ_phi;
    float m_MA_tbarFJ_m;
    int   m_MA_tbarFJ_nPartons;

    float m_MA_ttbar_pt;
    float m_MA_ttbar_eta;
    float m_MA_ttbar_phi;
    float m_MA_ttbar_m;
    int   m_MC_ttbar_type;

    float m_MA_ttbarFJ_pt;
    float m_MA_ttbarFJ_eta;
    float m_MA_ttbarFJ_phi;
    float m_MA_ttbarFJ_m;

    // Variables for the chi2 parametrization
    float m_MA_bh_pt;
    float m_MA_bh_eta;
    float m_MA_bh_phi;
    float m_MA_bh_m;
    int   m_MA_bh_idx;
    float m_MA_bh_dr;
    float m_MA_bh_tdr;

    float m_MA_bl_pt;
    float m_MA_bl_eta;
    float m_MA_bl_phi;
    float m_MA_bl_m;
    int   m_MA_bl_idx;
    float m_MA_bl_dr;
    float m_MA_bl_tdr;

    float m_MA_wh_pt;
    float m_MA_wh_eta;
    float m_MA_wh_phi;
    float m_MA_wh_m;
    float m_MA_wh_Njets;

    float m_MA_w1h_pt;
    float m_MA_w1h_eta;
    float m_MA_w1h_phi;
    float m_MA_w1h_m;
    int   m_MA_w1h_pdgId;
    int   m_MA_w1h_idx;
    float m_MA_w1h_dr;
    float m_MA_w1h_tdr;

    float m_MA_w2h_pt;
    float m_MA_w2h_eta;
    float m_MA_w2h_phi;
    float m_MA_w2h_m;
    int   m_MA_w2h_pdgId;
    int   m_MA_w2h_idx;
    float m_MA_w2h_dr;
    float m_MA_w2h_tdr;

    float m_MA_wl_pt;
    float m_MA_wl_eta;
    float m_MA_wl_phi;
    float m_MA_wl_m;

    float m_MA_w1l_pt;
    float m_MA_w1l_eta;
    float m_MA_w1l_phi;
    float m_MA_w1l_m;
    int   m_MA_w1l_pdgId;
    int   m_MA_w1l_idx;
    float m_MA_w1l_dr;
    float m_MA_w1l_tdr;
    float m_MA_w1l_NuDphi;

    float m_MA_w2l_pt;
    float m_MA_w2l_eta;
    float m_MA_w2l_phi;
    float m_MA_w2l_m;
    int   m_MA_w2l_pdgId;
    int   m_MA_w2l_idx;
    float m_MA_w2l_dr;
    float m_MA_w2l_tdr;
    float m_MA_w2l_NuDphi;

    float m_MA_th_pt;
    float m_MA_th_eta;
    float m_MA_th_phi;
    float m_MA_th_m;
    float m_MA_th_Njets;

    float m_MA_thFJ_pt;
    float m_MA_thFJ_eta;
    float m_MA_thFJ_phi;
    float m_MA_thFJ_m;
    int   m_MA_thFJ_nPartons;

    float m_MA_tl_pt;
    float m_MA_tl_eta;
    float m_MA_tl_phi;
    float m_MA_tl_m;
    float m_MA_tl_Njets;

    float m_MA_ttbar_lpj_pt;
    float m_MA_ttbar_lpj_eta;
    float m_MA_ttbar_lpj_phi;
    float m_MA_ttbar_lpj_m;
    int   m_MA_ttbar_lpj_Njets;

    //Partons in l+jets

    float m_MC_bh_pt;
    float m_MC_bh_eta;
    float m_MC_bh_phi;
    float m_MC_bh_m;

    float m_MC_bl_pt;
    float m_MC_bl_eta;
    float m_MC_bl_phi;
    float m_MC_bl_m;

    float m_MC_wh_pt;
    float m_MC_wh_eta;
    float m_MC_wh_phi;
    float m_MC_wh_m;

    float m_MC_w1h_pt;
    float m_MC_w1h_eta;
    float m_MC_w1h_phi;
    float m_MC_w1h_m;
    int   m_MC_w1h_pdgId;

    float m_MC_w2h_pt;
    float m_MC_w2h_eta;
    float m_MC_w2h_phi;
    float m_MC_w2h_m;
    int   m_MC_w2h_pdgId;

    float m_MC_wl_pt;
    float m_MC_wl_eta;
    float m_MC_wl_phi;
    float m_MC_wl_m;

    float m_MC_w1l_pt;
    float m_MC_w1l_eta;
    float m_MC_w1l_phi;
    float m_MC_w1l_m;
    int   m_MC_w1l_pdgId;

    float m_MC_w2l_pt;
    float m_MC_w2l_eta;
    float m_MC_w2l_phi;
    float m_MC_w2l_m;
    int   m_MC_w2l_pdgId;

    float m_MC_th_pt;
    float m_MC_th_eta;
    float m_MC_th_phi;
    float m_MC_th_m;

    float m_MC_tl_pt;
    float m_MC_tl_eta;
    float m_MC_tl_phi;
    float m_MC_tl_m;

    float m_MC_ttbar_lpj_beforeFSR_pt;
    float m_MC_ttbar_lpj_beforeFSR_eta;
    float m_MC_ttbar_lpj_beforeFSR_phi;
    float m_MC_ttbar_lpj_beforeFSR_m;

    float m_MC_ttbar_lpj_afterFSR_pt;
    float m_MC_ttbar_lpj_afterFSR_eta;
    float m_MC_ttbar_lpj_afterFSR_phi;
    float m_MC_ttbar_lpj_afterFSR_m;

    float m_chi2_all = -5000;

    float m_chi2_bh_pt;
    float m_chi2_bh_eta;
    float m_chi2_bh_phi;
    float m_chi2_bh_m;

    float m_chi2_bl_pt;
    float m_chi2_bl_eta;
    float m_chi2_bl_phi;
    float m_chi2_bl_m;

    float m_chi2_wh_pt;
    float m_chi2_wh_eta;
    float m_chi2_wh_phi;
    float m_chi2_wh_m;

    float m_chi2_w1h_pt;
    float m_chi2_w1h_eta;
    float m_chi2_w1h_phi;
    float m_chi2_w1h_m;
    int   m_chi2_w1h_pdgId;

    float m_chi2_w2h_pt;
    float m_chi2_w2h_eta;
    float m_chi2_w2h_phi;
    float m_chi2_w2h_m;
    int   m_chi2_w2h_pdgId;

    float m_chi2_wl_pt;
    float m_chi2_wl_eta;
    float m_chi2_wl_phi;
    float m_chi2_wl_m;

    float m_chi2_w1l_pt;
    float m_chi2_w1l_eta;
    float m_chi2_w1l_phi;
    float m_chi2_w1l_m;
    int   m_chi2_w1l_pdgId;

    float m_chi2_w2l_pt;
    float m_chi2_w2l_eta;
    float m_chi2_w2l_phi;
    float m_chi2_w2l_m;
    int   m_chi2_w2l_pdgId;

    float m_chi2_th_pt;
    float m_chi2_th_eta;
    float m_chi2_th_phi;
    float m_chi2_th_m;

    float m_chi2_tl_pt;
    float m_chi2_tl_eta;
    float m_chi2_tl_phi;
    float m_chi2_tl_m;

    float m_chi2_ttbar_pt;
    float m_chi2_ttbar_eta;
    float m_chi2_ttbar_phi;
    float m_chi2_ttbar_m;


    TtresNeutrinoBuilder* m_NeutrinoBuilder;
    double m_Units;
    ClassDef(TtresEventSaverFlatNtuple, 0);
};

}

#endif


