#ifndef NLARGEJETTTRESFHDNNTOPTAGSELECTOR_H_
#define NLARGEJETTTRESFHDNNTOPTAGSELECTOR_H_

#include "TopEventSelectionTools/SignValueSelector.h"
#include "BoostedJetTaggers/JSSWTopTaggerDNN.h"
namespace top {

/**
This is an DNN shape-based top-tagger derived by full-hadronic ttbar resonances searches with a sensitivity-based optimization method.
 */
class NLargeJetTtresFHDNNTopTagSelector : public SignValueSelector {
public:
    explicit NLargeJetTtresFHDNNTopTagSelector(const std::string& params, const std::string& region);

     bool apply(const top::Event& event) const override;

     std::unique_ptr<JSSWTopTaggerDNN> m_TopTaggerDNN = nullptr;
     std::string m_region; 
};

}

#endif
