#ifndef NLARGEJETTTRESSTNOSJETSELECTOR_H_
#define NLARGEJETTTRESSTNOSJETSELECTOR_H_

#include "TopEventSelectionTools/SignValueSelector.h"
#include "BoostedJetTaggers/SubstructureTopTaggerHelpers.h"
namespace top {

/**
 * @brief Does some stuff
 */
class NLargeJetTtresSTNoSJetSelector : public SignValueSelector {
public:
    explicit NLargeJetTtresSTNoSJetSelector(const std::string& params);

    /**
     * @brief Write a description ;)
     *
     * @param event The event to perform the operations on.
     * @return True if the event passes the selection, false otherwise.
     */
     bool apply(const top::Event& event) const override;
   
     SubstructureTopTagger *STL;

};

}

#endif
