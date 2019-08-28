import os
import sys
import subprocess
from itertools import islice
import ROOT
from TopExamples.grid import *
from HQTTtResonancesTools import clusters
import logging
# logging.captureWarnings(True)
msgfmt = '%(asctime)s %(levelname)-7s %(name)-35s %(message)s'
datefmt = '%H:%M:%S'

def getLogger(name = None, level = logging.DEBUG):
    mlogger = logging.getLogger(name)
    try:
        import coloredlogs
        coloredlogs.install(logger = mlogger, level = level, fmt = msgfmt, datefmt = datefmt)
    except ImportError:
        logging.basicConfig(format = msgfmt, datefmt = datefmt)
        mlogger.setLevel(level)
    return mlogger
mlogger = getLogger('top-xaod')

def get_fct(logger = 'logging'):
    if logger == 'logging':
        logger = mlogger
    if logger is None:
        def fct(idle, run, finish):
            l = "Idle: {:3},  Running: {:3},  Completed: {:3}".format(idle, run, finish)
            print l
    else:
        def fct(idle, run, finish):
            l = "Idle: {:3},  Running: {:3},  Completed: {:3}".format(idle, run, finish)
            logger.info(l)
    return fct

def create_executable(fname = 'top-xaod.sh'):
    lines = '''#!{SHELL}
pwd="$PWD"
export ATLAS_LOCAL_ROOT_BASE=/cvmfs/atlas.cern.ch/repo/ATLASLocalRootBase
source ${{ATLAS_LOCAL_ROOT_BASE}}/user/atlasLocalSetup.sh
export X509_USER_PROXY=$HOME/.globus/job_proxy.pem
cd {TestArea}
acmSetup
cd $pwd
top-xaod $1 $2'''.format(SHELL = os.getenv('SHELL'), TestArea = os.getenv("TestArea"))
    with open(fname, 'w') as f:
        f.write(lines)
    subprocess.check_call(['chmod', 'u+x', fname])
    return fname

