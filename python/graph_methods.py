import networkx as nx

import math

s = 0
t = 1
tf = 2
ti = 3
delta = 4
day_i = 5
day_f = 6

# discretize the input graph using the timestep as a window.
def discrete(arr):
    
    d = dict()

    for day in range (1, 29):
        d[day] = []

    for i in arr:
        for day in range(1, 29):
            if day >= int(i[day_i]) and day <= int(i[day_f]):
                d[day].append(i)
        
    print ('keys generated:')
    print (d.keys())
    
    return d

def combine_graphs (graphs):

    print ('--- Combining graphs')
    return reduce (lambda x, y: nx.compose (x, y), graphs.values())

def get_temporal_accumulator_graphs(temporal_graphs):
    temporal_accumulator_graphs = dict()

    g = nx.Graph()

    iter = 0

    for key, graph in temporal_graphs.iteritems():
        g = nx.compose(g, graph)
        temporal_accumulator_graphs[key] = g

        iter += 1
        if iter >= max_iter:
            break

    return temporal_accumulator_graphs

def get_temporal_graphs (f, timestep):

    g = nx.parse_edgelist (f, 
                           nodetype=int, 
                           data=(('tf', int), ('ti', int), ('delta', int), ('day_i', int), ('day_f', int)))

    d = discrete(f, timestep)
    
    temporal_graphs = dict()

    max_iteration = 28
    iteration = 0
    
    for k, v in d.iteritems():
        
        # nx.parse_edgelist requires a list of strings
        # where each element (string) represents a tuple:
        # node_from, node_to, time_begin, time_end
        arr = map(lambda x: ' '.join(x), v)
        try:
            # USC
            egraph = nx.parse_edgelist (arr, nodetype=int, data=( 
                ('tf', int), ('ti', int), ('delta', int), ('day_i', int), ('day_f', int) ) )
        except:
            # dartmouth
            try:
                egraph = nx.parse_edgelist (arr, nodetype=int, data=( ('tf', int), ('ti', int), ('delta', int), ('x1', int), ('y1', int), ('x2', int), ('y2', int) ) )
            except:
                # small graph just for test purposes
                egraph = nx.parse_edgelist (arr, nodetype=int, data=( ('tf', int), ('ti', int), ('x1', float), ('y1', float), ('x2', float), ('y2', float) ) )
        temporal_graphs[k] = egraph
        
        print 'generated event graph [#edges = {}] for timestep {}'.format(egraph.size(), k)
    
    return temporal_graphs