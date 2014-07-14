# A poor man's implementation of the statistics module
# found in Python 3.4:
# https://docs.python.org/3/library/statistics.html

import math

def mean(samples):
    return sum(samples) * 1.0 / len(samples)

def median(samples):
    samples = sorted(samples)
    n = len(samples)
    if n%2 == 1:
        return samples[n//2]
    else:
        return (samples[n//2 - 1] + samples[n//2])/2

def stdev(samples):
    avg = mean(samples)
    variance = list(map(lambda x: (x - avg)**2, samples))
    return math.sqrt(mean(variance))

