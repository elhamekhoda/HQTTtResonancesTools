#include "HQTTtResonancesTools/HQTTtResonancesToolsLoader.h"
#include "HQTTtResonancesTools/NLargeJetTtresSmoothedTTMassTau32Selector.h"
#include "HQTTtResonancesTools/NLargeJetTtresMassOnlySelector.h"
#include "HQTTtResonancesTools/TrackMV2c20Selector.h"
#include "HQTTtResonancesTools/TrackMV2c10Selector.h"
#include "HQTTtResonancesTools/NLargeJetTtresLeptonicSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresFakesSelector.h"
#include "HQTTtResonancesTools/NJetCloseLeptonSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresFHDNNTopTagSelector.h"
#include "HQTTtResonancesTools/NLargeJetAnyTopTagSelector.h"
#include "HQTTtResonancesTools/AngularCutsSLSelector.h"
#include "HQTTtResonancesTools/TruthMttSelector.h"
#include "TtResonancesTools/Chi2Selector.h"

#include <iostream>
#include <regex>
#include "TFile.h"

namespace top {

top::EventSelectorBase* HQTTtResonancesToolsLoader::initTool(const std::string& /*name*/, const std::string& line, TFile* /*outputFile*/, std::shared_ptr<top::TopConfig> config, EL::Worker* /*wk*/) {
    //get the first bit of the string and store it in toolname
    std::istringstream iss(line);
    std::string toolname;
    getline(iss, toolname, ' ');

    //any parameters?
    std::string param;
    if (line.size() > toolname.size())
        param = line.substr(toolname.size() + 1);

    if (toolname == "LARGEJETTTRESLEPTONIC_N")
        return new top::NLargeJetTtresLeptonicSelector(param);
    else if (toolname == "JETCLOSELEPTON_N")
        return new top::NJetCloseLeptonSelector(param);
    else if (toolname == "LOG10_CHI2")
        return new top::Chi2Selector(param);
    else if (toolname == "TRACKMV2C20_N")
        return new top::TrackMV2c20Selector(param);
    else if (toolname == "TRACKMV2C10_N")
        return new top::TrackMV2c10Selector(param);
    else if (toolname == "LARGEJETTTRESMASSONLY_N")
        return new top::NLargeJetTtresMassOnlySelector(param);
    else if (toolname == "LARGEJETTTRESFAKES_N")
        return new top::NLargeJetTtresFakesSelector(param);
    else if (toolname == "ANGULARCUTSSL")
        return new top::AngularCutsSLSelector(param);
    else if (toolname == "LARGEJETTTRESSTTMT_N")
        return new top::NLargeJetTtresSmoothedTTMassTau32Selector(param);
    else if (toolname == "LJET_DNNTOPTAG_CONTAINED80_N")
        return new top::NLargeJetTtresFHDNNTopTagSelector(param, "Contained80", config);
    else if (toolname == "LJET_DNNTOPTAG_CONTAINED50_N")
        return new top::NLargeJetTtresFHDNNTopTagSelector(param, "Contained50", config);
    else if (toolname == "LJET_DNNTOPTAG_INCLUSIVE80_N")
        return new top::NLargeJetTtresFHDNNTopTagSelector(param, "Inclusive80", config);
    else if (toolname == "LJET_DNNTOPTAG_INCLUSIVE50_N")
        return new top::NLargeJetTtresFHDNNTopTagSelector(param, "Inclusive50", config);
    else if (toolname == "LJET_DNNTOPTAG_TTRES0L0B_N")
        return new top::NLargeJetTtresFHDNNTopTagSelector(param, "ContainedTtres0L0B", config);
    else if (toolname == "LJET_DNNTOPTAG_TTRES0L1B_N")
        return new top::NLargeJetTtresFHDNNTopTagSelector(param, "ContainedTtres0L1B", config);
    else if (toolname == "LJET_DNNTOPTAG_TTRES0L2B_N")
        return new top::NLargeJetTtresFHDNNTopTagSelector(param, "ContainedTtres0L2B", config);
    else if (toolname.find("LJET_DNNTOPTAG_TTRES1L")==0) {
        std::smatch sm;
        if ( std::regex_match(toolname, sm, std::regex("LJET_DNNTOPTAG_TTRES1L(\\d+)EFF_N")) ) {
            return new top::NLargeJetTtresFHDNNTopTagSelector(param, "ContainedTtres1L" + sm[1].str() + "Eff", config);
        }
    }
    else if (toolname == "LJET_ANYTOPTAG_N")
        return new top::NLargeJetAnyTopTagSelector(param);
    else if (toolname == "TRUTH_TTBARMASS")
        return new top::TruthMttSelector(param, config);

    return nullptr;
}

}
