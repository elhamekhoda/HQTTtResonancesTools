#include "HQTTtResonancesTools/TtresEventSaverFlatNtuple.h"

#include "TopEventSelectionTools/TreeManager.h"
#include "PathResolver/PathResolver.h"

#include "xAODRootAccess/TEvent.h"
#include "TopEvent/EventTools.h"

#include <vector>
#include <iostream>
#include <string>
#include <fstream>

#include "TopPartons/PartonHistory.h"
#include "TopEventReconstructionTools/TtresNeutrinoBuilder.h"

#include "TopConfiguration/TopConfig.h"
#include "HQTTtResonancesTools/ObjectLoaderTtres.h"
#include "TtResonancesTools/TtresChi2.h"

#include "TopConfiguration/ConfigurationSettings.h"

#include "xAODCore/ShallowCopy.h"

// HEPTopTagger
//#define USE_HTT 1
#ifdef USE_HTT
#include "BoostedJetTaggers/configHTT_HQTT.h"
#endif

#include "HQTTtResonancesTools/WFilter.h"

using namespace std;

namespace top {

//Always initialise primitive types in the constructor
TtresEventSaverFlatNtuple::TtresEventSaverFlatNtuple() {

    top::ConfigurationSettings* configSettings = top::ConfigurationSettings::get();
    //weak = new WeakCorr::WeakCorrScaleFactorParam(PathResolverFindCalibFile("TtResonancesTools/EWcorr_param.root"));

    m_isTOPQ              = (configValueDefault("isTOPQ") == "True") ? true : false ;
    m_isSherpaW           = (configValueDefault("isSherpaW") == "True") ? true : false;
    m_runHtt              = (configValueDefault("TtresRunHTT", "False") == "True") ? true : false ;
#ifndef USE_HTT
    m_runHtt              = false;
#endif
    m_savePartons         = (configValueDefault("TtresSavePartons", "False") == "True") ? true : false ;
    m_saveFullTruthRecord = (configValueDefault("SaveFullTruthRecord", "True") == "True") ? true : false ;
    m_runEWK              = (configValueDefault("TtresrunEWK", "False") == "True") ? true : false ;
    m_dumpToolConfigTo    = configValueDefault("DumpToolConfigTo", "False"); // A string!

    if (m_isTOPQ) {
        m_trackjetcollection = "";
        m_ghostjetcollection = "";
    } else {
        m_trackjetcollection = "AntiKt2PV0TrackJets";
        m_ghostjetcollection = "GhostAntiKt2TrackJet";
        m_trackjetPtCut = 10e3; // for AntiKt2-> 10 GeV, for AntiKt3 -> 7 GeV, but JVT needs (?) to be applied for jets with pt < 50 GeV, so raise this to avoid that, as JVT not available
        m_trackjetMv2c10Cut = 0.66; // for AntiKt2 @ 70% ->  0.66
    }

    m_savePdfWeight = false;

    m_akt4truthjetcollection = "AntiKt4TruthWZJets";
    m_akt4truthjetcollectionWjets = "AntiKt4TruthJets";
    if (configSettings->value("TruthLargeRJetCollectionName") != "None") {
        m_akt10truthjetcollection = configSettings->value("TruthLargeRJetCollectionName");
    } else {
        m_akt10truthjetcollection = "";
    }
    m_saveTruthJets = true;
    m_isMC = true;
    m_chi2Sel = new top::Chi2Selector(" < 6");
}

TtresEventSaverFlatNtuple::~TtresEventSaverFlatNtuple() {
    if (m_chi2Sel)    delete m_chi2Sel;
    //if (weak) delete weak;
}

/**
 * Add our extra variable to each systematic TTree
 */
void TtresEventSaverFlatNtuple::initialize(std::shared_ptr<top::TopConfig> config, TFile* file, const std::vector<std::string>& extraBranches) {
    EventSaverFlatNtuple::initialize(config, file, extraBranches);

    m_savePdfWeight = config->saveLHAPDFEvent();
    m_LHAPDFSets = config->LHAPDFSets();

    STL80 = STTHelpers::configSubstTagger("LooseSmoothTopTag", "SmoothCut_80");
    STL50 = STTHelpers::configSubstTagger("LooseSmoothTopTag", "SmoothCut_50");

    m_smoothedTopTaggerMT80 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerMT80" ) );
    m_smoothedTopTaggerMT50 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerMT50" ) );
    m_smoothedTopTaggerTS80 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerTS80" ) );
    m_smoothedTopTaggerTS50 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerTS50" ) );
    m_smoothedTopTaggerQT80 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerQT80" ) );
    m_smoothedTopTaggerQT50 = std::unique_ptr<SmoothedTopTagger>( new SmoothedTopTagger( "SmoothedTopTaggerQT50" ) );
    m_bdtTopTagger80 = std::unique_ptr<JSSWTopTaggerBDT>( new JSSWTopTaggerBDT( "JSSWTopTaggerBDT80" ) );
    m_dnnTopTagger80 = std::unique_ptr<JSSWTopTaggerDNN>( new JSSWTopTaggerDNN( "JSSWTopTaggerDNN80" ) );
    m_topoTopTagger80 = std::unique_ptr<TopoclusterTopTagger>( new TopoclusterTopTagger( "TopoclusterTopTagger80" ) );

    top::check(m_smoothedTopTaggerMT80->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_MassTau32FixedSignalEfficiency80_MC15c_20161209.dat"), "Failed to set property for ConfigFile");
    top::check(m_smoothedTopTaggerMT50->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_MassTau32FixedSignalEfficiency50_MC15c_20161209.dat"), "Failed to set property for ConfigFile");
    top::check(m_smoothedTopTaggerTS80->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_Tau32Split23FixedSignalEfficiency80_MC15c_20161209.dat"), "Failed to set property for ConfigFile");
    top::check(m_smoothedTopTaggerTS50->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_Tau32Split23FixedSignalEfficiency50_MC15c_20161209.dat"), "Failed to set property for ConfigFile");
    top::check(m_smoothedTopTaggerQT80->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_QwTau32FixedSignalEfficiency80_MC15c_20161209.dat"), "Failed to set property for ConfigFile");
    top::check(m_smoothedTopTaggerQT50->setProperty( "ConfigFile",   "SmoothedTopTaggers/SmoothedTopTagger_AntiKt10LCTopoTrimmed_QwTau32FixedSignalEfficiency50_MC15c_20161209.dat"), "Failed to set property for ConfigFile");
    top::check(m_bdtTopTagger80->setProperty( "ConfigFile",   "JSSWTopTaggerBDT/JSSBDTTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC15c_20170824_BOOSTSetup80Eff.dat"), "Failed to set property for ConfigFile");
    top::check(m_dnnTopTagger80->setProperty( "ConfigFile",   "JSSWTopTaggerDNN/JSSDNNTagger_AntiKt10LCTopoTrimmed_TopQuarkContained_MC15c_20170824_BOOSTSetup80Eff.dat"), "Failed to set property for ConfigFile");
    top::check(m_topoTopTagger80->setProperty( "ConfigFile",   "TopoclusterTopTagger/TopoclusterTopTagger_AntiKt10LCTopoTrimmed_TopQuark_MC15c_20170511_ptweighted80Eff.dat"), "Failed to set property for ConfigFile");

    top::check(m_smoothedTopTaggerMT80->initialize(), "Initializing failed");
    top::check(m_smoothedTopTaggerMT50->initialize(), "Initializing failed");
    top::check(m_smoothedTopTaggerTS80->initialize(), "Initializing failed");
    top::check(m_smoothedTopTaggerTS50->initialize(), "Initializing failed");
    top::check(m_smoothedTopTaggerQT80->initialize(), "Initializing failed");
    top::check(m_smoothedTopTaggerQT50->initialize(), "Initializing failed");
    top::check(m_bdtTopTagger80->initialize(), "Initializing failed");
    top::check(m_dnnTopTagger80->initialize(), "Initializing failed");
    top::check(m_topoTopTagger80->initialize(), "Initializing failed");

    if (config->isMC()) {
        std::cout << "GENERATOR:" << config->getGenerators() << std::endl;
        //cout<< "I am here" <<endl;
        //n_b = config->btagging_num_B_eigenvars("MV2c10","FixedCutBEff_70");
        //n_b =0;
        //cout<< "I am here_1" <<endl;
        //n_c = config->btagging_num_C_eigenvars("FixedCutBEff_70");
        //n_c =0;
        //cout<< "I am here_2" <<endl;
        //n_l=0;
        //n_l = config->btagging_num_Light_eigenvars("FixedCutBEff_70");
        //cout<< "I am here_3" <<endl;
    }
    else {
        m_isMC = false;
        m_saveTruthJets = false;
        m_savePartons = false;
        m_akt10truthjetcollection = "";
        m_akt4truthjetcollection = "";
        m_akt4truthjetcollectionWjets = "";
        m_isSherpaW = false;
        n_b = 0;
        n_c = 0;
        n_l = 0;
    }
    cout << "I am here" << endl;
    m_config = config;
    // build HEPTopTagger
#ifdef USE_HTT
    if (m_runHtt) {
        for (auto treename : *config->systAllTTreeNames()) {
            m_HTT[treename.second] = buildFullHTTagger(treename.second);     ;
        }
    }
#endif

    if (m_config->doTopParticleLevel() && particleLevelTreeManager()) {
        auto partTree = particleLevelTreeManager();
        partTree->makeOutputVariable(m_part_ljet_tau32, "ljet_tau32");
        partTree->makeOutputVariable(m_part_ljet_tau32_wta, "ljet_tau32_wta");
    }

    for (auto TaggerBtagWP : m_config->bTagWP()) {
        //Default algorithms is mv2c10
        std::string m_tagger = TaggerBtagWP.first;
        std::string btagWP = TaggerBtagWP.second;
        btaggingAlgWP = "btag_SF_" + m_tagger + "_" + btagWP + "_nom"; //variable to acceess the nominal b-tagging SF
        btag_outputVar = "btag_SF_" + m_tagger + "_" + btagWP.substr( btagWP.length() - 2); //The b_tag SF gets saved with this name
    }


    for (auto systematicTree : treeManagers()) {

        if (m_LHAPDFSets.size() > 0) {
            for ( const auto& pdfset : m_LHAPDFSets )
                m_PDF_eventWeights[ pdfset ] = std::vector<float>();

            for ( auto& pdf : m_PDF_eventWeights )
                systematicTree->makeOutputVariable( pdf.second, pdf.first );
        }

        // save lumiblock
        systematicTree->makeOutputVariable(m_lumiblock, "lumiblock");
        systematicTree->makeOutputVariable(m_npv, "npv");
        systematicTree->makeOutputVariable(m_vtxz, "vtxz");

        // ####################
        // #
        // NEW
        // W+jets filter variable for Sherpa samples
        // => std filter: Hadron-filter, c-truth jet matching
        systematicTree->makeOutputVariable(m_Wfilter, "Wfilter_Sherpa");
        // => (2) filter: Hadron-filter, no truth jet matching
        systematicTree->makeOutputVariable(m_Wfilter_2, "Wfilter_Sherpa_nT");
        // => (3) filter: quark-filter, no truth jet matching
        systematicTree->makeOutputVariable(m_Wfilter_3, "Wfilter_Sherpa_Q");
        // => (4) filter: quark-filter, no truth jet matching
        systematicTree->makeOutputVariable(m_Wfilter_4, "Wfilter_Sherpa_Q_nT");
        // => (5) filter: extended Hadron-filter, c-truth jet matching
        systematicTree->makeOutputVariable(m_Wfilter_5, "Wfilter_Sherpa_ext");
        // => (6) filter: extended Hadron-filter, no truth jet matching
        systematicTree->makeOutputVariable(m_Wfilter_6, "Wfilter_Sherpa_ext_nT");

        // Sherpa 2.2 reweighting
        systematicTree->makeOutputVariable(m_Sherpa22_weight, "weight_Sherpa22_corr");

        systematicTree->makeOutputVariable(m_tjet_label, "tjet_label");
        systematicTree->makeOutputVariable(m_tjet_numConstituents, "tjet_numConstituents");

        systematicTree->makeOutputVariable(m_tjet_bTagSF_70, "tjet_" + btag_outputVar);
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_eigen_B_up, "tjet_bTagSF_70_eigen_B_up");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_eigen_B_down, "tjet_bTagSF_70_eigen_B_down");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_eigen_C_up, "tjet_bTagSF_70_eigen_C_up");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_eigen_C_down, "tjet_bTagSF_70_eigen_C_down");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_eigen_Light_up, "tjet_bTagSF_70_eigen_Light_up");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_eigen_Light_down, "tjet_bTagSF_70_eigen_Light_down");

        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_syst_extrapolation_up, "tjet_bTagSF_70_syst_extrapolation_up");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_syst_extrapolation_down, "tjet_bTagSF_70_syst_extrapolation_down");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_syst_extrapolation_from_charm_up, "tjet_bTagSF_70_syst_extrapolation_from_charm_up");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_syst_extrapolation_from_charm_down, "tjet_bTagSF_70_syst_extrapolation_from_charm_down");
#ifdef ENABLE_BTAG_DEBUG
        // envelope systematics
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_syst_B_up, "tjet_bTagSF_70_syst_B_up");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_syst_B_down, "tjet_bTagSF_70_syst_B_down");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_syst_C_up, "tjet_bTagSF_70_syst_C_up");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_syst_C_down, "tjet_bTagSF_70_syst_C_down");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_syst_Light_up, "tjet_bTagSF_70_syst_Light_up");
        systematicTree->makeOutputVariable(m_tjet_bTagSF_70_syst_Light_down, "tjet_bTagSF_70_syst_Light_down");

        systematicTree->makeOutputVariable(m_weight_trackjet_bTagSF_70_env_nom, "weight_trackjet_bTagSF_70_env_nom");
        systematicTree->makeOutputVariable(m_weight_trackjet_bTagSF_70_env_B_up, "weight_trackjet_bTagSF_70_env_B_up");
        systematicTree->makeOutputVariable(m_weight_trackjet_bTagSF_70_env_B_down, "weight_trackjet_bTagSF_70_env_B_down");
        systematicTree->makeOutputVariable(m_weight_trackjet_bTagSF_70_env_C_up, "weight_trackjet_bTagSF_70_env_C_up");
        systematicTree->makeOutputVariable(m_weight_trackjet_bTagSF_70_env_C_down, "weight_trackjet_bTagSF_70_env_C_down");
        systematicTree->makeOutputVariable(m_weight_trackjet_bTagSF_70_env_Light_up, "weight_trackjet_bTagSF_70_env_Light_up");
        systematicTree->makeOutputVariable(m_weight_trackjet_bTagSF_70_env_Light_down, "weight_trackjet_bTagSF_70_env_Light_down");

        systematicTree->makeOutputVariable(m_weight_trackjet_bTagSF_70_env_extrapolation_up, "weight_trackjet_bTagSF_70_env_extrapolation_up");
        systematicTree->makeOutputVariable(m_weight_trackjet_bTagSF_70_env_extrapolation_down, "weight_trackjet_bTagSF_70_env_extrapolation_down");
        systematicTree->makeOutputVariable(m_weight_trackjet_bTagSF_70_env_extrapolation_from_charm_up, "weight_trackjet_bTagSF_70_env_extrapolation_from_charm_up");
        systematicTree->makeOutputVariable(m_weight_trackjet_bTagSF_70_env_extrapolation_from_charm_down, "weight_trackjet_bTagSF_70_env_extrapolation_from_charm_down");
#endif

        // variable indicating Btagging category
        systematicTree->makeOutputVariable(m_Btagcat, "Btagcat");
        systematicTree->makeOutputVariable(m_NB_hadside, "NB_hadside");
        systematicTree->makeOutputVariable(m_NB_lepside, "NB_lepside");


        // save an int that is 0/1 if the large-R jet satisfies all basic cuts
        // including mass, sqrt(d_12), dr, dphi, etc.
        // so that we can save all other large-R jets as well just passing pt and eta cuts
        systematicTree->makeOutputVariable(m_ljet_good, "ljet_good");
        systematicTree->makeOutputVariable(m_ljet_notgood, "ljet_notgood");

        systematicTree->makeOutputVariable(m_ljet_tau32, "ljet_tau32");
        systematicTree->makeOutputVariable(m_ljet_tau32_wta, "ljet_tau32_wta");
        systematicTree->makeOutputVariable(m_ljet_tau21, "ljet_tau21");
        systematicTree->makeOutputVariable(m_ljet_tau21_wta, "ljet_tau21_wta");
#ifdef ENABLE_LJETSUBSTRUCTURE_DEBUG
        systematicTree->makeOutputVariable(m_ljet_ECF1, "ljet_ECF1");
        systematicTree->makeOutputVariable(m_ljet_ECF2, "ljet_ECF2");
        systematicTree->makeOutputVariable(m_ljet_ECF3, "ljet_ECF3");
        systematicTree->makeOutputVariable(m_ljet_C2, "ljet_C2");
        systematicTree->makeOutputVariable(m_ljet_D2, "ljet_D2");
        systematicTree->makeOutputVariable(m_ljet_MClike, "ljet_MClike");
#endif

        systematicTree->makeOutputVariable(m_ljet_good_sub80, "ljet_good_sub80");
        systematicTree->makeOutputVariable(m_ljet_good_sub50, "ljet_good_sub50");
        systematicTree->makeOutputVariable(m_ljet_good_smooth_mt80, "ljet_good_smooth_mt80");
        systematicTree->makeOutputVariable(m_ljet_good_smooth_mt50, "ljet_good_smooth_mt50");
        systematicTree->makeOutputVariable(m_ljet_good_smooth_ts80, "ljet_good_smooth_ts80");
        systematicTree->makeOutputVariable(m_ljet_good_smooth_ts50, "ljet_good_smooth_ts50");
        systematicTree->makeOutputVariable(m_ljet_good_smooth_qt80, "ljet_good_smooth_qt80");
        systematicTree->makeOutputVariable(m_ljet_good_smooth_qt50, "ljet_good_smooth_qt50");
        systematicTree->makeOutputVariable(m_ljet_good_bdt80, "ljet_good_bdt80");
        systematicTree->makeOutputVariable(m_ljet_good_dnn80, "ljet_good_dnn80");
        systematicTree->makeOutputVariable(m_ljet_good_topo80, "ljet_good_topo80");
        systematicTree->makeOutputVariable(m_ljet_bdt_score,   "ljet_BDTTopTag_score");
        systematicTree->makeOutputVariable(m_ljet_dnn_score,   "ljet_DNNTopTag_score");
        systematicTree->makeOutputVariable(m_ljet_topo_score,   "ljet_topoDNNTopTag_score");
        systematicTree->makeOutputVariable(m_ljet_angular_cuts, "ljet_angular_cuts"); // large-R jet angular cuts
        //track jet b-tagging variables
        systematicTree->makeOutputVariable(m_tjet_mv2c10mu,  "tjet_mv2c10mu");
        systematicTree->makeOutputVariable(m_tjet_mv2c10rnn,  "tjet_mv2c10rnn");
        systematicTree->makeOutputVariable(m_tjet_dl1_pu,  "tjet_dl1_pu");
        systematicTree->makeOutputVariable(m_tjet_dl1_pb,  "tjet_dl1_pb");
        systematicTree->makeOutputVariable(m_tjet_dl1_pc,  "tjet_dl1_pc");
        systematicTree->makeOutputVariable(m_tjet_dl1mu_pu,  "tjet_dl1mu_pu");
        systematicTree->makeOutputVariable(m_tjet_dl1mu_pb,  "tjet_dl1mu_pb");
        systematicTree->makeOutputVariable(m_tjet_dl1mu_pc,  "tjet_dl1mu_pc");
        systematicTree->makeOutputVariable(m_tjet_dl1rnn_pu,  "tjet_dl1rnn_pu");
        systematicTree->makeOutputVariable(m_tjet_dl1rnn_pb,  "tjet_dl1rnn_pb");
        systematicTree->makeOutputVariable(m_tjet_dl1rnn_pc,  "tjet_dl1rnn_pc");


        // book large-R calo jet trackjet b-tagging information
        systematicTree->makeOutputVariable(m_ljet_ghosttrackjet_idx, "ljet_ghosttrackjet_idx");
        systematicTree->makeOutputVariable(m_ljet_nghosttrackjet, "ljet_nghosttrackjet");
        systematicTree->makeOutputVariable(m_ljet_nghosttrackjetb, "ljet_nghosttrackjetb");
        systematicTree->makeOutputVariable(m_ljet_nghosttrackjetbb, "ljet_nghosttrackjetbb");
        systematicTree->makeOutputVariable(m_ljet_nghosttrackjetc, "ljet_nghosttrackjetc");
        systematicTree->makeOutputVariable(m_ljet_nghosttrackjetcc, "ljet_nghosttrackjetcc");
        systematicTree->makeOutputVariable(m_ljet_nghosttrackjetl, "ljet_nghosttrackjetl");

        systematicTree->makeOutputVariable(m_ljtmatch, "ljtmatch");

        systematicTree->makeOutputVariable(m_initial_type, "initial_type"); // add information about initial state  1-GG,2-UU,3-DD
        //systematicTree->makeOutputVariable(m_weight_EW,"weight_EW");

        // save an int that is 0/1 if the small-R jet is the one with dR < 1.5 to the lepton
        // and it is the highest p_T one
        systematicTree->makeOutputVariable(m_jet_closeToLepton, "jet_closeToLepton");

        // book small-R calo jet trackjet b-tagging information
        systematicTree->makeOutputVariable(m_jet_bTagSF_70, "jet_" + btag_outputVar);
        systematicTree->makeOutputVariable(m_jet_ghosttrackjet_idx, "jet_ghosttrackjet_idx");
        systematicTree->makeOutputVariable(m_jet_nghosttrackjet, "jet_nghosttrackjet");
        systematicTree->makeOutputVariable(m_jet_nghosttrackjetb, "jet_nghosttrackjetb");
        systematicTree->makeOutputVariable(m_jet_nghosttrackjetbb, "jet_nghosttrackjetbb");
        systematicTree->makeOutputVariable(m_jet_nghosttrackjetc, "jet_nghosttrackjetc");
        systematicTree->makeOutputVariable(m_jet_nghosttrackjetcc, "jet_nghosttrackjetcc");
        systematicTree->makeOutputVariable(m_jet_nghosttrackjetl, "jet_nghosttrackjetl");

        systematicTree->makeOutputVariable(m_jet_trueflav, "jet_trueflav");

        //for leptons
        //Adding flag for the tight leptons in the nominal ttree
        if (systematicTree->name() == "nominal") {
            systematicTree->makeOutputVariable(m_el_isTight, "el_isTight");
            systematicTree->makeOutputVariable(m_mu_isTight, "mu_isTight");
        }

        systematicTree->makeOutputVariable(m_el_d0,      "el_d0");
        systematicTree->makeOutputVariable(m_el_z0,      "el_z0");
        //systematicTree->makeOutputVariable(m_el_d0sig,   "el_d0sig");
        systematicTree->makeOutputVariable(m_el_z0sig,   "el_z0sig");

        systematicTree->makeOutputVariable(m_mu_d0,      "mu_d0");
        systematicTree->makeOutputVariable(m_mu_z0,      "mu_z0");
        //systematicTree->makeOutputVariable(m_mu_d0sig,   "mu_d0sig");
        systematicTree->makeOutputVariable(m_mu_z0sig,   "mu_z0sig");

        systematicTree->makeOutputVariable(m_truthparticle_type,      "truthparticle_type");
        systematicTree->makeOutputVariable(m_truthparticle_origin,      "truthparticle_origin");
        systematicTree->makeOutputVariable(m_truthparticle_pt,      "truthparticle_pt");
        systematicTree->makeOutputVariable(m_truthparticle_eta,      "truthparticle_eta");
        systematicTree->makeOutputVariable(m_truthparticle_phi,      "truthparticle_phi");
        systematicTree->makeOutputVariable(m_truthparticle_m,      "truthparticle_m");


#ifdef ENABLE_BTAG_DEBUG
        systematicTree->makeOutputVariable(m_jet_bTagSF_70, "jet_bTagSF_70");
        systematicTree->makeOutputVariable(m_jet_bTagSF_70_syst_B_up, "jet_bTagSF_70_syst_B_up");
        systematicTree->makeOutputVariable(m_jet_bTagSF_70_syst_B_down, "jet_bTagSF_70_syst_B_down");
        systematicTree->makeOutputVariable(m_jet_bTagSF_70_syst_C_up, "jet_bTagSF_70_syst_C_up");
        systematicTree->makeOutputVariable(m_jet_bTagSF_70_syst_C_down, "jet_bTagSF_70_syst_C_down");
        systematicTree->makeOutputVariable(m_jet_bTagSF_70_syst_Light_up, "jet_bTagSF_70_syst_Light_up");
        systematicTree->makeOutputVariable(m_jet_bTagSF_70_syst_Light_down, "jet_bTagSF_70_syst_Light_down");

        systematicTree->makeOutputVariable(m_jet_bTagSF_70_syst_extrapolation_up, "jet_bTagSF_70_syst_extrapolation_up");
        systematicTree->makeOutputVariable(m_jet_bTagSF_70_syst_extrapolation_down, "jet_bTagSF_70_syst_extrapolation_down");
        systematicTree->makeOutputVariable(m_jet_bTagSF_70_syst_extrapolation_from_charm_up, "jet_bTagSF_70_syst_extrapolation_from_charm_up");
        systematicTree->makeOutputVariable(m_jet_bTagSF_70_syst_extrapolation_from_charm_down, "jet_bTagSF_70_syst_extrapolation_from_charm_down");
        systematicTree->makeOutputVariable(m_weight_bTagSF_70_env_nom, "weight_bTagSF_70_env_nom");
        systematicTree->makeOutputVariable(m_weight_bTagSF_70_env_B_up, "weight_bTagSF_70_env_B_up");
        systematicTree->makeOutputVariable(m_weight_bTagSF_70_env_B_down, "weight_bTagSF_70_env_B_down");
        systematicTree->makeOutputVariable(m_weight_bTagSF_70_env_C_up, "weight_bTagSF_70_env_C_up");
        systematicTree->makeOutputVariable(m_weight_bTagSF_70_env_C_down, "weight_bTagSF_70_env_C_down");
        systematicTree->makeOutputVariable(m_weight_bTagSF_70_env_Light_up, "weight_bTagSF_70_env_Light_up");
        systematicTree->makeOutputVariable(m_weight_bTagSF_70_env_Light_down, "weight_bTagSF_70_env_Light_down");

        systematicTree->makeOutputVariable(m_weight_bTagSF_70_env_extrapolation_up, "weight_bTagSF_70_env_extrapolation_up");
        systematicTree->makeOutputVariable(m_weight_bTagSF_70_env_extrapolation_down, "weight_bTagSF_70_env_extrapolation_down");
        systematicTree->makeOutputVariable(m_weight_bTagSF_70_env_extrapolation_from_charm_up, "weight_bTagSF_70_env_extrapolation_from_charm_up");
        systematicTree->makeOutputVariable(m_weight_bTagSF_70_env_extrapolation_from_charm_down, "weight_bTagSF_70_env_extrapolation_from_charm_down");
