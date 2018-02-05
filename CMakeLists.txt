# Written by Elham: 2017-28-08 

# Declare the name of this package:
atlas_subdir( HQTTtResonancesTools None )
         
# This package depends on other packages:
atlas_depends_on_subdirs( PUBLIC
                          PhysicsAnalysis/TopPhys/xAOD/TopObjectSelectionTools
                          PhysicsAnalysis/TopPhys/xAOD/TopEventSelectionTools
                          TopConfiguration 
                          TopEvent
                          TopAnalysis
                          xAODJet
                          xAODEgamma
                          xAODTau
                          xAODMuon
                          xAODCore
                          xAODTruth
                          xAODRootAccess
                          PhysicsAnalysis/AnalysisCommon/AssociationUtils
                          AsgTools
                          TopPartons
                          TopEventReconstructionTools
                          TtResonancesTools
                          PhysicsAnalysis/JetTagging/JetTagPerformanceCalibration/xAODBTaggingEfficiency
                          BoostedJetTaggers 
                          PhysicsAnalysis/MuonID/MuonSelectorTools/MuonSelectorTools/MuonSelectorTools
                          JetRec
                          Reconstruction/Jet/JetCalibTools 
                          Tools/PathResolver)

# This package uses ROOT:
find_package( ROOT REQUIRED COMPONENTS Core Gpad Tree Hist RIO MathCore Graf )


# Custom definitions needed for this package:
add_definitions( -g )

# Generate a CINT dictionary source file:
atlas_add_root_dictionary( HQTTtResonancesTools _cintDictSource
                           ROOT_HEADERS HQTTtResonancesTools/*.h Root/LinkDef.h
                           EXTERNAL_PACKAGES ROOT )

# Build a library that other components can link against:
atlas_add_library( HQTTtResonancesTools Root/*.cxx Root/*.h Root/*.icc
                   HQTTtResonancesTools/*.h HQTTtResonancesTools/*.icc HQTTtResonancesTools/*/*.h
                   HQTTtResonancesTools/*/*.icc ${_cintDictSource}
                   PUBLIC_HEADERS HQTTtResonancesTools
                   LINK_LIBRARIES TopObjectSelectionTools
                                  TopEventSelectionTools
                                  TopConfiguration 
                                  TopEvent
                                  TopAnalysis
                                  xAODJet
                                  xAODEgamma
                                  xAODTau
                                  xAODMuon
                                  xAODCore
                                  xAODTruth
                                  xAODRootAccess
                                  AssociationUtilsLib
                                  AsgTools
                                  TopPartons
                                  TopEventReconstructionTools
                                  TtResonancesTools
                                  xAODBTaggingEfficiencyLib
                                  BoostedJetTaggersLib
                                  MuonSelectorToolsLib
                                  JetRecLib
                                  JetCalibToolsLib
                                  PathResolver
                                  ${ROOT_LIBRARIES}
                   INCLUDE_DIRS ${ROOT_INCLUDE_DIRS} )

# Install data files from the package:
atlas_install_data( share/* )

# Install the Python modules of the package:
atlas_install_python_modules( python/*.py )
