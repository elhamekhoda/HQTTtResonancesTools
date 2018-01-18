#include "HQTTtResonancesTools/MuonTtresFakes.h"
#include "TopEvent/EventTools.h"
#include <cmath>

namespace top {

MuonTtresFakes::MuonTtresFakes(double ptcut, IsolationBase *isolation) :
    MuonMC15(ptcut, isolation)
{
}

bool MuonTtresFakes::passSelection(const xAOD::Muon& mu) const
{

	if(MuonMC15::passSelection(mu)==false) return false;

	return true;
	    
}//passSelection

bool MuonTtresFakes::passSelectionLoose(const xAOD::Muon& mu) const
{
   
	if (mu.pt() < m_ptcut)
		return false;

	///-- https://twiki.cern.ch/twiki/bin/view/AtlasProtected/MCPAnalysisGuidelinesMC15 --///
	if (!m_muonSelectionToolLoose->accept(mu))
		return false;

	//isolation, if m_isolation != nullptr
	if (m_isolation && !m_isolation->passSelectionLoose(mu))
		return false;

	return true;    
    
}//passSelectionLoose


}//namespace top