#endif

        if (m_runHtt) {
            // HEPTopTagger branches
            systematicTree->makeOutputVariable(CA15_n, "CA15_n");
            systematicTree->makeOutputVariable(CA15_pt, "CA15_pt");
            systematicTree->makeOutputVariable(CA15_m, "CA15_m");
            systematicTree->makeOutputVariable(CA15_eta, "CA15_eta");
            systematicTree->makeOutputVariable(CA15_phi, "CA15_phi");
            systematicTree->makeOutputVariable(CA15_trk_bjets_n, "CA15_trk_bjets_n");

            systematicTree->makeOutputVariable(HTT_pt, "HTT_pt");
            systematicTree->makeOutputVariable(HTT_m, "HTT_m");
            systematicTree->makeOutputVariable(HTT_eta, "HTT_eta");
            systematicTree->makeOutputVariable(HTT_phi, "HTT_phi");

            systematicTree->makeOutputVariable(HTT_n, "HTT_n");
            systematicTree->makeOutputVariable(HTT_masswindow_n, "HTT_masswindow_n");


            systematicTree->makeOutputVariable(HTT_atan1312, "HTT_atan1312");
            systematicTree->makeOutputVariable(HTT_m23m123, "HTT_m23m123");

            // Di-top variables
            systematicTree->makeOutputVariable(HTT_tt_m, "HTT_tt_m");
            systematicTree->makeOutputVariable(HTT_tt_pt, "HTT_tt_pt");

        }

        // saving truth jet
        if (m_saveTruthJets) {
            systematicTree->makeOutputVariable(m_akt4truthjet_pt,  "akt4truthjet_pt");
            systematicTree->makeOutputVariable(m_akt4truthjet_eta, "akt4truthjet_eta");
            systematicTree->makeOutputVariable(m_akt4truthjet_phi, "akt4truthjet_phi");
            systematicTree->makeOutputVariable(m_akt4truthjet_m,   "akt4truthjet_m");

            systematicTree->makeOutputVariable(m_akt10truthjet_pt,  "akt10truthjet_pt");
            systematicTree->makeOutputVariable(m_akt10truthjet_eta, "akt10truthjet_eta");
            systematicTree->makeOutputVariable(m_akt10truthjet_phi, "akt10truthjet_phi");
            systematicTree->makeOutputVariable(m_akt10truthjet_m,   "akt10truthjet_m");
            systematicTree->makeOutputVariable(m_akt10truthjet_tau32, "akt10truthjet_tau32");
            systematicTree->makeOutputVariable(m_akt10truthjet_tau32_wta, "akt10truthjet_tau32_wta");
        }

        if (m_savePartons) {

            // ME
            systematicTree->makeOutputVariable(m_MC_px_me, "MC_px_me");
            systematicTree->makeOutputVariable(m_MC_py_me, "MC_py_me");
            systematicTree->makeOutputVariable(m_MC_pz_me, "MC_pz_me");
            systematicTree->makeOutputVariable(m_MC_e_me,  "MC_e_me");
            systematicTree->makeOutputVariable(m_MC_m_me,  "MC_m_me");
            systematicTree->makeOutputVariable(m_MC_id_me, "MC_id_me");
            systematicTree->makeOutputVariable(m_MC_Q_me,      "MC_Q_me");
            systematicTree->makeOutputVariable(m_MC_QxAOD_me,  "MC_QxAOD_me");
            systematicTree->makeOutputVariable(m_MC_aSxAOD_me, "MC_aSxAOD_me");

            //Partons
            systematicTree->makeOutputVariable(m_MC_b_from_t_pt,      "MC_b_from_t_pt");
            systematicTree->makeOutputVariable(m_MC_b_from_t_eta,     "MC_b_from_t_eta");
            systematicTree->makeOutputVariable(m_MC_b_from_t_phi,     "MC_b_from_t_phi");
            systematicTree->makeOutputVariable(m_MC_b_from_t_m,       "MC_b_from_t_m");

            systematicTree->makeOutputVariable(m_MC_b_from_tbar_pt,      "MC_b_from_tbar_pt");
            systematicTree->makeOutputVariable(m_MC_b_from_tbar_eta,     "MC_b_from_tbar_eta");
            systematicTree->makeOutputVariable(m_MC_b_from_tbar_phi,     "MC_b_from_tbar_phi");
            systematicTree->makeOutputVariable(m_MC_b_from_tbar_m,       "MC_b_from_tbar_m");

            systematicTree->makeOutputVariable(m_MC_Wdecay1_from_t_pt,      "MC_Wdecay1_from_t_pt");
            systematicTree->makeOutputVariable(m_MC_Wdecay1_from_t_eta,     "MC_Wdecay1_from_t_eta");
            systematicTree->makeOutputVariable(m_MC_Wdecay1_from_t_phi,     "MC_Wdecay1_from_t_phi");
            systematicTree->makeOutputVariable(m_MC_Wdecay1_from_t_m,       "MC_Wdecay1_from_t_m");
            systematicTree->makeOutputVariable(m_MC_Wdecay1_from_t_pdgId,   "MC_Wdecay1_from_t_pdgId");

            systematicTree->makeOutputVariable(m_MC_Wdecay2_from_t_pt,      "MC_Wdecay2_from_t_pt");
            systematicTree->makeOutputVariable(m_MC_Wdecay2_from_t_eta,     "MC_Wdecay2_from_t_eta");
            systematicTree->makeOutputVariable(m_MC_Wdecay2_from_t_phi,     "MC_Wdecay2_from_t_phi");
            systematicTree->makeOutputVariable(m_MC_Wdecay2_from_t_m,       "MC_Wdecay2_from_t_m");
            systematicTree->makeOutputVariable(m_MC_Wdecay2_from_t_pdgId,   "MC_Wdecay2_from_t_pdgId");

            systematicTree->makeOutputVariable(m_MC_Wdecay1_from_tbar_pt,      "MC_Wdecay1_from_tbar_pt");
            systematicTree->makeOutputVariable(m_MC_Wdecay1_from_tbar_eta,     "MC_Wdecay1_from_tbar_eta");
            systematicTree->makeOutputVariable(m_MC_Wdecay1_from_tbar_phi,     "MC_Wdecay1_from_tbar_phi");
            systematicTree->makeOutputVariable(m_MC_Wdecay1_from_tbar_m,       "MC_Wdecay1_from_tbar_m");
            systematicTree->makeOutputVariable(m_MC_Wdecay1_from_tbar_pdgId,   "MC_Wdecay1_from_tbar_pdgId");

            systematicTree->makeOutputVariable(m_MC_Wdecay2_from_tbar_pt,      "MC_Wdecay2_from_tbar_pt");
            systematicTree->makeOutputVariable(m_MC_Wdecay2_from_tbar_eta,     "MC_Wdecay2_from_tbar_eta");
            systematicTree->makeOutputVariable(m_MC_Wdecay2_from_tbar_phi,     "MC_Wdecay2_from_tbar_phi");
            systematicTree->makeOutputVariable(m_MC_Wdecay2_from_tbar_m,       "MC_Wdecay2_from_tbar_m");
            systematicTree->makeOutputVariable(m_MC_Wdecay2_from_tbar_pdgId,   "MC_Wdecay2_from_tbar_pdgId");

            systematicTree->makeOutputVariable(m_MC_W_from_t_pt,      "MC_W_from_t_pt");
            systematicTree->makeOutputVariable(m_MC_W_from_t_eta,     "MC_W_from_t_eta");
            systematicTree->makeOutputVariable(m_MC_W_from_t_phi,     "MC_W_from_t_phi");
            systematicTree->makeOutputVariable(m_MC_W_from_t_m,       "MC_W_from_t_m");

            systematicTree->makeOutputVariable(m_MC_W_from_tbar_pt,  "MC_W_from_tbar_pt");
            systematicTree->makeOutputVariable(m_MC_W_from_tbar_eta,  "MC_W_from_tbar_eta");
            systematicTree->makeOutputVariable(m_MC_W_from_tbar_phi,  "MC_W_from_tbar_phi");
            systematicTree->makeOutputVariable(m_MC_W_from_tbar_m,    "MC_W_from_tbar_m");

            systematicTree->makeOutputVariable(m_MC_i1_px,        "MC_i1_px");
            systematicTree->makeOutputVariable(m_MC_i1_py,        "MC_i1_py");
            systematicTree->makeOutputVariable(m_MC_i1_pz,        "MC_i1_pz");
            systematicTree->makeOutputVariable(m_MC_i1_m,         "MC_i1_m");
            systematicTree->makeOutputVariable(m_MC_i1_pid,       "MC_i1_pid");

            systematicTree->makeOutputVariable(m_MC_i2_px,   "MC_i2_px");
            systematicTree->makeOutputVariable(m_MC_i2_py,    "MC_i2_py");
            systematicTree->makeOutputVariable(m_MC_i2_pz,    "MC_i2_pz");
            systematicTree->makeOutputVariable(m_MC_i2_m,     "MC_i2_m");
            systematicTree->makeOutputVariable(m_MC_i2_pid,   "MC_i2_pid");

            systematicTree->makeOutputVariable(m_MC_t_pt,      "MC_t_pt");
            systematicTree->makeOutputVariable(m_MC_t_eta,     "MC_t_eta");
            systematicTree->makeOutputVariable(m_MC_t_phi,     "MC_t_phi");
            systematicTree->makeOutputVariable(m_MC_t_m,       "MC_t_m");

            systematicTree->makeOutputVariable(m_MC_tbar_pt,    "MC_tbar_pt");
            systematicTree->makeOutputVariable(m_MC_tbar_eta,    "MC_tbar_eta");
            systematicTree->makeOutputVariable(m_MC_tbar_phi,    "MC_tbar_phi");
            systematicTree->makeOutputVariable(m_MC_tbar_m,   "MC_tbar_m");

            // Top quark four-vector after FSR
            systematicTree->makeOutputVariable(m_MC_t_afterFSR_pt,    "MC_t_afterFSR_pt");
            systematicTree->makeOutputVariable(m_MC_t_afterFSR_eta,    "MC_t_afterFSR_eta");
            systematicTree->makeOutputVariable(m_MC_t_afterFSR_phi,    "MC_t_afterFSR_phi");
            systematicTree->makeOutputVariable(m_MC_t_afterFSR_m,   "MC_t_afterFSR_m");

            systematicTree->makeOutputVariable(m_MC_tbar_afterFSR_pt,    "MC_tbar_afterFSR_pt");
            systematicTree->makeOutputVariable(m_MC_tbar_afterFSR_eta,    "MC_tbar_afterFSR_eta");
            systematicTree->makeOutputVariable(m_MC_tbar_afterFSR_phi,    "MC_tbar_afterFSR_phi");
            systematicTree->makeOutputVariable(m_MC_tbar_afterFSR_m,   "MC_tbar_afterFSR_m");
            //
            systematicTree->makeOutputVariable(m_MC_ttbar_beforeFSR_pt,   "MC_ttbar_beforeFSR_pt");
            systematicTree->makeOutputVariable(m_MC_ttbar_beforeFSR_eta,  "MC_ttbar_beforeFSR_eta");
            systematicTree->makeOutputVariable(m_MC_ttbar_beforeFSR_phi,  "MC_ttbar_beforeFSR_phi");
            systematicTree->makeOutputVariable(m_MC_ttbar_beforeFSR_m,    "MC_ttbar_beforeFSR_m");

            systematicTree->makeOutputVariable(m_MC_ttbar_afterFSR_pt,   "MC_ttbar_afterFSR_pt");
            systematicTree->makeOutputVariable(m_MC_ttbar_afterFSR_eta,  "MC_ttbar_afterFSR_eta");
            systematicTree->makeOutputVariable(m_MC_ttbar_afterFSR_phi,  "MC_ttbar_afterFSR_phi");
            systematicTree->makeOutputVariable(m_MC_ttbar_afterFSR_m,    "MC_ttbar_afterFSR_m");
            systematicTree->makeOutputVariable(m_MC_ttbar_type, "MC_ttbar_type");

            //Matched jets
            systematicTree->makeOutputVariable(m_MA_b_from_t_pt,      "MA_b_from_t_pt");
            systematicTree->makeOutputVariable(m_MA_b_from_t_eta,     "MA_b_from_t_eta");
            systematicTree->makeOutputVariable(m_MA_b_from_t_phi,     "MA_b_from_t_phi");
            systematicTree->makeOutputVariable(m_MA_b_from_t_m,       "MA_b_from_t_m");
            systematicTree->makeOutputVariable(m_MA_b_from_t_recoidx, "MA_b_from_t_recoidx");
            systematicTree->makeOutputVariable(m_MA_b_from_t_recodr,  "MA_b_from_t_recodr");
            systematicTree->makeOutputVariable(m_MA_b_from_t_truthdr, "MA_b_from_t_truthdr");

            systematicTree->makeOutputVariable(m_MA_b_from_tbar_pt,      "MA_b_from_tbar_pt");
            systematicTree->makeOutputVariable(m_MA_b_from_tbar_eta,     "MA_b_from_tbar_eta");
            systematicTree->makeOutputVariable(m_MA_b_from_tbar_phi,     "MA_b_from_tbar_phi");
            systematicTree->makeOutputVariable(m_MA_b_from_tbar_m,       "MA_b_from_tbar_m");
            systematicTree->makeOutputVariable(m_MA_b_from_tbar_recoidx, "MA_b_from_tbar_recoidx");
            systematicTree->makeOutputVariable(m_MA_b_from_tbar_recodr,  "MA_b_from_tbar_recodr");
            systematicTree->makeOutputVariable(m_MA_b_from_tbar_truthdr, "MA_b_from_tbar_truthdr");

            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_t_pt,      "MA_Wdecay1_from_t_pt");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_t_eta,     "MA_Wdecay1_from_t_eta");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_t_phi,     "MA_Wdecay1_from_t_phi");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_t_m,       "MA_Wdecay1_from_t_m");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_t_recoidx, "MA_Wdecay1_from_t_recoidx");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_t_recodr,  "MA_Wdecay1_from_t_recodr");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_t_truthdr, "MA_Wdecay1_from_t_truthdr");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_t_NuDphi,  "MA_Wdecay1_from_t_NuDphi");

            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_t_pt,      "MA_Wdecay2_from_t_pt");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_t_eta,     "MA_Wdecay2_from_t_eta");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_t_phi,     "MA_Wdecay2_from_t_phi");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_t_m,       "MA_Wdecay2_from_t_m");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_t_recoidx, "MA_Wdecay2_from_t_recoidx");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_t_recodr,  "MA_Wdecay2_from_t_recodr");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_t_truthdr, "MA_Wdecay2_from_t_truthdr");

            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_tbar_pt,      "MA_Wdecay1_from_tbar_pt");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_tbar_eta,     "MA_Wdecay1_from_tbar_eta");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_tbar_phi,     "MA_Wdecay1_from_tbar_phi");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_tbar_m,       "MA_Wdecay1_from_tbar_m");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_tbar_recoidx, "MA_Wdecay1_from_tbar_recoidx");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_tbar_recodr,  "MA_Wdecay1_from_tbar_recodr");
            systematicTree->makeOutputVariable(m_MA_Wdecay1_from_tbar_truthdr, "MA_Wdecay1_from_tbar_truthdr");

            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_tbar_pt,      "MA_Wdecay2_from_tbar_pt");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_tbar_eta,     "MA_Wdecay2_from_tbar_eta");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_tbar_phi,     "MA_Wdecay2_from_tbar_phi");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_tbar_m,       "MA_Wdecay2_from_tbar_m");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_tbar_recoidx, "MA_Wdecay2_from_tbar_recoidx");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_tbar_recodr,  "MA_Wdecay2_from_tbar_recodr");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_tbar_truthdr, "MA_Wdecay2_from_tbar_truthdr");
            systematicTree->makeOutputVariable(m_MA_Wdecay2_from_tbar_NuDphi,  "MA_Wdecay2_from_tbar_NuDphi");

            // Reconstructed particles from the matched jets
            systematicTree->makeOutputVariable(m_MA_W_from_t_pt,    "MA_W_from_t_pt");
            systematicTree->makeOutputVariable(m_MA_W_from_t_eta,   "MA_W_from_t_eta");
            systematicTree->makeOutputVariable(m_MA_W_from_t_phi,   "MA_W_from_t_phi");
            systematicTree->makeOutputVariable(m_MA_W_from_t_m,     "MA_W_from_t_m");
            systematicTree->makeOutputVariable(m_MA_W_from_t_Njets, "MA_W_from_t_Njets");

            systematicTree->makeOutputVariable(m_MA_W_from_tbar_pt,    "MA_W_from_tbar_pt");
            systematicTree->makeOutputVariable(m_MA_W_from_tbar_eta,   "MA_W_from_tbar_eta");
            systematicTree->makeOutputVariable(m_MA_W_from_tbar_phi,   "MA_W_from_tbar_phi");
            systematicTree->makeOutputVariable(m_MA_W_from_tbar_m,     "MA_W_from_tbar_m");
            systematicTree->makeOutputVariable(m_MA_W_from_tbar_Njets, "MA_W_from_tbar_Njets");

            systematicTree->makeOutputVariable(m_MA_t_pt,    "MA_t_pt");
            systematicTree->makeOutputVariable(m_MA_t_eta,   "MA_t_eta");
            systematicTree->makeOutputVariable(m_MA_t_phi,   "MA_t_phi");
            systematicTree->makeOutputVariable(m_MA_t_m,     "MA_t_m");
            systematicTree->makeOutputVariable(m_MA_t_Njets, "MA_t_Njets");

            systematicTree->makeOutputVariable(m_MA_tbar_pt,    "MA_tbar_pt");
            systematicTree->makeOutputVariable(m_MA_tbar_eta,   "MA_tbar_eta");
            systematicTree->makeOutputVariable(m_MA_tbar_phi,   "MA_tbar_phi");
            systematicTree->makeOutputVariable(m_MA_tbar_m,     "MA_tbar_m");
            systematicTree->makeOutputVariable(m_MA_tbar_Njets, "MA_tbar_Njets");

            // th from akt0.4 jets
            systematicTree->makeOutputVariable(m_MA_ttbar_pt,   "MA_ttbar_pt");
            systematicTree->makeOutputVariable(m_MA_ttbar_eta,  "MA_ttbar_eta");
            systematicTree->makeOutputVariable(m_MA_ttbar_phi,  "MA_ttbar_phi");
            systematicTree->makeOutputVariable(m_MA_ttbar_m,    "MA_ttbar_m");

            // th from akt1.0 jet
            systematicTree->makeOutputVariable(m_MA_ttbarFJ_pt,   "MA_ttbarFJ_pt");
            systematicTree->makeOutputVariable(m_MA_ttbarFJ_eta,  "MA_ttbarFJ_eta");
            systematicTree->makeOutputVariable(m_MA_ttbarFJ_phi,  "MA_ttbarFJ_phi");
            systematicTree->makeOutputVariable(m_MA_ttbarFJ_m,    "MA_ttbarFJ_m");

            // only for l+jets
            systematicTree->makeOutputVariable(m_MA_bh_pt,    "MA_bh_pt");
            systematicTree->makeOutputVariable(m_MA_bh_eta,   "MA_bh_eta");
            systematicTree->makeOutputVariable(m_MA_bh_phi,   "MA_bh_phi");
            systematicTree->makeOutputVariable(m_MA_bh_m,     "MA_bh_m");
            systematicTree->makeOutputVariable(m_MA_bh_idx,   "MA_bh_idx");
            systematicTree->makeOutputVariable(m_MA_bh_dr,    "MA_bh_dr");
            systematicTree->makeOutputVariable(m_MA_bh_tdr,   "MA_bh_tdr");

            systematicTree->makeOutputVariable(m_MA_bl_pt,    "MA_bl_pt");
            systematicTree->makeOutputVariable(m_MA_bl_eta,   "MA_bl_eta");
            systematicTree->makeOutputVariable(m_MA_bl_phi,   "MA_bl_phi");
            systematicTree->makeOutputVariable(m_MA_bl_m,     "MA_bl_m");
            systematicTree->makeOutputVariable(m_MA_bl_idx,   "MA_bl_idx");
            systematicTree->makeOutputVariable(m_MA_bl_dr,    "MA_bl_dr");
            systematicTree->makeOutputVariable(m_MA_bl_tdr,   "MA_bl_tdr");

            systematicTree->makeOutputVariable(m_MA_wh_pt,     "MA_wh_pt");
            systematicTree->makeOutputVariable(m_MA_wh_eta,    "MA_wh_eta");
            systematicTree->makeOutputVariable(m_MA_wh_phi,    "MA_wh_phi");
            systematicTree->makeOutputVariable(m_MA_wh_m,      "MA_wh_m");
            systematicTree->makeOutputVariable(m_MA_wh_Njets,  "MA_wh_Njets");

            systematicTree->makeOutputVariable(m_MA_w1h_pt,    "MA_w1h_pt");
            systematicTree->makeOutputVariable(m_MA_w1h_eta,   "MA_w1h_eta");
            systematicTree->makeOutputVariable(m_MA_w1h_phi,   "MA_w1h_phi");
            systematicTree->makeOutputVariable(m_MA_w1h_m,     "MA_w1h_m");
            systematicTree->makeOutputVariable(m_MA_w1h_pdgId, "MA_w1h_pdgId");
            systematicTree->makeOutputVariable(m_MA_w1h_idx,   "MA_w1h_idx");
            systematicTree->makeOutputVariable(m_MA_w1h_dr,    "MA_w1h_dr");
            systematicTree->makeOutputVariable(m_MA_w1h_tdr,   "MA_w1h_tdr");

            systematicTree->makeOutputVariable(m_MA_w2h_pt,    "MA_w2h_pt");
            systematicTree->makeOutputVariable(m_MA_w2h_eta,   "MA_w2h_eta");
            systematicTree->makeOutputVariable(m_MA_w2h_phi,   "MA_w2h_phi");
            systematicTree->makeOutputVariable(m_MA_w2h_m,     "MA_w2h_m");
            systematicTree->makeOutputVariable(m_MA_w2h_pdgId, "MA_w2h_pdgId");
            systematicTree->makeOutputVariable(m_MA_w2h_idx,   "MA_w2h_idx");
            systematicTree->makeOutputVariable(m_MA_w2h_dr,    "MA_w2h_dr");
            systematicTree->makeOutputVariable(m_MA_w2h_tdr,   "MA_w2h_tdr");

            systematicTree->makeOutputVariable(m_MA_wl_pt,     "MA_wl_pt");
            systematicTree->makeOutputVariable(m_MA_wl_eta,    "MA_wl_eta");
            systematicTree->makeOutputVariable(m_MA_wl_phi,    "MA_wl_phi");
            systematicTree->makeOutputVariable(m_MA_wl_m,      "MA_wl_m");

            systematicTree->makeOutputVariable(m_MA_w1l_pt,    "MA_w1l_pt");
            systematicTree->makeOutputVariable(m_MA_w1l_eta,   "MA_w1l_eta");
            systematicTree->makeOutputVariable(m_MA_w1l_phi,   "MA_w1l_phi");
            systematicTree->makeOutputVariable(m_MA_w1l_m,     "MA_w1l_m");
            systematicTree->makeOutputVariable(m_MA_w1l_pdgId, "MA_w1l_pdgId");
            systematicTree->makeOutputVariable(m_MA_w1l_idx,   "MA_w1l_idx");
            systematicTree->makeOutputVariable(m_MA_w1l_dr,    "MA_w1l_dr");
            systematicTree->makeOutputVariable(m_MA_w1l_tdr,   "MA_w1l_tdr");
            systematicTree->makeOutputVariable(m_MA_w1l_NuDphi, "MA_w1l_NuDphi");

            systematicTree->makeOutputVariable(m_MA_w2l_pt,    "MA_w2l_pt");
            systematicTree->makeOutputVariable(m_MA_w2l_eta,   "MA_w2l_eta");
            systematicTree->makeOutputVariable(m_MA_w2l_phi,   "MA_w2l_phi");
            systematicTree->makeOutputVariable(m_MA_w2l_m,     "MA_w2l_m");
            systematicTree->makeOutputVariable(m_MA_w2l_pdgId, "MA_w2l_pdgId");
            systematicTree->makeOutputVariable(m_MA_w2l_idx,   "MA_w2l_idx");
            systematicTree->makeOutputVariable(m_MA_w2l_dr,    "MA_w2l_dr");
            systematicTree->makeOutputVariable(m_MA_w2l_tdr,   "MA_w2l_tdr");
            systematicTree->makeOutputVariable(m_MA_w2l_NuDphi, "MA_w2l_NuDphi");

            // th from akt0.4 jets
            systematicTree->makeOutputVariable(m_MA_th_pt,    "MA_th_pt");
            systematicTree->makeOutputVariable(m_MA_th_eta,   "MA_th_eta");
            systematicTree->makeOutputVariable(m_MA_th_phi,   "MA_th_phi");
            systematicTree->makeOutputVariable(m_MA_th_m,     "MA_th_m");
            systematicTree->makeOutputVariable(m_MA_th_Njets, "MA_th_Njets");

            // th from akt1.0 jet
            systematicTree->makeOutputVariable(m_MA_thFJ_pt,    "MA_thFJ_pt");
            systematicTree->makeOutputVariable(m_MA_thFJ_eta,   "MA_thFJ_eta");
            systematicTree->makeOutputVariable(m_MA_thFJ_phi,   "MA_thFJ_phi");
            systematicTree->makeOutputVariable(m_MA_thFJ_m,     "MA_thFJ_m");
            systematicTree->makeOutputVariable(m_MA_thFJ_nPartons, "MA_thFJ_nPartons");

            systematicTree->makeOutputVariable(m_MA_tl_pt,    "MA_tl_pt");
            systematicTree->makeOutputVariable(m_MA_tl_eta,   "MA_tl_eta");
            systematicTree->makeOutputVariable(m_MA_tl_phi,   "MA_tl_phi");
            systematicTree->makeOutputVariable(m_MA_tl_m,     "MA_tl_m");
            systematicTree->makeOutputVariable(m_MA_tl_Njets, "MA_tl_Njets");

            systematicTree->makeOutputVariable(m_MA_ttbar_lpj_pt,    "MA_ttbar_lpj_pt");
            systematicTree->makeOutputVariable(m_MA_ttbar_lpj_eta,   "MA_ttbar_lpj_eta");
            systematicTree->makeOutputVariable(m_MA_ttbar_lpj_phi,   "MA_ttbar_lpj_phi");
            systematicTree->makeOutputVariable(m_MA_ttbar_lpj_m,     "MA_ttbar_lpj_m");
            systematicTree->makeOutputVariable(m_MA_ttbar_lpj_Njets, "MA_ttbar_lpj_Njets");

            //partons in l+jets
            systematicTree->makeOutputVariable(m_MC_bh_pt,      "MC_bh_pt");
            systematicTree->makeOutputVariable(m_MC_bh_eta,     "MC_bh_eta");
            systematicTree->makeOutputVariable(m_MC_bh_phi,     "MC_bh_phi");
            systematicTree->makeOutputVariable(m_MC_bh_m,   "MC_bh_m");

            systematicTree->makeOutputVariable(m_MC_bl_pt,      "MC_bl_pt");
            systematicTree->makeOutputVariable(m_MC_bl_eta,     "MC_bl_eta");
            systematicTree->makeOutputVariable(m_MC_bl_phi,     "MC_bl_phi");
            systematicTree->makeOutputVariable(m_MC_bl_m,   "MC_bl_m");

            systematicTree->makeOutputVariable(m_MC_w1h_pt,      "MC_w1h_pt");
            systematicTree->makeOutputVariable(m_MC_w1h_eta,     "MC_w1h_eta");
            systematicTree->makeOutputVariable(m_MC_w1h_phi,     "MC_w1h_phi");
            systematicTree->makeOutputVariable(m_MC_w1h_m,       "MC_w1h_m");
            systematicTree->makeOutputVariable(m_MC_w1h_pdgId,   "MC_w1h_pdgId");

            systematicTree->makeOutputVariable(m_MC_w2h_pt,      "MC_w2h_pt");
            systematicTree->makeOutputVariable(m_MC_w2h_eta,     "MC_w2h_eta");
            systematicTree->makeOutputVariable(m_MC_w2h_phi,     "MC_w2h_phi");
            systematicTree->makeOutputVariable(m_MC_w2h_m,       "MC_w2h_m");
            systematicTree->makeOutputVariable(m_MC_w2h_pdgId,   "MC_w2h_pdgId");

            systematicTree->makeOutputVariable(m_MC_wh_pt,      "MC_wh_pt");
            systematicTree->makeOutputVariable(m_MC_wh_eta,     "MC_wh_eta");
            systematicTree->makeOutputVariable(m_MC_wh_phi,     "MC_wh_phi");
            systematicTree->makeOutputVariable(m_MC_wh_m,       "MC_wh_m");

            systematicTree->makeOutputVariable(m_MC_w1l_pt,      "MC_w1l_pt");
            systematicTree->makeOutputVariable(m_MC_w1l_eta,     "MC_w1l_eta");
            systematicTree->makeOutputVariable(m_MC_w1l_phi,     "MC_w1l_phi");
            systematicTree->makeOutputVariable(m_MC_w1l_m,       "MC_w1l_m");
            systematicTree->makeOutputVariable(m_MC_w1l_pdgId,   "MC_w1l_pdgId");

            systematicTree->makeOutputVariable(m_MC_w2l_pt,      "MC_w2l_pt");
            systematicTree->makeOutputVariable(m_MC_w2l_eta,     "MC_w2l_eta");
            systematicTree->makeOutputVariable(m_MC_w2l_phi,     "MC_w2l_phi");
            systematicTree->makeOutputVariable(m_MC_w2l_m,       "MC_w2l_m");
            systematicTree->makeOutputVariable(m_MC_w2l_pdgId,   "MC_w2l_pdgId");

            systematicTree->makeOutputVariable(m_MC_wl_pt,      "MC_wl_pt");
            systematicTree->makeOutputVariable(m_MC_wl_eta,     "MC_wl_eta");
            systematicTree->makeOutputVariable(m_MC_wl_phi,     "MC_wl_phi");
            systematicTree->makeOutputVariable(m_MC_wl_m,       "MC_wl_m");

            systematicTree->makeOutputVariable(m_MC_th_pt,    "MC_th_pt");
            systematicTree->makeOutputVariable(m_MC_th_eta,   "MC_th_eta");
            systematicTree->makeOutputVariable(m_MC_th_phi,   "MC_th_phi");
            systematicTree->makeOutputVariable(m_MC_th_m,     "MC_th_m");

            systematicTree->makeOutputVariable(m_MC_tl_pt,    "MC_tl_pt");
            systematicTree->makeOutputVariable(m_MC_tl_eta,   "MC_tl_eta");
            systematicTree->makeOutputVariable(m_MC_tl_phi,   "MC_tl_phi");
            systematicTree->makeOutputVariable(m_MC_tl_m,     "MC_tl_m");

            systematicTree->makeOutputVariable(m_MC_ttbar_lpj_beforeFSR_pt,   "MC_ttbar_lpj_beforeFSR_pt");
            systematicTree->makeOutputVariable(m_MC_ttbar_lpj_beforeFSR_eta,  "MC_ttbar_lpj_beforeFSR_eta");
            systematicTree->makeOutputVariable(m_MC_ttbar_lpj_beforeFSR_phi,  "MC_ttbar_lpj_beforeFSR_phi");
            systematicTree->makeOutputVariable(m_MC_ttbar_lpj_beforeFSR_m,    "MC_ttbar_lpj_beforeFSR_m");

            systematicTree->makeOutputVariable(m_MC_ttbar_lpj_afterFSR_pt,   "MC_ttbar_lpj_afterFSR_pt");
            systematicTree->makeOutputVariable(m_MC_ttbar_lpj_afterFSR_eta,  "MC_ttbar_lpj_afterFSR_eta");
            systematicTree->makeOutputVariable(m_MC_ttbar_lpj_afterFSR_phi,  "MC_ttbar_lpj_afterFSR_phi");
            systematicTree->makeOutputVariable(m_MC_ttbar_lpj_afterFSR_m,    "MC_ttbar_lpj_afterFSR_m");

            // chi2 in l+jets
            if (!m_runHtt) {
                systematicTree->makeOutputVariable(m_chi2_bh_pt,      "chi2_bh_pt");
                systematicTree->makeOutputVariable(m_chi2_bh_eta,     "chi2_bh_eta");
                systematicTree->makeOutputVariable(m_chi2_bh_phi,     "chi2_bh_phi");
                systematicTree->makeOutputVariable(m_chi2_bh_m, "chi2_bh_m");

                systematicTree->makeOutputVariable(m_chi2_bl_pt,      "chi2_bl_pt");
                systematicTree->makeOutputVariable(m_chi2_bl_eta,     "chi2_bl_eta");
                systematicTree->makeOutputVariable(m_chi2_bl_phi,     "chi2_bl_phi");
                systematicTree->makeOutputVariable(m_chi2_bl_m, "chi2_bl_m");

//        systematicTree->makeOutputVariable(m_chi2_w1h_pt,      "chi2_w1h_pt");
//             systematicTree->makeOutputVariable(m_chi2_w1h_eta,     "chi2_w1h_eta");
//             systematicTree->makeOutputVariable(m_chi2_w1h_phi,     "chi2_w1h_phi");
//             systematicTree->makeOutputVariable(m_chi2_w1h_m,       "chi2_w1h_m");
//        systematicTree->makeOutputVariable(m_chi2_w1h_pdgId,   "chi2_w1h_pdgId");
//
//        systematicTree->makeOutputVariable(m_chi2_w2h_pt,      "chi2_w2h_pt");
//             systematicTree->makeOutputVariable(m_chi2_w2h_eta,     "chi2_w2h_eta");
//             systematicTree->makeOutputVariable(m_chi2_w2h_phi,     "chi2_w2h_phi");
//             systematicTree->makeOutputVariable(m_chi2_w2h_m,       "chi2_w2h_m");
//        systematicTree->makeOutputVariable(m_chi2_w2h_pdgId,   "chi2_w2h_pdgId");

                systematicTree->makeOutputVariable(m_chi2_wh_pt,      "chi2_wh_pt");
                systematicTree->makeOutputVariable(m_chi2_wh_eta,     "chi2_wh_eta");
                systematicTree->makeOutputVariable(m_chi2_wh_phi,     "chi2_wh_phi");
                systematicTree->makeOutputVariable(m_chi2_wh_m,       "chi2_wh_m");
//
//        systematicTree->makeOutputVariable(m_chi2_w1l_pt,      "chi2_w1l_pt");
//             systematicTree->makeOutputVariable(m_chi2_w1l_eta,     "chi2_w1l_eta");
//             systematicTree->makeOutputVariable(m_chi2_w1l_phi,     "chi2_w1l_phi");
//             systematicTree->makeOutputVariable(m_chi2_w1l_m,       "chi2_w1l_m");
//        systematicTree->makeOutputVariable(m_chi2_w1l_pdgId,   "chi2_w1l_pdgId");
//
//        systematicTree->makeOutputVariable(m_chi2_w2l_pt,      "chi2_w2l_pt");
//             systematicTree->makeOutputVariable(m_chi2_w2l_eta,     "chi2_w2l_eta");
//             systematicTree->makeOutputVariable(m_chi2_w2l_phi,     "chi2_w2l_phi");
//             systematicTree->makeOutputVariable(m_chi2_w2l_m,       "chi2_w2l_m");
//        systematicTree->makeOutputVariable(m_chi2_w2l_pdgId,   "chi2_w2l_pdgId");

                systematicTree->makeOutputVariable(m_chi2_wl_pt,      "chi2_wl_pt");
                systematicTree->makeOutputVariable(m_chi2_wl_eta,     "chi2_wl_eta");
                systematicTree->makeOutputVariable(m_chi2_wl_phi,     "chi2_wl_phi");
                systematicTree->makeOutputVariable(m_chi2_wl_m,       "chi2_wl_m");

                systematicTree->makeOutputVariable(m_chi2_th_pt,    "chi2_th_pt");
                systematicTree->makeOutputVariable(m_chi2_th_eta,   "chi2_th_eta");
                systematicTree->makeOutputVariable(m_chi2_th_phi,   "chi2_th_phi");
                systematicTree->makeOutputVariable(m_chi2_th_m,     "chi2_th_m");

                systematicTree->makeOutputVariable(m_chi2_tl_pt,    "chi2_tl_pt");
                systematicTree->makeOutputVariable(m_chi2_tl_eta,   "chi2_tl_eta");
                systematicTree->makeOutputVariable(m_chi2_tl_phi,   "chi2_tl_phi");
                systematicTree->makeOutputVariable(m_chi2_tl_m,     "chi2_tl_m");

                systematicTree->makeOutputVariable(m_chi2_ttbar_pt,    "chi2_ttbar_pt");
                systematicTree->makeOutputVariable(m_chi2_ttbar_eta,   "chi2_ttbar_eta");
                systematicTree->makeOutputVariable(m_chi2_ttbar_phi,   "chi2_ttbar_phi");
                systematicTree->makeOutputVariable(m_chi2_ttbar_m,     "chi2_ttbar_m");
            }
        }
    }

    std::cout << "INFO: TtresEventSaverFlatNtuple has been initialized" << std::endl;
}

