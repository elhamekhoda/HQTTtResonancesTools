#include "HQTTtResonancesTools/MuonTtresFakesInclusiveSd0.h"
#include "TopEvent/EventTools.h"
#include <cmath>

namespace top {

MuonTtresFakesInclusiveSd0::MuonTtresFakesInclusiveSd0(double ptcut, IsolationBase *isolation) :
    MuonMC15(ptcut, isolation)
{
}

bool MuonTtresFakesInclusiveSd0::passSelection(const xAOD::Muon& mu) const
{

	if (mu.pt() < m_ptcut)
		return false;

	///-- https://twiki.cern.ch/twiki/bin/view/AtlasProtected/MCPAnalysisGuidelinesMC15 --///
	if (!m_muonSelectionTool->accept(mu))
		return false;

	//isolation, if m_isolation != nullptr
	if (m_isolation && !m_isolation->passSelection(mu))
		return false;

	// // TTVA:
// 	// see https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TrackingCPEOYE2015#Track_to_Vertex_Association   
// 	if( !mu.isAvailable<float>("d0sig") ){
// 		std::cout << "d0 significance not found for muon. "
//  			<< "Maybe no primary vertex? Won't accept." << std::endl;
// 		return false;
// 	}
//  
// 	float d0sig = mu.auxdataConst<float>("d0sig");
// 	if( std::abs(d0sig) < 3 )
// 		return false;
 
	if( !mu.isAvailable<float>("delta_z0_sintheta") ){
		std::cout << "delta z0*sin(theta) not found for muon. "
			<< "Maybe no primary vertex? Won't accept." << std::endl;
		return false;
	}
 
	float delta_z0_sintheta = mu.auxdataConst<float>("delta_z0_sintheta");
	if( std::abs(delta_z0_sintheta) >= 0.5 )
		return false;

	return true;
	    
}//passSelection

bool MuonTtresFakesInclusiveSd0::passSelectionLoose(const xAOD::Muon& mu) const
{
   
	if (mu.pt() < m_ptcut)
		return false;

	///-- https://twiki.cern.ch/twiki/bin/view/AtlasProtected/MCPAnalysisGuidelinesMC15 --///
	if (!m_muonSelectionToolLoose->accept(mu))
		return false;

	//isolation, if m_isolation != nullptr
	if (m_isolation && !m_isolation->passSelectionLoose(mu))
		return false;

	// // TTVA:
// 	// see https://twiki.cern.ch/twiki/bin/view/AtlasProtected/TrackingCPEOYE2015#Track_to_Vertex_Association   
// 	if( !mu.isAvailable<float>("d0sig") ){
// 		std::cout << "d0 significance not found for muon. "
//  			<< "Maybe no primary vertex? Won't accept." << std::endl;
// 		return false;
// 	}
//  
// 	float d0sig = mu.auxdataConst<float>("d0sig");
// 	if( std::abs(d0sig) < 3 )
// 		return false;
 
	if( !mu.isAvailable<float>("delta_z0_sintheta") ){
		std::cout << "delta z0*sin(theta) not found for muon. "
			<< "Maybe no primary vertex? Won't accept." << std::endl;
		return false;
	}
 
	float delta_z0_sintheta = mu.auxdataConst<float>("delta_z0_sintheta");
	if( std::abs(delta_z0_sintheta) >= 0.5 )
		return false;

	return true;    
    
}//passSelectionLoose


}//namespace top

