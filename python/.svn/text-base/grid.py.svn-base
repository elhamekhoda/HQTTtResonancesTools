# -*- coding: utf-8 -*-
import os
import sys
import distutils.spawn
import TopExamples.logger

try:
    import rucio.client
except:
    print TopExamples.logger.FAIL + 'DANGER DANGER DANGER' + TopExamples.logger.ENDC
    print 'Could not find rucio.client.Client. If you use setupATLAS (you should) then'
    print '"localSetupRucioClients" and run this again'
    sys.exit(1)

try:
    rucio = rucio.client.Client()
except:
    print TopExamples.logger.FAIL + 'DANGER DANGER DANGER' + TopExamples.logger.ENDC
    print 'Could not setup rucio.client.Client.'
    print 'Did you do voms-proxy-init -voms atlas ?'
    sys.exit(1)

availableDatasets = {}

class Sample:
    def __init__(self, name):
        self.name = name
        self.datasets = []

    def shortNameDatasets(self):
        output = []
        for ds in self.datasets:
            name = ds.replace('/','').split('.')
            output.append(name[1] + '.' + name[-1])
        return output

    def details(self, actuallyExists = []):
        n = len(self.datasets)

        suffix = ''
        if n != 1:
           suffix = 's'

        print TopExamples.logger.OKGREEN + '  %s ' % self.name + TopExamples.logger.ENDC + '(%d dataset%s)' % (n, suffix)
        for i, l in enumerate(self.datasets):
            note = 'Not found'
            if l in actuallyExists:
                note = 'Found'

            print '   -%2d %s %s' % (i, l, note)

def Add(name):
    availableDatasets[name] = Sample(name)
    return availableDatasets[name]

def AvailableDatasets():
    return availableDatasets

def Samples(names):
    samples = []
    for n in names:
        samples.append(availableDatasets[n])
    return samples

class Config:
    code = 'top-xaod'
    cutsFile = 'nocuts.txt'

    gridUsername = ''
    suffix = ''
    excludedSites = ''
    forceSite = ''
    noSubmit = False
    mergeType = 'Default' #None, Default, xAOD 
    destSE = ''
    memory = '4000' #in MB
    maxNFilesPerJob = ''
    official = 0

    def details(self):
        cutsFileIsARealFile = checkForFile(self.settingsFile)
        txt = '(' + TopExamples.logger.FAIL + 'not found' + TopExamples.logger.ENDC + ')'
        if cutsFileIsARealFile:
            txt = '(exists)'

        print TopExamples.logger.OKBLUE + 'Analysis Settings:' + TopExamples.logger.ENDC
        print ' -Code:          ', self.code
        print ' -CutsFile:      ', self.settingsFile, txt

        print TopExamples.logger.OKBLUE + 'Grid Settings:' + TopExamples.logger.ENDC
        print ' -GridUsername:  ', self.gridUsername
        print ' -Suffix:        ', self.suffix
        print ' -ExcludedSites: ', self.excludedSites
        print ' -ForceSite:     ', self.forceSite
        print ' -NoSubmit:      ', self.noSubmit
        print ' -MergeType:     ', self.mergeType, 'out of (None, Default, xAOD)'
        print ' -memory:        ', self.memory, 'in MB'
        print ' -maxNFilesPerJob', self.maxNFilesPerJob

        txt = self.destSE
        if len(txt) == 0:
            txt = '<not set>'
        print ' -DestSE         ', txt

        print ''

def checkMergeType(configuration):
    type = configuration.mergeType
    if type != "None" and type != "Default" and type != "xAOD":
        print "MergeType must be set to either None, Default or xAOD"
        print 'Not', type, "Since that doesn't make sense"
        sys.exit()

#If settings.txt doesn't exist your grid job will fail
def checkForFile(filename):
    return os.path.exists(filename)

def checkForPrun():
    if distutils.spawn.find_executable('prun') == None:
        print TopExamples.logger.FAIL + 'DANGER DANGER DANGER' + TopExamples.logger.ENDC
        print 'Could not find prun. If you use setupATLAS (you should) then'
        print '"localSetupPandaClient --noAthenaCheck" and run this again'
        sys.exit()

