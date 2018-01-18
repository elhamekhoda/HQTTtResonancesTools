#include "HQTTtResonancesTools/MuonTtres.h"
#include "TopEvent/EventTools.h"
#include <cmath>

namespace top {

MuonTtres::MuonTtres(double ptcut, IsolationBase *isolation) :
    MuonMC15(ptcut, isolation)
{
}

bool MuonTtres::passSelection(const xAOD::Muon& mu) const
{

    if(MuonMC15::passSelection(mu)==false) return false;

    return true;
}

bool MuonTtres::passSelectionLoose(const xAOD::Muon& mu) const
{
  
    if(MuonMC15::passSelectionLoose(mu)==false) return false;
  
    return true;
}

}//namespace top

