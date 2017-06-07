import sys
import os

import gen_graphs as gg
from recast_base import *

if __name__ == '__main__':
    # these are hardcoded info
    # you don't need to change them
    python_file = 'gen_graphs.py'
    p_rnd = [1e-3, 1e-4, 1e-5, 1e-6,]

    # python src/gen_graphs.py filename
    if (len(sys.argv) < 2):
        s = 'python src/recast_edgelist.py trace'
        sys.exit (s)

    #############################################

    in_folder = sys.argv[1]
    trace = os.path.basename(os.path.normpath(in_folder))

    im_folder_ep = 'images' + os.sep + 'recastd' + os.sep + trace + os.sep + 'ep'
    im_folder_to = 'images' + os.sep + 'recastd' + os.sep + trace + os.sep + 'to'
    im_folder_classifier = 'images' + os.sep + 'recastd' + os.sep + trace + os.sep + 'classifier'

    out_folder = 'recastd_class/' + trace
    
    if not os.path.exists(im_folder_ep):
        os.makedirs(im_folder_ep)
    if not os.path.exists(im_folder_to):
        os.makedirs(im_folder_to)
    if not os.path.exists(im_folder_classifier):
        os.makedirs(im_folder_classifier)
    if not os.path.exists(out_folder):
        os.makedirs(out_folder)

    # folder = 'recast_class' + os.sep + trace_name
    # folder_images = 'images' + os.sep + 'recast' + os.sep + trace_name + os.sep

    for f in os.listdir(in_folder):
        filename = os.path.join(in_folder, f)
        node_id = os.path.split(filename)[1].split('.')[0]
        sys.argv = [python_file, filename]

        gg.main()

        ep = get_ep(gg)
        random_ep = get_random_ep(gg)
        to = get_to(gg)
        random_to = get_random_to(gg)

        (friends, bridges, acquaintance, random) = map_relations(ep, random_ep, to, random_to, p_rnd) 

        # plot_ep(ep, random_ep, fname=im_folder_ep + os.sep + node_id + '.png')
        # plot_to(to, random_to, fname=im_folder_to + os.sep + node_id + '.png')
        # plot_relations (p_rnd, friends, bridges, acquaintance, random, 
                        # fname=im_folder_classifier + os.sep + node_id + '.png')

        save_edges (gg, ep, random_ep, to, random_to, p_rnd,
                    fname=out_folder + os.sep + node_id + '_', ext='.txt',
                    recastd=True)

#     #############################################

#         plot_ep(ep, random_ep, fname=folder_images + os.sep + 'ep.png')
#         plot_to(to, random_to, fname=folder_images + os.sep + 'to.png')
#         plot_relations(p_rnd, friends, bridges, acquaintance, random, fname=folder_images + os.sep + 'rel.png')

#         save_edges(gg, ep, random_ep, to, random_to, p_rnd, fname= folder + os.sep, ext='.txt')


# if __name__ == '__main__':
    

#     folder = in_folder

#     for f in os.listdir(folder):

#         filename = sys.argv[1]
#         path = os.path.split(filename)[0]
#         trace_name = os.path.split(filename)[1].split('.')[0]

#         sys.argv = [python_file, trace]
        
#         recast.main()
#         edge_persistence(figs_ep + os.sep + trace_name + '.png', save=False)
#         topological_overlap(figs_to + os.sep + trace_name + '.png', save=False)
#         save_edges(fname=out_folder + os.sep + trace_name, ext='.txt')
#         classify_results(figs_classifier + os.sep + trace_name + '.png', save=False)


#         plot_ep(ep, random_ep, fname=folder_images + os.sep + 'ep.png')
#         plot_to(to, random_to, fname=folder_images + os.sep + 'to.png')
#         plot_relations(friends, bridges, acquaintance, random, fname=folder_images + os.sep + 'rel.png')

#         save_edges(gg, random_ep, random_to, p_rnd, fname= folder + os.sep, ext='.txt')





# python_file = 'recast.py'

# trace = sys.argv[1]






    
    

