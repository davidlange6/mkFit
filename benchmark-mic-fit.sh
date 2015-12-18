#! /bin/bash

sed -i 's/constexpr int nTracks = 20000/constexpr int nTracks = 20000/g' Config.h 

make clean
make -j 8

for nth in 1 3 7 21 42 63 84 105 126 147 168 189 210
do
echo nth=${nth}
ssh mic0 ./mkFit-mic --fit-std-only --num-thr ${nth} >& log_mic_10x20k_FIT_NVU8_NTH${nth}.txt
done

#sed -i 's/define MPT_SIZE 16/define MPT_SIZE XX/g' Config.h
#for nvu in 1 2 4 8
#do
#echo nvu=${nvu}
#sed -i "s/define MPT_SIZE XX/define MPT_SIZE ${nvu} \/\/tmp/g" Config.h
#make clean
#make -j 8
#ssh mic0 ./mkFit-mic --fit-std-only --num-thr 1 >& log_mic_10x20k_FIT_NVU${nvu}_NTH1.txt
#sed -i "s/define MPT_SIZE ${nvu} \/\/tmp/define MPT_SIZE XX/g" Config.h
#done

#sed -i "s/define MPT_SIZE XX/define MPT_SIZE 16/g" Config.h

sed -i 's/constexpr int nTracks = 20000/constexpr int nTracks = 20000/g' Config.h 

make clean
make -j 8
