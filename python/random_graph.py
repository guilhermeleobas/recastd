# Discretize the input graph into timesteps (G_t) and generate random versions of it
# using networkX

import sys

import networkx as nx

from graph_methods import *

def dump (filename, graph):
    with file(filename, 'w') as f:
        for i in graph.edges_iter():
            f.write('{} {} 0 0\n'.format(i[0], i[1]))

def get_random_graphs(temporal_graphs):
    
    random_graphs = dict()
    
    # generate random graphs from temporal_graphs
    for k, v in temporal_graphs.iteritems():
        egraph = v
        rgraph = nx.expected_degree_graph(egraph.degree().values(), selfloops=False)

        # mapping = dict(zip(
        #     sorted(rgraph.nodes()),
        #     sorted(egraph.nodes())
        # ))
        # nx.relabel_nodes(rgraph, mapping, copy=False)
        random_graphs[k] = rgraph
        # print "generated random graph for timestep: {} - # edges: {}/{}".format(k, len(rgraph.edges()), len(egraph.edges()) )
    
    return random_graphs

# def join_graphs(graphs):
#     go = False
#     g = nx.Graph()
#     for k, v in graphs.iteritems():
#         if not go:
#             go = True
#             g = v
#             continue
#         else:
#             g = nx.compose(g, v)
#     return g

def main():
    filename = sys.argv[1]
    timestep = int(sys.argv[2])
    print filename

    f = file(filename, 'r')
    f = map(lambda x: x.split(), f.readlines())
    
    d_rng (f, timestep)
    
if __name__ == '__main__':
    main()
