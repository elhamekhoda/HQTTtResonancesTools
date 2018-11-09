#ifndef NLARGEJETANYTOPTAGSELECTOR_H_
#define NLARGEJETANYTOPTAGSELECTOR_H_


#include "TopEventSelectionTools/SignValueSelector.h"

namespace top {

/**
This is an DNN shape-based top-tagger derived by full-hadronic ttbar resonances searches with a sensitivity-based optimization method.
 */
class NLargeJetAnyTopTagSelector : public SignValueSelector {
public:
    explicit NLargeJetAnyTopTagSelector(const std::string& params);

     bool apply(const top::Event& event) const override;

};

}

#endif
