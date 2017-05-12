## Filename: ecdf.py

import numpy as np
import matplotlib.pyplot as plt

# a = [1, 2, 3, 4, 4, 5]
# b = ECDF(a)
# b.plot()
# print b(5), b(2)
    
def calc_ecdf(observations, linspace=None):
    observations = np.array(observations)

    observations.sort()

    def calc (x):
        counter = len(observations) - np.searchsorted(observations, x)
        # print "call with x = {} -.- counter = {} and return value = {}".format (x, counter, ((counter + 0.0) / len(observations)))
        return (float(counter) / len(observations))

    if (linspace is None):
        X = np.linspace(0.0, 1.0, 100)
    else:
        X = linspace

    ret = []
    for x in X:
        ret.append(calc(x))

    return ret