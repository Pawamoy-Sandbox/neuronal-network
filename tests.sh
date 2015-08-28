#!/bin/bash


usage() {
	echo "usage: $0 generic_network(g) | subject_optimized(o) ARGS" >&2
	echo "usage: $0 -s, --show-results" >&2
	exit 1
}

showres() {
	local tmp avg=0 min=${3:-1000000} max=0
	for gen in $(cat $1); do
		avg=$(echo $avg + $gen | bc -l)
		[ $gen -lt $min ] && min=$gen
		[ $gen -gt $max ] && max=$gen
	done
	avg=$(echo $avg / $(cat $1 | wc -l) | bc -l)
	tmp=${avg#*.}
	tmp=${tmp/[0]*/}
	echo "Moyenne : ${avg%.*}.$tmp"
	echo "Ecart   : $((max-min))"
}

ftest() {
	local gen
	for ((i=0; i<$iteration; i++)); do
		gen=$($main $1 $2 $3 $4 | head -n1)
		if echo $gen | grep -q '\.'; then
			continue
		fi
		gen=${gen% (*}
		gen=${gen##* }
		echo $gen
		echo $gen >> $output
		sleep 1
	done
	showres $output
}

iteration=10

case $1 in
	o|subject_optimized|./subject_optimized)
		main=./subject_optimized
	;;
	g|generic_network|./generic_network)
		main=./generic_network
	;;
	--show-results|-s)
		for file in result_*; do
			echo "${file//_/ }"
			showres $file
			echo
		done
		exit
	;;
	*) usage ;;
esac

[ -n "$2" ] && param=$2_$3_$4_$5 || param="0.85_0.1_1000000_100"
output=result_${main:2}_$param

ftest $2 $3 $4 $5

wait ${pids[@]}
