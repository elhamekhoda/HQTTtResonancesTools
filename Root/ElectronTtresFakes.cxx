#include "HQTTtResonancesTools/ElectronTtresFakes.h"
#include "TopEvent/EventTools.h"
//#include "TopObjectSelectionTools/ElectronCutBasedMC15.h"
#include "TopObjectSelectionTools/ElectronLikelihoodMC15.h"

namespace top {

ElectronTtresFakes::ElectronTtresFakes(double ptcut, bool vetoCrack, const std::string& quality, const std::string& qualityLoose, StandardIsolation* isolation) : 
    ElectronLikelihoodMC15(ptcut, vetoCrack, quality, qualityLoose, isolation)
{
}

bool ElectronTtresFakes::passSelection(const xAOD::Electron& el) const {

    if(ElectronLikelihoodMC15::passSelection(el)==false) return false;
    
    return true;
}

bool ElectronTtresFakes::passSelectionLoose(const xAOD::Electron& el) const {

    if (!passSelectionNoIsolation(el, m_operatingPointLoose_DF))
        return false;
		
    if (m_isolation && !m_isolation->passSelectionLoose(el))
        return false;

    return true;

}


bool ElectronTtresFakes::passSelectionNoIsolation(const xAOD::Electron& el,const std::string& operatingPoint) const {
      
    if (el.pt() < m_ptcut)
        return false;

    try {
      if (el.auxdataConst<int>(operatingPoint) != 1)
        return false;
    } catch(std::exception& e) {
      if (el.auxdataConst<char>(operatingPoint) != 1)
        return false;
    }

    //WARNING: Not all electrons keep clusters in the derivation
    //i.e. bad electrons (which is why we moved the check on the likelihood
    //before the check on the calo cluster)
    //This stops a crash
    //Good electrons should always have a cluster, if not then crash to warn us
    //Better than checking and silently doing nothing...
    if (std::fabs(el.caloCluster()->etaBE(2)) > 2.47)
        return false;

    if (m_vetoCrack && std::fabs(el.caloCluster()->etaBE(2)) > 1.37 && std::fabs(el.caloCluster()->etaBE(2)) < 1.52)
        return false;

    // Track-to-vertex association
    //if (!passTTVACuts(el))
    //  return false;

    return true;

}

}