def submit_to_cluster(config, allSamples, to_dir = None, cluster = 'condor', max_runtime = 3600*3, wait = True):
    if isinstance(cluster, basestring):
        cluster = clusters.from_name[cluster](cluster_type = None, cluster_status_update=(600,30), cluster_queue='None')
    # cluster.cluster_queue = '( OpSysAndVer == "CentOS7")'
    cluster.max_runtime = max_runtime
    checkMergeType(config)
    config.details()
    if not config.skipShowerCheck:
        for configFile in config.settingsFile.split(','):
            checkForShowerAlgorithm(allSamples, configFile)
    if config.checkPRW:
        for configFile in config.settingsFile.split(','):
            checkPRWFile(allSamples, configFile)

    tarfile = 'top-xaod.tar.gz'

    # Delete the old tarball if requested
    if not config.reuseTarBall:
        try:
            os.remove(tarfile)
        except OSError, e:
            #Number 2 is 'file doesn't exist' which is okay for us
            if e.errno == 2:
                pass
            else:
                raise


    #Check for cuts file
    for configFile in config.settingsFile.split(','):
        if not checkForFile(configFile):
            print logger.FAIL    + " Error - %s does not exist in this directory "%(configFile) + logger.ENDC
            print logger.WARNING + "       - Attempt to find this file in a sensible location... " + logger.ENDC
            settingsFilePath = ROOT.PathResolver.find_file(configFile, "DATAPATH", ROOT.PathResolver.RecursiveSearch)
            if settingsFilePath == "":
                print logger.FAIL + "DANGER DANGER. HIGH VOLTAGE" + logger.ENDC
                print '%s does not exist in this directory and cannot be found' % configFile
                print 'Please make it before submitting'
                sys.exit(1)
            else:
                print logger.WARNING + "       - Found an appropriate file " + logger.ENDC
                print logger.WARNING + "       - Will copy " + logger.ENDC + configFile + logger.WARNING + " from " + logger.ENDC + settingsFilePath
                print logger.WARNING + "       - Confirm this is okay before continuing " + logger.ENDC
                user_check = raw_input(logger.OKBLUE + "Type yes/y/Y in order to proceed ...: " + logger.ENDC)
                if(user_check != "yes" and user_check != "y" and user_check != "Y"):
                    print logger.FAIL + " Exiting submission " + logger.ENDC
                    sys.exit(2)
                print logger.OKGREEN + "       - Confirmed " + logger.ENDC
                os.system("cp %s %s"%(settingsFilePath,"./"))

    outputFiles = []
    for configFile in config.settingsFile.split(','):
        #Look in the cuts file for the output filename
        outputFilename = 'EMPTY'
        for l in open(configFile):
            #ignore text after comments
            if l.find('#') > -1:
                l = l.split('#')[0]

            if l.find('OutputFilename') > -1:
                outputFilename = l.replace('OutputFilename', '').strip()
                dname, basename = os.path.split(outputFilename)
                stem, ext = os.path.splitext(basename)
                outputFilename = os.path.join('{outdir}', dname, stem + '{id:06d}' + ext)
        if outputFilename == 'EMPTY':
            print logger.FAIL + 'OutputFilename not found in ' + configFile + logger.ENDC
            sys.exit(1)
        else:
            outputFiles.append(outputFilename)

    outputFilenames='' # string defining the output file(s)
    combineArgument='' # argument of command for combining the multiple output files, if need be
    if len(outputFiles) == 0: # at this stage this shouldn't happen
        print logger.FAIL + 'No OutputFileName found' + logger.ENDC
        sys.exit(1)
    elif len(outputFiles) ==1: # the simplest case
        outputFilenames = outputFiles[0]
    else: # len(outputFiles) >=2: multiple output files, or a single combined one
        # check if the output files are unique
        if len(outputFiles) != len(set(outputFiles)):
            print logger.FAIL + 'Two of the output file names are identical. Check the config files (separated by commas).' + logger.ENDC
            sys.exit(1)
        if config.combine_outputFile == None: # no combination asked
            iconfigFile = 0
            for configFile in outputFiles:
                cFile = configFile.replace(".root","_root") + ":" + configFile
                outputFilenames += cFile
                if iconfigFile != len(outputFiles)-1:
                    outputFilenames += ','
                    iconfigFile += 1
        else: # combination of the multiple output files
            # retrieving the prefixes
            prefixes = config.combine_prefixes.split(',')
            # check if the prefixes are unique
            if len(prefixes) != len(set(prefixes)):
                print logger.FAIL + 'Two of the prefixes (separated by commas) are identical. Check the combine_prefixes option.' + logger.ENDC
                sys.exit(1)
            # check if the prefixes and the output files have same length
            if len(prefixes) != len(outputFiles):
                print logger.FAIL + 'When combining the outputs, there should be as many prefixes as there are output files (separated by commas). Check the combine_prefixes and settingsFile options.' + logger.ENDC
                sys.exit(1)
            # using combined file name
            outputFilenames = config.combine_outputFile.replace(".root","_root") + ":" + config.combine_outputFile
            # building the input file argument of the combination
            iconfigFile = 0
            for outFile in outputFiles:
                combineArgument += outFile
                combineArgument += ":"
                combineArgument += prefixes[iconfigFile]
                if iconfigFile != len(outputFiles)-1:
                    combineArgument += ','
                    iconfigFile += 1


    #print outputFilenames

    these = []
    print logger.OKBLUE + 'For these samples' + logger.ENDC

    #Only submit jobs if the dataset actually exists (check with rucio)
    for sample in allSamples:
        currentDatasets = sample.datasets
        actuallyExists = []
        for ds in currentDatasets:
            # doing this check only for the first sample, in case of coma-separated list of samples with same DSID and same first tag of each type
            # a priori it's not a big deal if the additional datasets don't exist; panda will take care of it
            if checkDatasetExists(getShortenedConcatenatedSample(ds)):
                actuallyExists.append(ds)

        sample.details(actuallyExists)
        these += actuallyExists

    #check if it's a release - which automatically would set rootVer and cmtConfig
    if not config.CMake:
        findPackages()

    plural = ''
    if len(these) > 1:
        plural = 's'

    print ''
    print logger.OKBLUE + 'Starting submission of %d sample%s' % (len(these), plural) + logger.ENDC
    print ''
    exe = create_executable('top-xaod.sh')
    for i, d_concatenated in enumerate(these):
        d = getShortenedConcatenatedSample(d_concatenated) # in case of coma-separated list of samples with same DSID and same first tag of each type
        print logger.OKBLUE + 'Submitting %d of %d' % (i+1, len(these)) + logger.ENDC

        #Make the output dataset name
        #for group production it has to start with "group." and we asume that gridUsername is the name of the group (e.g. phys-top)
        if config.groupProduction:
            output = 'group.' + config.gridUsername + '.' + config.nameShortener(d) + '.' + config.suffix
        else:
            output = 'user.' + config.gridUsername + '.' + config.nameShortener(d) + '.' + config.suffix
        if to_dir == None:
            to_dir = os.curdir
        outdir = os.path.abspath(os.path.join(to_dir, output))
        logdir = os.path.join(outdir, 'log')
        if not os.path.exists(outdir):
            os.makedirs(outdir)
        if not os.path.exists(logdir):
            os.makedirs(logdir)
        job_id = cluster.get_identifier()
        for configf, inputsf in dump_inputs(d_concatenated, 'output'+'._{:06d}.input', config.settingsFile,
                                            dirname = os.path.relpath(logdir, outdir),
                                            rootdir = outdir,
                                            n = int(config.maxNFilesPerJob),
                                            source = config.source):
            cluster.submit(exe,
                           [configf, inputsf],
                           stdout = os.path.join(logdir, 'out.%s' % job_id),
                           stderr = os.path.join(logdir, 'out.%s' % job_id),
                           log    = os.path.join(logdir, 'log.%s' % job_id))
    if wait:
        cluster.wait(None, fct = get_fct(mlogger))


