
#===============================================================================
# IMPORT
#===============================================================================

import os
import sys
import exceptions
import math

#===============================================================================
# CLASSES
#===============================================================================

# an interval object
class interval:
    
    # constructor
    def __init__(self,chr,start,end,pred):
        self.chr = chr
        self.start = int(start)
        self.end = int(end)
        self.pred = float(pred)
        self.number = 1
        self.confidence = -1
        
        if self.pred < 0:
            self.pred = -1
        else:
            self.pred = 1
        # end
    # end def
    
    # length of this interval
    def length(self):
        return self.end-self.start+1
    # end
    
    # a "toString" method
    def __str__(self):
        global include_confidence_values
        
        if (include_confidence_values):
            return self.chr + ' ' + str(self.start) + ' ' + str(self.end) + ' ' + str(self.pred) + ' ' + str(self.confidence)
        else:
            return self.chr + ' ' + str(self.start) + ' ' + str(self.end) + ' ' + str(self.pred)  
        # end
    # end
# end

#===============================================================================
# FUNCTIONS
#===============================================================================

def sign(num):
    if num < 0:
        return -1
    else:
        return 1
    # end
# end

def span(f,s):
    return (max(s.end,f.end)-min(s.start,f.start))
# end

def gap(f,s):
    return max(0,s.start-f.end)
# end

# test if intervals a and b
def overlap(a,b,forgiveness):
    
    # stop if chromosomes are different
    if (a.chr != b.chr) or (sign(a.pred) != sign(b.pred)): 
        return 0
    # end
    
    # set 'first' and 'second' appropriately
    first = None; second = None
    if a.start <= b.start: 
        first = a
        second = b
    else: 
        first = b
        second = a
    # end
    
    if (span(first,second)*forgiveness >= gap(first,second)):
        return 1
    else:
        return 0
    # end
# end

#===============================================================================
# FUNCTIONS
#===============================================================================

def merge(this_interval,last_interval):
    
    global include_confidence_values
    
    new_start = min(this_interval.start,last_interval.start)
    new_end = max(this_interval.end,last_interval.end)

    merged = interval(this_interval.chr,new_start,new_end,this_interval.pred)
    merged.number = this_interval.number + last_interval.number
    
    if (include_confidence_values):
        merged.confidence = (this_interval.number*this_interval.confidence + last_interval.number*last_interval.confidence)/float(merged.number)
    # end
    
    return merged
# end

#===============================================================================
# PROCEDURES
#===============================================================================

chr_lengths = [ 3080419480, #whole genome
247249719,      #chr 1
242951149,      #chr 2
199501827,      #...
191273063,
180857866,
170899992,
158821424,
146274826,
140273252,
135374737,
134452384,
132349534,
114142980,
106368585,
100338915,
88827254,
78774742,
76117153,
63811651,
62435964,
46944323,
49691432,
154913754,      #...
57772954        #chr 24
            ]

if len(sys.argv) != 3:
    print 'USAGE: python merge_close_intervals_sorted.py <sorted_interval_file> <forgiveness (%)>'
    sys.exit()
# end

forgiveness = 0
include_confidence_values = 0

try:
    forgiveness = float(sys.argv[2])/100
except:
    print 'error parsing forgiveness'
# end

interval_file = None
chr = 0
try:
    interval_file = open(sys.argv[1],'r')
except:
    print 'error: parsing input arguments'
    sys.exit()
# end

nonredundant_intervals = []

# make intervals for all predictions
num = 0
last_interval = None

line = '-'
while (line != ''):
    
    # take an interval off the interval stack
    line = interval_file.readline()

    l = line.replace('\n','')
    l = line.replace('\r','')
    l = line.split()

    if (len(l) == 0):
        break
    # end

    # make an interval
    this_interval = interval(l[0],l[1],l[2],l[3])

    if (num == 0 and len(l) > 4):
        include_confidence_values = 1
    # end

    if (include_confidence_values):
        this_interval.confidence = float(l[4])
    # end

    if num == 0:
        nonredundant_intervals.append(this_interval)
        num += 1
        
    else:
        
        last_interval = nonredundant_intervals[num-1]
        
        if (overlap(this_interval,last_interval,forgiveness)):
            nonredundant_intervals.pop()
            new_interval = merge(this_interval,last_interval)
            nonredundant_intervals.append(new_interval)
        else:
            nonredundant_intervals.append(this_interval)
            num += 1
        # end
    # end   
# end

for nri in nonredundant_intervals:
    print str(nri)
# end

