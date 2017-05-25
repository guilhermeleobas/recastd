import networkx as nx

from graph_methods import *

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
