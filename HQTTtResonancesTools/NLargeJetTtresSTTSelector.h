#ifndef NLARGEJETTTRESSTTSELECTOR_H_
#define NLARGEJETTTRESSTTSELECTOR_H_

#include "TopEventSelectionTools/SignValueSelector.h"
#include "BoostedJetTaggers/SubstructureTopTaggerHelpers.h"
namespace top {

/**
 * @brief Does some stuff
 */
class NLargeJetTtresSTTSelector : public SignValueSelector {
public:
    explicit NLargeJetTtresSTTSelector(const std::string& params);

    /**
     * @brief Write a description ;)
     *
     * @param event The event to perform the operations on.
     * @return True if the event passes the selection, false otherwise.
     */
     bool apply(const top::Event& event) const override;

     SubstructureTopTagger *STT;
};

}

#endif
