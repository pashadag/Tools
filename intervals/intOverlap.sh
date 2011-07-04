#!/bin/sh

FILE1=$1
FILE2=$2

# Both files must be sorted interval files.
# Calculate the regions of overlap between the two files
# output: length of overlap, %overlap relative to first file, %overlap relative to second file
cat $FILE2 | intJoin > $$.2j
cat $FILE1 $FILE2 | sort -k1,1 -k2n,2 | intJoin | intSub $$.2j > $$.1s
LEN1=`calc_smother $FILE1 all | awk '{ print $3 }'`
LEN1M2=`calc_smother $$.1s all | awk '{ print $3 }'`
LENOVERLAP=`echo $LEN1 - $LEN1M2 | bc`
LEN2=`calc_smother $FILE2 all | awk '{ print $3 }'`
P1=`echo "scale=2; $LENOVERLAP / $LEN1 " | bc`
P2=`echo "scale=2; $LENOVERLAP / $LEN2 " | bc`
echo "$LENOVERLAP $P1 $P2"
#rm -rf $$.2j $$.1s

