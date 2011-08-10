#===============================================================================
# IMPORT
#===============================================================================

import sys
import random

#===============================================================================
# CONSTANTS / GLOBALS
#===============================================================================

args_expected = 2

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

#===============================================================================
# FUNCTIONS
#===============================================================================

def usage(argv,args):
    print "usage: python " + argv[0] + " " + args
# end

#===============================================================================
# PROCEDURES
#===============================================================================

args_given = len(sys.argv)

if (args_given != args_expected):
    usage(sys.argv, "<predictions>")
    sys.exit(1)
# end

#open the interval file
int_file = open(sys.argv[1])


for line in int_file:
    
    l = line.replace('\n','')
    l = line.replace('\r','')
    
    l = l.split()
    
    # break apart the line
    chr = l[0]
    c = chr
    
    if 'chr' in c:
        c = chr.replace('chr','')
    
        if c == 'X':
            c = 23
        elif c == 'Y':
            c = 24
        else:
            c = int(c)
        # end
    else:
        c = int(c)
    # end
    
    s = int(l[1])
    e = int(l[2])
    
    length = e - s
    
    lowerbound = 1
    upperbound = chr_lengths[c]-length
    
    new_s = random.randrange(lowerbound, upperbound)
    new_e = new_s + length

    print chr + " " + str(new_s) + " " + str(new_e),
    
    for i in range(3,len(l)):
        print l[i] + " ",
    # end
    
    print
    
# end
    














