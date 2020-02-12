#! /usr/bin/python

import numpy as np
import csv
import matplotlib.pyplot as plt

n = [[],[]]

# Assumes
with open('benchmark') as csvfile:
    alltext = csv.reader(csvfile)
    for i, row in enumerate(alltext):
        n[i%2].append(row[0])

print(n[0])
print(n[1])

fig = plt.figure()

ax = fig.add_subplot(1,1,1)

line, = ax.plot([2**(10+x) for x in range(len(n[0]))], n[0])
line, = ax.plot([2**(10+x) for x in range(len(n[1]))], n[1])

#ax.set_yscale('log')
ax.set_xscale('log')

plt.show()
