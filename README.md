# HQTTtResonancesTools

This Tool is developed to fulfill the purpose of the exotics ttbar resonances search in lepton+jets and all-hadronic channel.

## Current Version:

**AanalysisTop, 21.2.155** 
Make sure you setup AnalysisBase instead of AnalysisTop for release >=21.2.99

## How to setup:

1.  `setupATLAS` first of course
    ```bash
    setupATLAS
    ```

2.  make sure you can acess gitlab repositories properly.
    ```bash
    lsetup git
    kinit $CERN_USER@CERN.CH
    # If you have trouble using "kinit" then use https or ssh protocol for checking out the relevant tools. Instruction in step (4).
    ```

3.  acmSetup
    ```bash
    mkdir MyProject
    cd MyProject
    mkdir build source run
    cd build
    acmSetup --sourcedir=../source AnalysisBase,21.2.155
    ```
    **NOTE:** Next time when you come back, simply do `cd ./build/; acmSetup;`. If you've made some change, do `acm find_packages; acm compile;` as well.


4.  add __Pre-recommended top tagging tools__ and some other necessary packages and compile
    ```bash
    acm clone_project atlas-phys/exot/hqt/R21-ttbar-1lep/TtResonancesTools
    acm clone_project atlas-phys/exot/hqt/R21-ttbar-1lep/HQTTtResonancesTools
    acm find_packages
    acm compile
    ```
    **NOTE:** Trouble in using the above mentioned commands? Use the https/ssh urls.  
   Change `$ACM_GITLAB_PATH_PREFIX` before the above command:  
    **HTTPS Protocol:**
    ```bash
    export ACM_GITLAB_PATH_PREFIX=https://gitlab.cern.ch/
    ```
    **SSH Protocol:**
    ```bash
    export ACM_GITLAB_PATH_PREFIX=ssh://git@gitlab.cern.ch:7999/
    ```
    `top-xaod` is now available in your console.
5.  `cd $TestArea/../run/` and run it with `top-xaod <selections-file> <inputs-file>`  
    As an example:
    ```bash
    cd $TestArea/../run/
    cp $WorkDir_DIR/data/HQTTtResonancesTools/ttres-ljets-cuts-mc16-25ns.txt .
    echo "root://xrootd.echo.stfc.ac.uk:1094/atlas:datadisk/rucio/mc16_13TeV/db/10/DAOD_EXOT4.12719772._000001.pool.root.1" > input.txt
    top-xaod ttres-ljets-cuts-mc16-25ns.txt input.txt
    ```
    , in which [ttres-ljets-cuts-mc16-25ns.txt] is a `<selections-file>` and `input.txt` is a `<inputs-file>` listing all the input datasets separated by new line.  
    Here what we use is a sub-dataset of  
[mc16_13TeV.301333.Pythia8EvtGen_A14NNPDF23LO_zprime3000_tt.deriv.DAOD_EXOT4.e3723_s3126_r9364_r9315_p3390].  

6.  You can also submit your job to grid. Here is an example of generating all the MC16a Zprime signal samples with individual masses between 400GeV and 5000GeV.
    ```python
    #!/usr/bin/env python
    import getpass
    import os
    import TopExamples.grid
    import HQTTtResonancesTools.MC16a_EXOT4
    # Configuration
    config = TopExamples.grid.Config()
    config.CMake         = True
    config.code          = 'top-xaod'
    config.settingsFile  = 'ttres-ljets-cuts-mc16-25ns.txt'
    config.gridUsername  = os.environ["CERN_USER"]
    config.suffix        = '13022018v1'
    config.excludedSites = 'ANALY_SCINET'
    config.noSubmit      = False
    config.mergeType     = 'None' #'None', 'Default' or 'xAOD'
    config.memory        = '1000'
    config.destSE        = 'DESY-HH_LOCALGROUPDISK'
    config.maxNFilesPerJob  = '5'
    config.checkPRW      = True
    # Input Samples
    names   = []
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime400']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime500']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime750']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime1000']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime1250']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime1500']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime1750']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime2000']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime2250']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime2500']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime2750']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime3000']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime4000']
    names  += ['MC16a_13TeV_25ns_FS_EXOT4_Zprime5000']
    samples = TopExamples.grid.Samples(names)
    TopExamples.grid.submit(config, samples)
    ```
    Save this as `$TestArea/../run/submitToGrid-25ns-207.py` and similarly do (don't forget to `lsetup pyami rucio panda` first):
    ```bash
    cd $TestArea/../run/
    cp $WorkDir_DIR/data/HQTTtResonancesTools/ttres-ljets-cuts-mc16-25ns.txt .
    python submitToGrid-25ns-207.py
    ```

[ttres-ljets-cuts-mc16-25ns.txt]: https://gitlab.cern.ch/atlas-phys/exot/hqt/R21-ttbar-1lep/HQTTtResonancesTools/blob/master/share/ttres-ljets-cuts-mc16-25ns.txt
[mc16_13TeV.301333.Pythia8EvtGen_A14NNPDF23LO_zprime3000_tt.deriv.DAOD_EXOT4.e3723_s3126_r9364_r9315_p3565]: https://ami.in2p3.fr/AMI/servlet/net.hep.atlas.Database.Bookkeeping.AMI.Servlet.Command?Command=FormGetElementInfo+-GUI%3D%22yes%22+-amiAdvanced%3D%22ON%22+-amiLang%3D%22english%22+-elementID%3D%22320304%22+-entityName%3D%22dataset%22+-processingStep%3D%22production%22+-project%3D%22mc16_001%22+-uimode%3D%22bootstrap%22&Converter=%2Fxsl%2FAMIProdFrameHTML.xsl&Valider=Execute&Type=AtlasProduction
[TopNtupleAnalysis]: https://gitlab.cern.ch/atlas-phys/exot/hqt/R21-ttbar-1lep/TopNtupleAnalysis
[TtResonancesTools]: https://gitlab.cern.ch/atlas-phys/exot/hqt/R21-ttbar-1lep/TtResonancesTools
[TopDataPreparation]: https://gitlab.cern.ch/atlas/athena/tree/21.2/PhysicsAnalysis/TopPhys/TopPhysUtils/TopDataPreparation