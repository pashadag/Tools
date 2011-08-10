#!/bin/bash
IFS=$'\n'
PRED_BASE=$1
CONTIG=$2
CB=$3
EXT=${CB}_${CONTIG}

#for dgv comparison, play with this
#VAL_FILES="/home/pashadag/allbval/all.cnvs.loss.cnvs"
#PRED_ALL="/home/pashadag/cnv/datasets/dgv_all"
#PRED_LOSS="/home/pashadag/cnv/datasets/dgv_all.loss"
#PRED_GAIN="/home/pashadag/cnv/datasets/dgv_all.gain"

VAL_FILES="kidd_loss$EXT bentley1$EXT mcc.0.s.j.cnvs$EXT mcc.min$EXT mcc.max$EXT"
PRED_ALL=${PRED_BASE}.merged.cnvs
PRED_LOSS=${PRED_BASE}.loss.cnvs
PRED_GAIN=${PRED_BASE}.gain.cnvs
VAL_DIR=${SVN_BASE}/cnv/datasets/

echo "FILE	LOSS_ONLY	GAIN_ONLY	BOTH	NONE"

for VAL in `echo $VAL_FILES | tr ' ' '\n'`
do
	NONE=`cat ${VAL_DIR}/$VAL | bestOverlap $PRED_ALL full4 | awk '{ if ($NF == 0) print $0 }' | wc -l`
	BOTH=`cat ${VAL_DIR}/$VAL | bestOverlap $PRED_LOSS full4 | awk '{ if ($NF != 0) print $0 }' | bestOverlap $PRED_GAIN full4 | awk '{ if ($NF != 0) print $0 }' | wc -l`
	GAIN_ONLY=`cat ${VAL_DIR}/$VAL | bestOverlap $PRED_GAIN full4 | awk '{ if ($NF != 0) print $0 }' | bestOverlap $PRED_LOSS full4 | awk '{ if ($NF == 0) print $0 }' | wc -l`
	LOSS_ONLY=`cat ${VAL_DIR}/$VAL | bestOverlap $PRED_LOSS full4 | awk '{ if ($NF != 0) print $0 }' | bestOverlap $PRED_GAIN full4 | awk '{ if ($NF == 0) print $0 }' | wc -l`
	echo $VAL $LOSS_ONLY $GAIN_ONLY $BOTH $NONE
done


