from __future__ import unicode_literals

import sys
import os

import networkx as nx
import numpy as np

import platform

if platform.python_implementation() == 'CPython':
    import matplotlib.pyplot as plt
    import matplotlib
    from matplotlib import rcParams
    rcParams.update({'figure.autolayout': True})
    from matplotlib import rc
    rc('font',**{'family':'sans-serif','sans-serif':['Helvetica']})
    # matplotlib.rc('font', family='Arial')
    ## for Palatino and other serif fonts use:
    # rc('font',**{'family':'serif','serif':['Palatino']})
    rc('text', usetex=True)

import gen_graphs as gg
import classifier
import graph_methods
import ecdf

# # Clustering coefficient

# Slow, depending on the size of the graph

def plot_clustering(temporal_graphs, random_graphs, fname=None, name=None):

    def clustering(graphs):
        l = []
        for k, graph in graphs.iteritems():
            if len(graph) == 0:
                l.append(0)
                continue
            print "calculating average clustering for graph {} with #nodes = {} and #edges = {}".format(k, len(graph.nodes()), len(graph.edges()))
            l.append(nx.average_clustering(graph, count_zeros=True))
            
        return l

    cc_y_t = clustering(temporal_graphs)
    cc_y_r = clustering(random_graphs)

    cc_x = range(0, len(temporal_graphs))
    
    plt.title("persistence " + name)
    plt.plot(cc_x, cc_y_t, marker='o', label='Grafo Real', color='blue')
    plt.plot(cc_x, cc_y_r, marker='x', label='Grafo Aleatorio', color='green')
    plt.title('coef. de clusterizacao ' + name)
    plt.ylim(0, 1)
    plt.xlim(0, len(temporal_graphs))
    plt.ylabel("Coeficiente de clusterizacao")
    plt.xlabel("Tempo t (dias)")
    plt.legend(bbox_to_anchor=(0.98,0.05), loc=4, borderaxespad=0.)
    # plt.legend()
    plt.grid()
    plt.savefig (fname, format='eps')
    plt.cla()
    # plt.show()


# # Edge persistence

def get_ep(recast):
    ep = classifier.get_ep(gg.temporal_graphs)
    return ep

def get_random_ep(recast):
    random_ep = classifier.get_ep(gg.random_graphs)
    return random_ep

def plot_ep(ep, random_ep, fname = None, trace_name=None):

    x = np.linspace(0.0, 1.0, 100)

    flatten_ep = np.array(ep.values()).flatten()
    flatten_random_ep = np.array(random_ep.values()).flatten()
    
    y = ecdf.calc_ecdf(flatten_ep)
    y_r = ecdf.calc_ecdf(flatten_random_ep)

    # yrange = [0, 10**(-6), 10**(-3), 10**(0)]
    # ylabels = ['10^-6', '10^-3', '10^-0']

    plt.plot(x, y, marker='o', label='Grafo Real', color='blue')
    plt.plot(x, y_r, marker='x', label='Grafo Aleatorio', color='green')

    plt.yscale('log', basey=10)
    plt.xlim(0, 1)
    plt.ylim ([10**(-7), 10**(0)])
    print trace_name

    plt.title("persistence " + trace_name)

    plt.ylabel('$P[per(i, j) > x]$')
    plt.xlabel('$x$ [persistence]')

    plt.legend(bbox_to_anchor=(0.98,0.05), loc=4, borderaxespad=0.)
    # plt.legend()
    plt.grid()
    plt.savefig (fname, format='eps')
    plt.cla()
    # plt.show()


# # topological overlap

def get_to(recast):
    to = classifier.get_to(gg.aggregated_graph)
    return to

def get_random_to(recast):
    random_to = classifier.get_to(gg.random_aggregated_graph)
    return random_to

def plot_to(to, random_to, fname=None, trace_name=None):

    x = np.linspace(0.0, 1.0, 100)

    flatten_to = np.array(to.values()).flatten()
    flatten_random_to = np.array(random_to.values()).flatten()
    
    y = ecdf.calc_ecdf(flatten_to)
    y_r = ecdf.calc_ecdf(flatten_random_to)

    # yrange = [0, 10**(-6), 10**(-3), 10**(0)]
    # ylabels = ['10^-6', '10^-3', '10^-0']

    plt.plot(x, y, marker='o', label='Grafo Real', color='blue')
    plt.plot(x, y_r, marker='x', label='Grafo Aleatorio', color='green')

    plt.yscale('log', basey=10)
    plt.xlim(0, 1)
    # plt.ylim ([10**(-8), 10**(0)])
    plt.ylim ([10**(-6), 10**(0)])

    plt.title("topological overlap " + trace_name.split('_')[0])

    plt.ylabel(r'$P[to(i, j) > x]$')
    plt.xlabel("$x$ [topological overlap]")

    plt.legend(bbox_to_anchor=(0.05, 0.1), loc='lower left', borderaxespad=0.)
    # plt.legend()
    plt.grid()
    plt.savefig (fname, format='eps')
    plt.cla()
    # plt.show()  

