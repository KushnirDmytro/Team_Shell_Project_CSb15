#!/bin/bash
#script runs IntegralCalculator 

#options
#-c change directory with executable (default current)
#-t set number of times to run (default 1)

#default options args
directory=$PWD
runsNum=100

#other global variables



#check option args
#args of option should NOT be another option
checkargs () {
if [[ $OPTARG =~ ^-[c/t]$ ]]
then
echo "Option argument check failed!"
echo "Unknow argument $OPTARG for option $opt!"
exit 1
fi
}

#with ':' after option, because args for option is required
while getopts "c:t:" opt
do
case $opt in

c) checkargs 
directory=$OPTARG
echo "The directory was changed to : $OPTARG"
;;

t) checkargs
runsNum=$OPTARG
echo "The number of times to run was changed to : $OPTARG"
;;

*) 
echo "No reasonable options found!"
;;
esac
done


#other functions

run () {
    ($($directory/clientTCP 127.0.0.1 1 &));
}

main () {
    cd $directory
    directory=$PWD
    for i in `seq 0 $[runsNum-1]`;
    do
          outputs[$i]=" $[i+1] - $(run) "
		  echo "Start new client"

    done
}

#main
for i in `seq 0 $[runsNum-1]`; do ($directory/clientTCP 127.0.0.1 -f ./testfile.msh &); done


















