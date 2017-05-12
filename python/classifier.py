import os
from random import shuffle

from collections import deque

import networkx as nx
import numpy as np

import graph_methods as gm

global random
global social
global friends
global acquaintance
global bridges

random = 0
social = 1

friends = 2
acquaintance = 3
bridges = 4
# random = 0 # random defined above

def get_delta(temporal_graphs):
    # delta = tf-ti
    sum_delta = dict()

    for graph in temporal_graphs.itervalues():
        for (s, t) in graph.edges_iter():
            if (s, t) in sum_delta:
                sum_delta[(s, t)] += graph.get_edge_data(s, t)['delta']
            else:
                sum_delta[(s, t)] = graph.get_edge_data(s, t)['delta']

    return sum_delta

def set_delta(random_graphs, temporal_graphs):

    l = deque()
    aux = deque()
    
    for graph in temporal_graphs.itervalues():
        for (s, t) in graph.edges_iter():
            l.append( graph.get_edge_data(s, t)['delta'] )
            aux.append( graph.get_edge_data(s, t)['delta'] )

    print ("before len(t) = {}".format(len(l)))

    shuffle(l)
    shuffle(aux)

    for graph in random_graphs.itervalues():
        nx.set_edge_attributes (graph, 'delta', None)
        for (s, t) in graph.edges_iter():
            try:
                x = l.popleft()
            except:
                l = aux
                x = l.popleft()
            graph[s][t]['delta'] = x

    print ("after len(t) = {}".format(len(l)))


def calc_ecdf(observations, linspace=None):
    observations = np.array(observations)

    observations.sort()

    def calc (x):
        counter = len(observations) - np.searchsorted(observations, x)
        # print "call with x = {} -.- counter = {} and return value = {}".format (x, counter, ((counter + 0.0) / len(observations)))
        return (float(counter) / len(observations))

    if (linspace == None):
        # X = np.linspace(0.0, 1.0, 100)
        X = np.linspace(min(observations), max(observations), 100)
    else:
        X = linspace


    ret = []
    for x in X:
        ret.append((calc(x), x))

    return ret

# size is the absolute number of nodes in the network
def edge_persistence(l_graphs):

    ep = dict()

    t = len(l_graphs)

    for k, v in l_graphs.iteritems():
        for edge in v.edges_iter():
            x, y = edge[0], edge[1]
            if edge in ep:
                # ep[edge] += 1
                ep[(x, y)] += 1
                ep[(y, x)] += 1
            else:
                # ep[edge] = 1
                ep[(x, y)] = 1
                ep[(y, x)] = 1

    # print ("#edges = {}".format(len(ep.keys()))) ==> 66876
    print ("#edges = {}".format(len(ep.keys())))
    for k, v in ep.iteritems():
        ep[k] = (v+0.0)/t

    print ("#edges = {}".format(len(ep.keys())))
    return ep

def topological_overlap(agg_tgraph):

    to = dict()

    for edge in agg_tgraph.edges_iter():
        i = edge[0]
        j = edge[1]

        ni = set(agg_tgraph.neighbors(i))
        nj = set(agg_tgraph.neighbors(j))
        
        num = len(ni.intersection(nj))
        den = len(ni.union(nj))

        x = (num + 0.0)/((len(ni) - 1) + (len(nj) - 1) - num)

        # print 'nodes {}-{} with {}/{}'.format(i, j, num, den)
        
        if (den <= 0):
            to[(i, j)] = to[(j, i)] = 0.0
        else:
            to[(i, j)] = to[(j, i)] = (num + 0.0)/(den)
            # to[(i, j)] = (num + 0.0)/(den)
            # to[(i, j)] = to[(j, i)] = x

    return to

def get_x(d, p_rnd):
    # Find a value x such that F_(x) >= p_rnd
    F = calc_ecdf(d.values())
    # print (F)

    for p, x in F:
        if p <= p_rnd:
            return x
    return F[-1][1]

