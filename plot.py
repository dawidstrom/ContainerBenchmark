#!/usr/bin/python

import numpy as np
import csv
import matplotlib.pyplot as plt


# Assumes files are in same directory as where this is being run from.
def getValues(*args):
    def readFile(filename):
        values = []
        
        with open(filename) as csvfile:
            alltext = csv.reader(csvfile)
            for i, row in enumerate(alltext):
                values.append(float(row[0]))
        return values

    return (readFile(arg) for arg in args)

# Assumes len(numerators) == len(denominators).
# Numerator / Denominators.
def factor(numerators, denominators):
    return [numerators[i]/denominators[i] for i in range(len(denominators))]

# Assumes len(base) > 0, len(lists) > 0, and same length for base and each list.
def printFactor(ax, x_axis, bases, lists):
    line, = ax.plot(x_axis, [1 for i in range(len(x_axis))])

    for i in range(len(lists)):
        listFactor = factor(bases[i], lists[i])
        line, = ax.plot(x_axis, listFactor)


# Load values.
list10, list30, list100 = getValues('list10', 'list30', 'list100')

copy10 = getValues('copy10')
copy30 = getValues('copy30')
copy100 = getValues('copy100')

swap10 = getValues('swap10')
swap30 = getValues('swap30')
swap100 = getValues('swap100')

insert10 = getValues('insert10')
insert30 = getValues('insert30')
insert100 = getValues('insert100')

print(insert10)
print(insert10[0])
print(insert10[0][0])

# Create figure.
fig = plt.figure()
ax = fig.add_subplot(1,1,1)

# For these specific files I used the range 2^10..2^18.
x_axis = [2**(10+x) for x in range(10,18)]
    
printFactor(ax, x_axis, [list10, list30, list100], [insert10, insert30, insert100])
#printFactor(ax, x_axis, list30, [copy30, swap30, insert30])
#printFactor(ax, x_axis, list100, [copy100, swap100, insert100])

#ax.set_yscale('log')
ax.set_xscale('log')

plt.show()