def split_every(n, iterable):
    i = iter(iterable)
    piece = list(islice(i, n))
    while piece:
        yield piece
        piece = list(islice(i, n))

def dump_inputs(dids, input_f = None, config_f = None, dirname = None, rootdir = None, n = 5, source = 'grid', **kwds):
    sh = ROOT.SH.SampleHandler()
    if source != 'grid':
        ROOT.SH.readFileList(sh, 'sample', source)
    else:
        ROOT.SH.addGrid(sh, dids)

    sh.printContent()
    rootdir = rootdir if rootdir != None else os.curdir
    dirname = os.path.join(rootdir, dirname if dirname != None else os.curdir)
    
    # ROOT.SH.makeGridDirect(sh, config.destSE)
    config_f_fmt = '{1}{0}{2}'.format('._{:06d}', *os.path.splitext(os.path.basename(config_f)))
    for s in sh:
        for i, f in enumerate(split_every(n, s.makeFileList()), 1):
            with open(config_f) as config_of:
                lines = []
                outputFilename = 'EMPTY'
                for l in config_of:
                    if l.strip().startswith('OutputFilename'):
                        outputFilename = l.replace('OutputFilename', '').strip()
                        dname, basename = os.path.split(outputFilename)
                        stem, ext = os.path.splitext(basename)
                        outputFilename = os.path.join('{outdir}', dname, stem + '._{id:06d}' + ext).format(outdir = rootdir, id = i)
                        l = 'OutputFilename ' + outputFilename + '\n'
                    lines.append(l)
            if outputFilename == 'EMPTY':
                print logger.FAIL + 'OutputFilename not found in ' + configFile + logger.ENDC
                sys.exit(1)
            if os.path.exists(outputFilename):
                mlogger.info('Output("{}") exists. SKIP!'.format(outputFilename))
                break
            else:
                if input_f == None:
                    input_f = dids+'._{:06d}.input'
                _input_f = os.path.join(dirname, input_f.format(i))
                with open(_input_f, 'w') as of:
                    of.write('\n'.join(f))
                _config_f = os.path.join(dirname, config_f_fmt).format(i)

                with open(_config_f, 'w') as of:
                    of.writelines(lines)
                yield _config_f, _input_f

