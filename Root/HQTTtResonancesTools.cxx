#include "HQTTtResonancesTools/HQTTtResonancesToolsLoader.h"
#include "HQTTtResonancesTools/NLargeJetTtresSTSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSTmtSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresMassOnlySelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresST50NoLeptonSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresST80NoLeptonSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSTSimpleSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSTTau32Selector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSTMassSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSTWjetsSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSTTSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSTNoKinSelector.h"
#include "HQTTtResonancesTools/TrackMV2c20Selector.h"
#include "HQTTtResonancesTools/TrackMV2c10Selector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSLSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresSTNoSJetSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresLeptonicSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresAHSelector.h"
#include "HQTTtResonancesTools/NLargeJetTtresFakesSelector.h"
#include "HQTTtResonancesTools/NJetCloseLeptonSelector.h"
#include "HQTTtResonancesTools/NLargeJetVHresSelector.h"
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
    else if (toolname == "LARGEJETTTRESSTNOSJET_N")
        return new top::NLargeJetTtresSTNoSJetSelector(param);
    else if (toolname == "LARGEJETTTRESSL_N")
        return new top::NLargeJetTtresSLSelector(param);
    else if (toolname == "LARGEJETTTRESAH_N")
        return new top::NLargeJetTtresAHSelector(param);
    else if (toolname == "JETCLOSELEPTON_N")
        return new top::NJetCloseLeptonSelector(param);
    else if (toolname == "LARGEJETVHRES_N")
        return new top::NLargeJetVHresSelector(param);
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
    else if (toolname == "LARGEJETTTRESSTSIMPLE_N")
        return new top::NLargeJetTtresSTSimpleSelector(param);
    else if (toolname == "LARGEJETTTRESSTT_N")
        return new top::NLargeJetTtresSTTSelector(param);
    else if (toolname == "LARGEJETTTRESFAKES_N")
        return new top::NLargeJetTtresFakesSelector(param);	
    else if (toolname == "LARGEJETTTRESSTWJETS_N")
        return new top::NLargeJetTtresSTWjetsSelector(param);
    else if (toolname == "LARGEJETTTRESSTTAU32_N")
        return new top::NLargeJetTtresSTTau32Selector(param);
    else if (toolname == "LARGEJETTTRESSTMASS_N")
        return new top::NLargeJetTtresSTMassSelector(param);
    else if (toolname == "LARGEJETTTRESSTNOKIN_N")
        return new top::NLargeJetTtresSTNoKinSelector(param);
    else if (toolname == "LARGEJETTTRESST50NOLEPTON_N")
        return new top::NLargeJetTtresST50NoLeptonSelector(param);
    else if (toolname == "LARGEJETTTRESST80NOLEPTON_N")
        return new top::NLargeJetTtresST80NoLeptonSelector(param);
    else if (toolname == "ANGULARCUTSSL")
        return new top::AngularCutsSLSelector(param);
    else if (toolname == "LARGEJETTTRESSTMT_N")
        return new top::NLargeJetTtresSTmtSelector(param);




    return nullptr;
}

}
