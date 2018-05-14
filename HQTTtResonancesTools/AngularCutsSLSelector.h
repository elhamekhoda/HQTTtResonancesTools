#ifndef ANGULARCUTSSLSELECTOR_H_
#define ANGULARCUTSSLSELECTOR_H_

#include "TopEventSelectionTools/SignValueSelector.h"
#include "HQTTtResonancesTools/AngularCutsSL.h"

namespace top {

/**
 * @brief Does some stuff
 */
class AngularCutsSLSelector : public SignValueSelector {
public:
    explicit AngularCutsSLSelector(const std::string& params);

    /**
    * @brief Write a description ;)
    *
    * @param event The event to perform the operations on.
    * @return True if the event passes the selection, false otherwise.
    */
    bool apply(const top::Event& event) const override;
    AngularCutsSL *m_AngularCutsSL;
};

}

#endif
