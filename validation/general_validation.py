#===============================================================================
# IMPORT
#===============================================================================

import sys
import os
import math

#===============================================================================
# CLASS
#===============================================================================

# an interval object
class interval:
    
    # constructor
    def __init__(self,chr,start,end,ref):
        self.chr = chr
        self.start = int(start)
        self.end = int(end)
        self.ref = ref
    # end def
    
    # length of this interval
    def length(self):
        return self.end-self.start+1
    # end
    
    # a "toString" method
    def __str__(self):
        return self.chr + ' ' + str(self.start) + ' ' + str(self.end) + ' ' + str(self.length()) + ' ' + str(self.ref)
    # end
# end

#===============================================================================
# FUNCTIONS
#===============================================================================

def mean(x):
    
    sum = 0.0
    
    for el in x:
        sum += el
    # end
    
    return sum/len(x)
    
# end

def stdev(x):
    
    mean_x = mean(x);
    
    sum = 0.0
    for el in x:
        sum += (el-mean_x)*(el-mean_x)
    # end
    stdev_x = math.sqrt(sum/len(x))
    
    return mean_x, stdev_x
# end


def zscore(x):
    
    tmp = []
    
    mean_x, std_x = stdev(x)
    
    for el in x:
        tmp.append((el-mean_x)/std_x)
    # end
    
    return tmp    
# end

# compare source intervals with target intervals
def compare(source, target, out_file, missed_out_file, direction):
    
    # number of elements in 
    # source which overlap target
    overlaps = []
    
#===============================================================================
#    xx = []; ## TODO DEL
#    yy = []; ## TODO DEL
#    
#===============================================================================

    sum_xy = 0.0
    sum_x = 0.0
    sum_y = 0.0
    
    sum_x_squared = 0.0
    sum_y_squared = 0.0

    # go through each element in source
    for s in source:
        
        diff = -1
        best_overlapping_interval = None
        
        # go through each element in target
        for t in target:
            
            # check for overlap and print if so
            if overlap(s,t) != 0:
                this_diff = abs(t.length()-s.length())
                if ( diff == -1 ) or ( this_diff < diff ):
                        diff = this_diff
                        best_overlapping_interval = t
                    # end
                # ends
            # end
        # end
        
        if diff != -1:
            
            sum_x += s.length()
            sum_y += best_overlapping_interval.length()
            
            sum_xy += s.length()*best_overlapping_interval.length()
            
            sum_x_squared += s.length()*s.length()
            sum_y_squared += best_overlapping_interval.length()*best_overlapping_interval.length()
            
            ## TODO DEL            
#===============================================================================
#            xx.append(s.length())
#            yy.append(best_overlapping_interval.length())
#===============================================================================
            
            s1 = ''
            s2 = ''
            
            if direction == 'p2v':
                s1 = str(s)
                s2 = str(best_overlapping_interval)
            elif direction == 'v2p':
                s2 = str(s)
                s1 = str(best_overlapping_interval)
            # end
            
            out_file.write( str(overlap(s,best_overlapping_interval)) + ' ' + s1 + ' ' + s2 + '\n')
            overlaps.append(best_overlapping_interval)
        else:
            missed_out_file.write(str(s) + '\n')
        # end
        
    # end
    
    r_score = 0
    
    ## TODO DEL
#===============================================================================
#    if len(overlaps) != 0: 
#   
#        try: 
#            z_x=zscore(xx);
#            z_y=zscore(yy);
#            
#            sum_z = 0
#            for i in range(0,len(xx)):
#                sum_z += z_x[i]*z_y[i]
#            # end
#            r_score = sum_z/len(xx)
#            r_score = r_score*r_score
#        except:
#            r_score = 0
#        # end
#    # end
#===============================================================================
    
    n = len(overlaps)
    
    try:
        a = (n*sum_x_squared)-(sum_x*sum_x)
        b = (n*sum_y_squared)-(sum_y*sum_y)
        
        r_score = (n*sum_xy - sum_x*sum_y) / ( math.sqrt(a) * math.sqrt(b) )
   
    except:
        r_score = 0
    # end
    
    r_score = r_score*r_score
    
    return overlaps, r_score
# end

# test if intervals a and b (return the
# length of the interval)
def overlap(a,b):
    
    # stop if chromosomes are different
    if a.chr != b.chr: 
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
    
    # return the length of the overlap, if any
    if first.end >= second.end:
        return second.length()
    elif first.end >= second.start: 
        return first.end - second.start + 1
    else: 
        return 0
    # end
# end

#===============================================================================
# PROCEDURES
#===============================================================================

# check if there are enough input arguments
if len(sys.argv) < 4:
    print "USAGE: python general_validation.py <predictions> <validations> <tag> {print_pdfs}"
    sys.exit()
# end

print 'CNV Validation'
print '--------------'
print
print 'Settings:'

# open the prediction and validation files

pred_file = None
valid_file = None

