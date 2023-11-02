set -x

flowSize=12500000 # MB
latVal=0
delay=0
execCost=500 # MIPS
nbFlows=$(seq 1 1 10)
resFile="res2Hop-net.csv"
IFSPATH="/home/clem/Code/github.com/YAFS"

export LD_LIBRARY_PATH=~/Code/gitlab.com/nsnam/ns-allinone-3.39/ns-3.39/build/lib:~/Code/framagit.org/klement/simgrid_clean/build/lib

echo "simulator,type,nbF,execCost,lat,flowS,start,end,seed,src,dst" > ${resFile}
for i in ${nbFlows};
do
  valIFS=` echo "${latVal}/1000.0" | bc -l`
  java -jar IFogSim/iFogSim2_2hops.jar ${i} ${valIFS} ${flowSize} ${delay} ${execCost}> outIFS.log 2>&1
  awk -f IFogSim/parseIFS.awk -v nbFlows=${i} latency=${latVal}ms flowSize=${flowSize} execCost=${execCost} outIFS.log >> ${resFile}

  # YAFS
  PREVID=${PWD}
  cd ${IFSPATH}
  cd examples/Tutorial/
  python main1_2hops.py ${i} ${latVal} ${flowSize} ${delay} ${execCost}
  cd ${PREVID}
  awk -F ',' -f YAFS/parseYAFS.awk -v nbFlows=${i} latency=${latVal}ms flowSize=${flowSize} execCost=${execCost} ~/Code/github.com/YAFS/examples/Tutorial/results/sim_trace.csv >> ${resFile}

  ./simgrid/bench_1hop ./simgrid/platform_bench2hops.xml ${i} "${latVal}ms" ${flowSize} ${delay} ${execCost} > outSG.log 2>&1
  awk -f simgrid/parseSG.awk -v nbFlows=${i} latency=${latVal}ms flowSize=${flowSize} execCost=${execCost} outSG.log >> ${resFile}

  ./ns-3/wired_nhop --flowNumbers=${i} --latency=${latVal}ms --flowSize=${flowSize} --DelayBetweenFlows=${delay} --Seed=1 --nbHops=2 > outNS.log 2>&1
  awk -f ns-3/parseNS.awk -v nbFlows=${i} latency=${latVal}ms flowSize=${flowSize} execCost=${execCost} seed=1 outNS.log >> ${resFile}

done