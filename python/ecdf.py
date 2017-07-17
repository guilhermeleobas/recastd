## Filename: ecdf.py

import numpy as np
import sys
import platform
import os

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

# a = [1, 2, 3, 4, 4, 5]
# b = ECDF(a)
# b.plot()
# print b(5), b(2)
    
def calc_ecdf(observations, linspace=None):
    observations = np.array(observations)

    observations.sort()

    def calc (x):
        counter = len(observations) - np.searchsorted(observations, x)
        return (float(counter) / len(observations))

    if (linspace is None):
        X = np.linspace(0.0, 1.0, 100)
    else:
        X = linspace

    ret = []
    for x in X:
        ret.append(calc(x))

    return ret

def plot_ecdf(xs, ys, fs, name=None, fname=None):

    trace_name = os.path.split(fname)[1].split('.')[0]

    colors = dict()
    colors['Node'] = 'red'
    colors['Neighbors'] = 'green'
    colors['Graph'] = 'blue'

    markers = dict()
    markers['Node'] = 'o'
    markers['Neighbors'] = 'x'
    markers['Graph'] = 's'

    # x = np.linspace(0.0, 1.0, 100)
    
    # y = ecdf.calc_ecdf(flatten_ep)

    # yrange = [0, 10**(-6), 10**(-3), 10**(0)]
    # ylabels = ['10^-6', '10^-3', '10^-0']

    for (x, y, f) in zip(xs, ys, fs):
        plt.plot(x, y, label=f, color=colors[f], marker=markers[f])

    plt.yscale('log', basey=10)
    # plt.xlim(0, 1)
    # plt.ylim ([10**(-7), 10**(0)])

    plt.title("\# Bits enviados " + name)

    plt.ylabel('$P[X > x]$')
    plt.xlabel('$x$ [$\#bits$]')

    plt.legend(bbox_to_anchor=(0.98,0.05), loc=4, borderaxespad=0.)
    # plt.legend()
    plt.grid()
    plt.savefig (fname, format='eps')
    plt.cla()
    # plt.show()

def read_file(filename):
    arr = []
    with open(filename, 'r') as f:
        for line in f:
            arr.append(map(int, line.strip().split()))
    return arr

def get_ecdf(arr):
    flatten_arr = np.array(arr).flatten()
    x = np.linspace(0.0, max(flatten_arr), 100)
    y = calc_ecdf(flatten_arr, linspace=x)
    return (x, y)
    # plot_ecdf(x, y, 'ecdf')

def get_trace(filename):
    f = os.path.basename(os.path.normpath(filename)).\
            split('.')[0].split('_')[1]
    return f[0].upper() + f[1:]

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print 'python ecdf.py filename name'
        sys.exit(1)

    xs = []
    ys = []
    fs = []
    for filename in sys.argv[1:-1]:
        arr = read_file(filename)
        (x, y) = get_ecdf(arr)
        trace = get_trace(filename)
        xs.append(x)
        ys.append(y)
        fs.append(trace)

    name = sys.argv[-1]

    plot_ecdf(xs, ys, fs, name=name, fname='ecdf_' + name + '.eps')
        