void TtresEventSaverFlatNtuple::saveParticleLevelEvent(const top::ParticleLevelEvent& event) {
    if ( m_config->useTruthLargeRJets() ) {
        m_part_ljet_tau32.resize(event.m_largeRJets->size());
        m_part_ljet_tau32_wta.resize(event.m_largeRJets->size());

        int i = 0;
        for (const auto & jetPtr : * event.m_largeRJets) {
            try { m_part_ljet_tau32[i] = jetPtr->getAttribute<float>("Tau3") / jetPtr->getAttribute<float>("Tau2"); } catch (...) { }
            try { m_part_ljet_tau32_wta[i] = jetPtr->getAttribute<float>("Tau3_wta") / jetPtr->getAttribute<float>("Tau2_wta"); } catch (...) { }
            ++i;
        }
    }

    EventSaverFlatNtuple::saveParticleLevelEvent(event);
}

void TtresEventSaverFlatNtuple::saveEvent(const top::Event& event) {
    //calculate our extra variable:
#ifdef ENABLE_BTAG_DEBUG
    std::cout << "DEBUG: TtresEventSaverFlatNtuple::saveEvent" << std::endl;
#endif
    const xAOD::JetContainer* trackjets = &(event.m_trackJets);
    m_lumiblock = event.m_info->lumiBlock();
    int hadtop_index = -1;
    float max_pt = -1.0;

    m_vtxz = 0;
    m_npv = 0;
    //const xAOD::VertexContainer* vtx(nullptr);
    //top::check(evtStore()->retrieve(vtx,config->sgKeyPrimaryVertices()), "Failed to retrieve Primary Vertices");
    const xAOD::VertexContainer *m_primvtx = event.m_primaryVertices;
    for (const auto* const vtxPtr : *m_primvtx) {
        const xAOD::VxType::VertexType vtype = vtxPtr->vertexType();
        const int vmult = vtxPtr->trackParticleLinks().size();
        if ((vtype == xAOD::VxType::PriVtx || vtype == xAOD::VxType::PileUp) && vmult > 4) {
            ++m_npv;
            // assuming there is only one primary vertex
            if (vtype == 1)  m_vtxz = vtxPtr->z();
        }
    }//for

    //electrons
    m_el_d0.resize(event.m_electrons.size());
    m_el_z0.resize(event.m_electrons.size());
    //m_el_d0sig.resize(event.m_electrons.size());
    m_el_z0sig.resize(event.m_electrons.size());


    unsigned int k(0);
    for (const auto* const elPtr : event.m_electrons) {//loop on electrons
        // access Electron TrackParticle for d0/z0 information if avaialable
        //m_el_d0sig[k] = 0.;
        const xAOD::TrackParticle* eltrack = elPtr->trackParticle();
        m_el_d0[k] = eltrack->d0();
        // z0 is expressed internally wrt tracks defining vertex (=beamspot)
        // subtract off primary vtx z to get z0 wrt primary vertex
        m_el_z0[k] = eltrack->z0() + eltrack->vz() - m_vtxz;
        const xAOD::ParametersCovMatrix_t elcov = eltrack->definingParametersCovMatrix();
        //m_el_d0sig[k] = m_el_d0[k]/sqrt(elcov(0,0));
        m_el_z0sig[k] = m_el_z0[k] / sqrt(elcov(1, 1));
    }//for

    //muons
    m_mu_d0.resize(event.m_muons.size());
    m_mu_z0.resize(event.m_muons.size());
    //m_mu_d0sig.resize(event.m_muons.size());
    m_mu_z0sig.resize(event.m_muons.size());

    k = 0;
    for (const auto* const muPtr : event.m_muons) {//loop on muons
        //// access Muon TrackParticle for d0/z0 information if available
        m_mu_d0[k]    = 0.;
        m_mu_z0[k]    = 0.;
        //m_mu_d0sig[k] = 0.;
        m_mu_z0sig[k] = 0.;
        const xAOD::TrackParticle* mutrack = muPtr->primaryTrackParticle();
        if (mutrack != nullptr) {
            m_mu_d0[k] = mutrack->d0();
            m_mu_z0[k] = mutrack->z0() + mutrack->vz() - m_vtxz;
            const xAOD::ParametersCovMatrix_t mucov = mutrack->definingParametersCovMatrix();
            //m_mu_d0sig[k] = mutrack->d0()/sqrt(mucov(0,0));
            m_mu_z0sig[k] = m_mu_z0[k] / sqrt(mucov(1, 1));
        }
    }//for

    // --------- truth particle info  ------- //
    m_truthparticle_type.clear();
    m_truthparticle_origin.clear();
    m_truthparticle_pt.clear();
    m_truthparticle_eta.clear();
    m_truthparticle_phi.clear();
    m_truthparticle_m.clear();

    if (event.m_truth && m_saveFullTruthRecord) {
        for (const auto* const truPtr : *event.m_truth) {
            unsigned int tr_type(-99), tr_origin(-99);
            float tr_px(0), tr_py(0), tr_pz(0), tr_e(0);

            if (truPtr->isAvailable<unsigned int>("classifierParticleType"))
                tr_type = truPtr->auxdata<unsigned int>("classifierParticleType");
            if (truPtr->isAvailable<unsigned int>("classifierParticleOrigin"))
                tr_origin = truPtr->auxdata<unsigned int>("classifierParticleOrigin");

            if (truPtr->isAvailable<float>("px"))
                tr_px = truPtr->auxdata<float>("px");

            if (truPtr->isAvailable<float>("py"))
                tr_py = truPtr->auxdata<float>("py");

            if (truPtr->isAvailable<float>("pz"))
                tr_pz = truPtr->auxdata<float>("pz");

            if (truPtr->isAvailable<float>("e"))
                tr_e = truPtr->auxdata<float>("e");

            //int pdgId= truPtr->pdgId();
            TLorentzVector trParticle;
            trParticle.SetPxPyPzE(tr_px, tr_py, tr_pz, tr_e);

            m_truthparticle_type.push_back(tr_type);
            m_truthparticle_origin.push_back(tr_origin);
            m_truthparticle_pt.push_back(trParticle.Pt());
            m_truthparticle_eta.push_back(trParticle.Eta());
            m_truthparticle_phi.push_back(trParticle.Phi());
            m_truthparticle_m.push_back(trParticle.M());

            //std::cout<<"Type = "<<tr_type<<", Origin = "<<tr_origin<<", px = "<<tr_px<<std::endl;

        } // for loop over truth particles
    }

    // ---------  ################## -------- //
    int i = 0;

    //flag for the tight leptons in the nominal ttree

    m_el_isTight.resize(event.m_electrons.size());
    for (const auto* const elPtr : event.m_electrons) {

        if (!event.m_isLoose)
            if (elPtr->isAvailable<char>("passPreORSelection"))
                m_el_isTight[i] = elPtr->auxdataConst<char>("passPreORSelection");

    }

    m_mu_isTight.resize(event.m_muons.size());
    for (const auto* const muPtr : event.m_muons) {

        if (!event.m_isLoose)
            if (muPtr->isAvailable<char>("passPreORSelection"))
                m_mu_isTight[i] = muPtr->auxdataConst<char>("passPreORSelection");

    }

    calculateWjets(event);
    i = 0;
    size_t largeJets_size = event.m_largeJets.size();
    m_ljet_ghosttrackjet_idx.resize(largeJets_size, std::vector<int>());
    m_ljet_nghosttrackjet.resize(largeJets_size, 0);
    m_ljet_nghosttrackjetb.resize(largeJets_size, 0);
    m_ljet_nghosttrackjetbb.resize(largeJets_size, 0);
    m_ljet_nghosttrackjetc.resize(largeJets_size, 0);
    m_ljet_nghosttrackjetcc.resize(largeJets_size, 0);
    m_ljet_nghosttrackjetl.resize(largeJets_size, 0);


    m_ljet_good_sub80.resize(largeJets_size);
    m_ljet_good_sub50.resize(largeJets_size);
    m_ljet_good_smooth_mt80.resize(largeJets_size);
    m_ljet_good_smooth_mt50.resize(largeJets_size);
    m_ljet_good_smooth_ts80.resize(largeJets_size);
    m_ljet_good_smooth_ts50.resize(largeJets_size);
    m_ljet_good_smooth_qt80.resize(largeJets_size);
    m_ljet_good_smooth_qt50.resize(largeJets_size);
    m_ljet_good_bdt80.resize(largeJets_size);
    m_ljet_good_dnn80.resize(largeJets_size);
    m_ljet_good_topo80.resize(largeJets_size);
    m_ljet_bdt_score.resize(largeJets_size);
    m_ljet_dnn_score.resize(largeJets_size);
    m_ljet_topo_score.resize(largeJets_size);



    m_ljet_good.resize(largeJets_size);
    m_ljet_notgood.resize(largeJets_size);
    m_ljet_tau32.resize(largeJets_size);
    m_ljet_tau32_wta.resize(largeJets_size);
    m_ljet_tau21.resize(largeJets_size);
    m_ljet_tau21_wta.resize(largeJets_size);
#ifdef ENABLE_LJETSUBSTRUCTURE_DEBUG
    m_ljet_D2.resize(largeJets_size);
    m_ljet_C2.resize(largeJets_size);
    m_ljet_ECF1.resize(largeJets_size);
    m_ljet_ECF2.resize(largeJets_size);
    m_ljet_ECF3.resize(largeJets_size);
    m_ljet_MClike.resize(largeJets_size);
#endif
    m_ljtmatch = 0;
    m_ljet_angular_cuts.resize(largeJets_size);
    for (const auto* const jetPtr : event.m_largeJets) {
        m_ljet_good[i] = 0;
        m_ljet_notgood[i] = 0;
        m_ljet_tau32[i] = 0;
        m_ljet_tau32_wta[i] = 0;
        m_ljet_tau21[i] = 0;
        m_ljet_tau21_wta[i] = 0;
        m_ljet_ghosttrackjet_idx[i].clear();
//===================== By Elham ===============================================
        m_ljet_good_sub80[i] = 0;
        m_ljet_good_sub50[i] = 0;
        m_ljet_good_smooth_mt80[i] = 0;
        m_ljet_good_smooth_mt50[i] = 0;
        m_ljet_good_smooth_ts80[i] = 0;
        m_ljet_good_smooth_ts50[i] = 0;
        m_ljet_good_smooth_qt80[i] = 0;
        m_ljet_good_smooth_qt50[i] = 0;
        m_ljet_good_bdt80[i] = 0;
        m_ljet_good_dnn80[i] = 0;
        m_ljet_good_topo80[i] = 0;
        m_ljet_angular_cuts[i] = 0;
#ifdef ENABLE_LJETSUBSTRUCTURE_DEBUG
        m_ljet_D2[i] = 0;
        m_ljet_C2[i] = 0;
        m_ljet_ECF1[i] = 0;
        m_ljet_ECF2[i] = 0;
        m_ljet_ECF3[i] = 0;
        m_ljet_MClike[i] = -1;
#endif
        int good_sub_80 = 0, good_sub_50 = 0, good_smooth_mt80 = 0, good_smooth_mt50 = 0;
        int good_smooth_ts80 = 0, good_smooth_ts50 = 0, good_smooth_qt80 = 0, good_smooth_qt50 = 0;
        int good_bdt_80 = 0, good_dnn_80 = 0, good_topo_80 = 0;

        if (jetPtr->pt() > 300000 && std::fabs(jetPtr->eta()) < 2.0) {
            if (STL80->isTagged(*jetPtr) == true ) good_sub_80 = 1;
            if (STL50->isTagged(*jetPtr) == true ) good_sub_50 = 1;
            if (m_smoothedTopTaggerMT80->tag(*jetPtr)) good_smooth_mt80 = 1;
            if (m_smoothedTopTaggerMT50->tag(*jetPtr)) good_smooth_mt50 = 1;
            if (m_smoothedTopTaggerTS80->tag(*jetPtr)) good_smooth_ts80 = 1;
            if (m_smoothedTopTaggerTS50->tag(*jetPtr)) good_smooth_ts50 = 1;
            if (m_smoothedTopTaggerQT80->tag(*jetPtr)) good_smooth_qt80 = 1;
            if (m_smoothedTopTaggerQT50->tag(*jetPtr)) good_smooth_qt50 = 1;
            if (m_bdtTopTagger80->tag(*jetPtr)) good_bdt_80 = 1;
            if (m_dnnTopTagger80->tag(*jetPtr)) good_dnn_80 = 1;
            if (m_topoTopTagger80->tag(*jetPtr)) good_topo_80 = 1;
        }

        if (good_sub_80) m_ljet_good_sub80[i] = good_sub_80;
        if (good_sub_50) m_ljet_good_sub50[i] = good_sub_50;
        if (good_smooth_mt80) m_ljet_good_smooth_mt80[i] = good_smooth_mt80;
        if (good_smooth_mt50) m_ljet_good_smooth_mt50[i] = good_smooth_mt50;
        if (good_smooth_ts80) m_ljet_good_smooth_ts80[i] = good_smooth_ts80;
        if (good_smooth_ts50) m_ljet_good_smooth_ts50[i] = good_smooth_ts50;
        if (good_smooth_qt80) m_ljet_good_smooth_qt80[i] = good_smooth_qt80;
        if (good_smooth_qt50) m_ljet_good_smooth_qt50[i] = good_smooth_qt50;
        if (good_bdt_80) m_ljet_good_bdt80[i] = good_bdt_80;
        if (good_dnn_80) m_ljet_good_dnn80[i] = good_dnn_80;
        if (good_topo_80) m_ljet_good_topo80[i] = good_topo_80;
        if (jetPtr->isAvailable<float>("BDTTaggerTopQuark80_Score")) m_ljet_bdt_score[i] = jetPtr->auxdata<float>("BDTTaggerTopQuark80_Score");
        if (jetPtr->isAvailable<float>("DNNTaggerTopQuark80_Score")) m_ljet_dnn_score[i] = jetPtr->auxdata<float>("DNNTaggerTopQuark80_Score");
        if (jetPtr->isAvailable<float>("TopotaggerTopQuark80_Score")) m_ljet_topo_score[i] = jetPtr->auxdata<float>("TopotaggerTopQuark80_Score");
        if (jetPtr->isAvailable<int> ("angular_cuts")) m_ljet_angular_cuts[i] = jetPtr->auxdecor<int>("angular_cuts");


//==================================== End Elham ================================================


        if (jetPtr->isAvailable<int>("topTagged")) m_ljet_good[i] = jetPtr->auxdata<int>("topTagged");
        if (m_ljet_good[i] == 1) {
            if (jetPtr->pt() > max_pt) {max_pt = jetPtr->pt(); hadtop_index = i;}
        }

        try { // defined only by NLargeJetTtresSTWjetsSelector.cxx, for W CR, in which there is a veto against good top jet --> no conflict
            m_ljet_notgood[i] = jetPtr->auxdata<int>("notTopTagged");
            if (m_ljet_notgood[i] == 1) {
                if (jetPtr->pt() > max_pt) {max_pt = jetPtr->pt(); hadtop_index = i;}
            }
        } catch (...) { }
        try { m_ljet_tau32[i] = jetPtr->getAttribute<float>("Tau3") / jetPtr->getAttribute<float>("Tau2"); } catch (...) { }
        try { m_ljet_tau32_wta[i] = jetPtr->getAttribute<float>("Tau3_wta") / jetPtr->getAttribute<float>("Tau2_wta"); } catch (...) { }
        //        try { m_ljet_tau32_wta[i] = jetPtr->getAttribute<float>("Tau32_wta"); } catch (...) { try { m_ljet_tau32_wta[i] = jetPtr->getAttribute<float>("Tau3_wta")/jetPtr->getAttribute<float>("Tau2_wta"); } catch (...) { } }
        try { m_ljet_tau21[i] = jetPtr->getAttribute<float>("Tau2") / jetPtr->getAttribute<float>("Tau1"); } catch (...) { }
        try { m_ljet_tau21_wta[i] = jetPtr->getAttribute<float>("Tau2_wta") / jetPtr->getAttribute<float>("Tau1_wta"); } catch (...) { }
#ifdef ENABLE_LJETSUBSTRUCTURE_DEBUG
        try { m_ljet_ECF1[i] = jetPtr->getAttribute<float>("ECF1"); } catch (...) { }
        try { m_ljet_ECF2[i] = jetPtr->getAttribute<float>("ECF2"); } catch (...) { }
        try { m_ljet_ECF3[i] = jetPtr->getAttribute<float>("ECF3"); } catch (...) { }
        float c2 = 0;
        float d2 = 0;
        float ecf1 = m_ljet_ECF1[i];
        float ecf2 = m_ljet_ECF2[i];
        float ecf3 = m_ljet_ECF3[i];
        if (fabs(ecf2) > 0) {
            c2 = ecf3 * ecf1 / pow(ecf2, 2);
            d2 = ecf3 * pow(ecf1, 3) / pow(ecf2, 3);
        }
        m_ljet_C2[i] = c2;
        m_ljet_D2[i] = d2;
#endif

        // I want this to crash if they're not available!
        // Danilo: I want this to work when they are not available due to bug ATLJETMET-381
        try { m_ljet_nghosttrackjet[i] = jetPtr->getAttribute<int>("nGhostJets"); } catch (...) { }
        try { m_ljet_nghosttrackjetb[i] = jetPtr->getAttribute<int>("nTrueBGhostJets"); } catch (...) { }
        try { m_ljet_nghosttrackjetbb[i] = jetPtr->getAttribute<int>("nTrueBBGhostJets"); } catch (...) { }
        try { m_ljet_nghosttrackjetc[i] = jetPtr->getAttribute<int>("nTrueCGhostJets"); } catch (...) { }
        try { m_ljet_nghosttrackjetcc[i] = jetPtr->getAttribute<int>("nTrueCCGhostJets"); } catch (...) { }
        try { m_ljet_nghosttrackjetl[i] = jetPtr->getAttribute<int>("nTrueLGhostJets"); } catch (...) { }

        // store the ghost-associated trackjet indices
        try {
            if (m_ghostjetcollection != "") {
                const std::vector<ElementLink<xAOD::IParticleContainer> >& ghostJets =
                    jetPtr->getAttribute<std::vector<ElementLink<xAOD::IParticleContainer> > >(m_ghostjetcollection);

                for (const auto& link : ghostJets)
                    m_ljet_ghosttrackjet_idx[i].push_back(link.index());
            }
        } catch (...) {
        }

        if (m_ljet_good[i]) {
            if (m_trackjetcollection != "") {
                const xAOD::JetContainer* trackjetsOrig = 0;
                top::check(evtStore()->event()->retrieve(trackjetsOrig, m_trackjetcollection), "FAILURE");
                for (unsigned int k = 0; k < m_ljet_ghosttrackjet_idx[i].size(); ++k) {
                    int ti = m_ljet_ghosttrackjet_idx[i][k];
                    const xAOD::Jet *trackjetPtr = trackjetsOrig->at(ti);

                    double mvx = -999;
                    trackjetPtr->btagging()->MVx_discriminant("MV2c10", mvx);
                    if (trackjetPtr->pt() > 10e3 && std::fabs(trackjetPtr->eta()) < 2.5 &&
                            mvx > m_trackjetMv2c10Cut && trackjetPtr->numConstituents() >= 2) {
                        m_ljtmatch = 1;
                    }

                }
            }
        }

        ++i;
    }


    m_tjet_mv2c10mu.resize(event.m_trackJets.size());
    m_tjet_mv2c10rnn.resize(event.m_trackJets.size());
    m_tjet_dl1_pu.resize(event.m_trackJets.size());
    m_tjet_dl1_pb.resize(event.m_trackJets.size());
    m_tjet_dl1_pc.resize(event.m_trackJets.size());
    m_tjet_dl1mu_pu.resize(event.m_trackJets.size());
    m_tjet_dl1mu_pb.resize(event.m_trackJets.size());
    m_tjet_dl1mu_pc.resize(event.m_trackJets.size());
    m_tjet_dl1rnn_pu.resize(event.m_trackJets.size());
    m_tjet_dl1rnn_pb.resize(event.m_trackJets.size());
    m_tjet_dl1rnn_pc.resize(event.m_trackJets.size());


    i = 0;

    for (const auto* const jetPtr : event.m_trackJets) {

        if (jetPtr->pt() > 10e3 && std::fabs(jetPtr->eta()) < 2.5 && jetPtr->numConstituents() >= 2) {
            const xAOD::BTagging* btag(nullptr);
            btag = jetPtr->btagging();
            double mvx = -999;


            if (btag) btag->MVx_discriminant("MV2c10mu", mvx);
            m_tjet_mv2c10mu[i] = mvx;
            mvx = -999;
            if (btag) btag->MVx_discriminant("MV2c10rnn", mvx);
            m_tjet_mv2c10rnn[i] = mvx;

            if (btag) {
                double pu = -999, pb = -999, pc = -999;
                btag->pu("DL1", pu); btag->pb("DL1", pb); btag->pc("DL1", pc);
                m_tjet_dl1_pu[i] = pu; m_tjet_dl1_pb[i] = pb; m_tjet_dl1_pc[i] = pc;
                btag->pu("DL1mu", pu); btag->pb("DL1mu", pb); btag->pc("DL1mu", pc);
                m_tjet_dl1mu_pu[i] = pu; m_tjet_dl1mu_pb[i] = pb; m_tjet_dl1mu_pc[i] = pc;
                btag->pu("DL1rnn", pu); btag->pb("DL1rnn", pb); btag->pc("DL1rnn", pc);
                m_tjet_dl1rnn_pu[i] = pu; m_tjet_dl1rnn_pb[i] = pb; m_tjet_dl1rnn_pc[i] = pc;
                //std::cout << "dl1=" << m_tjet_dl1_pu[i] << std::endl;
            }
            ++i;
        }
    }



    if (m_runHtt) {
        // Execute HTT
        //httTool->execute();
        std::string treeName = (*m_config->systAllTTreeNames())[event.m_hashValue];
        m_HTT[treeName]->execute();

        const xAOD::JetContainer* CamKt15jets_HTT = 0;
        top::check(evtStore()->event()->retrieve(CamKt15jets_HTT,  Form("CA15LCTopoJets_HTT_%s", treeName.c_str())), "FAILURE");
        //const xAOD::JetContainer* CamKt15jets_HTT = 0;
        //top::check(evtStore()->event()->retrieve(CamKt15jets_HTT, "CamKt15LCTopoJets_HTT"), "FAILURE");

        // HTT Branches
        CA15_pt.resize(CamKt15jets_HTT->size());
        CA15_m.resize(CamKt15jets_HTT->size());
        CA15_eta.resize(CamKt15jets_HTT->size());
        CA15_phi.resize(CamKt15jets_HTT->size());
        CA15_trk_bjets_n.resize(CamKt15jets_HTT->size());

        TLorentzHTT.resize(CamKt15jets_HTT->size());
        TLorentzCA15.resize(CamKt15jets_HTT->size());

        HTT_pt.resize(CamKt15jets_HTT->size());
        HTT_m.resize(CamKt15jets_HTT->size());
        HTT_eta.resize(CamKt15jets_HTT->size());
        HTT_phi.resize(CamKt15jets_HTT->size());

        HTT_atan1312.resize(CamKt15jets_HTT->size());
        HTT_m23m123.resize(CamKt15jets_HTT->size());

        HTT_tt_m.resize(1);
        HTT_tt_pt.resize(1);

        int i = 0;
        int top_cands = 0;
        int top_cands_masswindow = 0;

        int n_CA15 = 0;

        i = 0; // restart counter
        for (xAOD::JetContainer::const_iterator jet_itr = CamKt15jets_HTT->begin(); jet_itr != CamKt15jets_HTT->end(); ++jet_itr) {
            const xAOD::Jet* CamKt15jet_HTT = *jet_itr;
            CA15_pt[i] = 0;
            CA15_m[i] = 0;
            CA15_eta[i] = -5000;
            CA15_phi[i] = -5000;

            CA15_trk_bjets_n[i] = -1;

            HTT_pt[i] = 0;
            HTT_m[i] = 0;
            HTT_eta[i] = -5000;
            HTT_phi[i] = -5000;

            HTT_m23m123[i] = 0;
            HTT_atan1312[i] = 0;

            HTT_tt_m[0] = 0;
            HTT_tt_pt[0] = 0;

            if (CamKt15jet_HTT->pt() < 150000) continue;
            if (abs(CamKt15jet_HTT->eta()) > 2.0) continue;
            n_CA15 += 1;

            try { CA15_pt[i] = CamKt15jet_HTT->pt();} catch (...) { }
            try { CA15_m[i] = CamKt15jet_HTT->m();} catch (...) { }
            try { CA15_eta[i] = CamKt15jet_HTT->eta();} catch (...) { }
            try { CA15_phi[i] = CamKt15jet_HTT->phi();} catch (...) { }

            TLorentzCA15[i] = TLorentzVector();
            TLorentzCA15[i].SetPtEtaPhiM(CA15_pt[i], CA15_eta[i], CA15_phi[i], CA15_m[i]);

            // count trk b-jets "inside" CA15 jet
            int trk_bjet_CA15_n = 0;
            if (m_trackjetcollection != "") {
                const xAOD::JetContainer* trackjets_0 = 0;
                top::check(evtStore()->event()->retrieve(trackjets_0, m_trackjetcollection), "FAILURE");
                int j = 0;

                for (xAOD::JetContainer::const_iterator trackjet_itr = trackjets_0->begin(); trackjet_itr != trackjets_0->end(); ++trackjet_itr) {
                    const xAOD::Jet *trackjetPtr = trackjets_0->at(j);
                    double mvx = -999;
                    trackjetPtr->btagging()->MVx_discriminant("MV2c10", mvx); // Get track jet MV2c10
                    TLorentzVector trackjet_lv;

                    if (trackjetPtr->pt() > 10e3 && std::fabs(trackjetPtr->eta()) < 2.5 && mvx > m_trackjetMv2c10Cut && trackjetPtr->numConstituents() >= 2)
                    {
                        trackjet_lv.SetPtEtaPhiM(trackjetPtr->pt(), trackjetPtr->eta(), trackjetPtr->phi(), 100.);
                        double deltaR_fj_bjet = trackjet_lv.DeltaR(TLorentzCA15[i]);
                        if (deltaR_fj_bjet < 1.5) {
                            trk_bjet_CA15_n += 1;
                        }
                    }
                    j++;
                }
            }
            // end of trk matching to CA15

            CA15_trk_bjets_n[i] = trk_bjet_CA15_n;
            // Get top candidate if there is one
            const xAOD::Jet* topCandidate = 0;
            bool ok = CamKt15jet_HTT->getAssociatedObject("HTTnormal", topCandidate);
            TLorentzHTT[i] = TLorentzVector();
            TLorentzHTT[i].SetPtEtaPhiM(0, 0, 0, 0);

            if ( ok && topCandidate != 0 ) {
                try { HTT_pt[i] =  topCandidate->pt(); } catch (...) { }
                try { HTT_m[i] =  topCandidate->m(); } catch (...) { }
                try { HTT_eta[i] =  topCandidate->eta(); } catch (...) { }
                try { HTT_phi[i] =  topCandidate->phi(); } catch (...) { }
                try { HTT_m23m123[i] =  topCandidate->getAttribute<float>("M23") / topCandidate->getAttribute<float>("M123"); } catch (...) { }
                try { HTT_atan1312[i] =  topCandidate->getAttribute<float>("Atan1312"); } catch (...) { }

                TLorentzHTT[i].SetPtEtaPhiM(topCandidate->pt(), topCandidate->eta(), topCandidate->phi(), topCandidate->m());

                if (topCandidate->m() > 140000 && topCandidate->m() < 210000) {
                    top_cands_masswindow += 1;
                }
                top_cands += 1;
            }
            i++;
        }
        CA15_n = n_CA15;
        HTT_n = top_cands;
        HTT_masswindow_n = top_cands_masswindow;

        // order top candidates by pt
        std::sort(TLorentzHTT.begin(), TLorentzHTT.end(), [](TLorentzVector left, TLorentzVector right) {
            return left.Pt() > right.Pt();
        });

        if (top_cands >= 2) {
            HTT_tt_m[0] = 0;
            HTT_tt_pt[0] = 0;
            TLorentzVector add_V = TLorentzHTT[0] + TLorentzHTT[1];
            HTT_tt_m[0] = add_V.M();
            HTT_tt_pt[0] = add_V.Pt();
        }
    }

    i = 0;
    int lepjet_index = -1;
    m_jet_closeToLepton.resize(event.m_jets.size());
    m_jet_ghosttrackjet_idx.resize(event.m_jets.size(), std::vector<int>());
    m_jet_nghosttrackjet.resize(event.m_jets.size(), 0);
    m_jet_nghosttrackjetb.resize(event.m_jets.size(), 0);
    m_jet_nghosttrackjetbb.resize(event.m_jets.size(), 0);
    m_jet_nghosttrackjetc.resize(event.m_jets.size(), 0);
    m_jet_nghosttrackjetcc.resize(event.m_jets.size(), 0);
    m_jet_nghosttrackjetl.resize(event.m_jets.size(), 0);
    m_jet_trueflav.resize(event.m_jets.size(), -1);
    for (const auto* const jetPtr : event.m_jets) {
        m_jet_closeToLepton[i] = 0;
        m_jet_ghosttrackjet_idx[i].clear();

        if (jetPtr->isAvailable<char>("closeToLepton")) {
            m_jet_closeToLepton[i] = jetPtr->auxdata<char>("closeToLepton");
            if (jetPtr->auxdata<char>("closeToLepton") == 1) {lepjet_index = i;}
        }
        // I want this to crash if they're not available!
        // Danilo: I want this to work when they are not available due to bug ATLJETMET-381
        try { m_jet_nghosttrackjet[i] = jetPtr->getAttribute<int>("nGhostJets"); } catch (...) { }
        try { m_jet_nghosttrackjetb[i] = jetPtr->getAttribute<int>("nTrueBGhostJets"); } catch (...) { }
        try { m_jet_nghosttrackjetbb[i] = jetPtr->getAttribute<int>("nTrueBBGhostJets"); } catch (...) { }
        try { m_jet_nghosttrackjetc[i] = jetPtr->getAttribute<int>("nTrueCGhostJets"); } catch (...) { }
        try { m_jet_nghosttrackjetcc[i] = jetPtr->getAttribute<int>("nTrueCCGhostJets"); } catch (...) { }
        try { m_jet_nghosttrackjetl[i] = jetPtr->getAttribute<int>("nTrueLGhostJets"); } catch (...) { }
        if (m_isMC)try { m_jet_trueflav[i] = jetPtr->getAttribute<int>("HadronConeExclTruthLabelID"); } catch (...) { }

        // store the ghost-associated trackjet indices
        try {
            if (m_ghostjetcollection != "") {
                const std::vector<ElementLink<xAOD::IParticleContainer> >& ghostJets =
                    jetPtr->getAttribute<std::vector<ElementLink<xAOD::IParticleContainer> > >(m_ghostjetcollection);

                for (const auto& link : ghostJets)
                    m_jet_ghosttrackjet_idx[i].push_back(link.index());
            }
        } catch (...) {
        }

        ++i;
    }


    m_MC_i1_px = -9999;
    m_MC_i1_py = -9999;
    m_MC_i1_pz = -9999;
    m_MC_i1_m = -9999;
    m_MC_i1_pid = -9999;
    m_MC_i2_px = -9999;
    m_MC_i2_py = -9999;
    m_MC_i2_pz = -9999;
    m_MC_i2_m = -9999;
    m_MC_i2_pid = -9999;

    m_initial_type = 0;
    if (event.m_truth) {
        const xAOD::TruthParticle* initialparton0 = nullptr;
        const xAOD::TruthParticle* initialparton1 = nullptr;
        const xAOD::TruthParticle* mediator = nullptr;
        const xAOD::TruthParticle * mc_top = 0;
        const xAOD::TruthParticle * mc_antitop = 0;
        for (const auto* const mcPtr : *event.m_truth) {
            if (mcPtr->status() != 3 /*Pythia6*/ && mcPtr->status() != 22 /*Pythia8*/) {
                continue;
            }
            if (mcPtr->pdgId() == 6) {
                mc_top = mcPtr;
                if (mc_antitop != NULL) {
                    break;
                }
            }
            else if (mcPtr->pdgId() == -6) {
                mc_antitop = mcPtr;
                if (mc_top != NULL) {
                    break;
                }
            }
            else {
                mediator = mcPtr;
            }

        }
        if (mc_top != NULL && mc_antitop != NULL) {
            if (mediator != nullptr) {
                if ( mediator->nParents() < 2) { std::cout << "ERROR: Could not get top parents!" << std::endl; }
                initialparton0 = mediator->parent(0);
                initialparton1 = mediator->parent(1);
            }
            else {
                if ( mc_top->nParents() < 2) { std::cout << "ERROR: Could not get top parents!" << std::endl; }
                initialparton0 = mc_top->parent(0);
                initialparton1 = mc_top->parent(1);
            }

            if (initialparton0 && initialparton1) {
                m_MC_i1_px = initialparton0->px();
                m_MC_i1_py = initialparton0->py();
                m_MC_i1_pz = initialparton0->pz();
                m_MC_i1_m = initialparton0->m();
                m_MC_i1_pid = initialparton0->pdgId();
                m_MC_i2_px = initialparton1->px();
                m_MC_i2_py = initialparton1->py();
                m_MC_i2_pz = initialparton1->pz();
                m_MC_i2_m = initialparton1->m();
                m_MC_i2_pid = initialparton1->pdgId();
            }
            if (mc_top->nParents() > 1 && m_runEWK) {
                const int pdg1 = abs(initialparton0->pdgId());
                const int pdg2 = abs(initialparton1->pdgId());
                const xAOD::TruthParticle * incomingQuark = 0;
                const xAOD::TruthParticle * outgoingParton = 0;
                for (unsigned int ii = 0; ii < initialparton0->nChildren(); ii++) {
                    const int pdgID = abs(initialparton0->child(ii)->pdgId());
                    // cout << "THE outgoing particle PDG ID:     " << pdgID << endl; //Elham
                    if (pdgID != 6) {
                        outgoingParton = initialparton0->child(ii);
                        if (!(pdgID == 1 || pdgID == 3 || pdgID == 5 || pdgID == 2 || pdgID == 4 || pdgID == 21)) {
                            std::cout << "ERROR: Could not determine outgoing parton!" << std::endl;
                        }
                        break;
                    }
                }
                if (outgoingParton == NULL) {
                    std::cout << "WARNING: Outgoing parton NULL!" << std::endl;
                }
                if (outgoingParton != NULL || initialparton0->nChildren() == 2) {
                    int eventType = 0;
                    if (pdg1 == pdg2) { // if gg/qqbar -> ttbar
                        switch (pdg1) {
                        case 21:
                            eventType = 1;
                            break;
                        case 1:
                        case 3:
                        case 5:
                            eventType = 3;
                            break;
                        case 2:
                        case 4:
                            eventType = 2;
                            break;
                        }
                        m_initial_type = eventType;
                    } else { // if gq -> ttbar
                        int pdgQ = pdg1;
                        if (pdg1 == 21) {
                            pdgQ = pdg2;
                            incomingQuark = initialparton1;
                        } else if (pdg2 == 21) {
                            incomingQuark = initialparton0;
                        } else {
                            std::cout << "ERROR: Could not determine type of initial state!" << std::endl;
                        }
                        switch (pdgQ) {
                        case 1:
                        case 3:
                        case 5:
                            eventType = 5;
                            break;
                        case 2:
                        case 4:
                            eventType = 4;
                            break;
                        default:
                            std::cout << "ERROR: Could not determine type of initial state!" << std::endl;
                        }

                        TVector3 hadronMomentumZDir(0., 0., TMath::Sign(1, incomingQuark->pz()));
                        const double thetaQ = hadronMomentumZDir.Angle(outgoingParton->p4().Vect());
                        if (thetaQ < (TMath::Pi() * 0.5)) {
                            m_initial_type = 1;
                        } else {
                            if (eventType == 5) {
                                m_initial_type = 3;
                            } else if (eventType == 4) {
                                m_initial_type = 2;
                            }
                        }
                    }
                }
            }
        }
    }

    float met_et = event.m_met->met();
    float met_phi = event.m_met->phi();
    int hadside = 0, lepside = 0;
    double jmv2;
    double m_chi2all = 10000000;
    double m_chi2lep = 10000000;
    double m_chi2had = 10000000;
    const xAOD::Electron * el_lep;
    const xAOD::Muon * mu_lep;
    TLorentzVector Plepton, Pleptop, Phadtop, Pneut, Plepjet, Pbjet;
    TLorentzVector met(0, 0, 0, 0);
    met.SetPtEtaPhiM(met_et, 0, met_phi, 0);
    std::vector<TLorentzVector*> jetVector;
    std::vector<bool> isJetBtagged;
    TtresNeutrinoBuilder builder = TtresNeutrinoBuilder("MeV");
    if ((event.m_muons.size()) > 0) {mu_lep = event.m_muons.at(0); Plepton.SetPtEtaPhiM(mu_lep->pt(), mu_lep->eta(), mu_lep->phi(), 0);}
    if ((event.m_electrons.size()) > 0) {el_lep = event.m_electrons.at(0); Plepton.SetPtEtaPhiM(el_lep->pt(), el_lep->eta(), el_lep->phi(), 0);}

    std::vector<TLorentzVector*> Nus;
    Nus = builder.candidatesFromWMass_Rotation(&Plepton, met_et, met_phi, true);
    Pneut = *Nus.at(0);



    if (hadtop_index > -1 && lepjet_index > -1) {
        const xAOD::Jet * hadtop = event.m_largeJets.at(hadtop_index);
        const xAOD::Jet * lepjet = event.m_jets.at(lepjet_index);
        Phadtop.SetPtEtaPhiE(hadtop->pt(), hadtop->eta(), hadtop->phi(), hadtop->e());
        Plepjet.SetPtEtaPhiE(lepjet->pt(), lepjet->eta(), lepjet->phi(), lepjet->e());
        Pleptop = Plepton + Plepjet + Pneut;
    }



    if (hadtop_index == -1 || lepjet_index == -1) {

        TtresChi2 resolved_tt("MeV");

        for (const auto* const jetPtr : event.m_jets) {
            TLorentzVector *jet = new TLorentzVector();

            jet->SetPtEtaPhiM(jetPtr->pt(), jetPtr->eta(), jetPtr->phi(), jetPtr->m());
            jetVector.push_back(jet);
            jetPtr->btagging()->MVx_discriminant("MV2c10", jmv2);
            isJetBtagged.push_back(jmv2 > 0.66);
        }
        int i_q1_W, i_q2_W, i_b_had, i_b_lep, ign1;
        bool pass = resolved_tt.findMinChiSquare(&Plepton, &jetVector, &isJetBtagged, &met, i_q1_W, i_q2_W, i_b_had, i_b_lep, ign1, m_chi2all, m_chi2had, m_chi2lep);
        if (pass == true) {

            Phadtop = (*jetVector.at(i_q1_W)) + (*jetVector.at(i_q2_W)) + (*jetVector.at(i_b_had));
            Pleptop = (*jetVector.at(i_b_lep)) + Pneut + Plepton;

        }

    }

    m_NB_hadside = 0;
    m_NB_lepside = 0;
    for (unsigned int k = 0; k < trackjets->size(); ++k) {
        const xAOD::Jet *trackjetPtr = trackjets->at(k);
        trackjetPtr->btagging()->MVx_discriminant("MV2c10", jmv2);
        if (jmv2 > 0.66) {
            Pbjet.SetPtEtaPhiM(trackjetPtr->pt(), trackjetPtr->eta(), trackjetPtr->phi(), trackjetPtr->m());
            if ((Pbjet.DeltaR(Phadtop)) < 1.0) {hadside = 1; m_NB_hadside += 1;}
            if ((Pbjet.DeltaR(Pleptop)) < 1.0) {lepside = 1; m_NB_lepside += 1;}
        }
    }


    if (hadside == 0 && lepside == 0) {m_Btagcat = 0;}
    if (hadside == 0 && lepside == 1) {m_Btagcat = 1;}
    if (hadside == 1 && lepside == 0) {m_Btagcat = 2;}
    if (hadside == 1 && lepside == 1) {m_Btagcat = 3;}



    m_tjet_numConstituents.resize(trackjets->size(), -1);
    m_tjet_label.resize(trackjets->size(), 1);

    m_tjet_bTagSF_70.resize(trackjets->size(), 1);
    m_tjet_bTagSF_70_eigen_B_up.resize(trackjets->size(), std::vector<float>());
    m_tjet_bTagSF_70_eigen_B_down.resize(trackjets->size(), std::vector<float>());
    m_tjet_bTagSF_70_eigen_C_up.resize(trackjets->size(), std::vector<float>());
    m_tjet_bTagSF_70_eigen_C_down.resize(trackjets->size(), std::vector<float>());
    m_tjet_bTagSF_70_eigen_Light_up.resize(trackjets->size(), std::vector<float>());
    m_tjet_bTagSF_70_eigen_Light_down.resize(trackjets->size(), std::vector<float>());
    m_tjet_bTagSF_70_syst_extrapolation_up.resize(trackjets->size(), 1);
    m_tjet_bTagSF_70_syst_extrapolation_down.resize(trackjets->size(), 1);
    m_tjet_bTagSF_70_syst_extrapolation_from_charm_up.resize(trackjets->size(), 1);
    m_tjet_bTagSF_70_syst_extrapolation_from_charm_down.resize(trackjets->size(), 1);
#ifdef ENABLE_BTAG_DEBUG
    m_tjet_bTagSF_70_syst_B_up.resize(trackjets->size(), 1);
    m_tjet_bTagSF_70_syst_B_down.resize(trackjets->size(), 1);
    m_tjet_bTagSF_70_syst_C_up.resize(trackjets->size(), 1);
    m_tjet_bTagSF_70_syst_C_down.resize(trackjets->size(), 1);
    m_tjet_bTagSF_70_syst_Light_up.resize(trackjets->size(), 1);
    m_tjet_bTagSF_70_syst_Light_down.resize(trackjets->size(), 1);
    m_weight_trackjet_bTagSF_70_env_nom = 1;
    m_weight_trackjet_bTagSF_70_env_B_up = 1;
    m_weight_trackjet_bTagSF_70_env_B_down = 1;
    m_weight_trackjet_bTagSF_70_env_C_up = 1;
    m_weight_trackjet_bTagSF_70_env_C_down = 1;
    m_weight_trackjet_bTagSF_70_env_Light_up = 1;
    m_weight_trackjet_bTagSF_70_env_Light_down = 1;
    m_weight_trackjet_bTagSF_70_env_extrapolation_up = 1;
    m_weight_trackjet_bTagSF_70_env_extrapolation_down = 1;
    m_weight_trackjet_bTagSF_70_env_extrapolation_from_charm_up = 1;
    m_weight_trackjet_bTagSF_70_env_extrapolation_from_charm_down = 1;
#endif
    for (unsigned int k = 0; k < trackjets->size(); ++k) {
        const xAOD::Jet *trackjetPtr = trackjets->at(k);
        m_tjet_label[k] = -1;
        trackjetPtr->getAttribute("HadronConeExclTruthLabelID", m_tjet_label[k]);
        m_tjet_numConstituents[k] = trackjetPtr->numConstituents();

        if (trackjetPtr->isAvailable<float>(btaggingAlgWP))
            m_tjet_bTagSF_70[k] = trackjetPtr->auxdataConst<float>(btaggingAlgWP);

        /*
               m_tjet_bTagSF_70_eigen_B_up[k].resize(n_b, 1);
               m_tjet_bTagSF_70_eigen_B_down[k].resize(n_b, 1);
               for (int l = 0; l < n_b; ++l) {
                 if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_B_"+std::to_string(l)+"__1up"))
                   m_tjet_bTagSF_70_eigen_B_up[k][l] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_B_"+std::to_string(l)+"__1up");
                 if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_B_"+std::to_string(l)+"__1down"))
                 m_tjet_bTagSF_70_eigen_B_down[k][l] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_B_"+std::to_string(l)+"__1down");
               }
               m_tjet_bTagSF_70_eigen_C_up[k].resize(n_c, 1);
               m_tjet_bTagSF_70_eigen_C_down[k].resize(n_c, 1);
               for (int l = 0; l < n_c; ++l) {
                 if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_C_"+std::to_string(l)+"__1up"))
                   m_tjet_bTagSF_70_eigen_C_up[k][l] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_C_"+std::to_string(l)+"__1up");
                 if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_C_"+std::to_string(l)+"__1down"))
                   m_tjet_bTagSF_70_eigen_C_down[k][l] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_C_"+std::to_string(l)+"__1down");
               }
               m_tjet_bTagSF_70_eigen_Light_up[k].resize(n_l, 1);
               m_tjet_bTagSF_70_eigen_Light_down[k].resize(n_l, 1);
               for (int l = 0; l < n_l; ++l) {
                 if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_Light_"+std::to_string(l)+"__1up"))
                   m_tjet_bTagSF_70_eigen_Light_up[k][l] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_Light_"+std::to_string(l)+"__1up");
                 if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_Light_"+std::to_string(l)+"__1down"))
                   m_tjet_bTagSF_70_eigen_Light_down[k][l] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_Eigen_Light_"+std::to_string(l)+"__1down");
               }
        */
        if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_extrapolation__1up"))
            m_tjet_bTagSF_70_syst_extrapolation_up[k] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_extrapolation__1up");
        if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_extrapolation__1down"))
            m_tjet_bTagSF_70_syst_extrapolation_down[k] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_extrapolation__1down");

        if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_extrapolation from charm__1up"))
            m_tjet_bTagSF_70_syst_extrapolation_from_charm_up[k] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_extrapolation from charm__1up");
        if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_extrapolation from charm__1down"))
            m_tjet_bTagSF_70_syst_extrapolation_from_charm_down[k] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_extrapolation from charm__1down");
#ifdef ENABLE_BTAG_DEBUG

        if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_B_systematics__1up"))
            m_tjet_bTagSF_70_syst_B_up[k] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_B_systematics__1up");
        if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_B_systematics__1down"))
            m_tjet_bTagSF_70_syst_B_down[k] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_B_systematics__1down");

        if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_C_systematics__1up"))
            m_tjet_bTagSF_70_syst_C_up[k] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_C_systematics__1up");
        if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_C_systematics__1down"))
            m_tjet_bTagSF_70_syst_C_down[k] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_C_systematics__1down");

        if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_Light_systematics__1up"))
            m_tjet_bTagSF_70_syst_Light_up[k] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_Light_systematics__1up");
        if (trackjetPtr->isAvailable<float>("btag_SF_FixedCutBEff_70_FT_EFF_Light_systematics__1down"))
            m_tjet_bTagSF_70_syst_Light_down[k] = trackjetPtr->auxdataConst<float>("btag_SF_FixedCutBEff_70_FT_EFF_Light_systematics__1down");

        if (trackjetPtr->pt() > 10e3 && std::fabs(trackjetPtr->eta()) < 2.5 && trackjetPtr->numConstituents() >= 2) {
            m_weight_trackjet_bTagSF_70_env_nom *= m_tjet_bTagSF_70[k];
            m_weight_trackjet_bTagSF_70_env_B_up *= m_tjet_bTagSF_70_syst_B_up[k];
            m_weight_trackjet_bTagSF_70_env_B_down *= m_tjet_bTagSF_70_syst_B_down[k];
            m_weight_trackjet_bTagSF_70_env_C_up *= m_tjet_bTagSF_70_syst_C_up[k];
            m_weight_trackjet_bTagSF_70_env_C_down *= m_tjet_bTagSF_70_syst_C_down[k];
            m_weight_trackjet_bTagSF_70_env_Light_up *= m_tjet_bTagSF_70_syst_Light_up[k];
            m_weight_trackjet_bTagSF_70_env_Light_down *= m_tjet_bTagSF_70_syst_Light_down[k];
            m_weight_trackjet_bTagSF_70_env_extrapolation_up *= m_tjet_bTagSF_70_syst_extrapolation_up[k];
            m_weight_trackjet_bTagSF_70_env_extrapolation_down *= m_tjet_bTagSF_70_syst_extrapolation_down[k];
            m_weight_trackjet_bTagSF_70_env_extrapolation_from_charm_up *= m_tjet_bTagSF_70_syst_extrapolation_from_charm_up[k];
            m_weight_trackjet_bTagSF_70_env_extrapolation_from_charm_down *= m_tjet_bTagSF_70_syst_extrapolation_from_charm_down[k];
        }

#endif
    }

    //storing the calojet btagging SF
    m_jet_bTagSF_70.resize(event.m_jets.size(), 1);
    for (unsigned int k = 0; k < event.m_jets.size(); ++k) {
        const xAOD::Jet *jetPtr = event.m_jets.at(k);

        if (jetPtr->isAvailable<float>(btaggingAlgWP))
            m_jet_bTagSF_70[k] = jetPtr->auxdataConst<float>(btaggingAlgWP);
    }

#ifdef ENABLE_BTAG_DEBUG
    m_jet_bTagSF_70.resize(event.m_jets.size(), 1);
    m_jet_bTagSF_70_syst_B_up.resize(event.m_jets.size(), 1);
    m_jet_bTagSF_70_syst_B_down.resize(event.m_jets.size(), 1);
    m_jet_bTagSF_70_syst_C_up.resize(event.m_jets.size(), 1);
    m_jet_bTagSF_70_syst_C_down.resize(event.m_jets.size(), 1);
    m_jet_bTagSF_70_syst_Light_up.resize(event.m_jets.size(), 1);
    m_jet_bTagSF_70_syst_Light_down.resize(event.m_jets.size(), 1);
    m_weight_bTagSF_70_env_nom = 1;
    m_weight_bTagSF_70_env_B_up = 1;
    m_weight_bTagSF_70_env_B_down = 1;
    m_weight_bTagSF_70_env_C_up = 1;
    m_weight_bTagSF_70_env_C_down = 1;
    m_weight_bTagSF_70_env_Light_up = 1;
    m_weight_bTagSF_70_env_Light_down = 1;
    m_jet_bTagSF_70_syst_extrapolation_up.resize(event.m_jets.size(), 1);
    m_jet_bTagSF_70_syst_extrapolation_down.resize(event.m_jets.size(), 1);
    m_jet_bTagSF_70_syst_extrapolation_from_charm_up.resize(event.m_jets.size(), 1);
    m_jet_bTagSF_70_syst_extrapolation_from_charm_down.resize(event.m_jets.size(), 1);
    m_weight_bTagSF_70_env_extrapolation_up = 1;
    m_weight_bTagSF_70_env_extrapolation_down = 1;
    m_weight_bTagSF_70_env_extrapolation_from_charm_up = 1;
    m_weight_bTagSF_70_env_extrapolation_from_charm_down = 1;
    for (unsigned int k = 0; k < event.m_jets.size(); ++k) {
        const xAOD::Jet *jetPtr = event.m_jets.at(k);

        if (jetPtr->isAvailable<float>(btaggingAlgWP))
            m_jet_bTagSF_70[k] = jetPtr->auxdataConst<float>(btaggingAlgWP);
        if (jetPtr->isAvailable<float>("btag_SF_70_FT_EFF_B_systematics__1up"))
            m_jet_bTagSF_70_syst_B_up[k] = jetPtr->auxdataConst<float>("btag_SF_70_FT_EFF_B_systematics__1up");
        if (jetPtr->isAvailable<float>("btag_SF_70_FT_EFF_B_systematics__1down"))
            m_jet_bTagSF_70_syst_B_down[k] = jetPtr->auxdataConst<float>("btag_SF_70_FT_EFF_B_systematics__1down");

        if (jetPtr->isAvailable<float>("btag_SF_70_FT_EFF_C_systematics__1up"))
            m_jet_bTagSF_70_syst_C_up[k] = jetPtr->auxdataConst<float>("btag_SF_70_FT_EFF_C_systematics__1up");
        if (jetPtr->isAvailable<float>("btag_SF_70_FT_EFF_C_systematics__1down"))
            m_jet_bTagSF_70_syst_C_down[k] = jetPtr->auxdataConst<float>("btag_SF_70_FT_EFF_C_systematics__1down");

        if (jetPtr->isAvailable<float>("btag_SF_70_FT_EFF_Light_systematics__1up"))
            m_jet_bTagSF_70_syst_Light_up[k] = jetPtr->auxdataConst<float>("btag_SF_70_FT_EFF_Light_systematics__1up");
        if (jetPtr->isAvailable<float>("btag_SF_70_FT_EFF_Light_systematics__1down"))
            m_jet_bTagSF_70_syst_Light_down[k] = jetPtr->auxdataConst<float>("btag_SF_70_FT_EFF_Light_systematics__1down");

        if (jetPtr->isAvailable<float>("btag_SF_70_FT_EFF_extrapolation__1up"))
            m_jet_bTagSF_70_syst_extrapolation_up[k] = jetPtr->auxdataConst<float>("btag_SF_70_FT_EFF_extrapolation__1up");
        if (jetPtr->isAvailable<float>("btag_SF_70_FT_EFF_extrapolation__1down"))
            m_jet_bTagSF_70_syst_extrapolation_down[k] = jetPtr->auxdataConst<float>("btag_SF_70_FT_EFF_extrapolation__1down");

        if (jetPtr->isAvailable<float>("btag_SF_70_FT_EFF_extrapolation from charm__1up"))
            m_jet_bTagSF_70_syst_extrapolation_from_charm_up[k] = jetPtr->auxdataConst<float>("btag_SF_70_FT_EFF_extrapolation from charm__1up");
        if (jetPtr->isAvailable<float>("btag_SF_70_FT_EFF_extrapolation from charm__1down"))
            m_jet_bTagSF_70_syst_extrapolation_from_charm_down[k] = jetPtr->auxdataConst<float>("btag_SF_70_FT_EFF_extrapolation from charm__1down");

        if (jetPtr->auxdataConst<char>("good")) {
            m_weight_bTagSF_70_env_nom *= m_jet_bTagSF_70[k];
            m_weight_bTagSF_70_env_B_up *= m_jet_bTagSF_70_syst_B_up[k];
            m_weight_bTagSF_70_env_B_down *= m_jet_bTagSF_70_syst_B_down[k];
            m_weight_bTagSF_70_env_C_up *= m_jet_bTagSF_70_syst_C_up[k];
            m_weight_bTagSF_70_env_C_down *= m_jet_bTagSF_70_syst_C_down[k];
            m_weight_bTagSF_70_env_Light_up *= m_jet_bTagSF_70_syst_Light_up[k];
            m_weight_bTagSF_70_env_Light_down *= m_jet_bTagSF_70_syst_Light_down[k];
            m_weight_bTagSF_70_env_extrapolation_up *= m_jet_bTagSF_70_syst_extrapolation_up[k];
            m_weight_bTagSF_70_env_extrapolation_down *= m_jet_bTagSF_70_syst_extrapolation_down[k];
            m_weight_bTagSF_70_env_extrapolation_from_charm_up *= m_jet_bTagSF_70_syst_extrapolation_from_charm_up[k];
            m_weight_bTagSF_70_env_extrapolation_from_charm_down *= m_jet_bTagSF_70_syst_extrapolation_from_charm_down[k];
        }
    }
#endif

    if (m_saveTruthJets) {
        const xAOD::JetContainer* akt4truthjets = nullptr;
        if (m_akt4truthjetcollection != "") {
            top::check(evtStore()->event()->retrieve(akt4truthjets, m_akt4truthjetcollection), "FAILURE");
            m_akt4truthjet_pt.resize(akt4truthjets->size());
            m_akt4truthjet_eta.resize(akt4truthjets->size());
            m_akt4truthjet_phi.resize(akt4truthjets->size());
            m_akt4truthjet_m.resize(akt4truthjets->size());

            i = 0;
            for (xAOD::JetContainer::const_iterator jet_itr = akt4truthjets->begin(); jet_itr != akt4truthjets->end(); ++jet_itr) {
                const xAOD::Jet* akt4truthjetPtr = *jet_itr;

                m_akt4truthjet_pt[i] = akt4truthjetPtr->pt();
                m_akt4truthjet_eta[i] = akt4truthjetPtr->eta();
                m_akt4truthjet_phi[i] = akt4truthjetPtr->phi();
                m_akt4truthjet_m[i] = akt4truthjetPtr->m();
                ++i;
            }
        }

        const xAOD::JetContainer* akt10truthjets = nullptr;
        if (m_akt10truthjetcollection != "") {

            top::check(evtStore()->event()->retrieve(akt10truthjets, m_akt10truthjetcollection), "FAILURE");

            m_akt10truthjet_pt.resize(akt10truthjets->size());
            m_akt10truthjet_eta.resize(akt10truthjets->size());
            m_akt10truthjet_phi.resize(akt10truthjets->size());
            m_akt10truthjet_m.resize(akt10truthjets->size());
            m_akt10truthjet_tau32.resize(akt10truthjets->size());
            m_akt10truthjet_tau32_wta.resize(akt10truthjets->size());
            // setup extra branches here

            i = 0;
            for (xAOD::JetContainer::const_iterator jet_itr = akt10truthjets->begin(); jet_itr != akt10truthjets->end(); ++jet_itr) {
                const xAOD::Jet* akt10truthjetPtr = *jet_itr;

                m_akt10truthjet_pt[i] = akt10truthjetPtr->pt();
                m_akt10truthjet_eta[i] = akt10truthjetPtr->eta();
                m_akt10truthjet_phi[i] = akt10truthjetPtr->phi();
                m_akt10truthjet_m[i] = akt10truthjetPtr->m();
                try { m_akt10truthjet_tau32[i] = akt10truthjetPtr->auxdata<float>("Tau3") / akt10truthjetPtr->auxdata<float>("Tau2"); } catch (...) { }
                try { m_akt10truthjet_tau32_wta[i] = akt10truthjetPtr->auxdata<float>("Tau3_wta") / akt10truthjetPtr->auxdata<float>("Tau2_wta"); } catch (...) { }
                // do dr matching here
                ++i;
            }
        }
    }


    if (m_savePartons) {

        // Initialize variables with a default value
        IniVariables();

        // Fill the ME variables
        const xAOD::TruthEventContainer* truthEvents = nullptr;
        top::check( evtStore()->retrieve(truthEvents, m_config->sgKeyTruthEvent()) , "Failed to retrieve truth PDF info" );
        top::check( ( truthEvents->size() == 1 ), "More than one truth event, not sure how to cope with PDF info" );
        FillME(event.m_truth, truthEvents);

        const xAOD::JetContainer *akt4truthjets = nullptr;
        top::check(evtStore()->event()->retrieve(akt4truthjets, "AntiKt4TruthJets"), "FAILURE");

        const xAOD::JetContainer *akt10truthjets = nullptr;
        top::check(evtStore()->event()->retrieve(akt10truthjets, m_akt10truthjetcollection), "FAILURE");

        const xAOD::PartonHistoryContainer* topPartonCont = nullptr;
        //top::check(evtStore()->event()->retrieve(topPartonCont, m_config->sgKeyTopPartonHistory()), "FAILURE"); //m_config is private ...
        top::check(evtStore()->event()->retrieve(topPartonCont, "TopPartonHistory"), "FAILURE");
        const xAOD::PartonHistory *topParton = topPartonCont->at(0);

        // get partons
        TLorentzVector parton_t_p4;
        TLorentzVector parton_tbar_p4;
        TLorentzVector parton_b_from_t_p4;
        TLorentzVector parton_b_from_tbar_p4;
        TLorentzVector parton_Wdecay1_from_t_p4;
        TLorentzVector parton_Wdecay2_from_t_p4;
        TLorentzVector parton_Wdecay1_from_tbar_p4;
        TLorentzVector parton_Wdecay2_from_tbar_p4;

        TLorentzVector sum_of_nu_parton;
        TLorentzVector sum_of_lep_reco;

        TLorentzVector * met = new TLorentzVector();
        met->SetPxPyPzE(event.m_met->mpx(), event.m_met->mpy(), 0, event.m_met->met());
        TLorentzVector * nu_from_met(NULL);
        //TLorentzVector * nu_from_met = new TLorentzVector();

        if (topParton->auxdata<float>("MC_b_from_t_pt") > 0)  parton_b_from_t_p4.SetPtEtaPhiM(topParton->auxdata<float>("MC_b_from_t_pt"), topParton->auxdata<float>("MC_b_from_t_eta"), topParton->auxdata<float>("MC_b_from_t_phi"), topParton->auxdata<float>("MC_b_from_t_m"));
        TLorentzFill(parton_b_from_t_p4, m_MC_b_from_t_m, m_MC_b_from_t_pt, m_MC_b_from_t_eta, m_MC_b_from_t_phi);

        if (topParton->auxdata<float>("MC_b_from_tbar_pt") > 0) parton_b_from_tbar_p4.SetPtEtaPhiM(topParton->auxdata<float>("MC_b_from_tbar_pt"), topParton->auxdata<float>("MC_b_from_tbar_eta"), topParton->auxdata<float>("MC_b_from_tbar_phi"), topParton->auxdata<float>("MC_b_from_tbar_m"));
        TLorentzFill(parton_b_from_tbar_p4, m_MC_b_from_tbar_m, m_MC_b_from_tbar_pt, m_MC_b_from_tbar_eta, m_MC_b_from_tbar_phi);

        if (topParton->auxdata<float>("MC_Wdecay1_from_t_pt") > 0) parton_Wdecay1_from_t_p4.SetPtEtaPhiM(topParton->auxdata<float>("MC_Wdecay1_from_t_pt"), topParton->auxdata<float>("MC_Wdecay1_from_t_eta"), topParton->auxdata<float>("MC_Wdecay1_from_t_phi"), topParton->auxdata<float>("MC_Wdecay1_from_t_m"));
        TLorentzFill(parton_Wdecay1_from_t_p4, m_MC_Wdecay1_from_t_m, m_MC_Wdecay1_from_t_pt, m_MC_Wdecay1_from_t_eta, m_MC_Wdecay1_from_t_phi);
        m_MC_Wdecay1_from_t_pdgId = topParton->auxdata<int>("MC_Wdecay1_from_t_pdgId");

        if (topParton->auxdata<float>("MC_Wdecay2_from_t_pt") > 0) parton_Wdecay2_from_t_p4.SetPtEtaPhiM(topParton->auxdata<float>("MC_Wdecay2_from_t_pt"), topParton->auxdata<float>("MC_Wdecay2_from_t_eta"), topParton->auxdata<float>("MC_Wdecay2_from_t_phi"), topParton->auxdata<float>("MC_Wdecay2_from_t_m"));
        TLorentzFill(parton_Wdecay2_from_t_p4, m_MC_Wdecay2_from_t_m, m_MC_Wdecay2_from_t_pt, m_MC_Wdecay2_from_t_eta, m_MC_Wdecay2_from_t_phi);
        m_MC_Wdecay2_from_t_pdgId = topParton->auxdata<int>("MC_Wdecay2_from_t_pdgId");

        if (topParton->auxdata<float>("MC_Wdecay1_from_tbar_pt") > 0) parton_Wdecay1_from_tbar_p4.SetPtEtaPhiM(topParton->auxdata<float>("MC_Wdecay1_from_tbar_pt"), topParton->auxdata<float>("MC_Wdecay1_from_tbar_eta"), topParton->auxdata<float>("MC_Wdecay1_from_tbar_phi"), topParton->auxdata<float>("MC_Wdecay1_from_tbar_m"));
        TLorentzFill(parton_Wdecay1_from_tbar_p4, m_MC_Wdecay1_from_tbar_m, m_MC_Wdecay1_from_tbar_pt, m_MC_Wdecay1_from_tbar_eta, m_MC_Wdecay1_from_tbar_phi);
        m_MC_Wdecay1_from_tbar_pdgId = topParton->auxdata<int>("MC_Wdecay1_from_tbar_pdgId");

        if (topParton->auxdata<float>("MC_Wdecay2_from_tbar_pt") > 0) parton_Wdecay2_from_tbar_p4.SetPtEtaPhiM(topParton->auxdata<float>("MC_Wdecay2_from_tbar_pt"), topParton->auxdata<float>("MC_Wdecay2_from_tbar_eta"), topParton->auxdata<float>("MC_Wdecay2_from_tbar_phi"), topParton->auxdata<float>("MC_Wdecay2_from_tbar_m"));
        TLorentzFill(parton_Wdecay2_from_tbar_p4, m_MC_Wdecay2_from_tbar_m, m_MC_Wdecay2_from_tbar_pt, m_MC_Wdecay2_from_tbar_eta, m_MC_Wdecay2_from_tbar_phi);
        m_MC_Wdecay2_from_tbar_pdgId = topParton->auxdata<int>("MC_Wdecay2_from_tbar_pdgId");

        if (topParton->auxdata<float>("MC_W_from_t_pt") > 0) {
            m_MC_W_from_t_pt  = topParton->auxdata<float>("MC_W_from_t_pt");
            m_MC_W_from_t_eta = topParton->auxdata<float>("MC_W_from_t_eta");
            m_MC_W_from_t_phi = topParton->auxdata<float>("MC_W_from_t_phi");
            m_MC_W_from_t_m   = topParton->auxdata<float>("MC_W_from_t_m");
        }

        if (topParton->auxdata<float>("MC_W_from_tbar_pt") > 0) {
            m_MC_W_from_tbar_pt  = topParton->auxdata<float>("MC_W_from_tbar_pt");
            m_MC_W_from_tbar_eta = topParton->auxdata<float>("MC_W_from_tbar_eta");
            m_MC_W_from_tbar_phi = topParton->auxdata<float>("MC_W_from_tbar_phi");
            m_MC_W_from_tbar_m   = topParton->auxdata<float>("MC_W_from_tbar_m");
        }

        if (topParton->auxdata<float>("MC_t_beforeFSR_pt") > 0) {
            m_MC_t_pt  = topParton->auxdata<float>("MC_t_beforeFSR_pt");
            m_MC_t_eta = topParton->auxdata<float>("MC_t_beforeFSR_eta");
            m_MC_t_phi = topParton->auxdata<float>("MC_t_beforeFSR_phi");
            m_MC_t_m   = topParton->auxdata<float>("MC_t_beforeFSR_m");
            parton_t_p4.SetPtEtaPhiM(m_MC_t_pt, m_MC_t_eta, m_MC_t_phi, m_MC_t_m);
        }

        if (topParton->auxdata<float>("MC_tbar_beforeFSR_pt") > 0) {
            m_MC_tbar_pt  = topParton->auxdata<float>("MC_tbar_beforeFSR_pt");
            m_MC_tbar_eta = topParton->auxdata<float>("MC_tbar_beforeFSR_eta");
            m_MC_tbar_phi = topParton->auxdata<float>("MC_tbar_beforeFSR_phi");
            m_MC_tbar_m   = topParton->auxdata<float>("MC_tbar_beforeFSR_m");
            parton_tbar_p4.SetPtEtaPhiM(m_MC_tbar_pt, m_MC_tbar_eta, m_MC_tbar_phi, m_MC_tbar_m);
        }

        if (topParton->auxdata<float>("MC_ttbar_beforeFSR_pt") > 0) {
            m_MC_ttbar_beforeFSR_pt  = topParton->auxdata<float>("MC_ttbar_beforeFSR_pt");
            m_MC_ttbar_beforeFSR_eta = topParton->auxdata<float>("MC_ttbar_beforeFSR_eta");
            m_MC_ttbar_beforeFSR_phi = topParton->auxdata<float>("MC_ttbar_beforeFSR_phi");
            m_MC_ttbar_beforeFSR_m   = topParton->auxdata<float>("MC_ttbar_beforeFSR_m");
        }

        if (topParton->auxdata<float>("MC_ttbar_afterFSR_pt") > 0) {
            m_MC_ttbar_afterFSR_pt  = topParton->auxdata<float>("MC_ttbar_afterFSR_pt");
            m_MC_ttbar_afterFSR_eta = topParton->auxdata<float>("MC_ttbar_afterFSR_eta");
            m_MC_ttbar_afterFSR_phi = topParton->auxdata<float>("MC_ttbar_afterFSR_phi");
            m_MC_ttbar_afterFSR_m   = topParton->auxdata<float>("MC_ttbar_afterFSR_m");
        }

        TLorentzVector b_from_t_lv;
        TLorentzVector b_from_tbar_lv;

        TLorentzVector W1decay_from_t_lv;
        TLorentzVector W2decay_from_t_lv;

        TLorentzVector W1decay_from_tbar_lv;
        TLorentzVector W2decay_from_tbar_lv;

        b_from_t_lv.SetPtEtaPhiM(m_MC_b_from_t_pt, m_MC_b_from_t_eta, m_MC_b_from_t_phi, m_MC_b_from_t_m);
        W1decay_from_t_lv.SetPtEtaPhiM(m_MC_Wdecay1_from_t_pt, m_MC_Wdecay1_from_t_eta, m_MC_Wdecay1_from_t_phi, m_MC_Wdecay1_from_t_m);
        W2decay_from_t_lv.SetPtEtaPhiM(m_MC_Wdecay2_from_t_pt, m_MC_Wdecay2_from_t_eta, m_MC_Wdecay2_from_t_phi, m_MC_Wdecay2_from_t_m);

        b_from_tbar_lv.SetPtEtaPhiM(m_MC_b_from_tbar_pt,  m_MC_b_from_tbar_eta, m_MC_b_from_tbar_phi, m_MC_b_from_tbar_m);
        W1decay_from_tbar_lv.SetPtEtaPhiM(m_MC_Wdecay1_from_tbar_pt, m_MC_Wdecay1_from_tbar_eta, m_MC_Wdecay1_from_tbar_phi, m_MC_Wdecay1_from_tbar_m);
        W2decay_from_tbar_lv.SetPtEtaPhiM(m_MC_Wdecay2_from_tbar_pt, m_MC_Wdecay2_from_tbar_eta, m_MC_Wdecay2_from_tbar_phi, m_MC_Wdecay2_from_tbar_m);

        TLorentzVector temp1;
        temp1 = b_from_t_lv + W1decay_from_t_lv + W2decay_from_t_lv;
        m_MC_t_afterFSR_pt  = temp1.Pt();
        m_MC_t_afterFSR_eta = temp1.Eta();
        m_MC_t_afterFSR_phi = temp1.Phi();
        m_MC_t_afterFSR_m   = temp1.M();

        TLorentzVector temp2;
        temp2 = b_from_tbar_lv + W1decay_from_tbar_lv + W2decay_from_tbar_lv;

        m_MC_tbar_afterFSR_pt  = temp2.Pt();
        m_MC_tbar_afterFSR_eta = temp2.Eta();
        m_MC_tbar_afterFSR_phi = temp2.Phi();
        m_MC_tbar_afterFSR_m   = temp2.M();

        //if (m_runEWK && event.m_truth) {
        //  double EWSF = weak->getWeight(m_MC_t_pt,m_MC_t_eta,m_MC_t_phi,m_MC_t_m,m_MC_tbar_pt,m_MC_tbar_eta,m_MC_tbar_phi,m_MC_tbar_m,m_initial_type);
        //  m_weight_EW = EWSF;
        //}

        // matching criteria
        float MAcriteria_fatjets  = 1.0;
        float MAcriteria_jets     = 0.4;
        float MAcriteria_chLep    = 0.2;
        float MAcriteria_neutrino = 1. ;

        // had parton => truthjet index
        int   MC_truth_b_from_t_idx(-1);
        int   MC_truth_b_from_tbar_idx(-1);
        int   MC_truth_Wdecay1_from_t_idx(-1);
        int   MC_truth_Wdecay2_from_t_idx(-1);
        int   MC_truth_Wdecay1_from_tbar_idx(-1);
        int   MC_truth_Wdecay2_from_tbar_idx(-1);
        int   MC_truth_t_idx(-1);
        int   MC_truth_tbar_idx(-1);

        // Decay mode of the W boson
        std::string W_t_DecayMode    = "";
        int t_type = 0;
        WbosonDecayMode(m_MC_Wdecay1_from_t_pdgId, m_MC_Wdecay2_from_t_pdgId, W_t_DecayMode, t_type);

        std::string W_tbar_DecayMode = "";
        int tbar_type = 0;
        WbosonDecayMode(m_MC_Wdecay1_from_tbar_pdgId, m_MC_Wdecay2_from_tbar_pdgId, W_tbar_DecayMode, tbar_type);

        //-----------------------------------------------------------
        // b jets: parton -> truthjet -> recojet
        //-----------------------------------------------------------

        // b from t
        if (parton_b_from_t_p4.Pt() > 0) {

            MA_hadParton_truthjet(parton_b_from_t_p4, MC_truth_b_from_t_idx, m_MA_b_from_t_truthdr, MAcriteria_jets);

            if (MC_truth_b_from_t_idx >= 0) {

                MA_truthjet_recojet(event, akt4truthjets->at(MC_truth_b_from_t_idx), m_MA_b_from_t_recodr, m_MA_b_from_t_recoidx,
                                    m_MA_b_from_t_pt, m_MA_b_from_t_eta, m_MA_b_from_t_phi, m_MA_b_from_t_m, MAcriteria_jets);

            }//if
        }//if(parton_b_from_t_p4.Pt()>0)

        // b from tbar
        if (parton_b_from_tbar_p4.Pt() > 0) {

            MA_hadParton_truthjet(parton_b_from_tbar_p4, MC_truth_b_from_tbar_idx, m_MA_b_from_tbar_truthdr, MAcriteria_jets);

            if (MC_truth_b_from_tbar_idx >= 0) {

                MA_truthjet_recojet(event, akt4truthjets->at(MC_truth_b_from_tbar_idx), m_MA_b_from_tbar_recodr, m_MA_b_from_tbar_recoidx,
                                    m_MA_b_from_tbar_pt, m_MA_b_from_tbar_eta, m_MA_b_from_tbar_phi, m_MA_b_from_tbar_m, MAcriteria_jets);
            }//if
        }//if(parton_b_from_tbar_p4.Pt()>0)

        //-----------------------------------------------------------
        // matching W from top
        //-----------------------------------------------------------
        // Hadronic W decay: parton -> truthjet -> recojet
        //-----------------------------------------------------------
        // Leptonic W decay: parton -> reco objects (ch lep and met)
        //-----------------------------------------------------------

        if (W_t_DecayMode == "Hadronic") {

            // Wdecay1 from t
            if (parton_Wdecay1_from_t_p4.Pt() > 0) {

                MA_hadParton_truthjet(parton_Wdecay1_from_t_p4, MC_truth_Wdecay1_from_t_idx, m_MA_Wdecay1_from_t_truthdr, MAcriteria_jets);

                if (MC_truth_Wdecay1_from_t_idx >= 0) {

                    MA_truthjet_recojet(event, akt4truthjets->at(MC_truth_Wdecay1_from_t_idx), m_MA_Wdecay1_from_t_recodr, m_MA_Wdecay1_from_t_recoidx,
                                        m_MA_Wdecay1_from_t_pt, m_MA_Wdecay1_from_t_eta, m_MA_Wdecay1_from_t_phi, m_MA_Wdecay1_from_t_m, MAcriteria_jets);

                }//if
            }//if(parton_Wdecay1_from_t_p4.Pt()>0)

            // Wdecay2 from t
            if (parton_Wdecay2_from_t_p4.Pt() > 0) {

                MA_hadParton_truthjet(parton_Wdecay2_from_t_p4, MC_truth_Wdecay2_from_t_idx, m_MA_Wdecay2_from_t_truthdr, MAcriteria_jets);

                if (MC_truth_Wdecay2_from_t_idx >= 0) {

                    MA_truthjet_recojet(event, akt4truthjets->at(MC_truth_Wdecay2_from_t_idx), m_MA_Wdecay2_from_t_recodr, m_MA_Wdecay2_from_t_recoidx,
                                        m_MA_Wdecay2_from_t_pt, m_MA_Wdecay2_from_t_eta, m_MA_Wdecay2_from_t_phi, m_MA_Wdecay2_from_t_m, MAcriteria_jets);
                }//if
            }//if(parton_Wdecay2_from_t_p4.Pt()>0)


            // t matching to fat jet
            if (parton_t_p4.Pt() > 0) {
                float m_MA_tFJ_truthdr(-1);
                MA_hadParton_truthjet(parton_t_p4, MC_truth_t_idx, m_MA_tFJ_truthdr, MAcriteria_fatjets, true);

                if (MC_truth_t_idx >= 0) {

                    float m_MA_tFJ_recodr(-1);
                    int m_MA_tFJ_recoidx(-1);
                    MA_truthjet_recojet(event, akt10truthjets->at(MC_truth_t_idx), m_MA_tFJ_recodr, m_MA_tFJ_recoidx,
                                        m_MA_tFJ_pt, m_MA_tFJ_eta, m_MA_tFJ_phi, m_MA_tFJ_m, MAcriteria_fatjets, true);
                    if (m_MA_tFJ_recoidx >= 0) {
                        if (parton_Wdecay1_from_t_p4.Pt() > 0 && parton_Wdecay1_from_t_p4.DeltaR(event.m_largeJets.at(m_MA_tFJ_recoidx)->p4()) < 0.75)m_MA_tFJ_nPartons++;
                        if (parton_Wdecay2_from_t_p4.Pt() > 0 && parton_Wdecay2_from_t_p4.DeltaR(event.m_largeJets.at(m_MA_tFJ_recoidx)->p4()) < 0.75)m_MA_tFJ_nPartons++;
                        if (parton_Wdecay2_from_t_p4.Pt() > 0 && parton_Wdecay2_from_t_p4.DeltaR(event.m_largeJets.at(m_MA_tFJ_recoidx)->p4()) < 0.75)m_MA_tFJ_nPartons++;
                    }
                }//if
            }//if(parton_t_p4.Pt()>0)



        } else if (W_t_DecayMode == "Leptonic") {

            // Ch lepton
            if (m_MC_Wdecay2_from_t_pdgId == -11 && parton_Wdecay2_from_t_p4.Pt() > 0) {
                m_MA_Wdecay2_from_t_truthdr = -1;
                MA_eParton_reco(event, parton_Wdecay2_from_t_p4, m_MA_Wdecay2_from_t_recoidx, m_MA_Wdecay2_from_t_recodr, MAcriteria_chLep);

                if (m_MA_Wdecay2_from_t_recoidx >= 0) {
                    sum_of_lep_reco += event.m_electrons.at(m_MA_Wdecay2_from_t_recoidx)->p4();
                    sum_of_nu_parton += parton_Wdecay1_from_t_p4;
                    TLorentzFill(event.m_electrons.at(m_MA_Wdecay2_from_t_recoidx)->p4(), m_MA_Wdecay2_from_t_m, m_MA_Wdecay2_from_t_pt, m_MA_Wdecay2_from_t_eta, m_MA_Wdecay2_from_t_phi);
                }//((m_MA_Wdecay2_from_t_recoidx>=0)

            } else if (m_MC_Wdecay2_from_t_pdgId == -13 && parton_Wdecay2_from_t_p4.Pt() > 0) {
                m_MA_Wdecay2_from_t_truthdr = -1;
                MA_muParton_reco(event, parton_Wdecay2_from_t_p4, m_MA_Wdecay2_from_t_recoidx, m_MA_Wdecay2_from_t_recodr, MAcriteria_chLep);

                if (m_MA_Wdecay2_from_t_recoidx >= 0) {
                    sum_of_lep_reco += event.m_muons.at(m_MA_Wdecay2_from_t_recoidx)->p4();
                    sum_of_nu_parton += parton_Wdecay1_from_t_p4;
                    TLorentzFill(event.m_muons.at(m_MA_Wdecay2_from_t_recoidx)->p4(), m_MA_Wdecay2_from_t_m, m_MA_Wdecay2_from_t_pt, m_MA_Wdecay2_from_t_eta, m_MA_Wdecay2_from_t_phi);
                }//if(m_MA_Wdecay2_from_t_recoidx>=0)

            } else if (m_MC_Wdecay2_from_t_pdgId == -15 && parton_Wdecay2_from_t_p4.Pt() > 0) {
                m_MA_Wdecay2_from_t_truthdr = -1;
                const xAOD::ElectronContainer electrons = event.m_electrons;
                const xAOD::MuonContainer muons = event.m_muons;

                if (electrons.size() > 0) {
                    MA_eParton_reco(event, parton_Wdecay2_from_t_p4, m_MA_Wdecay2_from_t_recoidx, m_MA_Wdecay2_from_t_recodr, MAcriteria_chLep);
                    if (m_MA_Wdecay2_from_t_recoidx >= 0) {
                        sum_of_lep_reco += event.m_electrons.at(m_MA_Wdecay2_from_t_recoidx)->p4();
                        sum_of_nu_parton += parton_Wdecay1_from_t_p4;
                        TLorentzFill(event.m_electrons.at(m_MA_Wdecay2_from_t_recoidx)->p4(), m_MA_Wdecay2_from_t_m, m_MA_Wdecay2_from_t_pt, m_MA_Wdecay2_from_t_eta, m_MA_Wdecay2_from_t_phi);
                    }//if(m_MA_Wdecay2_from_t_recoidx>=0)

                } else if (muons.size() > 0) {
                    MA_muParton_reco(event, parton_Wdecay2_from_t_p4, m_MA_Wdecay2_from_t_recoidx, m_MA_Wdecay2_from_t_recodr, MAcriteria_chLep);
                    if (m_MA_Wdecay2_from_t_recoidx >= 0) {
                        sum_of_lep_reco += event.m_muons.at(m_MA_Wdecay2_from_t_recoidx)->p4();
                        sum_of_nu_parton += parton_Wdecay1_from_t_p4;
                        TLorentzFill(event.m_muons.at(m_MA_Wdecay2_from_t_recoidx)->p4(), m_MA_Wdecay2_from_t_m, m_MA_Wdecay2_from_t_pt, m_MA_Wdecay2_from_t_eta, m_MA_Wdecay2_from_t_phi);
                    }//if(m_MA_Wdecay2_from_t_recoidx>=0)

                }//else if
            }//else if
        }//if(W_t_DecayMode=="Leptonic")


        //-----------------------------------------------------------
        // matching W from tbar
        //-----------------------------------------------------------
        // Hadronic W decay: parton -> truthjet -> recojet
        //-----------------------------------------------------------
        // Leptonic W decay: parton -> reco objects (ch lep and met)
        //-----------------------------------------------------------

        if (W_tbar_DecayMode == "Hadronic") {

            // Wdecay1 from tbar
            if (parton_Wdecay1_from_tbar_p4.Pt() > 0) {

                MA_hadParton_truthjet(parton_Wdecay1_from_tbar_p4, MC_truth_Wdecay1_from_tbar_idx, m_MA_Wdecay1_from_tbar_truthdr, MAcriteria_jets);

                if (MC_truth_Wdecay1_from_tbar_idx >= 0) {

                    MA_truthjet_recojet(event, akt4truthjets->at(MC_truth_Wdecay1_from_tbar_idx), m_MA_Wdecay1_from_tbar_recodr, m_MA_Wdecay1_from_tbar_recoidx,
                                        m_MA_Wdecay1_from_tbar_pt, m_MA_Wdecay1_from_tbar_eta, m_MA_Wdecay1_from_tbar_phi, m_MA_Wdecay1_from_tbar_m, MAcriteria_jets);
                }//if
            }//if(parton_Wdecay1_from_tbar_p4.Pt()>0)

            // Wdecay2 from tbar
            if (parton_Wdecay2_from_tbar_p4.Pt() > 0) {

                MA_hadParton_truthjet(parton_Wdecay2_from_tbar_p4, MC_truth_Wdecay2_from_tbar_idx, m_MA_Wdecay2_from_tbar_truthdr, MAcriteria_jets);

                if (MC_truth_Wdecay2_from_tbar_idx >= 0) {

                    MA_truthjet_recojet(event, akt4truthjets->at(MC_truth_Wdecay2_from_tbar_idx), m_MA_Wdecay2_from_tbar_recodr, m_MA_Wdecay2_from_tbar_recoidx,
                                        m_MA_Wdecay2_from_tbar_pt, m_MA_Wdecay2_from_tbar_eta, m_MA_Wdecay2_from_tbar_phi, m_MA_Wdecay2_from_tbar_m, MAcriteria_jets);
                }//if
            }//if(parton_Wdecay2_from_tbar_p4.Pt()>0)

            // tbar matching to fat jet
            if (parton_tbar_p4.Pt() > 0) {
                float m_MA_tbarFJ_truthdr(-1);
                MA_hadParton_truthjet(parton_tbar_p4, MC_truth_tbar_idx, m_MA_tbarFJ_truthdr, MAcriteria_fatjets, true);

                if (MC_truth_tbar_idx >= 0) {

                    float m_MA_tbarFJ_recodr(-1);
                    int m_MA_tbarFJ_recoidx(-1);
                    MA_truthjet_recojet(event, akt10truthjets->at(MC_truth_tbar_idx), m_MA_tbarFJ_recodr, m_MA_tbarFJ_recoidx,
                                        m_MA_tbarFJ_pt, m_MA_tbarFJ_eta, m_MA_tbarFJ_phi, m_MA_tbarFJ_m, MAcriteria_fatjets, true);
                    if (m_MA_tbarFJ_recoidx >= 0) {
                        if (parton_Wdecay1_from_tbar_p4.Pt() > 0 && parton_Wdecay1_from_tbar_p4.DeltaR(event.m_largeJets.at(m_MA_tbarFJ_recoidx)->p4()) < 0.75)m_MA_tbarFJ_nPartons++;
                        if (parton_Wdecay2_from_tbar_p4.Pt() > 0 && parton_Wdecay2_from_tbar_p4.DeltaR(event.m_largeJets.at(m_MA_tbarFJ_recoidx)->p4()) < 0.75)m_MA_tbarFJ_nPartons++;
                        if (parton_Wdecay2_from_tbar_p4.Pt() > 0 && parton_Wdecay2_from_tbar_p4.DeltaR(event.m_largeJets.at(m_MA_tbarFJ_recoidx)->p4()) < 0.75)m_MA_tbarFJ_nPartons++;
                    }
                }//if
            }//if(parton_tbar_p4.Pt()>0)


        } else if (W_tbar_DecayMode == "Leptonic") {

            // Ch lepton
            if (m_MC_Wdecay1_from_tbar_pdgId == 11 && parton_Wdecay1_from_tbar_p4.Pt() > 0) {
                m_MA_Wdecay1_from_tbar_truthdr = -1;
                MA_eParton_reco(event, parton_Wdecay1_from_tbar_p4, m_MA_Wdecay1_from_tbar_recoidx, m_MA_Wdecay1_from_tbar_recodr, MAcriteria_chLep);

                if (m_MA_Wdecay1_from_tbar_recoidx >= 0) {
                    sum_of_lep_reco += event.m_electrons.at(m_MA_Wdecay1_from_tbar_recoidx)->p4();
                    sum_of_nu_parton += parton_Wdecay2_from_tbar_p4;
                    TLorentzFill(event.m_electrons.at(m_MA_Wdecay1_from_tbar_recoidx)->p4(), m_MA_Wdecay1_from_tbar_m, m_MA_Wdecay1_from_tbar_pt, m_MA_Wdecay1_from_tbar_eta, m_MA_Wdecay1_from_tbar_phi);
                }//if(m_MA_Wdecay1_from_tbar_idx>=0)

            } else if (m_MC_Wdecay1_from_tbar_pdgId == 13 && parton_Wdecay1_from_tbar_p4.Pt() > 0) {
                m_MA_Wdecay1_from_tbar_truthdr = -1;
                MA_muParton_reco(event, parton_Wdecay1_from_tbar_p4, m_MA_Wdecay1_from_tbar_recoidx, m_MA_Wdecay1_from_tbar_recodr, MAcriteria_chLep);

                if (m_MA_Wdecay1_from_tbar_recoidx >= 0) {
                    sum_of_lep_reco += event.m_muons.at(m_MA_Wdecay1_from_tbar_recoidx)->p4();
                    sum_of_nu_parton += parton_Wdecay2_from_tbar_p4;
                    TLorentzFill(event.m_muons.at(m_MA_Wdecay1_from_tbar_recoidx)->p4(), m_MA_Wdecay1_from_tbar_m, m_MA_Wdecay1_from_tbar_pt, m_MA_Wdecay1_from_tbar_eta, m_MA_Wdecay1_from_tbar_phi);
                }//if(m_MA_Wdecay1_from_tbar_idx>=0)

            } else if (m_MC_Wdecay1_from_tbar_pdgId == 15 && parton_Wdecay1_from_tbar_p4.Pt() > 0) {
                m_MA_Wdecay1_from_tbar_truthdr = -1;
                const xAOD::ElectronContainer electrons = event.m_electrons;
                const xAOD::MuonContainer muons = event.m_muons;

                if (electrons.size() > 0) {
                    MA_eParton_reco(event, parton_Wdecay1_from_tbar_p4, m_MA_Wdecay1_from_tbar_recoidx, m_MA_Wdecay1_from_tbar_recodr, MAcriteria_chLep);
                    if (m_MA_Wdecay1_from_tbar_recoidx >= 0) {
                        sum_of_lep_reco += event.m_electrons.at(m_MA_Wdecay1_from_tbar_recoidx)->p4();
                        sum_of_nu_parton += parton_Wdecay2_from_tbar_p4;
                        TLorentzFill(event.m_electrons.at(m_MA_Wdecay1_from_tbar_recoidx)->p4(), m_MA_Wdecay1_from_tbar_m, m_MA_Wdecay1_from_tbar_pt, m_MA_Wdecay1_from_tbar_eta, m_MA_Wdecay1_from_tbar_phi);
                    }//if(m_MA_Wdecay1_from_tbar_idx>=0)

                } else if (muons.size() > 0) {
                    MA_muParton_reco(event, parton_Wdecay1_from_tbar_p4, m_MA_Wdecay1_from_tbar_recoidx, m_MA_Wdecay1_from_tbar_recodr, MAcriteria_chLep);
                    if (m_MA_Wdecay1_from_tbar_recoidx >= 0) {
                        sum_of_lep_reco += event.m_muons.at(m_MA_Wdecay1_from_tbar_recoidx)->p4();
                        sum_of_nu_parton += parton_Wdecay2_from_tbar_p4;
                        TLorentzFill(event.m_muons.at(m_MA_Wdecay1_from_tbar_recoidx)->p4(), m_MA_Wdecay1_from_tbar_m, m_MA_Wdecay1_from_tbar_pt, m_MA_Wdecay1_from_tbar_eta, m_MA_Wdecay1_from_tbar_phi);
                    }//if(m_MA_Wdecay1_from_tbar_idx>=0)

                }//else if
            }//else if
        }//if(W_tbar_DecayMode=="Leptonic")

        //MET

        if (W_t_DecayMode == "Leptonic") {
            if (sum_of_nu_parton.Pt() > 0 && sum_of_lep_reco.Pt() > 0) {
                nu_from_met = MA_nuParton_met(met, sum_of_nu_parton, sum_of_lep_reco, m_MA_Wdecay1_from_t_NuDphi);

                if (m_MA_Wdecay1_from_t_NuDphi < MAcriteria_neutrino) {
                    m_MA_Wdecay1_from_t_truthdr = -1;
                    m_MA_Wdecay1_from_t_recodr  = m_MA_Wdecay1_from_t_NuDphi;
                    m_MA_Wdecay1_from_t_recoidx = 0;
                    TLorentzFill(*nu_from_met, m_MA_Wdecay1_from_t_m, m_MA_Wdecay1_from_t_pt, m_MA_Wdecay1_from_t_eta, m_MA_Wdecay1_from_t_phi);
                }//if
            }//if
        }//if

        if (W_tbar_DecayMode == "Leptonic") {
            if (sum_of_nu_parton.Pt() > 0 && sum_of_lep_reco.Pt() > 0) {
                nu_from_met = MA_nuParton_met(met, sum_of_nu_parton, sum_of_lep_reco, m_MA_Wdecay2_from_tbar_NuDphi);

                if (m_MA_Wdecay2_from_tbar_NuDphi < MAcriteria_neutrino) {
                    m_MA_Wdecay2_from_tbar_truthdr = -1;
                    m_MA_Wdecay2_from_tbar_recodr  = m_MA_Wdecay2_from_tbar_NuDphi;
                    m_MA_Wdecay2_from_tbar_recoidx = 0;
                    TLorentzFill(*nu_from_met, m_MA_Wdecay2_from_tbar_m, m_MA_Wdecay2_from_tbar_pt, m_MA_Wdecay2_from_tbar_eta, m_MA_Wdecay2_from_tbar_phi);
                }//if
            }//if
        }//if

        //if(topParton)delete topParton;
        //if(akt4truthjets)delete akt4truthjets;
        if (nu_from_met)delete nu_from_met;
        if (met)delete met;
        Reconstruction_w_top_ttbar(W_t_DecayMode, t_type, W_tbar_DecayMode, tbar_type); // Reconstruction of particles from the matched reco objetcs
        Fill_MA_semileptonic(W_t_DecayMode, W_tbar_DecayMode); // Reconstruction of particles for semi-leptonic cases (-->define tl, wl, th, wh, ...)
#ifdef ENABLE_LJETSUBSTRUCTURE_DEBUG
        unsigned int k = 0;
        float Rmatch = 0.75; //this is a variable: R_match=0.75*R_ljet!
        TLorentzVector MC_bh_p4, MC_w1h_p4, MC_w2h_p4;
        MC_bh_p4.SetPtEtaPhiM(m_MC_bh_pt, m_MC_bh_eta, m_MC_bh_phi, m_MC_bh_m);
        MC_w1h_p4.SetPtEtaPhiM(m_MC_w1h_pt, m_MC_w1h_eta, m_MC_w1h_phi, m_MC_w1h_m);
        MC_w2h_p4.SetPtEtaPhiM(m_MC_w2h_pt, m_MC_w2h_eta, m_MC_w2h_phi, m_MC_w2h_m);
        for (const auto* const jetPtr : event.m_largeJets)
        {
            if ((MC_w1h_p4.DeltaR(jetPtr->p4()) < Rmatch) && (MC_w2h_p4.DeltaR(jetPtr->p4()) < Rmatch))
            {
                if (MC_bh_p4.DeltaR(jetPtr->p4()) < Rmatch) {m_ljet_MClike[k] = 2; }
                else {m_ljet_MClike[k] = 1; }
            }
            else { m_ljet_MClike[k] = 0; }
            k++;
        }
#endif

        if (m_chi2Sel->apply(event) && (!m_runHtt)) {
            TtresChi2* chi2Tool = m_chi2Sel->getChi2Tool();
            TLorentzFill(chi2Tool->getResult_Wl(), m_chi2_wl_m, m_chi2_wl_pt, m_chi2_wl_eta, m_chi2_wl_phi);
            TLorentzFill(chi2Tool->getResult_Wh(), m_chi2_wh_m, m_chi2_wh_pt, m_chi2_wh_eta, m_chi2_wh_phi);
            TLorentzFill(chi2Tool->getResult_Bl(), m_chi2_bl_m, m_chi2_bl_pt, m_chi2_bl_eta, m_chi2_bl_phi);
            TLorentzFill(chi2Tool->getResult_Bh(), m_chi2_bh_m, m_chi2_bh_pt, m_chi2_bh_eta, m_chi2_bh_phi);
            TLorentzFill(chi2Tool->getResult_Tl(), m_chi2_tl_m, m_chi2_tl_pt, m_chi2_tl_eta, m_chi2_tl_phi);
            TLorentzFill(chi2Tool->getResult_Th(), m_chi2_th_m, m_chi2_th_pt, m_chi2_th_eta, m_chi2_th_phi);
            TLorentzFill(chi2Tool->getResult_TT(), m_chi2_ttbar_m, m_chi2_ttbar_pt, m_chi2_ttbar_eta, m_chi2_ttbar_phi);
        }


    }//if (m_savePartons)

    if (m_savePdfWeight) {
        const xAOD::TruthEventContainer* truthEvent(nullptr);
        top::check( evtStore()->retrieve(truthEvent, "TruthEvents") , "Failed to retrieve truth PDF info" );
        for ( auto tePtr : *truthEvent )
            for ( const std::string& pdf_name : m_LHAPDFSets )
                if ( tePtr->isAvailable< std::vector<float> >("AnalysisTop_" + pdf_name + "_Weights") )
                    m_PDF_eventWeights[ pdf_name ] = tePtr->auxdata< std::vector<float> >("AnalysisTop_" + pdf_name + "_Weights");
    }
#ifdef ENABLE_BTAG_DEBUG
    std::cout << "DEBUG: TtresEventSaverFlatNtuple::saveEvent ->end. Now running EventSaverFlatNtuple::saveEvent(event)" << std::endl;
#endif

    EventSaverFlatNtuple::saveEvent(event);
#ifdef ENABLE_BTAG_DEBUG
    std::cout << "DEBUG: TtresEventSaverFlatNtuple::EventSaverFlatNtuple::saveEvent --> end" << std::endl;
#endif
}