print_pdfs = 0
try:
    if len(sys.argv) == 5:
        print_pdfs = sys.argv[4]
	#print "Dude!:" + print_pdfs + ":did you see that?" 
    # end
except:
    pass
# end 

try:
    pred_file = open(sys.argv[1])
    valid_file = open(sys.argv[2])
except:
    print 'error opening files'
    sys.exit()
# end

print '\tpredictions file : ' + sys.argv[1]
print '\tvalidations file : ' + sys.argv[2]
print

tag = sys.argv[3]

# indices for chromosome, start, end, and ref
# column indicies
chr_ind = 0
start_ind = 1
end_ind = 2
# ref_ind = 3

# arrays to store prediction and validation intervals
validations = []
predictions = []

# make intervals for all predictions
for line in pred_file:
    l = line.split()
    ref = 'Medvedev(2009)'
    try:
        pass
        #ref = l[ref_ind]
    except:
        pass
    # end
    predictions.append(interval(l[chr_ind],l[start_ind],l[end_ind],ref))
# end

# make intervals for all validations
for line in valid_file:
    l = line.split()
    #validations.append(interval(l[chr_ind],l[start_ind],l[end_ind],l[ref_ind]))
    validations.append(interval(l[chr_ind],l[start_ind],l[end_ind],os.path.basename(sys.argv[2])))
# end

if len(validations) == 0 or len(predictions) == 0:
    print 'error: one of the files is empty'
    sys.exit()
# end

# make room for the results
p2v_file = open('results/' + tag + '.p2v.val','w')
missed_p2v_file = open('results/' + tag + '.p2v.missed.val','w')
v2p_file = open('results/' + tag + '.v2p.val','w')
missed_v2p_file = open('results/' + tag + '.v2p.missed.val','w')

print 'Comparing predictions --> validations'
overlaps1, r_score1 = compare(predictions,validations,p2v_file,missed_p2v_file,'p2v')
p2v_file.close()
missed_p2v_file.close()
print 'Done\n'

print 'Comparing validations --> predictions'
overlaps2, r_score2 = compare(validations,predictions,v2p_file,missed_v2p_file,'v2p')
v2p_file.close()
missed_v2p_file.close()
print 'Done\n'

true_positives = len(overlaps1)
tp_percentage = float(str(float(true_positives)*100/len(predictions))[0:5])
false_negatives = len(validations) - len(overlaps2)
fn_percentage = float(str(float(false_negatives)*100/len(validations))[0:5])

print 'Results: '
print '\t Predictions --> Validations:' 
print '\t\t Number of prediction intervals: ' + str(len(predictions))
print '\t\t True positives: ' + str(true_positives) + ' Percentage: ' + str(tp_percentage) + '%'
print '\t\t ?False? (not-validated) positives: ' + str(len(predictions)-true_positives)
print '\t\t R-Score: ' + str(r_score1)
print
print '\t Validations --> Predictions:'
print '\t\t Number of validation intervals: ' + str(len(validations))
print '\t\t ?True? (coincide w\ predictions) positives: ' + str(len(validations)-false_negatives)
print '\t\t False negatives: ' + str(false_negatives) + ' Percentage: ' + str(fn_percentage) + '%'
print '\t\t R-Score: ' + str(r_score2)
print
print '** to parse output, pipe into : grep "hash symbol" **'
print '#\tpreds\ttp\ttp%\t(p-tp)\tr_1^2\t#vals\t(v-fn)\tfn\tfn%\tr_2^2'
print '#\t' + ( str(len(predictions)) + '\t' + str(true_positives) + '\t' + str(tp_percentage) + '\t' + str(len(predictions)-true_positives) 
    + '\t' + str(r_score1)[0:6] + '\t' + str(len(validations)) + '\t' + str(len(validations)-false_negatives) + '\t' + str(false_negatives) + '\t' + str(fn_percentage)
    + '\t' + str(r_score2)[0:6] )
print

if print_pdfs:
    print 'Printing graphs with MATLAB:'
    
    try:
        print '\tprinting to ./' + p2v_file.name + '..[format]'
        os.popen('/opt/matlab/bin/matlab -nosplash -nodesktop -r "cnv_call_size_correlation(\'./' + p2v_file.name + '\', 0, 0)"')
        
        print '\tprinting to ./' + v2p_file.name + '.[format]'
        os.popen('/opt/matlab/bin/matlab -nosplash -nodesktop -r "cnv_call_size_correlation(\'./' + v2p_file.name + '\', 0, 0)"')
        print '\tprinting to ./' + v2p_file.name + '.' + print_pdfs + '.[format]'
        os.popen('/opt/matlab/bin/matlab -nosplash -nodesktop -r "cnv_call_size_correlation(\'./' + v2p_file.name + '\', ' + print_pdfs + ', ' + print_pdfs +  ')"')
        print 'Graphs printed.'
    except:
        print 'Printing graphs failed.'
    # end
# end
