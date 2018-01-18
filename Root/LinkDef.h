#include "HQTTtResonancesTools/ObjectLoaderTtres.h"
#include "HQTTtResonancesTools/TtresEventSaverFlatNtuple.h"
#include "HQTTtResonancesTools/HQTTtResonancesToolsLoader.h"
#include "HQTTtResonancesTools/TtbarPartonHistorySaver.h"

#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ nestedclass;

//for loading the object selection at run time
#pragma link C++ class top::ObjectLoaderTtres+;
#pragma link C++ class top::HQTTtResonancesToolsLoader+;
#pragma link C++ class top::TtresEventSaverFlatNtuple+;
#pragma link C++ class top::TtbarPartonHistorySaver+;

#endif
