import sys
import os

import gen_graphs as gg
from recast_base import *


if __name__ == '__main__':
    # these are hardcoded info
    # you don't need to change them
    python_file = 'gen_graphs.py'
    p_rnd = [1e-2, 1e-3, 1e-4, 1e-5, 1e-6,]

    # python src/gen_graphs.py filename
    if (len(sys.argv) < 2):
        s = 'python recast_edgelist.py trace'
        sys.exit (s)

    filename = sys.argv[1]
    path = os.path.split(filename)[0]
    trace_name = os.path.split(filename)[1].split('.')[0]
    
    sys.argv = [python_file, filename]

    gg.main()

    ep = get_ep(gg)
    random_ep = get_random_ep(gg)
    to = get_to(gg)
    random_to = get_random_to(gg)

    (friends, bridges, acquaintance, random) = map_relations(ep, random_ep, to, random_to, p_rnd)

    folder = 'recast_class' + os.sep + trace_name
    folder_images = 'images' + os.sep + 'recast' + os.sep + trace_name + os.sep

    if not os.path.exists(folder):
        os.makedirs(folder)
    if not os.path.exists(folder_images):
        os.makedirs(folder_images)
    
    plot_clustering(gg.temporal_graphs,
            gg.random_graphs,
            fname=folder_images + os.sep + 'cc.eps',
            name=trace_name)
    plot_ep(ep, random_ep, fname=folder_images + os.sep + 'ep.eps', trace_name=trace_name)
    plot_to(to, random_to, fname=folder_images + os.sep + 'to.eps', trace_name=trace_name)
    plot_relations(p_rnd, friends, bridges, acquaintance, random, fname=folder_images + os.sep + 'rel.eps', 
        trace_name=trace_name)

    save_edges(gg, ep, random_ep, to, random_to, p_rnd, fname= folder + os.sep, ext='.txt')
