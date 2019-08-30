# Written by Elham: 2017-28-08 

# Declare the name of this package:
atlas_subdir( HQTTtResonancesTools None )
         
# This package depends on other packages:
atlas_depends_on_subdirs( PUBLIC
                          # AnalysisTop
                          TopCPTools
                          TopObjectSelectionTools
                          TopEventSelectionTools
                          TopConfiguration 
                          TopEvent
                          TopAnalysis
                          TopEventReconstructionTools
                          TopPartons
                          # xAOD Objects
                          xAODJet
                          xAODEgamma
                          xAODTau
                          xAODMuon
                          xAODCore
                          xAODTruth
                          xAODRootAccess
                          # CP Tools
                          JetRec
                          BoostedJetTaggers 
                          Reconstruction/Jet/JetCalibTools
                          PhysicsAnalysis/AnalysisCommon/AssociationUtils
                          PhysicsAnalysis/AnalysisCommon/ParticleJetTools
                          PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/xAODBTaggingEfficiency
                          PhysicsAnalysis/MuonID/MuonSelectorTools/MuonSelectorTools/MuonSelectorTools
                          # TtResonances
                          TtResonancesTools
                          ZprimeRWGT
                          # Basic
                          AsgTools
                          PRIVATE
                          Tools/PathResolver)

# This package uses ROOT:
find_package( ROOT REQUIRED COMPONENTS Core Gpad Tree Hist RIO MathCore Graf )

# Custom definitions needed for this package:
add_definitions( -g )

# Optinal Libraries
list(APPEND OPT_LIBRARIES)

# Configurable Options
option(ENABLE_LJETSUBSTRUCTURE_DEBUG "This enables jet substructures debug outputs" OFF)
option(ENABLE_BTAG_DEBUG "This enables b-tagging debug outputs" OFF)

if( EXISTS ${CMAKE_SOURCE_DIR}/ZprimeRWGT/CMakeLists.txt )
	set(ENABLE_ZPRIMERWGT_DEFAULT ON)
else()
	set(ENABLE_ZPRIMERWGT_DEFAULT OFF)
endif()
option(ENABLE_ZPRIMERWGT "This enables SSM Z' to HVT Z' reweighting" ${ENABLE_ZPRIMERWGT_DEFAULT})

string(ASCII 27 ESC)
if (ENABLE_LJETSUBSTRUCTURE_DEBUG)
  message(STATUS "${ESC}[36;1mEnables jet substructure debug variables${ESC}[m")
  add_definitions(-D ENABLE_LJETSUBSTRUCTURE_DEBUG)
else()
  message(STATUS "${ESC}[33;1mDisables jet substructure debug variables${ESC}[m")
endif()

if (ENABLE_BTAG_DEBUG)
  message(STATUS "${ESC}[36;1mEnables b-tagging debug variables${ESC}[m")
  add_definitions(-D ENABLE_BTAG_DEBUG)
else()
  message(STATUS "${ESC}[33;1mDisables b-tagging debug variables${ESC}[m")
endif()

if (ENABLE_ZPRIMERWGT)
  message(STATUS "${ESC}[36;1mEnables SSM Z' to HVT Z' reweighting${ESC}[m")
  add_definitions(-D ENABLE_ZPRIMERWGT)
  list(APPEND OPT_LIBRARIES ZprimeRWGT)
else()
  message(STATUS "${ESC}[33;1mDisables SSM Z' to HVT Z' reweighting${ESC}[m")
endif()

# Generate a CINT dictionary source file:
atlas_add_root_dictionary( HQTTtResonancesTools _cintDictSource
                           ROOT_HEADERS HQTTtResonancesTools/*.h Root/LinkDef.h
                           EXTERNAL_PACKAGES ROOT )

# Build a library that other components can link against:
atlas_add_library( HQTTtResonancesTools Root/*.cxx Root/*.h Root/*.icc
                   HQTTtResonancesTools/*.h HQTTtResonancesTools/*.icc HQTTtResonancesTools/*/*.h
                   HQTTtResonancesTools/*/*.icc ${_cintDictSource}
                   PUBLIC_HEADERS HQTTtResonancesTools
                   LINK_LIBRARIES # AnalysisTop
                                  TopCPTools
                                  TopObjectSelectionTools
                                  TopEventSelectionTools
                                  TopConfiguration 
                                  TopEvent
                                  TopEventReconstructionTools
                                  TopPartons
                                  TopAnalysis
                                  # xAOD Objects
                                  xAODJet
                                  xAODEgamma
                                  xAODTau
                                  xAODMuon
                                  xAODCore
                                  xAODTruth
                                  xAODRootAccess
                                  # CP Tools
                                  AssociationUtilsLib
                                  xAODBTaggingEfficiencyLib
                                  MuonSelectorToolsLib
                                  BoostedJetTaggersLib
                                  JetRecLib
                                  JetCalibToolsLib
                                  ParticleJetToolsLib
                                  # TtResonances
                                  TtResonancesTools
                                  # Basic
                                  AsgTools
                                  PathResolver
                                  ${ROOT_LIBRARIES}
                                  ${OPT_LIBRARIES}
                   INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} )

# Install data files from the package:
atlas_install_data( share/* )

# Install the Python modules of the package:
atlas_install_python_modules( python/*.py )