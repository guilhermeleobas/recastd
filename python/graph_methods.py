import networkx as nx

import math

s = 0
t = 1
tf = 2
ti = 3
delta = 4
day_i = 5
day_f = 6
weight = 2
timestep = 86400
    
def discrete(f):
    
    d = dict()
    
    
    if len(f[0]) == 5:
    
        min_ti = 0x3f3f3f3f
        
        for line in f:
            if int(line[ti]) < min_ti:
                min_ti = int(line[ti])
                
        if len(f[0]) == 5:
            for i, x in enumerate(f):
                di = ( int(x[ti]) - min_ti) / timestep
                df = ( int(x[tf]) - min_ti) / timestep
                f[i].append(str(di))
                f[i].append(str(df))
        
    for day in range (0, 29):
        d[day] = []
    
    for i in f:
        for day in range(0, 29):
            if day >= int(i[day_i]) and day <= int(i[day_f]):
                d[day].append(i)
    
    for k in d.keys():
        if len(d[k]) == 0:
            d.pop(k)

    return d

def combine_graphs (graphs):

    # print ('--- Combining graphs')
    return reduce (lambda x, y: nx.compose (x, y), graphs.values())

def get_temporal_accumulator_graphs(temporal_graphs):
    temporal_accumulator_graphs = dict()

    g = nx.Graph()

    for key, graph in temporal_graphs.iteritems():
        g = nx.compose(g, graph)
        temporal_accumulator_graphs[key] = g

    return temporal_accumulator_graphs

def get_temporal_graphs (f):

    d = discrete(f)
    
    temporal_graphs = dict()

    for k, v in d.iteritems():
        
        # nx.parse_edgelist requires a list of strings
        # where each element (string) represents a tuple:
        # node_from, node_to, time_begin, time_end
        arr = map(lambda x: ' '.join(x), v)
        
        egraph = nx.parse_edgelist (arr, 
                                   nodetype=int, 
                                   data=(('tf', int), ('ti', int), ('delta', int), ('day_i', int), ('day_f', int)),
                                   create_using=nx.Graph())
        # print 'gen for k={}'.format(k)
        temporal_graphs[k] = egraph
    
    return temporal_graphs