#I see a crash if I setup with rcSetup Top,1.4.0
#and don't run rc find_packages
#so let's run this by default
def findPackages():
   cmd = 'cd $ROOTCOREBIN/../; rc find_packages'

   debug = False

   if debug:
       print 'finding packages'
       print cmd

   for l in os.popen(cmd).readlines():
       if debug:
           print l.strip()

   if debug:
       print 'done'

#Given a list of datasets, the command to run and a mode (egamma, muons) this
#submits one prun job per run
#This is mostly for internal use of the code
def submit(config, allSamples):
  checkForPrun()
  checkMergeType(config)
  config.details()
  tarfile = 'top-xaod.tar.gz'

  #We don't want to use an old, out-of-date file
  #Delete the file if it exists
  try:
      os.remove(tarfile)
  except OSError, e:
      #Number 2 is 'file doesn't exist' which is okay for us
      if e.errno == 2:
          pass
      else:
          raise

  #Check for cuts file
  if not checkForFile(config.settingsFile):
      print TopExamples.logger.FAIL + "DANGER DANGER. HIGH VOLTAGE" + TopExamples.logger.ENDC
      print '%s does not exist in this directory' % config.settingsFile
      print 'please make it before submitting'

      if config.settingsFile == 'dil-cuts.txt' or config.settingsFile == 'ljets-cuts.txt':
         print 'or... copy it with'
         print 'cp $ROOTCOREBIN/data/TopAnalysis/%s .' % config.settingsFile

      sys.exit(1)

  #Look in the cuts file for the output filename
  outputFilename = 'EMPTY'
  for l in open(config.settingsFile):
      #ignore text after comments
      if l.find('#') > -1:
          l = l.split('#')[0]

      if l.find('OutputFilename') > -1:
          outputFilename = l.replace('OutputFilename', '').strip()

  if outputFilename == 'EMPTY':
      print 'OutputFilename not found in %s' % config.settingsFile
      sys.exit(1)

  #print outputFilename

  these = []
  print TopExamples.logger.OKBLUE + 'For these samples' + TopExamples.logger.ENDC
  
  #Only submit jobs if the dataset actually exists (check with rucio)
  for sample in allSamples:
      currentDatasets = sample.datasets
      actuallyExists = []
      for ds in currentDatasets:
          if checkDatasetExists(ds):
              actuallyExists.append(ds)

      sample.details(actuallyExists)
      these += actuallyExists

  #check if it's a release - which automatically would set rootVer and cmtConfig
  findPackages()

  plural = ''
  if len(these) > 1:
      plural = 's'

  print ''
  print TopExamples.logger.OKBLUE + 'Starting submission of %d sample%s' % (len(these), plural) + TopExamples.logger.ENDC
  print ''

  isfirst = True
  for i, d in enumerate(these):
     print TopExamples.logger.OKBLUE + 'Submitting %d of %d' % (i+1, len(these)) + TopExamples.logger.ENDC

     splitted = d.split('.')
     
     runNumber = splitted[1]
     txt = splitted[2]
     if splitted[0] == "user":
         runNumber = splitted[2]
         txt = splitted[3]
     derivation = splitted[-2]
     tags = splitted[-1].replace('/','')

     #grid complains dataset names are too long
     #stupid grid
     if len(txt) > 20:
         txt = txt.split('_')[0]

     n = runNumber + '.' + txt + '.' + derivation + '.' + tags

     #Make the output dataset name
     output = 'user.' + config.gridUsername + '.' + n + '.' + config.suffix
     if config.official:
        output = 'group.' + config.gridUsername + '.' + n + '.' + config.suffix

     cmd = 'prun \\\n'
     cmd += '--inDS=' + d + ' \\\n'
     cmd += '--outDS=' + output + ' \\\n'
     cmd += '--useRootCore \\\n'
     cmd += '--writeInputToTxt=IN:in.txt \\\n'
     cmd += '--outputs=%s \\\n' % outputFilename
     cmd += '--exec="%s %s in.txt" \\\n' % (config.code, config.settingsFile)
     if config.official:
        cmd += '--official \\\n'

     #you might really hate a certain site
     if len(config.excludedSites) > 0:
         cmd += '--excludedSite=' + config.excludedSites + ' \\\n'

     #you might really like a certain site
     if len(config.forceSite) > 0:
         cmd += '--site ' + config.forceSite + ' \\\n'

     #tar-up the first time only, to save time when submitting
     if isfirst:
         cmd += '--outTarBall=%s \\\n' % tarfile
         isfirst = False
     else:
         cmd += '--inTarBall=%s \\\n' % tarfile

     #maybe you don't want to submit the job?
     if config.noSubmit:
         cmd += '--noSubmit \\\n'

     #fewer files = happy grid
     if config.mergeType != "None":
         cmd += '--mergeOutput \\\n'

     #Write the output to a specific storage element?
     if len(config.destSE) > 0:
         cmd += '--destSE=\"%s\" \\\n' % config.destSE

     #xAOD merging - slide 9 https://indico.cern.ch/event/401703/contribution/2/2/material/slides/0.pdf
     #Only add TriggerMenuMetaDataTool option when xAODTriggerCnv is in the release (not in 2.3.15)
     #--mergeScript="xAODMerge -m xAODMaker::TriggerMenuMetaDataTool %OUT \`echo %IN | sed \'s/,/ /g\'\`" \\\n'
     if config.mergeType == "xAOD":
         cmd += '--mergeScript="xAODMerge %OUT \`echo %IN | sed \'s/,/ /g\'\`" \\\n'
         
     #how much memory, not sure what default is when not set
     if len(config.memory) > 0:
         cmd += '--memory=%s \\\n' % config.memory
         
     #how many files
     if len(config.maxNFilesPerJob) > 0:
         cmd += '--maxNFilesPerJob=%s \\\n' % config.maxNFilesPerJob

     #Make sure the command actually ends
     cmd += '\n'

     #show people what you're about to do
     print cmd

     #then do it
     for l in os.popen(cmd).readlines():
         print '>> ', l.strip()

