/*
  Copyright (C) 2002-2017 CERN for the benefit of the ATLAS collaboration
*/

#include "TopEventSelectionTools/SignValueSelector.h"

namespace top {
    class TopConfig;

class TruthMttSelector : public SignValueSelector {
public:
    explicit TruthMttSelector(const std::string& params, std::shared_ptr<top::TopConfig> config);
    bool apply(const top::Event& event) const override;
    const std::string getFullCutName(){return SignValueSelector::valueStringDelimReplace(); };
private:
    bool m_isMC;
    std::vector<unsigned long> m_mcChannelNumbers;
};
}
