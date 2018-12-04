/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/
#include <string>
#include <algorithm>
#include "HQTTtResonancesTools/TruthMttSelector.h"
#include "TopConfiguration/TopConfig.h"
#include "TopConfiguration/Tokenize.h"
#include "TopEvent/EventTools.h"

namespace top {

TruthMttSelector::TruthMttSelector(const std::string& params, std::shared_ptr<top::TopConfig> config) :
    SignValueSelector("TRUTH_TTBARMASS_SELECTOR", params, true, true) {
        m_isMC = config->isMC();
        // std::cout << "doTopPartonHistory? " << config->doTopPartonHistory() << "\n";
        if ( m_isMC && !config->doTopPartonHistory() ) {
            std::cout << "You need to activate `doTopPartonHistory` in `TopConfig` in order to use this tool!\n";
            exit(1);
        }
        std::vector<std::string> tokens;
        tokenize(valueString(), tokens, ",");

        for ( auto cutString: tokens ){
            m_mcChannelNumbers.push_back(std::stoul(cutString, nullptr, 0));
        }
        if ( m_mcChannelNumbers.empty() ) {
            std::cout << "You need to provide a comma separated MC channel number list!\n";
            exit(1);
        }
}

bool TruthMttSelector::apply(const top::Event& event) const {
    unsigned long MCchannel_number = 0;
    if ( !m_isMC ) {
        return true;
    } else {
        MCchannel_number = event.m_info->mcChannelNumber();
        if ( std::find(m_mcChannelNumbers.begin(), m_mcChannelNumbers.end(), MCchannel_number) == m_mcChannelNumbers.end() ) { // This is not a selected ttbar event
            return true;
        } else { // This is a selected ttbar event
            if ( event.m_partonHistory->isAvailable<float>("MC_ttbar_beforeFSR_m") ) {
                bool passed = checkFloat(event.m_partonHistory->auxdata<float>("MC_ttbar_beforeFSR_m"), multiplicity());
                // std::cout << "mtt Cut:" << multiplicity() << "\n";
                // std::cout << "MC_ttbar_beforeFSR_m: " << event.m_partonHistory->auxdata<float>("MC_ttbar_beforeFSR_m") << "\n";
                // std::cout << "pass? " << passed << "\n";
                return passed;
            } else {
                return true;
            }

        }
    }
}

}