#Make a directory if it doesn't exist
def makeDirectory(outputDirectory):
    try:
        os.makedirs(outputDirectory)
        print 'Made directory', outputDirectory
    except:
        #directory already exists
        pass

#Loop through the samples and change the AOD-style name to a DAOD_TOPQ one
#The user needs to give derivationToUse (DAOD_TOPQ1 or DAOD_TOPQ2)
#The p-tag for the production
#And the list of samples
def convertAODtoTOPQ(derivationToUse, ptag, samples):
    for sample in samples:
        for i, ds in enumerate(sample.datasets):
            sample.datasets[i] = ds.replace('AOD', derivationToUse).replace('/', '') + '_' + ptag

#Get a list of dataset names matching some pattern with stars in, using dq2-ls
def listDatasets(theScope, datasetPattern):
    response = rucio.list_dids(scope = theScope, filters = {'name' : datasetPattern})

    names = []
    for l in response:
        names.append(l)

    return names

#Download all datasets that match the pattern, to the outputDirectory
def download(theScope, datasetPattern, outputDirectory):
    #Make sure the directory where these need to go exists
    makeDirectory(outputDirectory)

    #Get datasets matching the pattern
    intdatasets = listDatasets(theScope, datasetPattern)
    txt = ''
    if len(intdatasets) != 1:
        txt = 's'

    print 'Found %d dataset%s:' % (len(intdatasets), txt)

    for ds in intdatasets:
        print '    %s' % ds

    for j, d in enumerate(intdatasets):
        cmd = 'rucio download %s:%s --dir %s' % (theScope, d, outputDirectory)
        print TopExamples.logger.OKBLUE + 'Dataset %d of %d: %s' % (j+1, len(intdatasets), cmd) + TopExamples.logger.ENDC

        #Run the dq2 command
        os.system(cmd)

#Prun submits to datasets that don't exist, so do a quick check first...
def checkDatasetExists(name):
    theScope = name.split('.')[0]
    #name = 'data15_13TeV.00266904.physics_Main.merge.DAOD_TOPQ1.f594_m1435_p2361'
    if theScope == "user":
           theScope = name.split('.')[0] + "." + name.split('.')[1]
    reply = listDatasets(theScope, name)
    #print reply
    return len(reply) == 1

if __name__ == '__main__':
    print "You don't run this directly!"
    print 'For an example, see 01SubmitToGrid.py'
