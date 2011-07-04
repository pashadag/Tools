#!/bin/bash
IFS=$'\n'

if [ $# -ne 2 ]; then
	echo $0 chromosome modulus
	exit
fi


for line in `cat ${SVN_BASE}/cnv/chrom_list`
do

done

$0 $RUN ${CONTIG}

if [ "${MODE}" = "debut" ]; then

fi

echo 4 + 5 = $((4 + 5))
echo 7 - 7 = $[ 7 - 7 ]
echo 4 x 6 = $((3 * 2))
echo 6 / 3 = $((6 / 3))
echo 8 % 7 = $((8 % 7))
echo 2 ^ 8 = $[ 2 ** 8 ]

echo "scale=2; $1 * $2" | bc 


NUM1=2
NUM2=1
if [ $NUM1 -eq $NUM2 ]; then
#	lt < 	gt > 	le <= 	ge >= 	eq == 	ne !=
	echo "Both Values are equal"
else 
	echo "Values are NOT equal"
fi 

until [ -z "$1" ]  # Until all parameters used up...
do
	echo -n "$1 "
	shift
done