void TtresEventSaverFlatNtuple::DeltaR_min(TLorentzVector p1, TLorentzVector p2, int i, float & tmp_dr, int & truth_idx) {

    float dr = p1.DeltaR(p2);
    if (dr < tmp_dr) {
        tmp_dr = dr;
        truth_idx = i;
    }

}//DeltaR_min

void TtresEventSaverFlatNtuple::MA_eParton_reco(const top::Event& event, TLorentzVector parton_e, int & reco_idx, float & reco_dr, float criteria) {

    float tmp_idx = -1;
    float tmp_dr  = criteria;

    const xAOD::ElectronContainer electrons = event.m_electrons;
    int j = 0;
    for (xAOD::ElectronContainer::const_iterator e_itr = electrons.begin(); e_itr != electrons.end(); ++e_itr) {
        const xAOD::Electron * lep = *e_itr;
        float dr = lep->p4().DeltaR(parton_e);
        if (dr < tmp_dr) {
            tmp_dr = dr;
            tmp_idx = j;
        }
        ++j;
    }//for

    if (tmp_dr > 0 && tmp_dr < criteria) {
        reco_idx = tmp_idx;
        reco_dr  = tmp_dr;

    } else {
        reco_idx = -1;
        reco_dr  = -1;

    }//tmp_dr>0 && tmp_dr<criteria)

}//PartonRecoLeptonMA_e

