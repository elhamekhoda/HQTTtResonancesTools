#! /usr/bin/env python2.7
import argparse
import re
from collections import OrderedDict
import ROOT
ROOT.PyConfig.IgnoreCommandLineOptions = True
try:
    from PathResolver import PathResolver
except:
    class PathResolver(object):
        @staticmethod
        def FindCalibFile(s):
            return s
        def __getattr__(self, attr):
            if attr not in dir(self):
                return lambda x, *args: (x,) + args if args else x # identity function

import pyAMI.client
import pyAMI.atlas.api as AtlasAPI

client = pyAMI.client.Client('atlas')

AtlasAPI.init()


def is_single_tagged(pattern):
    '''
    It is usually not recommended to use not single-tagged datasets
    '''

def get_datasets(*args, **kwds):
    datasets = AtlasAPI.list_datasets(client, *args, **kwds)
    ret = OrderedDict((ds['ldn'], ds) for ds in datasets)
    return ret

def retrieve_datasets(pattern = 'data%_13TeV%', ami_tags = 'r9264_p3083_p3628', grls = ('default',), only_latest_ptag = False, data_type = 'DAOD_EXOT7'):
    print 'searching for pattern: {}'.format(pattern)
    is_data = pattern.startswith('data')
    id_number_key = 'run_number' if is_data else 'dataset_number'
    kwds = dict(patterns = pattern, type = data_type, ami_tags = ami_tags, fields = ['ami_tags', 'prodsys_status', id_number_key], order = [id_number_key, 'ami_tags'])
    
    grls = get_grls(*grls)

    ret = get_datasets(**kwds)
    for ds in ret:
        ret.setdefault(ds, {}).setdefault('in_grl', False if is_data else True)

    datasets_grl = OrderedDict()
    if is_data:
        for grl in grls:
            datasets_grl.update(get_datasets(**dict(grl = PathResolver.FindCalibFile(grl), **kwds)))

    id_numbers = set() # for data it's run_number but for MC it's dataset_number. Note that the `latest_ptags` algorithm requires datasets listed in reversed order of `id_numbers` 
    for ds, attr in ret.items()[::-1]:
        attr['in_grl'] = (ds in datasets_grl) if is_data else True # For MC, my protocol is that it is always True!
        attr['all_available'] = (attr['prodsys_status'] == u'ALL EVENTS AVAILABLE')
        attr['id_number'] = id_number = attr[id_number_key]
        if id_number not in id_numbers:
            id_numbers.add(id_number)
            attr['latest_ptags'] = True
        else:
            attr['latest_ptags'] = False
        single_tagged(attr)
        preselect(attr)
        

    if only_latest_ptag:
        for ds in ret:
            if not (ret[ds]['latest_ptags'] or ret[ds]['all_available']):
                del ret[ds]

    return ret

def single_tagged(attr):
    count = OrderedDict()
    attr['duplicated_tags'] = []
    for tag in attr['ami_tags'].split('_'):
        tag_k = tag[:1]
        if not count.has_key(tag_k):
            count.setdefault(tag_k, 1)
        else:
            count[tag_k] += 1
    attr['single_tagged'] = True
    for k, v in count.iteritems():
        if v > 1:
            attr['single_tagged'] = False
            attr['duplicated_tags'].append((k, v))


def preselect(attr):
    preselected = OrderedDict()
    preselected['in_grl'] = attr['in_grl']
    if not attr['in_grl']:
        attr['reason_list']  = ['# Not in GRL']
    else:
        attr['reason_list']  = [None]

    preselected['all_available'] = attr['all_available']
    if not attr['all_available']:
        attr['reason_list'] += ['# prodsysStatus: "{}"'.format(attr['prodsys_status']) + ('' if attr['in_grl'] else " BUT Not in GRL!")]
    else:
        attr['reason_list'] += [None]

    preselected['latest_ptags'] = attr['latest_ptags']
    if not attr['latest_ptags']:
        attr['reason_list'] += ['# not with latest p-tag!']
    else:
        attr['reason_list'] += [None]

    preselected['single_tagged'] = attr['single_tagged']
    if not attr['single_tagged']:
        attr['reason_list'] += ['# not with single ami-tagged! ({})'.format(' & '.join('{} {}-tag'.format(v, k) for k, v in attr['duplicated_tags']))]
    else:
        attr['reason_list'] += [None]

    attr['preselected'] = all(preselected.itervalues()) # it must be all true for datasets to be pre-selected
    attr['reason'] = ' '.join(r for r in attr['reason_list'] if r is not None)

def get_grls(*grls):
    if grls == ('default',):
        return ['GoodRunsLists/data18_13TeV/20181111/data18_13TeV.periodAllYear_DetStatus-v102-pro22-04_Unknown_PHYS_StandardGRL_All_Good_25ns_Triggerno17e33prim.xml',
                'GoodRunsLists/data17_13TeV/20180618/data17_13TeV.periodAllYear_DetStatus-v97-pro21-17_Unknown_PHYS_StandardGRL_All_Good_25ns_Triggerno17e33prim.xml',
                'GoodRunsLists/data16_13TeV/20180129/data16_13TeV.periodAllYear_DetStatus-v89-pro21-01_DQDefects-00-02-04_PHYS_StandardGRL_All_Good_25ns.xml',
                'GoodRunsLists/data15_13TeV/20170619/data15_13TeV.periodAllYear_DetStatus-v89-pro21-02_Unknown_PHYS_StandardGRL_All_Good_25ns.xml']
    else:
        return grls


def pprint_lines(pattern, grl, ami_tags, data_type, skim = False):
    grls = get_grls(*grl)
    for grl in grls:
        yield '# GRL: ' + grl
    yield '['
    for ds, attr in retrieve_datasets(pattern, grls = grls, ami_tags = ami_tags, data_type = data_type).iteritems():
        commented = ''
        if not attr['preselected']:
            if not skim:
                commented = '#'
            else:
                continue
        yield '{commented}\'{ds}\', {reason}'.format(commented = commented, ds = ds, reason = attr['reason'])
    yield ']'

if __name__ == '__main__':
    parser =argparse.ArgumentParser(formatter_class = argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('pattern', nargs = '?', default = 'data%_13TeV%', help = 'Search Pattern. "%%" is the wild card symbol')
    parser.add_argument('--grl', nargs = '*', default = ['default'], help = 'GoodRunsLists.')
    parser.add_argument('--ami-tags', default = None, help = 'AMI Tags')
    parser.add_argument('--data-type', default = None, help = 'Data Type')
    # parser.add_argument('--only-print-cmd', action='store_true')
    parser.add_argument('--skim', action='store_true')
    args = parser.parse_args()
    for l in list(pprint_lines(**vars(args))):
        print l