def classify_to(to, random_to, p_rnd):
    # given the topological overlap and a value (double) p_rnd, classify the relations
    #
    # if to[(i, j)] > x, then the relation is considered as social (1)
    # otherwise, random (0)
    rto = dict()

    x_to = get_x(random_to, p_rnd)

    cr = 0

    for edge, value in to.iteritems():
        if value > x_to:
            rto[edge] = social
        else:
            cr += 1
            rto[edge] = random

    # print 'cr = {}/{}'.format(cr, len(to))

    return rto
    
def classify_ep(ep, random_ep, p_rnd):
    # given the edge persistence (dict from edge to value) and a value (double) p_rnd, classify the relations
    #
    # if ep(i, j) > x, then the relation is considered as social (1)
    # otherwise, random (0)
    rep = dict()

    x_ep = get_x(random_ep, p_rnd)

    # print ("x_ep value = ", x_ep)

    cr = 0

    for edge, value in ep.iteritems():
        if value > x_ep:
            # print (edge, "social")
            rep[edge] = social
        else:
            cr += 1
            rep[edge] = random

    print 'cr = {}/{}'.format(cr, len(ep))

    return rep
    
def classify_relationships(rep, rto):
    
    # given the edge persistence and topological overlap matrix
    # classify relations between two nodes in the network using the relations below:
    # if rep = social and rto = social
    #   relation => friend
    # if rep = random and rto = social
    #   relation => acquaintance
    # if rep = social and rto = random
    #   relation => bridges
    # if rep = random and rto = random 
    #   relation => random

    # being E the set of edges
    # E(rto) contains (or is equal to) E(rep)

    relations = dict()

    for edge in rep.iterkeys():
        if (rep[edge] == social and rto[edge] == social):
            relations[edge] = friends
        elif (rep[edge] == random and rto[edge] == social):
            relations[edge] = acquaintance
        elif (rep[edge] == social and rto[edge] == random):
            relations[edge] = bridges
        else:
            relations[edge] = random

    return relations

def get_ep (temporal_graphs):
    print 'generating edge_persistence'
    ep = edge_persistence (temporal_graphs)
    return ep

def get_to (aggregate_graph):    
    print 'generating topological overlap'
    to = topological_overlap (aggregate_graph)
    return to

def classify (ep, random_ep, to, random_to, p_rnd):
    # print 'classifying edge persistence'
    # rep = classify_ep (ep, random_ep, p_rnd)

    # print 'classifying topological overlap'
    # rto = classify_to (to, random_to, p_rnd)
    
    # print 'combining edge persistence and topological overlap'
    # relations = classify_relationships (rep, rto)
    # return relations

    x_ep = get_x (random_ep, p_rnd)
    x_to = get_x (random_to, p_rnd)

    relations = dict()

    for edge in ep.iterkeys():
        if (ep[edge] > x_ep and to[edge] > x_to):
            relations[edge] = friends
        elif (ep[edge] <= x_ep and to[edge] > x_to):
            relations[edge] = acquaintance
        elif (ep[edge] > x_ep and to[edge] <= x_to):
            relations[edge] = bridges
        else:
            relations[edge] = random

    return relations

def combine_relations(relations):
    d_r = dict()

    d_r[friends] = 0
    d_r[acquaintance] = 0
    d_r[bridges] = 0
    d_r[random] = 0
    
    for edge, relation in relations.iteritems():
        d_r[relation] += 1

    return d_r

def combine_relations_with_time(relations):
    d_r = dict()

    d_r['ppp'] = 0
    d_r['ppn'] = 0
    d_r['pnp'] = 0
    d_r['pnn'] = 0

    d_r['npp'] = 0
    d_r['npn'] = 0
    d_r['nnp'] = 0
    d_r['nnn'] = 0
    
    for edge, relation in relations.iteritems():
        d_r[relation] += 1

    return d_r
    
def main():
    return 0
    
if __name__ == '__main__':
    main()   
