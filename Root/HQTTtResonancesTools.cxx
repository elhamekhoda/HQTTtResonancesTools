#include "HQTTtResonancesTools/HQTTtResonancesToolsLoader.h"
#include "HQTTtResonancesTools/NLargeJetTtresSTSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSmoothedTTMassTau32Selector.h"
#include "HQTTtResonancesTools/NLargeJetTtresMassOnlySelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSubstructureTopTag50Selector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSubstructureTopTag80Selector.h"
#include "HQTTtResonancesTools/TrackMV2c20Selector.h"
#include "HQTTtResonancesTools/TrackMV2c10Selector.h"
#include "HQTTtResonancesTools/NLargeJetTtresLeptonicSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresFakesSelector.h"
#include "HQTTtResonancesTools/NJetCloseLeptonSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresFHDNNTopTagSelector.h"
#include "HQTTtResonancesTools/NLargeJetAnyTopTagSelector.h"
#include "HQTTtResonancesTools/AngularCutsSLSelector.h"
#include "TtResonancesTools/Chi2Selector.h"

#include <iostream>

#include "TFile.h"

namespace top {

top::EventSelectorBase* HQTTtResonancesToolsLoader::initTool(const std::string& /*name*/, const std::string& line, TFile* /*outputFile*/, std::shared_ptr<top::TopConfig> /*config*/, EL::Worker* /*wk*/) {
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
    else if (toolname == "LARGEJETTTRESST_N")
        return new top::NLargeJetTtresSTSelector(param);
    else if (toolname == "LARGEJETTTRESMASSONLY_N")
        return new top::NLargeJetTtresMassOnlySelector(param);
    else if (toolname == "LARGEJETTTRESFAKES_N")
        return new top::NLargeJetTtresFakesSelector(param);
    else if (toolname == "LARGEJETTTRESSUBTOPTAG50_N")
        return new top::NLargeJetTtresSubstructureTopTag50Selector(param);
    else if (toolname == "LARGEJETTTRESSUBTOPTAG80_N")
        return new top::NLargeJetTtresSubstructureTopTag80Selector(param);
    else if (toolname == "ANGULARCUTSSL")
        return new top::AngularCutsSLSelector(param);
    else if (toolname == "LARGEJETTTRESSTTMT_N")
        return new top::NLargeJetTtresSmoothedTTMassTau32Selector(param);
    else if (toolname == "LARGEJETTTRES0L0BDNNTOPTAG_N")
        return new top::NLargeJetTtresFHDNNTopTagSelector(param, "0L0B");
    else if (toolname == "LARGEJETTTRES0L1BDNNTOPTAG_N")
        return new top::NLargeJetTtresFHDNNTopTagSelector(param, "0L1B");
    else if (toolname == "LARGEJETTTRES0L2BDNNTOPTAG_N")
        return new top::NLargeJetTtresFHDNNTopTagSelector(param, "0L2B");
    else if (toolname == "NLARGEJETANYTOPTAG_N")
        return new top::NLargeJetAnyTopTagSelector(param);

    return nullptr;
}

}
