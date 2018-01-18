#ifndef NLARGEJETTTRESLEPTONICSELECTOR_H_
#define NLARGEJETTTRESLEPTONICSELECTOR_H_

#include "TopEventSelectionTools/SignValueSelector.h"

namespace top {

/**
 * @brief Does some stuff
 */
class NLargeJetTtresLeptonicSelector : public SignValueSelector {
public:
    explicit NLargeJetTtresLeptonicSelector(const std::string& params);

    /**
     * @brief Write a description ;)
     *
     * @param event The event to perform the operations on.
     * @return True if the event passes the selection, false otherwise.
     */
     bool apply(const top::Event& event) const override;
};

}

#endif
