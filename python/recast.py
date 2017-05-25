import sys
import os

import networkx as nx
import numpy as np

import random_graph as rg
import graph_methods as td
import classifier as cl

# to understand these variables, read the paper cited on README.md!

# discrete version {g_0, g_1, ..., g_T} (T is the last graph) of the input graph. 
temporal_graphs = None 

# For i from 0 to T, the temporal_accumulator_graph G_i is the union of {g_0, g_1, ..., g_T}
temporal_accumulator_graphs = None

# Union of all temporal graphs, or G_T
aggregated_graph = None

random_graphs = None # random version of temporal_graphs
random_temporal_accumulator_graphs = None # random version of temporal accumulator graphs
random_aggregated_graph = None # random version of aggregated graph

def main():
    # python src/recast.py filename
    if (len(sys.argv) < 2):
        s = '\
python src/recast.py filename'
        sys.exit (s)

    filename = sys.argv[1]

    path = os.path.split(filename)[0]
    
    global temporal_graphs
    global aggregated_graph
    global random_graphs
    global temporal_accumulator_graphs
    global random_temporal_accumulator_graphs
    global random_aggregated_graph

    # print ('--- -- --- -- --- -- ---')
    print ("-- reading input file {} -- ".format(filename))
    fi = file(filename, 'r')
    f = map (lambda x: x.split(), fi.readlines())
    
    # print ('--- -- --- -- --- -- ---')
    # print ('-- Generating temporal graphs --')
    temporal_graphs = td.get_temporal_graphs(f)
    # print('')

    # print ('--- -- --- -- --- -- ---')
    # print ('-- Generating random graphs')
    random_graphs = rg.get_random_graphs(temporal_graphs)
    # print('')

    # print ('--- -- --- -- --- -- ---')
    # print ('-- Generating temporal accumulator graphs')
    # temporal_accumulator_graphs = td.get_temporal_accumulator_graphs(temporal_graphs)
    # print('')
    #
    # print ('--- -- --- -- --- -- ---')
    # print ('-- Generating random temporal accumulator graphs')
    # random_temporal_accumulator_graphs = td.get_temporal_accumulator_graphs (random_graphs)
    # print('')

    # print ('--- -- --- -- --- -- ---')
    # print ('-- Generating aggregated graph')
    aggregated_graph = td.combine_graphs(temporal_graphs)
    # print('')

    # print ('--- -- --- -- --- -- ---')
    # print ('-- Generating random aggregated graph')
    random_aggregated_graph = td.combine_graphs(random_graphs)
    # print('')

    # return [temporal_graphs, temporal_accumulator_graphs, aggregated_graph,
    #         random_graphs, random_temporal_accumulator_graphs, random_aggregated_graph]
    
if __name__ == '__main__':
    main()