void TtresEventSaverFlatNtuple::MA_muParton_reco(const top::Event& event, TLorentzVector parton_mu, int & reco_idx, float & reco_dr, float criteria) {

    float tmp_idx = -1;
    float tmp_dr  = criteria;

    const xAOD::MuonContainer muons = event.m_muons;
    int j = 0;
    for (xAOD::MuonContainer::const_iterator mu_itr = muons.begin(); mu_itr != muons.end(); ++mu_itr) {
        const xAOD::Muon * lep = *mu_itr;
        float dr = lep->p4().DeltaR(parton_mu);
        if (dr < tmp_dr) {
            tmp_dr = dr;
            tmp_idx = j;
        }
        ++j;
    }//for

    if (tmp_dr > 0 && tmp_dr < criteria) {
        reco_idx = tmp_idx;
        reco_dr  = tmp_dr;

    } else {
        reco_idx = -1;
        reco_dr  = -1;

    }//tmp_dr>0 && tmp_dr<criteria)

}//PartonRecoLeptonMA_mu

void TtresEventSaverFlatNtuple::MA_hadParton_truthjet(TLorentzVector p1, int & truth_idx, float & truth_dr, float criteria, bool useFatJet) {

    float tmp_dr  = criteria;
    float tmp_idx = -1 ;

    const xAOD::JetContainer *akttruthjets = nullptr;
    if (useFatJet)
        top::check(evtStore()->event()->retrieve(akttruthjets, m_akt10truthjetcollection), "FAILURE");
    else
        top::check(evtStore()->event()->retrieve(akttruthjets, "AntiKt4TruthJets"), "FAILURE");

    int i = 0;
    for (xAOD::JetContainer::const_iterator jet_itr = akttruthjets->begin(); jet_itr != akttruthjets->end(); ++jet_itr) {
        const xAOD::Jet* akt4truthjetPtr = *jet_itr;

        float dr = p1.DeltaR(akt4truthjetPtr->p4());
        if (dr < tmp_dr) {
            tmp_dr = dr;
            tmp_idx = i;
        }
        ++i;

    }//for

    if (tmp_dr > 0 && tmp_dr < criteria) {

        truth_idx = tmp_idx;
        truth_dr  = tmp_dr;
    } else {

        truth_idx = -1;
        truth_dr  = -1;
    }//tmp_dr>0 && tmp_dr<criteria)

}//MA_hadParton_truthjet

