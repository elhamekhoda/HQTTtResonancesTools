#include "HQTTtResonancesTools/TrackMV2c10Selector.h"
#include "TopEvent/EventTools.h"

#include <algorithm>

namespace top {

TrackMV2c10Selector::TrackMV2c10Selector(const std::string& params) :
            SignValueSelector("TRACKMV2C10_N", params, true) {
    checkMultiplicityIsInteger();
}



bool TrackMV2c10Selector::apply(const top::Event& event) const {
    auto func = [&](const xAOD::Jet* const jetPtr) {
        double mv2c10_discriminant = 0.;
        const bool hasMv2c10 = jetPtr->btagging()->MVx_discriminant("MV2c10", mv2c10_discriminant);
        if (hasMv2c10 && jetPtr->pt() > 10e3 && std::fabs(jetPtr->eta()) < 2.5 && jetPtr->numConstituents() >= 2)
            return mv2c10_discriminant > value();

        return false;
    };

    auto count = std::count_if(event.m_trackJets.begin(), event.m_trackJets.end(), func);
    return checkInt(count, multiplicity());
}

}
