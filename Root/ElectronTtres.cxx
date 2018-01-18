#include "HQTTtResonancesTools/ElectronTtres.h"
#include "TopEvent/EventTools.h"
//#include "TopObjectSelectionTools/ElectronCutBasedMC15.h"
#include "TopObjectSelectionTools/ElectronLikelihoodMC15.h"

namespace top {

ElectronTtres::ElectronTtres(double ptcut, bool vetoCrack, const std::string& quality, const std::string& qualityLoose, StandardIsolation* isolation) : 
    ElectronLikelihoodMC15(ptcut, vetoCrack, quality, qualityLoose, isolation)
{
}

bool ElectronTtres::passSelection(const xAOD::Electron& el) const {

    if(ElectronLikelihoodMC15::passSelection(el)==false) return false;

    return true;
}

bool ElectronTtres::passSelectionLoose(const xAOD::Electron& el) const {

    if(ElectronLikelihoodMC15::passSelectionLoose(el)==false) return false;

    return true;

}

}