TLorentzVector * TtresEventSaverFlatNtuple::MA_nuParton_met(TLorentzVector * met, TLorentzVector parton_Nu,  TLorentzVector reco_chLep, float & dphi) {

    std::string units = "MeV";
    if (units == "MeV")        m_Units = 1000.;
    else if (units == "GeV")   m_Units = 1.;
    TtresNeutrinoBuilder m_NeutrinoBuilder(units);

    //dphi = met->DeltaPhi(sum_of_nu_parton);
    dphi = TMath::Abs(TVector2::Phi_mpi_pi(met->Phi() - parton_Nu.Phi()));
    //std::cout << "dphi: " << dphi << std::endl;

    std::vector<TLorentzVector*> nu_vector = m_NeutrinoBuilder.candidatesFromWMass_Rotation(&reco_chLep, met, true);
    if (nu_vector.size() > 1)   delete nu_vector.at(1);

    return nu_vector.at(0);

}//PartonRecoLeptonMA_mu


void TtresEventSaverFlatNtuple::TLorentzFill(const TLorentzVector& p1, float & mass, float & pt, float & eta, float & phi) {

    mass  = p1.M();
    pt    = p1.Pt();
    eta   = p1.Eta();
    phi   = p1.Phi();

}//partonFill

void TtresEventSaverFlatNtuple::MA_truthjet_recojet(const top::Event& event, const xAOD::Jet* trueJet, float & reco_dr, int & reco_idx, float & pt, float & eta, float & phi, float & mass, float criteria, bool useFatJet) {


    TLorentzVector akttruthjets_p4 = trueJet->p4();
    reco_dr  = criteria;
    reco_idx = -1;

    // match truth jets with the reco jets
    xAOD::JetContainer jets = useFatJet ? event.m_largeJets : event.m_jets;

    int i = 0;
    for (xAOD::JetContainer::const_iterator jet_itr = jets.begin(); jet_itr != jets.end(); ++jet_itr) {
        const xAOD::Jet* jetPtr = *jet_itr;

        // check the dr between this parton's truth jet and the reco jet
        float dr = akttruthjets_p4.DeltaR(jetPtr->p4());
        if (dr < reco_dr) {
            reco_dr = dr;
            reco_idx = i;
            pt   = jets.at(i)->pt();
            eta  = jets.at(i)->eta();
            phi  = jets.at(i)->phi();
            mass = jets.at(i)->m();
        }
        ++i;
    }
}//MA_truthjet_recojet

int TtresEventSaverFlatNtuple::AnalyzeRecoIndex_forW(TLorentzVector & w_p4, TLorentzVector Wdecay1_p4, const int reco_Wdecay1_idx, TLorentzVector Wdecay2_p4, const int reco_Wdecay2_idx) {

    int w_Njets = -1;
    if (reco_Wdecay1_idx >= 0 && reco_Wdecay2_idx >= 0) {
        if (reco_Wdecay1_idx != reco_Wdecay2_idx) {
            w_p4 = Wdecay1_p4 + Wdecay2_p4;
            w_Njets = 2;
        } else {
            w_p4 = Wdecay1_p4;
            w_Njets = 1;
        }
    }

    return w_Njets;

}//AnalyzeRecoIndex_forW


int TtresEventSaverFlatNtuple::AnalyzeRecoIndex_forTop(TLorentzVector & t_p4, TLorentzVector w_p4, TLorentzVector b_p4, TLorentzVector Wdecay1_p4, TLorentzVector Wdecay2_p4, const int reco_b_idx, const int reco_Wdecay1_idx, const int reco_Wdecay2_idx) {

    int t_Njets = AnalyzeRecoIndex_forW(w_p4, Wdecay1_p4, reco_Wdecay1_idx, Wdecay2_p4, reco_Wdecay2_idx);

    if (reco_b_idx == reco_Wdecay1_idx || reco_b_idx == reco_Wdecay2_idx) {
        t_p4 = w_p4;
    }
    else if (reco_b_idx >= 0) {
        t_p4 = w_p4 + b_p4;
        t_Njets++;
    }
    return t_Njets;

}//AnalyzeRecoIndex_forTop


void TtresEventSaverFlatNtuple::WbosonDecayMode(int Wdecay1_pdgId, int Wdecay2_pdgId, std::string & W_DecayMode, int & t_type) {

    std::string Wdecay1_decayMode  = "";
    std::string Wdecay2_decayMode  = "";
    t_type = 0;

    if ( Wdecay1_pdgId != 0 && Wdecay2_pdgId != 0 ) {
        if ( fabs(Wdecay1_pdgId) < 6 && fabs(Wdecay2_pdgId) < 6 ) {
            t_type = 1;
            W_DecayMode = "Hadronic";
        } else if ( (Wdecay1_pdgId == 12 && Wdecay2_pdgId == -11) || (Wdecay1_pdgId == 11 && Wdecay2_pdgId == -12) ) {
            t_type = 11;
            W_DecayMode = "Leptonic";
        } else if ( (Wdecay1_pdgId == 14 && Wdecay2_pdgId == -13) || (Wdecay1_pdgId == 13 && Wdecay2_pdgId == -14) ) {
            t_type = 13;
            W_DecayMode = "Leptonic";
        } else if ( (Wdecay1_pdgId == 15 && Wdecay2_pdgId == -16) || (Wdecay1_pdgId == 16 && Wdecay2_pdgId == -15) ) {
            t_type = 15;
            W_DecayMode = "Leptonic";
        }
    }

}//ttbarDecayMode


void TtresEventSaverFlatNtuple::Reconstruction_w_top_ttbar(std::string W_t_DecayMode, int t_type, std::string W_tbar_DecayMode, int tbar_type) {

    // Reconstruction of particles from the matched jets

    TLorentzVector MA_b_from_t_p4;
    MA_b_from_t_p4.SetPtEtaPhiM(m_MA_b_from_t_pt, m_MA_b_from_t_eta, m_MA_b_from_t_phi, m_MA_b_from_t_m);

    TLorentzVector MA_b_from_tbar_p4;
    MA_b_from_tbar_p4.SetPtEtaPhiM(m_MA_b_from_tbar_pt, m_MA_b_from_tbar_eta, m_MA_b_from_tbar_phi, m_MA_b_from_tbar_m);

    TLorentzVector MA_Wdecay1_from_t_p4;
    MA_Wdecay1_from_t_p4.SetPtEtaPhiM(m_MA_Wdecay1_from_t_pt, m_MA_Wdecay1_from_t_eta, m_MA_Wdecay1_from_t_phi, m_MA_Wdecay1_from_t_m);

    TLorentzVector MA_Wdecay2_from_t_p4;
    MA_Wdecay2_from_t_p4.SetPtEtaPhiM(m_MA_Wdecay2_from_t_pt, m_MA_Wdecay2_from_t_eta, m_MA_Wdecay2_from_t_phi, m_MA_Wdecay2_from_t_m);

    TLorentzVector MA_Wdecay1_from_tbar_p4;
    MA_Wdecay1_from_tbar_p4.SetPtEtaPhiM(m_MA_Wdecay1_from_tbar_pt, m_MA_Wdecay1_from_tbar_eta, m_MA_Wdecay1_from_tbar_phi, m_MA_Wdecay1_from_tbar_m);

    TLorentzVector MA_Wdecay2_from_tbar_p4;
    MA_Wdecay2_from_tbar_p4.SetPtEtaPhiM(m_MA_Wdecay2_from_tbar_pt, m_MA_Wdecay2_from_tbar_eta, m_MA_Wdecay2_from_tbar_phi, m_MA_Wdecay2_from_tbar_m);

    TLorentzVector MA_W_from_t_p4;
    TLorentzVector MA_W_from_tbar_p4;
    TLorentzVector MA_t_p4;
    TLorentzVector MA_tbar_p4;
    TLorentzVector MA_ttbar_p4;
    TLorentzVector MA_tFJ_p4;
    TLorentzVector MA_tbarFJ_p4;
    TLorentzVector MA_ttbarFJ_p4;

    // W from t reconstruction

    if (m_MA_Wdecay1_from_t_pt > 0 && m_MA_Wdecay2_from_t_pt > 0) {

        if (W_t_DecayMode == "Hadronic")
            m_MA_W_from_t_Njets = AnalyzeRecoIndex_forW(MA_W_from_t_p4, MA_Wdecay1_from_t_p4, m_MA_Wdecay1_from_t_recoidx, MA_Wdecay2_from_t_p4, m_MA_Wdecay2_from_t_recoidx);

        else if (W_t_DecayMode == "Leptonic") {
            MA_W_from_t_p4 = MA_Wdecay1_from_t_p4 + MA_Wdecay2_from_t_p4;
            m_MA_W_from_t_Njets = 0;
        }
        TLorentzFill(MA_W_from_t_p4, m_MA_W_from_t_m, m_MA_W_from_t_pt, m_MA_W_from_t_eta, m_MA_W_from_t_phi);
    }//if

    // W from tbar reconstruction
    if (m_MA_Wdecay1_from_tbar_pt > 0 && m_MA_Wdecay2_from_tbar_pt > 0) {

        if (W_tbar_DecayMode == "Hadronic")
            m_MA_W_from_tbar_Njets = AnalyzeRecoIndex_forW(MA_W_from_tbar_p4, MA_Wdecay1_from_tbar_p4, m_MA_Wdecay1_from_tbar_recoidx, MA_Wdecay2_from_tbar_p4, m_MA_Wdecay2_from_tbar_recoidx);

        else if (W_tbar_DecayMode == "Leptonic") {
            MA_W_from_tbar_p4 = MA_Wdecay1_from_tbar_p4 + MA_Wdecay2_from_tbar_p4;
            m_MA_W_from_tbar_Njets = 0;
        }
        TLorentzFill(MA_W_from_tbar_p4, m_MA_W_from_tbar_m, m_MA_W_from_tbar_pt, m_MA_W_from_tbar_eta, m_MA_W_from_tbar_phi);
    }//if

    // top reconstruction
    if (m_MA_W_from_t_pt > 0 && m_MA_b_from_t_pt > 0) {

        if (W_t_DecayMode == "Hadronic")
            m_MA_t_Njets = AnalyzeRecoIndex_forTop(MA_t_p4, MA_W_from_t_p4, MA_b_from_t_p4, MA_Wdecay1_from_t_p4, MA_Wdecay2_from_t_p4, m_MA_b_from_t_recoidx, m_MA_Wdecay1_from_t_recoidx, m_MA_Wdecay2_from_t_recoidx);

        else if (W_t_DecayMode == "Leptonic") {
            MA_t_p4 = MA_Wdecay1_from_t_p4 + MA_Wdecay2_from_t_p4 + MA_b_from_t_p4 ;
            if (m_MA_b_from_t_recoidx >= 0)  m_MA_t_Njets = 1;
        }
        TLorentzFill(MA_t_p4, m_MA_t_m, m_MA_t_pt, m_MA_t_eta, m_MA_t_phi);

    }//if

    // tbar reconstruction
    if (m_MA_W_from_tbar_pt > 0 && m_MA_b_from_tbar_pt > 0) {

        if (W_tbar_DecayMode == "Hadronic")
            m_MA_tbar_Njets = AnalyzeRecoIndex_forTop(MA_tbar_p4, MA_W_from_tbar_p4, MA_b_from_tbar_p4, MA_Wdecay1_from_tbar_p4, MA_Wdecay2_from_tbar_p4, m_MA_b_from_tbar_recoidx, m_MA_Wdecay1_from_tbar_recoidx, m_MA_Wdecay2_from_tbar_recoidx);

        else if (W_tbar_DecayMode == "Leptonic") {
            MA_tbar_p4 = MA_Wdecay1_from_tbar_p4 + MA_Wdecay2_from_tbar_p4 + MA_b_from_tbar_p4 ;
            if (m_MA_b_from_tbar_recoidx >= 0)  m_MA_tbar_Njets = 1;
        }
        TLorentzFill(MA_tbar_p4, m_MA_tbar_m, m_MA_tbar_pt, m_MA_tbar_eta, m_MA_tbar_phi);

    }//if


    // ttbar reconstruction
    if (m_MC_t_pt > 0 && m_MC_tbar_pt > 0) {
        //m_MC_ttbar_type = t_type*100 + tbar_type; //bqq'bev 113, bqq'bmv 115, bevbev 1111, bmvbev 1311, ...
        m_MC_ttbar_type = 1;
        if ( t_type > 1 && tbar_type == 1) {
            m_MC_ttbar_type = t_type;
        }
        else if ( tbar_type > 1 && t_type == 1) {
            m_MC_ttbar_type = tbar_type;
        }
        else if ( t_type > 1 && tbar_type > 1 ) {
            m_MC_ttbar_type = (t_type < tbar_type) ? t_type * 100 + tbar_type : tbar_type * 100 + t_type; // ee:1111, emu:1112, etau:1113, mumu:1212, ...
        }

        //m_MC_ttbar_type = t_type*100 + tbar_type; //bqq'bev 113, bqq'bmv 115, bevbev 1111, bmvbev 1311, ...
        if (m_MA_t_pt > 0 && m_MA_tbar_pt > 0) {
            MA_ttbar_p4 = MA_t_p4 + MA_tbar_p4;
            TLorentzFill(MA_ttbar_p4, m_MA_ttbar_m, m_MA_ttbar_pt, m_MA_ttbar_eta, m_MA_ttbar_phi);
        }

        // uusing fat jet
        if (m_MA_tFJ_pt > 0 && m_MA_tbar_pt > 0) {
            MA_tFJ_p4.SetPtEtaPhiM( m_MA_tFJ_pt, m_MA_tFJ_eta, m_MA_tFJ_phi,  m_MA_tFJ_m);
            MA_ttbarFJ_p4 = MA_tFJ_p4 + MA_tbar_p4;
            TLorentzFill(MA_ttbarFJ_p4, m_MA_ttbarFJ_m, m_MA_ttbarFJ_pt, m_MA_ttbarFJ_eta, m_MA_ttbarFJ_phi);
        }
        else if (m_MA_t_pt > 0 && m_MA_tbarFJ_pt > 0) {
            MA_tbarFJ_p4.SetPtEtaPhiM( m_MA_tbarFJ_pt, m_MA_tbarFJ_eta, m_MA_tbarFJ_phi,  m_MA_tbarFJ_m);
            MA_ttbarFJ_p4 = MA_t_p4 + MA_tbarFJ_p4;
            TLorentzFill(MA_ttbarFJ_p4, m_MA_ttbarFJ_m, m_MA_ttbarFJ_pt, m_MA_ttbarFJ_eta, m_MA_ttbarFJ_phi);
        }
    }

}//Reconstruction_w_top_ttbar()

