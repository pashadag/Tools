#cat jcv/$1.table | awk '{ print $10 }' | uniq -c | awk '{ if ($1 > 1) print $2 }' | sort > jcv/$1.multitons
cat jcv/$1.table | awk '{if ($4 > 1) print $10 }' | uniq > jcv/$1.multitons
cat jcv/$1.im | awk '{ print "$14_F3 F" "\t" $8 "\t" $9 "\t" $0}' > jcv/$1.temp1 
cat jcv/$1.im | awk '{ print "$14_R3 R" "\t" $11 "\t" $12 "\t" $0}' > jcv/$1.temp2
cat jcv/$1.temp1 jcv/$1.temp2 | sort -k1,1  > jcv/$1.temp3
join -t " " jcv/$1.temp3 jcv/$1.multitons | ./drop_first_col | sort -k2,2 -k3n,3  > jcv/$1.im.mul.ord
#./graph_builder jcv_chr20.ini bla build_read_classes
