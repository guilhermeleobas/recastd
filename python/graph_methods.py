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
    
def discrete(f):
    
    d = dict()
    
    # for enc in f:
    #     for day in range(int(enc[day_i]), int(enc[day_f])+1):
    #         if (enc[s], enc[t], day) not in d:
    #             d[(enc[s], enc[t],str(day))] = 1
    #         else:
    #             d[(enc[s], enc[t], str(day))] += 1
                
    # out = dict()
    
    # for i in range(0, 29):
    #     out[str(i)] = []
        
    # for (ss, tt, day), v in d.iteritems():
    #     out[day].append([ss, tt, str(v)])
        
    # return out
        
    for day in range (0, 29):
        d[day] = []
    
    for i in f:
        for day in range(0, 29):
            if day >= int(i[day_i]) and day <= int(i[day_f]):
                d[day].append(i)
    
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
        
        try:
            egraph = nx.parse_edgelist (arr, 
                                       nodetype=int, 
                                       data=(('tf', int), ('ti', int), ('delta', int), ('day_i', int), ('day_f', int)),
                                       create_using=nx.Graph())
        except:
            egraph = nx.parse_edgelist(arr,
                                       nodetype=int,
                                       data=(('weight', int), ),
                                       create_using=nx.Graph())

        temporal_graphs[k] = egraph
        
        # print 'generated event graph [#edges = {}] for timestep {}'.format(egraph.size(), k)
    
    return temporal_graphs
