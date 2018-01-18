#include "HQTTtResonancesTools/TrackMV2c20Selector.h"
#include "TopEvent/EventTools.h"

#include <algorithm>

namespace top {

TrackMV2c20Selector::TrackMV2c20Selector(const std::string& params) :
            SignValueSelector("TRACKMV2C20_N", params, true) {
    checkMultiplicityIsInteger();
}

bool TrackMV2c20Selector::apply(const top::Event& event) const {
    auto func = [&](const xAOD::Jet* const jetPtr) {
        double mv2c20_discriminant = 0.;
        const bool hasMv2c20 = jetPtr->btagging()->MVx_discriminant("MV2c20", mv2c20_discriminant);
        if (hasMv2c20 && jetPtr->pt() > 10e3 && std::fabs(jetPtr->eta()) < 2.5 && jetPtr->numConstituents() >= 2)
            return mv2c20_discriminant > value();

        return false;
    };

    auto count = std::count_if(event.m_trackJets.begin(), event.m_trackJets.end(), func);
    return checkInt(count, multiplicity());
}

}
