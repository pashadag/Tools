#!/bin/bash
IFS=$'\n'
cat $1 | awk '{ print $1, $2, $3 }' > a.blob
cat $2 | awk '{ print $1, $2, $3 }' > b.blob
cat $3 | awk '{ print $1, $2, $3 }' > c.blob

FILEa=a.blob
FILEb=b.blob
FILEc=c.blob

a=`cat $FILEa | bestOverlap $FILEb full2 | awk '{ if ($6 == 0) print $0 }' | bestOverlap $FILEc full2 | awk '{ if ($9 == 0) print $0 }' | wc -l`
b=`cat $FILEb | bestOverlap $FILEa full2 | awk '{ if ($6 == 0) print $0 }' | bestOverlap $FILEc full2 | awk '{ if ($9 == 0) print $0 }' | wc -l`
c=`cat $FILEc | bestOverlap $FILEb full2 | awk '{ if ($6 == 0) print $0 }' | bestOverlap $FILEa full2 | awk '{ if ($9 == 0) print $0 }' | wc -l`

ab=`cat $FILEa | bestOverlap $FILEb full3 | awk '{ if ($6 != 0) print $0 }' | bestOverlap $FILEc full3 | awk '{ if ($9 == 0) print $0 }'  | wc -l`
ba=`cat $FILEb | bestOverlap $FILEa full3 | awk '{ if ($6 != 0) print $0 }' | bestOverlap $FILEc full3 | awk '{ if ($9 == 0) print $0 }'  | wc -l`
bc=`cat $FILEb | bestOverlap $FILEc full3 | awk '{ if ($6 != 0) print $0 }' | bestOverlap $FILEa full3 | awk '{ if ($9 == 0) print $0 }' | wc -l`
cb=`cat $FILEc | bestOverlap $FILEb full3 | awk '{ if ($6 != 0) print $0 }' | bestOverlap $FILEa full3 | awk '{ if ($9 == 0) print $0 }' | wc -l`
ca=`cat $FILEc | bestOverlap $FILEa full3 | awk '{ if ($6 != 0) print $0 }' | bestOverlap $FILEb full3 | awk '{ if ($9 == 0) print $0 }' | wc -l`
ac=`cat $FILEa | bestOverlap $FILEc full3 | awk '{ if ($6 != 0) print $0 }' | bestOverlap $FILEb full3 | awk '{ if ($9 == 0) print $0 }' | wc -l`

abc=`cat $FILEa | bestOverlap $FILEb full3 | awk '{ if ($6 != 0) print $0 }' | bestOverlap $FILEc full3 | awk '{ if ($9 != 0) print $0 }' | wc -l`
bac=`cat $FILEb | bestOverlap $FILEa full3 | awk '{ if ($6 != 0) print $0 }' | bestOverlap $FILEc full3 | awk '{ if ($9 != 0) print $0 }' | wc -l`
cba=`cat $FILEc | bestOverlap $FILEb full3 | awk '{ if ($6 != 0) print $0 }' | bestOverlap $FILEa full3 | awk '{ if ($9 != 0) print $0 }' | wc -l`

abavg=`echo "scale=2; ( $ab + $ba ) / 2" | bc`
bcavg=`echo "scale=2; ( $bc + $cb ) / 2" | bc`
caavg=`echo "scale=2; ( $ca + $ac ) / 2" | bc`

abcavg=`echo "scale=2; ( $abc + $bac + $cba ) / 3" | bc`

echo a $a
echo b $b
echo c $c
echo ab $ab $ba avg $abavg
echo bc $bc $cb avg $bcavg
echo ca $ca $ca avg $caavg
echo abc $abc $bac $cba avg $abcavg

echo Plug these into http://www.cs.kent.ac.uk/people/staff/pjr/EulerVennCircles/EulerVennApplet.html
exit


a=`cat $FILEa | wc -l`
b=`cat $FILEb | wc -l`
c=`cat $FILEc | wc -l`