void TtresEventSaverFlatNtuple::Fill_MA_semileptonic(std::string W_t_DecayMode, std::string W_tbar_DecayMode) {

    if (W_t_DecayMode == "Hadronic" && W_tbar_DecayMode == "Leptonic") {

        m_MA_bh_pt    = m_MA_b_from_t_pt;
        m_MA_bh_eta   = m_MA_b_from_t_eta;
        m_MA_bh_phi   = m_MA_b_from_t_phi;
        m_MA_bh_m     = m_MA_b_from_t_m;
        m_MA_bh_idx   = m_MA_b_from_t_recoidx;
        m_MA_bh_dr    = m_MA_b_from_t_recodr;
        m_MA_bh_tdr   = m_MA_b_from_t_truthdr;

        m_MA_wh_pt    = m_MA_W_from_t_pt;
        m_MA_wh_eta   = m_MA_W_from_t_eta;
        m_MA_wh_phi   = m_MA_W_from_t_phi;
        m_MA_wh_m     = m_MA_W_from_t_m;
        m_MA_wh_Njets = m_MA_W_from_t_Njets;

        m_MA_w1h_pt   = m_MA_Wdecay1_from_t_pt;
        m_MA_w1h_eta  = m_MA_Wdecay1_from_t_eta;
        m_MA_w1h_phi  = m_MA_Wdecay1_from_t_phi;
        m_MA_w1h_m    = m_MA_Wdecay1_from_t_m;
        m_MA_w1h_pdgId = m_MC_Wdecay1_from_t_pdgId;
        m_MA_w1h_idx  = m_MA_Wdecay1_from_t_recoidx;
        m_MA_w1h_dr   = m_MA_Wdecay1_from_t_recodr;
        m_MA_w1h_tdr  = m_MA_Wdecay1_from_t_truthdr;

        m_MA_w2h_pt   = m_MA_Wdecay2_from_t_pt;
        m_MA_w2h_eta  = m_MA_Wdecay2_from_t_eta;
        m_MA_w2h_phi  = m_MA_Wdecay2_from_t_phi;
        m_MA_w2h_m    = m_MA_Wdecay2_from_t_m;
        m_MA_w2h_pdgId = m_MC_Wdecay2_from_t_pdgId;
        m_MA_w2h_idx  = m_MA_Wdecay2_from_t_recoidx;
        m_MA_w2h_dr   = m_MA_Wdecay2_from_t_recodr;
        m_MA_w2h_tdr  = m_MA_Wdecay2_from_t_truthdr;

        m_MA_th_pt    = m_MA_t_pt;
        m_MA_th_eta   = m_MA_t_eta;
        m_MA_th_phi   = m_MA_t_phi;
        m_MA_th_m     = m_MA_t_m;
        m_MA_th_Njets = m_MA_t_Njets;

        m_MA_thFJ_pt    = m_MA_tFJ_pt;
        m_MA_thFJ_eta   = m_MA_tFJ_eta;
        m_MA_thFJ_phi   = m_MA_tFJ_phi;
        m_MA_thFJ_m     = m_MA_tFJ_m;
        m_MA_thFJ_nPartons     = m_MA_tFJ_nPartons;

        m_MA_bl_pt    = m_MA_b_from_tbar_pt;
        m_MA_bl_eta   = m_MA_b_from_tbar_eta;
        m_MA_bl_phi   = m_MA_b_from_tbar_phi;
        m_MA_bl_m     = m_MA_b_from_tbar_m;
        m_MA_bl_idx   = m_MA_b_from_tbar_recoidx;
        m_MA_bl_dr    = m_MA_b_from_tbar_recodr;
        m_MA_bl_tdr   = m_MA_b_from_tbar_truthdr;

        m_MA_wl_pt    = m_MA_W_from_tbar_pt;
        m_MA_wl_eta   = m_MA_W_from_tbar_eta;
        m_MA_wl_phi   = m_MA_W_from_tbar_phi;
        m_MA_wl_m     = m_MA_W_from_tbar_m;

        m_MA_w1l_pt     = m_MA_Wdecay1_from_tbar_pt;
        m_MA_w1l_eta    = m_MA_Wdecay1_from_tbar_eta;
        m_MA_w1l_phi    = m_MA_Wdecay1_from_tbar_phi;
        m_MA_w1l_m      = m_MA_Wdecay1_from_tbar_m;
        m_MA_w1l_pdgId  = m_MC_Wdecay1_from_tbar_pdgId;
        m_MA_w1l_idx    = m_MA_Wdecay1_from_tbar_recoidx;
        m_MA_w1l_dr     = m_MA_Wdecay1_from_tbar_recodr;
        m_MA_w1l_tdr    = m_MA_Wdecay1_from_tbar_truthdr;
        m_MA_w1l_NuDphi = -1;

        m_MA_w2l_pt     = m_MA_Wdecay2_from_tbar_pt;
        m_MA_w2l_eta    = m_MA_Wdecay2_from_tbar_eta;
        m_MA_w2l_phi    = m_MA_Wdecay2_from_tbar_phi;
        m_MA_w2l_m      = m_MA_Wdecay2_from_tbar_m;
        m_MA_w2l_pdgId  = m_MC_Wdecay2_from_tbar_pdgId;
        m_MA_w2l_idx    = m_MA_Wdecay2_from_tbar_recoidx;
        m_MA_w2l_dr     = m_MA_Wdecay2_from_tbar_recodr;
        m_MA_w2l_tdr    = m_MA_Wdecay2_from_tbar_truthdr;
        m_MA_w2l_NuDphi = m_MA_Wdecay2_from_tbar_NuDphi;

        m_MA_tl_pt    = m_MA_tbar_pt;
        m_MA_tl_eta   = m_MA_tbar_eta;
        m_MA_tl_phi   = m_MA_tbar_phi;
        m_MA_tl_m     = m_MA_tbar_m;
        m_MA_tl_Njets = m_MA_tbar_Njets;

        m_MA_ttbar_lpj_pt  = m_MA_ttbar_pt;
        m_MA_ttbar_lpj_eta = m_MA_ttbar_eta;
        m_MA_ttbar_lpj_phi = m_MA_ttbar_phi;
        m_MA_ttbar_lpj_m   = m_MA_ttbar_m;

        if (m_MA_t_Njets > 0 && m_MA_tbar_Njets > 0)  m_MA_ttbar_lpj_Njets    = m_MA_t_Njets + m_MA_tbar_Njets;

        //Partons

        m_MC_bh_pt    = m_MC_b_from_t_pt;
        m_MC_bh_eta   = m_MC_b_from_t_eta;
        m_MC_bh_phi   = m_MC_b_from_t_phi;
        m_MC_bh_m     = m_MC_b_from_t_m;

        m_MC_wh_pt    = m_MC_W_from_t_pt;
        m_MC_wh_eta   = m_MC_W_from_t_eta;
        m_MC_wh_phi   = m_MC_W_from_t_phi;
        m_MC_wh_m     = m_MC_W_from_t_m;

        m_MC_w1h_pt   = m_MC_Wdecay1_from_t_pt;
        m_MC_w1h_eta  = m_MC_Wdecay1_from_t_eta;
        m_MC_w1h_phi  = m_MC_Wdecay1_from_t_phi;
        m_MC_w1h_m    = m_MC_Wdecay1_from_t_m;
        m_MC_w1h_pdgId = m_MC_Wdecay1_from_t_pdgId;

        m_MC_w2h_pt   = m_MC_Wdecay2_from_t_pt;
        m_MC_w2h_eta  = m_MC_Wdecay2_from_t_eta;
        m_MC_w2h_phi  = m_MC_Wdecay2_from_t_phi;
        m_MC_w2h_m    = m_MC_Wdecay2_from_t_m;
        m_MC_w2h_pdgId = m_MC_Wdecay2_from_t_pdgId;

        m_MC_th_pt    = m_MC_t_pt;
        m_MC_th_eta   = m_MC_t_eta;
        m_MC_th_phi   = m_MC_t_phi;
        m_MC_th_m     = m_MC_t_m;

        m_MC_bl_pt    = m_MC_b_from_tbar_pt;
        m_MC_bl_eta   = m_MC_b_from_tbar_eta;
        m_MC_bl_phi   = m_MC_b_from_tbar_phi;
        m_MC_bl_m     = m_MC_b_from_tbar_m;

        m_MC_wl_pt    = m_MC_W_from_tbar_pt;
        m_MC_wl_eta   = m_MC_W_from_tbar_eta;
        m_MC_wl_phi   = m_MC_W_from_tbar_phi;
        m_MC_wl_m     = m_MC_W_from_tbar_m;

        m_MC_w1l_pt   = m_MC_Wdecay1_from_tbar_pt;
        m_MC_w1l_eta  = m_MC_Wdecay1_from_tbar_eta;
        m_MC_w1l_phi  = m_MC_Wdecay1_from_tbar_phi;
        m_MC_w1l_m    = m_MC_Wdecay1_from_tbar_m;
        m_MC_w1l_pdgId = m_MC_Wdecay1_from_tbar_pdgId;

        m_MC_w2l_pt   = m_MC_Wdecay2_from_tbar_pt;
        m_MC_w2l_eta  = m_MC_Wdecay2_from_tbar_eta;
        m_MC_w2l_phi  = m_MC_Wdecay2_from_tbar_phi;
        m_MC_w2l_m    = m_MC_Wdecay2_from_tbar_m;
        m_MC_w2l_pdgId = m_MC_Wdecay2_from_tbar_pdgId;

        m_MC_tl_pt    = m_MC_tbar_pt;
        m_MC_tl_eta   = m_MC_tbar_eta;
        m_MC_tl_phi   = m_MC_tbar_phi;
        m_MC_tl_m     = m_MC_tbar_m;

        m_MC_ttbar_lpj_beforeFSR_pt  = m_MC_ttbar_beforeFSR_pt;
        m_MC_ttbar_lpj_beforeFSR_eta = m_MC_ttbar_beforeFSR_eta;
        m_MC_ttbar_lpj_beforeFSR_phi = m_MC_ttbar_beforeFSR_phi;
        m_MC_ttbar_lpj_beforeFSR_m   = m_MC_ttbar_beforeFSR_m;

        m_MC_ttbar_lpj_afterFSR_pt  = m_MC_ttbar_afterFSR_pt;
        m_MC_ttbar_lpj_afterFSR_eta = m_MC_ttbar_afterFSR_eta;
        m_MC_ttbar_lpj_afterFSR_phi = m_MC_ttbar_afterFSR_phi;
        m_MC_ttbar_lpj_afterFSR_m   = m_MC_ttbar_afterFSR_m;


    } else if (W_t_DecayMode == "Leptonic" && W_tbar_DecayMode == "Hadronic") {

        m_MA_bh_pt    = m_MA_b_from_tbar_pt;
        m_MA_bh_eta   = m_MA_b_from_tbar_eta;
        m_MA_bh_phi   = m_MA_b_from_tbar_phi;
        m_MA_bh_m     = m_MA_b_from_tbar_m;
        m_MA_bh_idx   = m_MA_b_from_tbar_recoidx;
        m_MA_bh_dr    = m_MA_b_from_tbar_recodr;
        m_MA_bh_tdr   = m_MA_b_from_tbar_truthdr;

        m_MA_wh_pt    = m_MA_W_from_tbar_pt;
        m_MA_wh_eta   = m_MA_W_from_tbar_eta;
        m_MA_wh_phi   = m_MA_W_from_tbar_phi;
        m_MA_wh_m     = m_MA_W_from_tbar_m;
        m_MA_wh_Njets = m_MA_W_from_tbar_Njets;

        m_MA_w1h_pt   = m_MA_Wdecay1_from_tbar_pt;
        m_MA_w1h_eta  = m_MA_Wdecay1_from_tbar_eta;
        m_MA_w1h_phi  = m_MA_Wdecay1_from_tbar_phi;
        m_MA_w1h_m    = m_MA_Wdecay1_from_tbar_m;
        m_MA_w1h_pdgId = m_MC_Wdecay1_from_tbar_pdgId;
        m_MA_w1h_idx  = m_MA_Wdecay1_from_tbar_recoidx;
        m_MA_w1h_dr   = m_MA_Wdecay1_from_tbar_recodr;
        m_MA_w1h_tdr  = m_MA_Wdecay1_from_tbar_truthdr;

        m_MA_w2h_pt   = m_MA_Wdecay2_from_tbar_pt;
        m_MA_w2h_eta  = m_MA_Wdecay2_from_tbar_eta;
        m_MA_w2h_phi  = m_MA_Wdecay2_from_tbar_phi;
        m_MA_w2h_m    = m_MA_Wdecay2_from_tbar_m;
        m_MA_w2h_pdgId = m_MC_Wdecay2_from_tbar_pdgId;
        m_MA_w2h_idx  = m_MA_Wdecay2_from_tbar_recoidx;
        m_MA_w2h_dr   = m_MA_Wdecay2_from_tbar_recodr;
        m_MA_w2h_tdr  = m_MA_Wdecay2_from_tbar_truthdr;

        m_MA_th_pt    = m_MA_tbar_pt;
        m_MA_th_eta   = m_MA_tbar_eta;
        m_MA_th_phi   = m_MA_tbar_phi;
        m_MA_th_m     = m_MA_tbar_m;
        m_MA_th_Njets = m_MA_tbar_Njets;

        m_MA_thFJ_pt    = m_MA_tbarFJ_pt;
        m_MA_thFJ_eta   = m_MA_tbarFJ_eta;
        m_MA_thFJ_phi   = m_MA_tbarFJ_phi;
        m_MA_thFJ_m     = m_MA_tbarFJ_m;
        m_MA_thFJ_nPartons     = m_MA_tbarFJ_nPartons;

        m_MA_bl_pt    = m_MA_b_from_t_pt;
        m_MA_bl_eta   = m_MA_b_from_t_eta;
        m_MA_bl_phi   = m_MA_b_from_t_phi;
        m_MA_bl_m     = m_MA_b_from_t_m;
        m_MA_bl_idx   = m_MA_b_from_t_recoidx;
        m_MA_bl_dr    = m_MA_b_from_t_recodr;
        m_MA_bl_tdr   = m_MA_b_from_t_truthdr;

        m_MA_wl_pt    = m_MA_W_from_t_pt;
        m_MA_wl_eta   = m_MA_W_from_t_eta;
        m_MA_wl_phi   = m_MA_W_from_t_phi;
        m_MA_wl_m     = m_MA_W_from_t_m;


        m_MA_w1l_pt     = m_MA_Wdecay1_from_t_pt;
        m_MA_w1l_eta    = m_MA_Wdecay1_from_t_eta;
        m_MA_w1l_phi    = m_MA_Wdecay1_from_t_phi;
        m_MA_w1l_m      = m_MA_Wdecay1_from_t_m;
        m_MA_w1l_pdgId  = m_MC_Wdecay1_from_t_pdgId;
        m_MA_w1l_idx    = m_MA_Wdecay1_from_t_recoidx;
        m_MA_w1l_dr     = m_MA_Wdecay1_from_t_recodr;
        m_MA_w1l_tdr    = m_MA_Wdecay1_from_t_truthdr;
        m_MA_w1l_NuDphi = m_MA_Wdecay1_from_t_NuDphi;

        m_MA_w2l_pt     = m_MA_Wdecay2_from_t_pt;
        m_MA_w2l_eta    = m_MA_Wdecay2_from_t_eta;
        m_MA_w2l_phi    = m_MA_Wdecay2_from_t_phi;
        m_MA_w2l_m      = m_MA_Wdecay2_from_t_m;
        m_MA_w2l_pdgId  = m_MC_Wdecay2_from_t_pdgId;
        m_MA_w2l_idx    = m_MA_Wdecay2_from_t_recoidx;
        m_MA_w2l_dr     = m_MA_Wdecay2_from_t_recodr;
        m_MA_w2l_tdr    = m_MA_Wdecay2_from_t_truthdr;
        m_MA_w2l_NuDphi = -1;

        m_MA_tl_pt    = m_MA_t_pt;
        m_MA_tl_eta   = m_MA_t_eta;
        m_MA_tl_phi   = m_MA_t_phi;
        m_MA_tl_m     = m_MA_t_m;
        m_MA_tl_Njets = m_MA_t_Njets;

        m_MA_ttbar_lpj_pt  = m_MA_ttbar_pt;
        m_MA_ttbar_lpj_eta = m_MA_ttbar_eta;
        m_MA_ttbar_lpj_phi = m_MA_ttbar_phi;
        m_MA_ttbar_lpj_m   = m_MA_ttbar_m;

        if (m_MA_t_Njets > 0 && m_MA_tbar_Njets > 0)  m_MA_ttbar_lpj_Njets    = m_MA_t_Njets + m_MA_tbar_Njets;

        //Partons

        m_MC_bh_pt    = m_MC_b_from_tbar_pt;
        m_MC_bh_eta   = m_MC_b_from_tbar_eta;
        m_MC_bh_phi   = m_MC_b_from_tbar_phi;
        m_MC_bh_m     = m_MC_b_from_tbar_m;

        m_MC_wh_pt    = m_MC_W_from_tbar_pt;
        m_MC_wh_eta   = m_MC_W_from_tbar_eta;
        m_MC_wh_phi   = m_MC_W_from_tbar_phi;
        m_MC_wh_m     = m_MC_W_from_tbar_m;

        m_MC_w1h_pt   = m_MC_Wdecay1_from_tbar_pt;
        m_MC_w1h_eta  = m_MC_Wdecay1_from_tbar_eta;
        m_MC_w1h_phi  = m_MC_Wdecay1_from_tbar_phi;
        m_MC_w1h_m    = m_MC_Wdecay1_from_tbar_m;
        m_MC_w1h_pdgId = m_MC_Wdecay1_from_tbar_pdgId;

        m_MC_w2h_pt   = m_MC_Wdecay2_from_tbar_pt;
        m_MC_w2h_eta  = m_MC_Wdecay2_from_tbar_eta;
        m_MC_w2h_phi  = m_MC_Wdecay2_from_tbar_phi;
        m_MC_w2h_m    = m_MC_Wdecay2_from_tbar_m;
        m_MC_w2h_pdgId = m_MC_Wdecay2_from_tbar_pdgId;

        m_MC_th_pt    = m_MC_tbar_pt;
        m_MC_th_eta   = m_MC_tbar_eta;
        m_MC_th_phi   = m_MC_tbar_phi;
        m_MC_th_m     = m_MC_tbar_m;

        m_MC_bl_pt    = m_MC_b_from_t_pt;
        m_MC_bl_eta   = m_MC_b_from_t_eta;
        m_MC_bl_phi   = m_MC_b_from_t_phi;
        m_MC_bl_m     = m_MC_b_from_t_m;

        m_MC_wl_pt    = m_MC_W_from_t_pt;
        m_MC_wl_eta   = m_MC_W_from_t_eta;
        m_MC_wl_phi   = m_MC_W_from_t_phi;
        m_MC_wl_m     = m_MC_W_from_t_m;

        m_MC_w1l_pt   = m_MC_Wdecay1_from_t_pt;
        m_MC_w1l_eta  = m_MC_Wdecay1_from_t_eta;
        m_MC_w1l_phi  = m_MC_Wdecay1_from_t_phi;
        m_MC_w1l_m    = m_MC_Wdecay1_from_t_m;
        m_MC_w1l_pdgId = m_MC_Wdecay1_from_t_pdgId;

        m_MC_w2l_pt   = m_MC_Wdecay2_from_t_pt;
        m_MC_w2l_eta  = m_MC_Wdecay2_from_t_eta;
        m_MC_w2l_phi  = m_MC_Wdecay2_from_t_phi;
        m_MC_w2l_m    = m_MC_Wdecay2_from_t_m;
        m_MC_w2l_pdgId = m_MC_Wdecay2_from_t_pdgId;

        m_MC_tl_pt    = m_MC_t_pt;
        m_MC_tl_eta   = m_MC_t_eta;
        m_MC_tl_phi   = m_MC_t_phi;
        m_MC_tl_m     = m_MC_t_m;

        m_MC_ttbar_lpj_beforeFSR_pt  = m_MC_ttbar_beforeFSR_pt;
        m_MC_ttbar_lpj_beforeFSR_eta = m_MC_ttbar_beforeFSR_eta;
        m_MC_ttbar_lpj_beforeFSR_phi = m_MC_ttbar_beforeFSR_phi;
        m_MC_ttbar_lpj_beforeFSR_m   = m_MC_ttbar_beforeFSR_m;

        m_MC_ttbar_lpj_afterFSR_pt  = m_MC_ttbar_afterFSR_pt;
        m_MC_ttbar_lpj_afterFSR_eta = m_MC_ttbar_afterFSR_eta;
        m_MC_ttbar_lpj_afterFSR_phi = m_MC_ttbar_afterFSR_phi;
        m_MC_ttbar_lpj_afterFSR_m   = m_MC_ttbar_afterFSR_m;
    }

}//Fill_MA_semileptonic()

void TtresEventSaverFlatNtuple::IniVariables() {
    // Default values
    m_MC_px_me.clear();
    m_MC_py_me.clear();
    m_MC_pz_me.clear();
    m_MC_e_me.clear();
    m_MC_m_me.clear();
    m_MC_id_me.clear();
    m_MC_Q_me = -5000;
    m_MC_QxAOD_me.clear();
    m_MC_aSxAOD_me.clear();

    m_MC_b_from_t_pt       = -5000;
    m_MC_b_from_t_eta      = -5000;
    m_MC_b_from_t_phi      = -5000;
    m_MC_b_from_t_m        = -5000;

    m_MC_b_from_tbar_pt      = -5000;
    m_MC_b_from_tbar_eta     = -5000;
    m_MC_b_from_tbar_phi     = -5000;
    m_MC_b_from_tbar_m       = -5000;

    m_MC_Wdecay1_from_t_pt       = -5000;
    m_MC_Wdecay1_from_t_eta      = -5000;
    m_MC_Wdecay1_from_t_phi      = -5000;
    m_MC_Wdecay1_from_t_m        = -5000;
    m_MC_Wdecay1_from_t_pdgId    = 0;

    m_MC_Wdecay2_from_t_pt       = -5000;
    m_MC_Wdecay2_from_t_eta      = -5000;
    m_MC_Wdecay2_from_t_phi      = -5000;
    m_MC_Wdecay2_from_t_m        = -5000;
    m_MC_Wdecay2_from_t_pdgId    = 0;

    m_MC_Wdecay1_from_tbar_pt    = -5000;
    m_MC_Wdecay1_from_tbar_eta      = -5000;
    m_MC_Wdecay1_from_tbar_phi      = -5000;
    m_MC_Wdecay1_from_tbar_m     = -5000;
    m_MC_Wdecay1_from_tbar_pdgId   = 0;

    m_MC_Wdecay2_from_tbar_pt    = -5000;
    m_MC_Wdecay2_from_tbar_eta      = -5000;
    m_MC_Wdecay2_from_tbar_phi      = -5000;
    m_MC_Wdecay2_from_tbar_m     = -5000;
    m_MC_Wdecay2_from_tbar_pdgId   = 0;

    m_MC_W_from_t_pt   = -5000;
    m_MC_W_from_t_eta     = -5000;
    m_MC_W_from_t_phi     = -5000;
    m_MC_W_from_t_m    = -5000;

    m_MC_W_from_tbar_pt      = -5000;
    m_MC_W_from_tbar_eta     = -5000;
    m_MC_W_from_tbar_phi     = -5000;
    m_MC_W_from_tbar_m       = -5000;

    m_MC_t_pt     = -5000;
    m_MC_t_eta     = -5000;
    m_MC_t_phi     = -5000;
    m_MC_t_m      = -5000;

    m_MC_tbar_pt      = -5000;
    m_MC_tbar_eta     = -5000;
    m_MC_tbar_phi     = -5000;
    m_MC_tbar_m       = -5000;

    m_MC_ttbar_beforeFSR_pt  = -5000;
    m_MC_ttbar_beforeFSR_eta = -5000;
    m_MC_ttbar_beforeFSR_phi = -5000;
    m_MC_ttbar_beforeFSR_m   = -5000;

    m_MC_ttbar_afterFSR_pt  = -5000;
    m_MC_ttbar_afterFSR_eta = -5000;
    m_MC_ttbar_afterFSR_phi = -5000;
    m_MC_ttbar_afterFSR_m   = -5000;

    //Matched jets
    m_MA_b_from_t_pt = -5000;
    m_MA_b_from_t_eta = -5000;
    m_MA_b_from_t_phi = -5000;
    m_MA_b_from_t_m  = -5000;
    m_MA_b_from_t_recoidx  = -1;
    m_MA_b_from_t_recodr   = -1;
    m_MA_b_from_t_truthdr  = -1;

    m_MA_b_from_tbar_pt  = -5000;
    m_MA_b_from_tbar_eta = -5000;
    m_MA_b_from_tbar_phi = -5000;
    m_MA_b_from_tbar_m   = -5000;
    m_MA_b_from_tbar_recoidx = -1;
    m_MA_b_from_tbar_recodr  = -1;
    m_MA_b_from_tbar_truthdr = -1;

    m_MA_Wdecay1_from_t_pt   = -5000;
    m_MA_Wdecay1_from_t_eta  = -5000;
    m_MA_Wdecay1_from_t_phi  = -5000;
    m_MA_Wdecay1_from_t_m    = -5000;
    m_MA_Wdecay1_from_t_recoidx  = -1;
    m_MA_Wdecay1_from_t_recodr   = -1;
    m_MA_Wdecay1_from_t_truthdr  = -1;
    m_MA_Wdecay1_from_t_NuDphi   = -1;

    m_MA_Wdecay2_from_t_pt  = -5000;
    m_MA_Wdecay2_from_t_eta = -5000;
    m_MA_Wdecay2_from_t_phi = -5000;
    m_MA_Wdecay2_from_t_m   = -5000;
    m_MA_Wdecay2_from_t_recoidx  = -1;
    m_MA_Wdecay2_from_t_recodr   = -1;
    m_MA_Wdecay2_from_t_truthdr  = -1;

    m_MA_Wdecay1_from_tbar_pt  = -5000;
    m_MA_Wdecay1_from_tbar_eta = -5000;
    m_MA_Wdecay1_from_tbar_phi = -5000;
    m_MA_Wdecay1_from_tbar_m   = -5000;
    m_MA_Wdecay1_from_tbar_recoidx  = -1;
    m_MA_Wdecay1_from_tbar_recodr  = -1;
    m_MA_Wdecay1_from_tbar_truthdr = -1;

    m_MA_Wdecay2_from_tbar_pt   = -5000;
    m_MA_Wdecay2_from_tbar_eta  = -5000;
    m_MA_Wdecay2_from_tbar_phi  = -5000;
    m_MA_Wdecay2_from_tbar_m    = -5000;
    m_MA_Wdecay2_from_tbar_recoidx = -1;
    m_MA_Wdecay2_from_tbar_recodr  = -1;
    m_MA_Wdecay2_from_tbar_truthdr = -1;
    m_MA_Wdecay2_from_tbar_NuDphi = -1;

    // Reconstructed particles from the matched jets
    m_MA_W_from_t_pt    = -5000;
    m_MA_W_from_t_eta   = -5000;
    m_MA_W_from_t_phi   = -5000;
    m_MA_W_from_t_m     = -5000;
    m_MA_W_from_t_Njets = -1;

    m_MA_W_from_tbar_pt    = -5000;
    m_MA_W_from_tbar_eta   = -5000;
    m_MA_W_from_tbar_phi   = -5000;
    m_MA_W_from_tbar_m     = -5000;
    m_MA_W_from_tbar_Njets = -1;

    m_MA_t_pt      = -5000;
    m_MA_t_eta     = -5000;
    m_MA_t_phi     = -5000;
    m_MA_t_m       = -5000;
    m_MA_t_Njets = -1;

    m_MA_tFJ_pt    = -5000;
    m_MA_tFJ_eta   = -5000;
    m_MA_tFJ_phi   = -5000;
    m_MA_tFJ_m     = -5000;
    m_MA_tFJ_nPartons     = 0;

    m_MA_tbar_pt     = -5000;
    m_MA_tbar_eta    = -5000;
    m_MA_tbar_phi    = -5000;
    m_MA_tbar_m      = -5000;
    m_MA_tbar_Njets  = -1;

    m_MA_tbarFJ_pt     = -5000;
    m_MA_tbarFJ_eta    = -5000;
    m_MA_tbarFJ_phi    = -5000;
    m_MA_tbarFJ_m      = -5000;
    m_MA_tbarFJ_nPartons     = 0;

    m_MA_ttbar_pt   = -5000;
    m_MA_ttbar_eta  = -5000;
    m_MA_ttbar_phi  = -5000;
    m_MA_ttbar_m    = -5000;

    m_MA_ttbarFJ_pt   = -5000;
    m_MA_ttbarFJ_eta  = -5000;
    m_MA_ttbarFJ_phi  = -5000;
    m_MA_ttbarFJ_m    = -5000;

    m_MC_ttbar_type = -1;

    //only for l+jets
    m_MA_bh_pt    = -5000;
    m_MA_bh_eta   = -5000;
    m_MA_bh_phi   = -5000;
    m_MA_bh_m    = -5000;
    m_MA_bh_idx   = -1;
    m_MA_bh_dr    = -1;
    m_MA_bh_tdr   = -1;

    m_MA_bl_pt    = -5000;
    m_MA_bl_eta   = -5000;
    m_MA_bl_phi   = -5000;
    m_MA_bl_m    = -5000;
    m_MA_bl_idx   = -1;
    m_MA_bl_dr    = -1;
    m_MA_bl_tdr   = -1;

    m_MA_wh_pt    = -5000;
    m_MA_wh_eta   = -5000;
    m_MA_wh_phi   = -5000;
    m_MA_wh_m    = -5000;
    m_MA_wh_Njets = -1;

    m_MA_w1h_pt   = -5000;
    m_MA_w1h_eta  = -5000;
    m_MA_w1h_phi  = -5000;
    m_MA_w1h_m   = -5000;
    m_MA_w1h_pdgId = 0;
    m_MA_w1h_idx  = -1;
    m_MA_w1h_dr   = -1;
    m_MA_w1h_tdr  = -1;

    m_MA_w2h_pt   = -5000;
    m_MA_w2h_eta  = -5000;
    m_MA_w2h_phi  = -5000;
    m_MA_w2h_m   = -5000;
    m_MA_w2h_pdgId = 0;
    m_MA_w2h_idx  = -1;
    m_MA_w2h_dr   = -1;
    m_MA_w2h_tdr  = -1;

    m_MA_wl_pt    = -5000;
    m_MA_wl_eta   = -5000;
    m_MA_wl_phi   = -5000;
    m_MA_wl_m    = -5000;

    m_MA_w1l_pt    = -5000;
    m_MA_w1l_eta   = -5000;
    m_MA_w1l_phi   = -5000;
    m_MA_w1l_m    = -5000;
    m_MA_w1l_pdgId = 0;
    m_MA_w1l_idx   = -1;
    m_MA_w1l_dr    = -1;
    m_MA_w1l_tdr   = -1;
    m_MA_w1l_NuDphi = -1;

    m_MA_w2l_pt    = -5000;
    m_MA_w2l_eta   = -5000;
    m_MA_w2l_phi   = -5000;
    m_MA_w2l_m    = -5000;
    m_MA_w2l_pdgId = 0;
    m_MA_w2l_idx   = -1;
    m_MA_w2l_dr    = -1;
    m_MA_w2l_tdr   = -1;
    m_MA_w2l_NuDphi = -1;

    m_MA_th_pt    = -5000;
    m_MA_th_eta   = -5000;
    m_MA_th_phi   = -5000;
    m_MA_th_m     = -5000;
    m_MA_th_Njets = -1;

    m_MA_thFJ_pt    = -5000;
    m_MA_thFJ_eta   = -5000;
    m_MA_thFJ_phi   = -5000;
    m_MA_thFJ_m     = -5000;
    m_MA_thFJ_nPartons     = 0;

    m_MA_tl_pt    = -5000;
    m_MA_tl_eta   = -5000;
    m_MA_tl_phi   = -5000;
    m_MA_tl_m     = -5000;
    m_MA_tl_Njets = -1;

    m_MA_ttbar_lpj_pt    = -5000;
    m_MA_ttbar_lpj_eta   = -5000;
    m_MA_ttbar_lpj_phi   = -5000;
    m_MA_ttbar_lpj_m     = -5000;
    m_MA_ttbar_lpj_Njets = -1;

    //partons in l+jets
    m_MC_bh_pt    = -5000;
    m_MC_bh_eta   = -5000;
    m_MC_bh_phi   = -5000;
    m_MC_bh_m    = -5000;

    m_MC_bl_pt    = -5000;
    m_MC_bl_eta   = -5000;
    m_MC_bl_phi   = -5000;
    m_MC_bl_m    = -5000;

    m_MC_w1h_pt   = -5000;
    m_MC_w1h_eta  = -5000;
    m_MC_w1h_phi  = -5000;
    m_MC_w1h_m    = -5000;
    m_MC_w1h_pdgId = 0;

    m_MC_w2h_pt   = -5000;
    m_MC_w2h_eta  = -5000;
    m_MC_w2h_phi  = -5000;
    m_MC_w2h_m    = -5000;
    m_MC_w2h_pdgId = 0;

    m_MC_wh_pt    = -5000;
    m_MC_wh_eta   = -5000;
    m_MC_wh_phi   = -5000;
    m_MC_wh_m    = -5000;

    m_MC_w1l_pt   = -5000;
    m_MC_w1l_eta  = -5000;
    m_MC_w1l_phi  = -5000;
    m_MC_w1l_m    = -5000;
    m_MC_w1l_pdgId = 0;

    m_MC_w2l_pt   = -5000;
    m_MC_w2l_eta  = -5000;
    m_MC_w2l_phi  = -5000;
    m_MC_w2l_m    = -5000;
    m_MC_w2l_pdgId = 0;

    m_MC_wl_pt    = -5000;
    m_MC_wl_eta   = -5000;
    m_MC_wl_phi   = -5000;
    m_MC_wl_m    = -5000;

    m_MC_th_pt    = -5000;
    m_MC_th_eta   = -5000;
    m_MC_th_phi   = -5000;
    m_MC_th_m    = -5000;

    m_MC_tl_pt    = -5000;
    m_MC_tl_eta   = -5000;
    m_MC_tl_phi   = -5000;
    m_MC_tl_m    = -5000;

    m_MC_ttbar_lpj_beforeFSR_pt  = -5000;
    m_MC_ttbar_lpj_beforeFSR_eta = -5000;
    m_MC_ttbar_lpj_beforeFSR_phi = -5000;
    m_MC_ttbar_lpj_beforeFSR_m   = -5000;

    m_MC_ttbar_lpj_afterFSR_pt  = -5000;
    m_MC_ttbar_lpj_afterFSR_eta = -5000;
    m_MC_ttbar_lpj_afterFSR_phi = -5000;
    m_MC_ttbar_lpj_afterFSR_m   = -5000;

    // chi2 in l+jets
    m_chi2_bh_pt    = -5000;
    m_chi2_bh_eta   = -5000;
    m_chi2_bh_phi   = -5000;
    m_chi2_bh_m     = -5000;

    m_chi2_bl_pt    = -5000;
    m_chi2_bl_eta   = -5000;
    m_chi2_bl_phi   = -5000;
    m_chi2_bl_m     = -5000;

//     m_chi2_w1h_pt   = -5000;
//     m_chi2_w1h_eta  = -5000;
//     m_chi2_w1h_phi  = -5000;
//     m_chi2_w1h_m    = -5000;
//     m_chi2_w1h_pdgId= 0;
//
//     m_chi2_w2h_pt   = -5000;
//     m_chi2_w2h_eta  = -5000;
//     m_chi2_w2h_phi  = -5000;
//     m_chi2_w2h_m    = -5000;
//     m_chi2_w2h_pdgId= 0;

    m_chi2_wh_pt    = -5000;
    m_chi2_wh_eta   = -5000;
    m_chi2_wh_phi   = -5000;
    m_chi2_wh_m     = -5000;

//     m_chi2_w1l_pt   = -5000;
//     m_chi2_w1l_eta  = -5000;
//     m_chi2_w1l_phi  = -5000;
//     m_chi2_w1l_m    = -5000;
//     m_chi2_w1l_pdgId= 0;
//
//     m_chi2_w2l_pt   = -5000;
//     m_chi2_w2l_eta  = -5000;
//     m_chi2_w2l_phi  = -5000;
//     m_chi2_w2l_m    = -5000;
//     m_chi2_w2l_pdgId= 0;

    m_chi2_wl_pt    = -5000;
    m_chi2_wl_eta   = -5000;
    m_chi2_wl_phi   = -5000;
    m_chi2_wl_m     = -5000;

    m_chi2_th_pt    = -5000;
    m_chi2_th_eta   = -5000;
    m_chi2_th_phi   = -5000;
    m_chi2_th_m     = -5000;

    m_chi2_tl_pt    = -5000;
    m_chi2_tl_eta   = -5000;
    m_chi2_tl_phi   = -5000;
    m_chi2_tl_m     = -5000;

    m_chi2_ttbar_pt  = -5000;
    m_chi2_ttbar_eta = -5000;
    m_chi2_ttbar_phi = -5000;
    m_chi2_ttbar_m   = -5000;

}//IniVariables

