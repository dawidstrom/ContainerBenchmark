#! /usr/bin/python

import numpy as np
import csv
import matplotlib.pyplot as plt

n = [[],[]]

# Assumes
with open('../Debug/benchmarkCopy10') as csvfile:
    alltext = csv.reader(csvfile)
    for i, row in enumerate(alltext):
        n[i%2].append(float(row[0]))

print(n[0])
print(n[1])


fig = plt.figure()

ax = fig.add_subplot(1,1,1)

size = len(n[0])

factor = [n[0][x]/n[1][x] for x in range(len(n[0]))]

#line, = ax.plot([2**(10+x) for x in range(len(factor))], factor)
line, = ax.plot([2**(10+x) for x in range(size)], n[0][:size])
line, = ax.plot([2**(10+x) for x in range(size)], n[1][:size])

ax.set_yscale('log')
ax.set_xscale('log')
#ax.set_ylim(-1,1);

plt.show()

print(factor)
