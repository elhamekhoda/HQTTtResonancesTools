#include "HQTTtResonancesTools/ObjectLoaderTtres.h"
#include "TopEvent/EventTools.h"
#include "TopConfiguration/TopConfig.h"

#include "TopObjectSelectionTools/TopObjectSelection.h"

#include "TopObjectSelectionTools/ElectronLikelihoodMC15.h"
#include "TopObjectSelectionTools/MuonMC15.h"
#include "TopObjectSelectionTools/TrackJetMC15.h"

#include "TopObjectSelectionTools/IsolationTools.h"
#include "HQTTtResonancesTools/ElectronTtres.h"
#include "HQTTtResonancesTools/ElectronTtresFakes.h"
#include "HQTTtResonancesTools/MuonTtres.h"
#include "HQTTtResonancesTools/MuonTtresFakes.h"
#include "HQTTtResonancesTools/MuonTtresFakesInvSd0.h"
#include "HQTTtResonancesTools/MuonTtresFakesInclusiveSd0.h"
#include "HQTTtResonancesTools/JetTtres.h"
#include "TopObjectSelectionTools/OverlapRemovalASG.h"
//#include "HQTTtResonancesTools/OverlapRemovalASGTtres.h"

#include "TopConfiguration/ConfigurationSettings.h"

namespace top {

top::TopObjectSelection* ObjectLoaderTtres::init(std::shared_ptr<top::TopConfig> topConfig) {

    top::TopObjectSelection* objectSelection = new top::TopObjectSelection(topConfig->objectSelectionName());
    top::check(objectSelection->setProperty( "config" , topConfig ) , "Failed to setProperty for top::TopObjectSelection" );
    top::check(objectSelection->initialize() , "Failed to initialize top::TopObjectSelection" ); 
    
    std::cout << "Going to use " << topConfig->electronID() << " / " << topConfig->electronIDLoose() << " for (tight/loose) electrons" <<std::endl;   
    
    top::ConfigurationSettings* configSettings = top::ConfigurationSettings::get();
    int fakeLeptonSel = 0;
    if (configSettings->value("fakeLeptonSel") =="True") {
      fakeLeptonSel = 1;
    } else if (configSettings->value("fakeLeptonSel") =="MuInvSd0") {
      fakeLeptonSel = 2;
    } else if (configSettings->value("fakeLeptonSel") =="MuNoSd0") {
      fakeLeptonSel = 3;
    }
    
    
    if (fakeLeptonSel == 1)	std::cout << "Using fake lepton definition for the loose lepton selection" << std::endl; 
    if (fakeLeptonSel == 2)	std::cout << "Using std. electron definition and muon definition with inverted S(d0) cut for the CR selection." << std::endl;
    if (fakeLeptonSel == 3)	std::cout << "Using std. electron definition and muon definition without cutting on S(d0) for the CR selection." << std::endl;
    
    //Electron selection
    if (topConfig->electronID().find("LH") != std::string::npos && topConfig->electronIDLoose().find("LH") != std::string::npos) {
    
      if (fakeLeptonSel == 1){
        objectSelection->electronSelection(new top::ElectronTtresFakes(topConfig->electronPtcut(),
                                           topConfig->electronVetoLArCrack(),
                                           topConfig->electronID(), topConfig->electronIDLoose(),
                                           new top::StandardIsolation(topConfig->electronIsolation() ,
                                                                      topConfig->electronIsolationLoose())));
      } else if (fakeLeptonSel == 2 || fakeLeptonSel == 3){
	objectSelection->electronSelection(new top::ElectronTtres(topConfig->electronPtcut(),
                                           topConfig->electronVetoLArCrack(),
                                           topConfig->electronID(), topConfig->electronIDLoose(),
                                           new top::StandardIsolation(topConfig->electronIsolation() ,
                                                                      topConfig->electronIsolationLoose())));							      
      } else {	
	objectSelection->electronSelection(new top::ElectronTtres(topConfig->electronPtcut(),
                                           topConfig->electronVetoLArCrack(),
                                           topConfig->electronID(), topConfig->electronIDLoose(),
                                           new top::StandardIsolation(topConfig->electronIsolation() ,
                                                                      topConfig->electronIsolationLoose())));							      
      }//fakeLeptonSel							      
								      
    } else {
        std::cout << "\nHo hum\n";
        //std::cout << "Not sure it makes sense to use a mix of LH and cut-based electrons for the tight/loose definitions\n";
        std::cout << "Tight electron definition is " << topConfig->electronID() << "\n";
        std::cout << "Loose electron definition is " << topConfig->electronIDLoose() << "\n";
        std::cout << "If it does make sense, feel free to fix this\n";
        exit(1);
    }
    
    //Muon selection
    if (fakeLeptonSel == 1){
       objectSelection->muonSelection(new top::MuonTtresFakes(topConfig->muonPtcut(),
                                      new top::StandardIsolation(topConfig->muonIsolation() ,
                                                                 topConfig->muonIsolationLoose())));
    } else if (fakeLeptonSel == 2) {
       objectSelection->muonSelection(new top::MuonTtresFakesInvSd0(topConfig->muonPtcut(),
                                      new top::StandardIsolation(topConfig->muonIsolation() ,
                                                                 topConfig->muonIsolationLoose())));
    } else if (fakeLeptonSel == 3) {
       objectSelection->muonSelection(new top::MuonTtresFakesInclusiveSd0(topConfig->muonPtcut(),
                                      new top::StandardIsolation(topConfig->muonIsolation() ,
                                                                 topConfig->muonIsolationLoose())));
    } else {
       objectSelection->muonSelection(new top::MuonTtres(topConfig->muonPtcut(),
                                      new top::StandardIsolation(topConfig->muonIsolation() ,
                                                                 topConfig->muonIsolationLoose())));
    }//fakeLeptonSel							 

    std::cout << "Going to use " << topConfig->muonQuality() << " / " << topConfig->muonQualityLoose() << " for (tight/loose) muons" <<std::endl;   

    objectSelection->jetSelection(new top::JetTtres(topConfig->jetPtcut(), topConfig->jetEtacut(), true, topConfig->fwdJetAndMET(), topConfig->trackJetPtcut()));
    objectSelection->largeJetSelection(new top::JetTtres(topConfig->largeRJetPtcut(), topConfig->largeRJetEtacut(), false, "default", topConfig->trackJetPtcut()));
    objectSelection->trackJetSelection(new top::TrackJetMC15(topConfig->trackJetPtcut(), topConfig->trackJetEtacut()));

//  objectSelection->overlapRemovalPostSelection(new top::OverlapRemovalASG());
    
/*    if (topConfig->applyElectronInJetSubtraction())
      objectSelection->overlapRemovalPostSelection(new top::OverlapRemovalASGTtres(true));
    else
      objectSelection->overlapRemovalPostSelection(new top::OverlapRemovalASGTtres(false));
*/
    ///-- Overlap removal --///
    /// single parameter: boolean to do OR with large-R jets
    if (!topConfig->isTruthDxAOD()) {
      objectSelection->overlapRemovalPostSelection(new top::OverlapRemovalASG( (topConfig->doLargeJetOverlapRemoval() && topConfig->useLargeRJets())) );
    }
    return objectSelection;
}

}


