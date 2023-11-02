set -x

flowSize=50000000 # MB
latVal=$(seq 0 50 101)
delay=0
execCost=500 # MIPS
nbFlows=$(seq 1 1 5)
resFile="res2Hop-lat.csv"
IFSPATH="/home/clem/Code/github.com/YAFS"

export LD_LIBRARY_PATH=~/Code/gitlab.com/nsnam/ns-allinone-3.39/ns-3.39/build/lib:~/Code/framagit.org/klement/simgrid_clean/build/lib

echo "simulator,type,nbF,execCost,lat,flowS,start,end,seed,src,dst" > ${resFile}
for i in ${nbFlows};
do
  for lat in ${latVal};
  do
    valIFS=` echo "${lat}/1000.0" | bc -l`
    java -jar IFogSim/iFogSim2_2hops.jar ${i} ${valIFS} ${flowSize} ${delay} ${execCost}> outIFS.log 2>&1
    awk -f IFogSim/parseIFS.awk -v nbFlows=${i} latency=${lat}ms flowSize=${flowSize} execCost=${execCost} outIFS.log >> ${resFile}

    # YAFS
    PREVID=${PWD}
    cd ${IFSPATH}
    cd examples/Tutorial/
    python main1_2hops.py ${i} ${lat} ${flowSize} ${delay} ${execCost}
    cd ${PREVID}
    awk -F ',' -f YAFS/parseYAFS.awk -v nbFlows=${i} latency=${lat}ms flowSize=${flowSize} execCost=${execCost} ~/Code/github.com/YAFS/examples/Tutorial/results/sim_trace.csv >> ${resFile}


    ./simgrid/bench_1hop ./simgrid/platform_bench2hops.xml ${i} "${lat}ms" ${flowSize} ${delay} ${execCost} > outSG.log 2>&1
    awk -f simgrid/parseSG.awk -v nbFlows=${i} latency=${lat}ms flowSize=${flowSize} execCost=${execCost} outSG.log >> ${resFile}


    ./ns-3/wired_nhop --flowNumbers=${i} --latency=${lat}ms --flowSize=${flowSize} --DelayBetweenFlows=${delay} --Seed=1 --nbHops=2 > outNS.log 2>&1
    awk -f ns-3/parseNS.awk -v nbFlows=${i} latency=${lat}ms flowSize=${flowSize} execCost=${execCost} seed=1 outNS.log >> ${resFile}

  done
done