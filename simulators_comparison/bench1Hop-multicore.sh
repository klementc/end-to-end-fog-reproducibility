set -x

flowSize=3125000 # MB
latVal=0
delay=0
execCost=1000 # MIPS
nbFlows=$(seq 1 1 10)
nbCore=$(seq 1 1 2)
resFile="res1Hop-multicore.csv"
IFSPATH="/home/clem/Code/github.com/YAFS"

export LD_LIBRARY_PATH=~/Code/gitlab.com/nsnam/ns-allinone-3.39/ns-3.39/build/lib:~/Code/framagit.org/klement/simgrid_clean/build/lib

echo "simulator,type,nbF,execCost,lat,flowS,start,end,seed,src,dst,nbCore" > ${resFile}
for i in ${nbFlows};
do
  for c in ${nbCore};
  do
  valIFS=` echo "${latVal}/1000.0" | bc -l`
  java -jar IFogSim/iFogSim2_multicore.jar ${i} ${valIFS} ${flowSize} ${delay} ${execCost} ${c} > outIFS.log 2>&1
  awk -f IFogSim/parseIFS.awk -v nbFlows=${i} nbCore=${c} latency=${latVal}ms flowSize=${flowSize} execCost=${execCost} outIFS.log >> ${resFile}
  done

done