void TtresEventSaverFlatNtuple::calculateWjets(const top::Event &event) {

    ////////////////////////////////////////////////////// W+jets
    // ################################################################
    // #
    // NEW
    // W+jets filter for Sherpa samples:
    //-> calculate W-filter variable:
    // -1 : default
    //  0 : m_isSherpaW = false : Data, non-Wsample (check missing!)=> do not calculate
    //  1 : c  (only c or anti-c)
    //  2 : cc (c and anti-c)
    //  3 : b  (only b or anti-b)
    //  4 : bb (b and anti-b)
    //  5 : lf (neither b nor c)
    // -2 : ERROR

    // default value:
    m_Wfilter = -1;
    m_Wfilter_2 = -1;
    m_Wfilter_3 = -1;
    m_Wfilter_4 = -1;
    m_Wfilter_5 = -1;
    m_Wfilter_6 = -1;
    m_Sherpa22_weight = 1;

    // --------

    // current Sherpa filter: hadron filter, c-truth-jet-matching:
    // settings
    bool find_cQuark = false;  // (c)
    bool find_cHadron = true;  // (charm)
    bool find_bQuark = false;  // (b)
    bool find_bHadron = true;  // (bottom)
    if (find_cQuark && find_cHadron) std::cout << "ERROR: set c-quark filter AND c-hadron filter ==> Choose only 1!" << std::endl;
    if (find_bQuark && find_bHadron) std::cout << "ERROR: set b-quark filter AND b-hadron filter ==> Choose only 1!" << std::endl;

    bool require_truthJets_forC = true;
    bool require_truthJets_forB = false;

    // variables needed
    bool W_plus_C    = false;
    bool W_plus_cc   = false;
    bool W_plus_anyC = false;
    bool W_plus_anyB = false;
    bool W_plus_B    = false;
    bool W_plus_bb   = false;

    int  help_antiCcounter = 0;
    int  help_Ccounter = 0;
    bool help_isC = false;
    bool help_isantiC = false;
    int  help_antiBcounter = 0;
    int  help_Bcounter = 0;
    bool help_isB = false;
    bool help_isantiB = false;

    // <===============
    // quantitativ filter check
    // Settings 2: hadron filter, no truth-jet-matching
    bool find_cQuark_2 = false;  // (c)
    bool find_cHadron_2 = true;  // (charm)
    bool find_bQuark_2 = false;  // (b)
    bool find_bHadron_2 = true;  // (bottom)
    if (find_cQuark_2 && find_cHadron_2) std::cout << "ERROR: (2) set c-quark filter AND c-hadron filter ==> Choose only 1!" << std::endl;
    if (find_bQuark_2 && find_bHadron_2) std::cout << "ERROR: (2) set b-quark filter AND b-hadron filter ==> Choose only 1!" << std::endl;
    bool require_truthJets_forC_2 = false;
    bool require_truthJets_forB_2 = false;

    // variables for setup-2
    bool W_plus_C_2    = false;
    bool W_plus_cc_2   = false;
    bool W_plus_anyC_2 = false;
    bool W_plus_anyB_2 = false;
    bool W_plus_B_2    = false;
    bool W_plus_bb_2   = false;
    int  help_antiCcounter_2 = 0;
    int  help_Ccounter_2 = 0;
    bool help_isC_2 = false;
    bool help_isantiC_2 = false;
    int  help_antiBcounter_2 = 0;
    int  help_Bcounter_2 = 0;
    bool help_isB_2 = false;
    bool help_isantiB_2 = false;

    // Settings 3: quark filter, c-truth-jet-matching
    bool find_cQuark_3  = true;  // (c)
    bool find_cHadron_3 = false; // (charm)
    bool find_bQuark_3  = true;  // (b)
    bool find_bHadron_3 = false; // (bottom)
    if (find_cQuark_3 && find_cHadron_3) std::cout << "ERROR: (3) set c-quark filter AND c-hadron filter ==> Choose only 1!" << std::endl;
    if (find_bQuark_3 && find_bHadron_3) std::cout << "ERROR: (3) set b-quark filter AND b-hadron filter ==> Choose only 1!" << std::endl;
    bool require_truthJets_forC_3 = true;
    bool require_truthJets_forB_3 = false;

    // variables for setup-3
    bool W_plus_C_3    = false;
    bool W_plus_cc_3   = false;
    bool W_plus_anyC_3 = false;
    bool W_plus_anyB_3 = false;
    bool W_plus_B_3    = false;
    bool W_plus_bb_3   = false;
    int  help_antiCcounter_3 = 0;
    int  help_Ccounter_3 = 0;
    bool help_isC_3 = false;
    bool help_isantiC_3 = false;
    int  help_antiBcounter_3 = 0;
    int  help_Bcounter_3 = 0;
    bool help_isB_3 = false;
    bool help_isantiB_3 = false;

    // Settings 4: quark filter, no truth-jet-matching
    bool find_cQuark_4  = true;  // (c)
    bool find_cHadron_4 = false; // (charm)
    bool find_bQuark_4  = true;  // (b)
    bool find_bHadron_4 = false; // (bottom)
    if (find_cQuark_4 && find_cHadron_4) std::cout << "ERROR: (4) set c-quark filter AND c-hadron filter ==> Choose only 1!" << std::endl;
    if (find_bQuark_4 && find_bHadron_4) std::cout << "ERROR: (4) set b-quark filter AND b-hadron filter ==> Choose only 1!" << std::endl;
    bool require_truthJets_forC_4 = false;
    bool require_truthJets_forB_4 = false;

    // variables for setup-4
    bool W_plus_C_4    = false;
    bool W_plus_cc_4   = false;
    bool W_plus_anyC_4 = false;
    bool W_plus_anyB_4 = false;
    bool W_plus_B_4    = false;
    bool W_plus_bb_4   = false;
    int  help_antiCcounter_4 = 0;
    int  help_Ccounter_4 = 0;
    bool help_isC_4 = false;
    bool help_isantiC_4 = false;
    int  help_antiBcounter_4 = 0;
    int  help_Bcounter_4 = 0;
    bool help_isB_4 = false;
    bool help_isantiB_4 = false;

    // Settings 5: extended Hadron filter, c-truth-jet-matching
    bool find_cQuark_5  = false;  // (c)
    bool find_cHadron_5 = true; // (charm)
    bool find_bQuark_5  = false;  // (b)
    bool find_bHadron_5 = true; // (bottom)
    if (find_cQuark_5 && find_cHadron_5) std::cout << "ERROR: (5) set c-quark filter AND c-hadron filter ==> Choose only 1!" << std::endl;
    if (find_bQuark_5 && find_bHadron_5) std::cout << "ERROR: (5) set b-quark filter AND b-hadron filter ==> Choose only 1!" << std::endl;
    bool require_truthJets_forC_5 = true;
    bool require_truthJets_forB_5 = false;

    // variables for setup-5
    bool W_plus_C_5    = false;
    bool W_plus_cc_5   = false;
    bool W_plus_anyC_5 = false;
    bool W_plus_anyB_5 = false;
    bool W_plus_B_5    = false;
    bool W_plus_bb_5   = false;
    int  help_antiCcounter_5 = 0;
    int  help_Ccounter_5 = 0;
    bool help_isC_5 = false;
    bool help_isantiC_5 = false;
    int  help_antiBcounter_5 = 0;
    int  help_Bcounter_5 = 0;
    bool help_isB_5 = false;
    bool help_isantiB_5 = false;

    // Settings 6: extended Hadron filter, no truth-jet-matching
    bool find_cQuark_6  = false;  // (c)
    bool find_cHadron_6 = true; // (charm)
    bool find_bQuark_6  = false;  // (b)
    bool find_bHadron_6 = true; // (bottom)
    if (find_cQuark_6 && find_cHadron_6) std::cout << "ERROR: (6) set c-quark filter AND c-hadron filter ==> Choose only 1!" << std::endl;
    if (find_bQuark_6 && find_bHadron_6) std::cout << "ERROR: (6) set b-quark filter AND b-hadron filter ==> Choose only 1!" << std::endl;
    bool require_truthJets_forC_6 = false;
    bool require_truthJets_forB_6 = false;

    // variables for setup-6
    bool W_plus_C_6    = false;
    bool W_plus_cc_6   = false;
    bool W_plus_anyC_6 = false;
    bool W_plus_anyB_6 = false;
    bool W_plus_B_6    = false;
    bool W_plus_bb_6   = false;
    int  help_antiCcounter_6 = 0;
    int  help_Ccounter_6 = 0;
    bool help_isC_6 = false;
    bool help_isantiC_6 = false;
    int  help_antiBcounter_6 = 0;
    int  help_Bcounter_6 = 0;
    bool help_isB_6 = false;
    bool help_isantiB_6 = false;
    // ===============>

    std::vector<TLorentzVector> truth_jets_C;
    truth_jets_C.clear();
    std::vector<TLorentzVector> truth_jets_B;
    truth_jets_B.clear();

    // settings for truth jets:
    double truth_pt_min_C = 15000.0;
    double truth_eta_max_C = 3.0;
    double truth_pt_min_B = 15000.0;
    double truth_eta_max_B = 3.0;

    bool store_Sherpa22_weight = false;

    if (m_isSherpaW) { // = is MC
        // is valid for Sherpa and Sherpa 2.2 !!!
        float MCchannel_number = 0.;
        MCchannel_number = event.m_info->mcChannelNumber();
        //if (MCchannel_number < 361300 ||
        //    (MCchannel_number>361371 && MCchannel_number<363331) ||
        //    (MCchannel_number>363354 && MCchannel_number<363436) ||
        //    MCchannel_number>363483) {
        if (! ( (MCchannel_number >= 363331 && MCchannel_number <= 363354) || \
                (MCchannel_number >= 363436 && MCchannel_number <= 363483) || \
                (MCchannel_number >= 364156 && MCchannel_number <= 364197)
              ) ) {
            m_isSherpaW = false;
        }
        // if sample is (W) Sherpa 2.2 sample
        if ( (MCchannel_number >= 363331 && MCchannel_number <= 363354) || \
             (MCchannel_number >= 363436 && MCchannel_number <= 363483) || \
             (MCchannel_number >= 364156 && MCchannel_number <= 364197)
           )
            store_Sherpa22_weight = true;
    }

    if (!m_isSherpaW ) {
        m_Wfilter = 0;
        m_Wfilter_2 = 0;
        m_Wfilter_3 = 0;
        m_Wfilter_4 = 0;
        m_Wfilter_5 = 0;
        m_Wfilter_6 = 0;
        m_Sherpa22_weight = 1;
    }

    // apply filter
    if (m_isSherpaW) {
        if (require_truthJets_forC || require_truthJets_forB || store_Sherpa22_weight ) {
            //Load truth jets
            truth_jets_C.clear(); // filled after truth jets cut aplied
            truth_jets_B.clear(); // filled after truth jets cut aplied
            int counter_Sherpa22_Njets = 0;

            // apply cuts to truth jets
            const xAOD::JetContainer* akt4truthjets = nullptr;
            if (m_akt4truthjetcollectionWjets != "") {
                top::check(evtStore()->event()->retrieve(akt4truthjets, m_akt4truthjetcollectionWjets), "FAILURE");
                for (xAOD::Jet *jet : (xAOD::JetContainer) *akt4truthjets) {
                    TLorentzVector oneTruthJet;
                    oneTruthJet.SetPtEtaPhiE(jet->pt(), jet->eta(), jet->phi(), jet->e());

                    if (store_Sherpa22_weight) {
                        if (oneTruthJet.Pt() > 20000 && fabs(oneTruthJet.Eta()) < 4.5) counter_Sherpa22_Njets++;
                    }

                    if (require_truthJets_forC &&
                            oneTruthJet.Pt() > truth_pt_min_C &&
                            fabs(oneTruthJet.Eta()) < truth_eta_max_C)
                        truth_jets_C.push_back(oneTruthJet);
                    if (require_truthJets_forB &&
                            oneTruthJet.Pt() > truth_pt_min_B &&
                            fabs(oneTruthJet.Eta()) < truth_eta_max_B)
                        truth_jets_B.push_back(oneTruthJet);
                } // truth jet loop
            } else {
                std::cout << " ERROR truth jets loading did not work" << std::endl;
                m_Wfilter  = -2;
                if (require_truthJets_forB_2 || require_truthJets_forC_2) m_Wfilter_2 = -2;
                if (require_truthJets_forB_3 || require_truthJets_forC_3) m_Wfilter_3 = -2;
                if (require_truthJets_forB_4 || require_truthJets_forC_4) m_Wfilter_4 = -2;
                if (require_truthJets_forB_5 || require_truthJets_forC_5) m_Wfilter_5 = -2;
                if (require_truthJets_forB_6 || require_truthJets_forC_6) m_Wfilter_6 = -2;
            } // if (m_akt4truthjetcollection != "")else

            if (counter_Sherpa22_Njets > 8) counter_Sherpa22_Njets = 8;
            if (store_Sherpa22_weight) {
                // initialisation
                float  m_v_ZNJet0TagSherpaCorr[9];
                m_v_ZNJet0TagSherpaCorr[0] = 1.0007 - 1;
                m_v_ZNJet0TagSherpaCorr[1] = 0.9904 - 1;
                m_v_ZNJet0TagSherpaCorr[2] = 0.9884 - 1;
                m_v_ZNJet0TagSherpaCorr[3] = 0.9128 - 1;
                m_v_ZNJet0TagSherpaCorr[4] = 0.8114 - 1;
                m_v_ZNJet0TagSherpaCorr[5] = 0.7833 - 1;
                m_v_ZNJet0TagSherpaCorr[6] = 0.6872 - 1;
                m_v_ZNJet0TagSherpaCorr[7] = 0.6424 - 1;
                m_v_ZNJet0TagSherpaCorr[8] = 0.6424 - 1;

                // Correction
                // Function of njettruth which is the number of truth jets with pt>20 and |eta|<4.5
                float scale = 1.;
                scale = m_v_ZNJet0TagSherpaCorr[counter_Sherpa22_Njets];
                float Norm = 1.007; // ensure the overall normalisation agrees with the Strict integration setting
                m_Sherpa22_weight = 1 + scale * Norm;
            }
        } // if requiere_truthJet || store Sherpa_corr weight

        for ( const auto* const truthPart : * (event.m_truth) ) {
            int pdgId = truthPart->pdgId();
            // ### step 1a
            if (find_cQuark) {
                Wfind_cquark(require_truthJets_forC, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC, help_isC, help_isantiC);
                if (help_isC) help_Ccounter++;
                if (help_isantiC) help_antiCcounter++;
            }
            // ### step 2-1a
            if (find_cQuark_2) {
                Wfind_cquark(require_truthJets_forC_2, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC_2, help_isC_2, help_isantiC_2);
                if (help_isC_2) help_Ccounter_2++;
                if (help_isantiC_2) help_antiCcounter_2++;
            }
            // ### step 3-1a
            if (find_cQuark_3) {
                Wfind_cquark(require_truthJets_forC_3, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC_3, help_isC_3, help_isantiC_3);
                if (help_isC_3) help_Ccounter_3++;
                if (help_isantiC_3) help_antiCcounter_3++;
            }
            // ### step 4-1a
            if (find_cQuark_4) {
                Wfind_cquark(require_truthJets_forC_4, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC_4, help_isC_4, help_isantiC_4);
                if (help_isC_4) help_Ccounter_4++;
                if (help_isantiC_4) help_antiCcounter_4++;
            }
            // ### step 5-1a
            if (find_cQuark_5) {
                Wfind_cquark(require_truthJets_forC_5, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC_5, help_isC_5, help_isantiC_5);
                if (help_isC_5) help_Ccounter_5++;
                if (help_isantiC_5) help_antiCcounter_5++;
            }
            // ### step 6-1a
            if (find_cQuark_6) {
                Wfind_cquark(require_truthJets_forC_6, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC_6, help_isC_6, help_isantiC_6);
                if (help_isC_6) help_Ccounter_6++;
                if (help_isantiC_6) help_antiCcounter_6++;
            }

            // ### step 1b
            if (find_cHadron) {
                Wfind_chadron(require_truthJets_forC, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC,  help_isC, help_isantiC);
                if (help_isC) help_Ccounter++;
                if (help_isantiC) help_antiCcounter++;
            }
            // ### step 2-1b
            if (find_cHadron_2) {
                Wfind_chadron(require_truthJets_forC_2, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC_2,  help_isC_2, help_isantiC_2);
                if (help_isC_2) help_Ccounter_2++;
                if (help_isantiC_2) help_antiCcounter_2++;
            }
            // ### step 3-1b
            if (find_cHadron_3) {
                Wfind_chadron(require_truthJets_forC_3, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC_3,  help_isC_3, help_isantiC_3);
                if (help_isC_3) help_Ccounter_3++;
                if (help_isantiC_3) help_antiCcounter_3++;
            }
            // ### step 4-1b
            if (find_cHadron_4) {
                Wfind_chadron(require_truthJets_forC_4, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC_4,  help_isC_4, help_isantiC_4);
                if (help_isC_4) help_Ccounter_4++;
                if (help_isantiC_4) help_antiCcounter_4++;
            }
            // ### step 5-1b
            if (find_cHadron_5) {
                Wfind_chadron_ext(require_truthJets_forC_5, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC_5,  help_isC_5, help_isantiC_5);
                if (help_isC_5) help_Ccounter_5++;
                if (help_isantiC_5) help_antiCcounter_5++;
            }
            // ### step 6-1b
            if (find_cHadron_6) {
                Wfind_chadron_ext(require_truthJets_forC_6, truth_jets_C, pdgId, truthPart->p4(), W_plus_anyC_6,  help_isC_6, help_isantiC_6);
                if (help_isC_6) help_Ccounter_6++;
                if (help_isantiC_6) help_antiCcounter_6++;
            }

            // ### step 2a
            if (find_bQuark) {
                Wfind_bquark(require_truthJets_forB, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB, help_isB, help_isantiB);
                if (help_isB) help_Bcounter++;
                if (help_isantiB) help_antiBcounter++;
            } // find b-Quark
            // ### step 2-2a
            if (find_bQuark_2) {
                Wfind_bquark(require_truthJets_forB_2, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB_2, help_isB_2, help_isantiB_2);
                if (help_isB_2) help_Bcounter_2++;
                if (help_isantiB_2) help_antiBcounter_2++;
            } // find b-Quark
            // ### step 3-2a
            if (find_bQuark_3) {
                Wfind_bquark(require_truthJets_forB_3, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB_3, help_isB_3, help_isantiB_3);
                if (help_isB_3) help_Bcounter_3++;
                if (help_isantiB_3) help_antiBcounter_3++;
            } // find b-Quark
            // ### step 4-2a
            if (find_bQuark_4) {
                Wfind_bquark(require_truthJets_forB_4, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB_4, help_isB_4, help_isantiB_4);
                if (help_isB_4) help_Bcounter_4++;
                if (help_isantiB_4) help_antiBcounter_4++;
            } // find b-Quark
            // ### step 5-2a
            if (find_bQuark_5) {
                Wfind_bquark(require_truthJets_forB_5, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB_5, help_isB_5, help_isantiB_5);
                if (help_isB_5) help_Bcounter_5++;
                if (help_isantiB_5) help_antiBcounter_5++;
            } // find b-Quark
            // ### step 6-2a
            if (find_bQuark_6) {
                Wfind_bquark(require_truthJets_forB_6, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB_6, help_isB_6, help_isantiB_6);
                if (help_isB_6) help_Bcounter_6++;
                if (help_isantiB_6) help_antiBcounter_6++;
            } // find b-Quark

            // ### step 2b
            if (find_bHadron) {
                Wfind_bhadron(require_truthJets_forB, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB, help_isB, help_isantiB);
                if (help_isB) help_Bcounter++;
                if (help_isantiB) help_antiBcounter++;
            } // find b-Hadron
            // ### step 2-2b
            if (find_bHadron_2) {
                Wfind_bhadron(require_truthJets_forB_2, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB_2, help_isB_2, help_isantiB_2);
                if (help_isB_2) help_Bcounter_2++;
                if (help_isantiB_2) help_antiBcounter_2++;
            } // find b-Hadron
            // ### step 3-2b
            if (find_bHadron_3) {
                Wfind_bhadron(require_truthJets_forB_3, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB_3, help_isB_3, help_isantiB_3);
                if (help_isB_3) help_Bcounter_3++;
                if (help_isantiB_3) help_antiBcounter_3++;
            } // find b-Hadron
            // ### step 4-2b
            if (find_bHadron_4) {
                Wfind_bhadron(require_truthJets_forB_4, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB_4, help_isB_4, help_isantiB_4);
                if (help_isB_4) help_Bcounter_4++;
                if (help_isantiB_4) help_antiBcounter_4++;
            } // find b-Hadron
            // ### step 5-2b
            if (find_bHadron_5) {
                Wfind_bhadron_ext(require_truthJets_forB_5, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB_5, help_isB_5, help_isantiB_5);
                if (help_isB_5) help_Bcounter_5++;
                if (help_isantiB_5) help_antiBcounter_5++;
            } // find b-Hadron
            // ### step 6-2b
            if (find_bHadron_6) {
                Wfind_bhadron_ext(require_truthJets_forB_6, truth_jets_B, pdgId, truthPart->p4(), W_plus_anyB_6, help_isB_6, help_isantiB_6);
                if (help_isB_6) help_Bcounter_6++;
                if (help_isantiB_6) help_antiBcounter_6++;
            } // find b-Hadron

        } // loop truthPart

        Wset_Cfilter(W_plus_anyC, help_Ccounter, help_antiCcounter, W_plus_C, W_plus_cc);
        Wset_Bfilter(W_plus_anyB, help_Bcounter, help_antiBcounter, W_plus_B, W_plus_bb);

        // W-filter 2
        Wset_Cfilter(W_plus_anyC_2, help_Ccounter_2, help_antiCcounter_2, W_plus_C_2, W_plus_cc_2);
        Wset_Bfilter(W_plus_anyB_2, help_Bcounter_2, help_antiBcounter_2, W_plus_B_2, W_plus_bb_2);
        // W-filter 3
        Wset_Cfilter(W_plus_anyC_3, help_Ccounter_3, help_antiCcounter_3, W_plus_C_3, W_plus_cc_3);
        Wset_Bfilter(W_plus_anyB_3, help_Bcounter_3, help_antiBcounter_3, W_plus_B_3, W_plus_bb_3);
        // W-filter 4
        Wset_Cfilter(W_plus_anyC_4, help_Ccounter_4, help_antiCcounter_4, W_plus_C_4, W_plus_cc_4);
        Wset_Bfilter(W_plus_anyB_4, help_Bcounter_4, help_antiBcounter_4, W_plus_B_4, W_plus_bb_4);
        // W-filter 5
        Wset_Cfilter(W_plus_anyC_5, help_Ccounter_5, help_antiCcounter_5, W_plus_C_5, W_plus_cc_5);
        Wset_Bfilter(W_plus_anyB_5, help_Bcounter_5, help_antiBcounter_5, W_plus_B_5, W_plus_bb_5);
        // W-filter 6
        Wset_Cfilter(W_plus_anyC_6, help_Ccounter_6, help_antiCcounter_6, W_plus_C_6, W_plus_cc_6);
        Wset_Bfilter(W_plus_anyB_6, help_Bcounter_6, help_antiBcounter_6, W_plus_B_6, W_plus_bb_6);

        if (W_plus_C  && !W_plus_anyB ) {
            m_Wfilter = 1;
        }
        if (W_plus_cc && !W_plus_anyB) {
            m_Wfilter = 2;
        }
        if (W_plus_B) {
            m_Wfilter = 3;
        }
        if (W_plus_bb) {
            m_Wfilter = 4;
        }
        if (!W_plus_anyB && !W_plus_anyC) {
            m_Wfilter = 5;
        }
        // ############### check different settings
//    std::cout<<" (2) W-filter, hadron, no truth jet matching"<<std::endl;
        if (W_plus_C_2  && !W_plus_anyB_2)    m_Wfilter_2 = 1;
        if (W_plus_cc_2 && !W_plus_anyB_2)    m_Wfilter_2 = 2;
        if (W_plus_B_2)                       m_Wfilter_2 = 3;
        if (W_plus_bb_2)                      m_Wfilter_2 = 4;
        if (!W_plus_anyB_2 && !W_plus_anyC_2) m_Wfilter_2 = 5;
        // ####
//    std::cout<<" (3) W-filter, quark, c-truth jet matching"<<std::endl;
        if (W_plus_C_3  && !W_plus_anyB_3)    m_Wfilter_3 = 1;
        if (W_plus_cc_3 && !W_plus_anyB_3)    m_Wfilter_3 = 2;
        if (W_plus_B_3)                       m_Wfilter_3 = 3;
        if (W_plus_bb_3)                      m_Wfilter_3 = 4;
        if (!W_plus_anyB_3 && !W_plus_anyC_3) m_Wfilter_3 = 5;
        // ####
//    std::cout<<" (4) W-filter, quark, no truth jet matching"<<std::endl;
        if (W_plus_C_4  && !W_plus_anyB_4)    m_Wfilter_4 = 1;
        if (W_plus_cc_4 && !W_plus_anyB_4)    m_Wfilter_4 = 2;
        if (W_plus_B_4)                       m_Wfilter_4 = 3;
        if (W_plus_bb_4)                      m_Wfilter_4 = 4;
        if (!W_plus_anyB_4 && !W_plus_anyC_4) m_Wfilter_4 = 5;
        // ####
//    std::cout<<" (5) W-filter, ext. hadron, c-truth jet matching"<<std::endl;
        if (W_plus_C_5  && !W_plus_anyB_5)    m_Wfilter_5 = 1;
        if (W_plus_cc_5 && !W_plus_anyB_5)    m_Wfilter_5 = 2;
        if (W_plus_B_5)                       m_Wfilter_5 = 3;
        if (W_plus_bb_5)                      m_Wfilter_5 = 4;
        if (!W_plus_anyB_5 && !W_plus_anyC_5) m_Wfilter_5 = 5;
        // ####
//    std::cout<<" (6) W-filter, ext. hadron,  notruth jet matching"<<std::endl;
        if (W_plus_C_6  && !W_plus_anyB_6)    m_Wfilter_6 = 1;
        if (W_plus_cc_6 && !W_plus_anyB_6)    m_Wfilter_6 = 2;
        if (W_plus_B_6)                       m_Wfilter_6 = 3;
        if (W_plus_bb_6)                      m_Wfilter_6 = 4;
        if (!W_plus_anyB_6 && !W_plus_anyC_6) m_Wfilter_6 = 5;
        // ##################### end different setups

        // START Comparison:
        TString Filter_naming[7];
        Filter_naming[0] = "noSherpa sample";
        Filter_naming[1] = "W+c";
        Filter_naming[2] = "W+cc";
        Filter_naming[3] = "W+b";
        Filter_naming[4] = "W+bb";
        Filter_naming[5] = "W+lf";
        Filter_naming[6] = "default = not set";

        /*
        if ((m_Wfilter!=m_Wfilter_2 || m_Wfilter!=m_Wfilter_3 || m_Wfilter!=m_Wfilter_4 || m_Wfilter!=m_Wfilter_5 || m_Wfilter!=m_Wfilter_6) && debug){
          int filter=m_Wfilter;
          if (filter<0) filter=6;
          //std::cout<<" Standard filter (hadron, c-truth-match): "<<Filter_naming[filter]<<std::endl;
          if (m_Wfilter_2<0) {filter=6;} else filter = m_Wfilter_2;
          //std::cout<<" (2) hadron, no truth-matching : "<<Filter_naming[filter]<<std::endl;
          if (m_Wfilter_3<0) {filter=6;} else filter = m_Wfilter_3;
          //std::cout<<" (3) quark, c-truth-matching : "<<Filter_naming[filter]<<std::endl;
          if (m_Wfilter_4<0) {filter=6;} else filter = m_Wfilter_4;
          //std::cout<<" (4) quark, no truth-matching : "<<Filter_naming[filter]<<std::endl;
          if (m_Wfilter_5<0) {filter=6;} else filter = m_Wfilter_5;
          //std::cout<<" (5) ext. hadron, c-truth-matching : "<<Filter_naming[filter]<<std::endl;
          if (m_Wfilter_6<0) {filter=6;} else filter = m_Wfilter_6;
          //std::cout<<" (6) ext. hadron, no truth-matching : "<<Filter_naming[filter]<<std::endl;
        }*/
        // END Comparison
    } //if isSherpaW
}

int TtresEventSaverFlatNtuple::FindInVector(vector<int>& v, int x) {
    std::vector<int>::iterator it = find(v.begin(), v.end(), x);
    int index = (it != v.end()) ? distance(v.begin(), it) : -1;
    return index;
}
void TtresEventSaverFlatNtuple::PrintME(const xAOD::TruthParticle* mcPtr, int depth, int maxDepth) {
    if (depth > maxDepth) return;
    std::cout << "[" << depth << "]" << " id=" << mcPtr->pdgId() << " st=" << mcPtr->status() << " bc=" << mcPtr->barcode() << ", E=" << mcPtr->e() / 1000. << " GeV" << std::endl;
    if (mcPtr->nParents()) cout << "  parents:";
    for (unsigned int x = 0 ; x < mcPtr->nParents() ; ++x) {
        const xAOD::TruthParticle* parent = mcPtr->parent(x);
        if (parent) cout << " id=" << parent->pdgId() << ":bc=" << parent->barcode() << ",";
    }
    if (mcPtr->nParents()) cout << endl;
    if (mcPtr->nChildren()) cout << "  children:";
    for (unsigned int x = 0 ; x < mcPtr->nChildren() ; ++x) {
        const xAOD::TruthParticle* child = mcPtr->child(x);
        if (child) cout << " id=" << child->pdgId() << ":bc=" << child->barcode() << ",";
    }
    if (mcPtr->nChildren()) cout << endl;
}
void TtresEventSaverFlatNtuple::FillME(const xAOD::TruthParticleContainer* truthparticles, const xAOD::TruthEventContainer* truthevents) {
    // Find the indices of the ME participants
    TMapTSiarr mebarcodes;
    vector<int> vtmp;
    vector<int> vtmp0;
    vtmp.push_back(-1);
    vtmp.push_back(-1);
    mebarcodes.insert(make_pair("tops", vtmp));
    mebarcodes.insert(make_pair("parents1", vtmp)); // initialise with a [-1,-1] vector
    mebarcodes.insert(make_pair("parents2", vtmp)); // initialise with a [-1,-1] vector
    mebarcodes.insert(make_pair("sisters1", vtmp0)); // initialise with an empty vector
    mebarcodes.insert(make_pair("sisters2", vtmp0)); // initialise with an empty vector
    if (truthparticles) {
        int n = 0;
        for (const auto* const mcPtr : *truthparticles) {
            // PrintME(mcPtr,n);
            ++n;
            if (abs(mcPtr->pdgId()) == 6  && (mebarcodes["tops"][0] < 0 || mebarcodes["tops"][1] < 0)) {
                int     iTop = (mebarcodes["tops"][0] < 0) ? 0 : 1;
                TString sTop = (iTop == 0) ? "1" : "2";
                mebarcodes["tops"][iTop] = mcPtr->barcode();
                int nparents = mcPtr->nParents();
                for (int j = 0 ; j < nparents ; ++j) {
                    const xAOD::TruthParticle* parent = mcPtr->parent(j);
                    if (!parent) continue; // it is NULL...
                    mebarcodes["parents" + sTop][j] = parent->barcode();
                    int nsisters = parent->nChildren();
                    for (int k = 0 ; k < nsisters ; ++k) {
                        const xAOD::TruthParticle* sister = parent->child(k);
                        if (!sister)                                                       continue; // it is NULL...
                        if (abs(sister->pdgId()) == 6)                                       continue; // it is a top...
                        if (FindInVector(mebarcodes["sisters" + sTop], sister->barcode()) >= 0) continue; // it is already listed
                        mebarcodes["sisters" + sTop].push_back(sister->barcode());
                    }
                }
            }
        }
    }
    // Fill the vectors for ME^2
    // gg/gq/qq-->tt/ttg/ttgg/ttgq/ttqq
    if (mebarcodes["tops"][0] >= 0     && mebarcodes["tops"][1] >= 0     &&
            mebarcodes["parents1"][0] >= 0 && mebarcodes["parents1"][1] >= 0 &&
            mebarcodes["parents2"][0] >= 0 && mebarcodes["parents2"][1] >= 0) {
        vector<int> finalMEbarcodes; // the incoming partons are always first, the tops are second and the extra partons are final (if any)
        finalMEbarcodes.push_back(mebarcodes["parents1"][0]);
        finalMEbarcodes.push_back(mebarcodes["parents1"][1]);
        finalMEbarcodes.push_back(mebarcodes["tops"][0]);
        finalMEbarcodes.push_back(mebarcodes["tops"][1]);
        if (mebarcodes["sisters1"].size() > 0) finalMEbarcodes.push_back(mebarcodes["sisters1"][0]);
        if (mebarcodes["sisters1"].size() > 1) finalMEbarcodes.push_back(mebarcodes["sisters1"][1]);
        float Q = 0.;
        for (unsigned int k = 0 ; k < finalMEbarcodes.size() ; ++k) {
            const xAOD::TruthParticle* pReal = NULL;
            for (unsigned int j = 0 ; j < truthparticles->size() ; ++j) {
                const xAOD::TruthParticle* p = truthparticles->at(j);
                if (p->barcode() != finalMEbarcodes[k]) continue;
                pReal = p;
                break;
            }
            if (!pReal) continue;
            // cout << "ME particle["<<k<<"]"<<" id="<<pReal->pdgId()<<" st="<<pReal->status()<<" bc="<<pReal->barcode()<<endl;
            m_MC_px_me.push_back(pReal->px() / 1000.);
            m_MC_py_me.push_back(pReal->py() / 1000.);
            m_MC_pz_me.push_back(pReal->pz() / 1000.);
            m_MC_e_me.push_back(pReal->e() / 1000.);
            m_MC_m_me.push_back(pReal->m() / 1000.);
            m_MC_id_me.push_back(pReal->pdgId());
            if (k < 2) continue; // skip the 2 incoming partons
            float pT2 = pReal->px() * pReal->px() + pReal->py() * pReal->py();
            float m2 = pReal->m() * pReal->m();
            Q += sqrt(m2 + pT2);
        }
        m_MC_Q_me = Q / 2.;
        for (auto truthevent : *truthevents) {
            (void)truthevent;
            float QxAOD = -1;
            float aSxAOD = -1;
            //truthevent->pdfInfoParameter(QxAOD, xAOD::TruthEvent::PdfParam::Q);
            //truthevent->pdfInfoParameter(aSxAOD, xAOD::TruthEvent::PdfParam::Scale); // just a placeholder - PMG should fix this...
            m_MC_QxAOD_me.push_back(QxAOD);
            m_MC_aSxAOD_me.push_back(aSxAOD);
        }
    }
}

void TtresEventSaverFlatNtuple::finalize() {
    EventSaverFlatNtuple::finalize();
    if ( m_dumpToolConfigTo != "False") {
    std::cout << ">>> dumping ToolConig to" << " \"" << m_dumpToolConfigTo << "\" "  "<<<" << std::endl;
    dumpToolConfig(m_dumpToolConfigTo);
    }
  }

void TtresEventSaverFlatNtuple::dumpToolConfig(std::string fname) {
    std::ofstream out(fname);
    std::streambuf *coutbuf = std::cout.rdbuf();
    std::cout.rdbuf(out.rdbuf());
    asg::ToolStore::dumpToolConfig();
    std::cout.rdbuf(coutbuf);
  }

const std::string& TtresEventSaverFlatNtuple::configValueDefault(const std::string& key, const std::string& default_value) const {
    // To be nice to other analysis, I think it is neccessary to give those dynamic keys a default value so that it behave like vanilla AnalysisTop,
    //  and don't throw non-neccessary errors.
    try {
        return top::ConfigurationSettings::get()->value(key);
    }
    catch (...) {
        return default_value;
    }
    }

}//namespace top


