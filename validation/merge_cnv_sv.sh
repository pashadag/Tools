#!/bin/bash

IFS=$'\n'
RUN=$1
CONTIG=$2
TOL=$3

if [ "${CONTIG}" = "every" ]
then
	for line in `cat ${SVN_BASE}/cnv/chrom_list`
	do
		CONTIG=chr${line}
		echo Doing ${CONTIG}...
		$0 $RUN ${CONTIG} ${TOL}
	done
else
	if [ "${CONTIG}" = "all" ]
	then
		#bring all the dgs together
		rm -Rf ${SVN_BASE}/cnv/results/all/working.${RUN}/all.dgs
		for line in `cat ${SVN_BASE}/cnv/chrom_list`
		do
			CONTIG=`echo ${line} | awk '{print "chr" $1}'`
			cat ${SVN_BASE}/cnv/results/${CONTIG}/working.${RUN}/${CONTIG}.dgs >> ${SVN_BASE}/cnv/results/all/working.${RUN}/all.dgs
		done
		CONTIG=all
	fi

	WORKING_DIR=${SVN_BASE}/cnv/results/${CONTIG}/working.${RUN}
	DGFILE=${WORKING_DIR}/${CONTIG}.dgs
	SVFILE=${WORKING_DIR}/${CONTIG}.svs
	CNSVSFILE=${WORKING_DIR}/${CONTIG}.cnsvs

	MAXCNV=50000
	MAXSV=3000

	#first double every edge
	#then replace the 0-3 edge type with a "+ -" type of system

 cat ${DGFILE} | awk '{ print $0; if ($5 == 0) print $1, $2, $4, $3, "1", $6; if ($5 == 1) print $1, $2, $4, $3, "0", $6; if (($5 == 2) || ($5 == 3)) print $1, $2, $4, $3, $5, $6; }' | awk '{ if ($5 == 0) $5 = "+ -"; else if ($5 == 1) $5 = "- +"; else if ($5 == 2) $5 = "+ +"; else if ($5 == 3) $5 = "- -"; print $0 }' |   sort -k2,2 -k3n,3 -k4n,4 | sbsJoin2 | awk -v maxcnv=${MAXCNV} -v maxsv=${MAXSV} '{ if ((($10 - $3) < maxsv) && ($5 != $12) && ($11 != $3) && (($11 - $4) < maxcnv) && (($4 - $11) < maxcnv)) print $2, $3, $10, $2, $4, $11 }' | tee ${CNSVSFILE} | awk '{ print $1, $2, $3 }' > ${SVFILE}

 #cat dude | sbsJoin2 | awk '{ if ((($10 - $3) < 500) && ($5 != $12) && ($11 != $3) && (($11 - $4) < 10000) && (($4 - $11) < 10000)) print $2, $3, $10, $4, $11 }' | less


fi