def save_edges(gg, ep, random_ep, to, random_to, p_rnd, fname=None, ext=None, recastd=False):
    
    for p in p_rnd:

        x_ep = classifier.get_x (random_ep, p)
        x_to = classifier.get_x (random_to, p)

        relations = dict()

        filename = fname + os.sep + str(p) + ext 
        if recastd :
            filename = fname + str(p) + ext
        f = open(filename, 'w')
        print ('saving {}'.format(filename))

        for edge in gg.aggregated_graph.edges_iter():#ep.iterkeys():
            x = edge[0]
            y = edge[1]
            f.write(str(x) + ' ' + str(y) + ' ')
            if (ep[edge] > x_ep and to[edge] > x_to):
                f.write(str(classifier.friends) + '\n')
            elif (ep[edge] <= x_ep and to[edge] > x_to):
                f.write(str(classifier.acquaintance) + '\n')
            elif (ep[edge] > x_ep and to[edge] <= x_to):
                f.write(str(classifier.bridges) + '\n')
            else:
                f.write(str(classifier.random) + '\n')
                
            f.write(str(y) + ' ' + str(x) + ' ')
            if (ep[edge] > x_ep and to[edge] > x_to):
                f.write(str(classifier.friends) + '\n')
            elif (ep[edge] <= x_ep and to[edge] > x_to):
                f.write(str(classifier.acquaintance) + '\n')
            elif (ep[edge] > x_ep and to[edge] <= x_to):
                f.write(str(classifier.bridges) + '\n')
            else:
                f.write(str(classifier.random) + '\n')

        f.close()


# # Classify results

def classify (ep, random_ep, to, random_to, p_rnd):
    # print 'classifying edge persistence'
    # rep = classify_ep (ep, random_ep, p_rnd)

    # print 'classifying topological overlap'
    # rto = classify_to (to, random_to, p_rnd)
    
    # print 'combining edge persistence and topological overlap'
    # relations = classify_relationships (rep, rto)
    # return relations

    x_ep = classifier.get_x (random_ep, p_rnd)
    x_to = classifier.get_x (random_to, p_rnd)
    
    print x_ep, x_to
    
    # x_to -= 0.15

    relations = dict()

    for edge in gg.aggregated_graph.edges_iter():#ep.iterkeys():
        if (ep[edge] > x_ep and to[edge] > x_to):
            relations[edge] = classifier.friends
        elif (ep[edge] <= x_ep and to[edge] > x_to):
            relations[edge] = classifier.acquaintance
        elif (ep[edge] > x_ep and to[edge] <= x_to):
            relations[edge] = classifier.bridges
        else:
            relations[edge] = classifier.random

    return relations


def map_relations (ep, random_ep, to, random_to, p_rnd):
    results = []
    for p in p_rnd:
        print 'classify for p_rnd', p
        results.append(classify(ep, random_ep, to, random_to, p))

    friends = []
    bridges = []
    acquaintance = []
    random = []

    for i in results:
        r = classifier.combine_relations(i)
        random.append(r[0])
        friends.append(r[2])
        acquaintance.append(r[3])
        bridges.append(r[4])

    print (friends)
    print (bridges)
    print (acquaintance)
    print (random)

    return (friends, bridges, acquaintance, random)

def plot_relations (p_rnd, friends, bridges, acquaintance, random, fname=None, trace_name=None):

    plt.plot(p_rnd, friends, marker='x', label='Amigos', color='#22941a')
    plt.plot(p_rnd, bridges, marker='+', label='Pontes', color='#fc6907')
    plt.plot(p_rnd, acquaintance, marker='s', label='Conhecidos', color='#1661a7')
    plt.plot(p_rnd, random, marker='o', label='Desconhecidos', color='#cc0d19')

    # plt.xticks(p_rnd)

    # plt.xlim ([10**(-6), 10**(-3)])
    plt.yscale('log', basey=10)
    plt.xscale('log', basey=10)

    # plt.ylim ([1e3, 1e6])

    # plt.yticks(yrange, ylabels)

    plt.title(r'Classificacao ' + trace_name.split('_')[0])

    plt.ylabel("Numero de arestas")
    plt.xlabel(r'$p_{rnd}$')

    plt.legend(bbox_to_anchor=(0.97, 0.97), loc='upper right', borderaxespad=0.)

    plt.savefig (fname, format='eps')
    plt.cla()

