set -x

flowSize=6250000.0 # MB
latVal=0
delay=0
execCost=500 # MIPS
nbFlows=$(seq 1 1 10)
seed=$(seq 1 1 5)
resFile="resWiFi-net.csv"
export LD_LIBRARY_PATH=~/Code/gitlab.com/nsnam/ns-allinone-3.39/ns-3.39/build/lib:~/Code/framagit.org/klement/simgrid_clean/build/lib

echo "simulator,type,nbF,execCost,lat,flowS,start,end,seed,src,dst" > ${resFile}
for i in ${nbFlows};
do
  valIFS=` echo "${latVal}/1000.0" | bc -l`
  java -jar IFogSim/iFogSim2_wifi_jar.jar ${i} ${flowSize} ${delay} ${execCost}> outIFS.log 2>&1
  awk -f IFogSim/parseIFS.awk -v nbFlows=${i} latency=${latVal}ms flowSize=${flowSize} execCost=${execCost} outIFS.log >> ${resFile}

  ./simgrid/bench_wifi ./simgrid/platform_wifi.xml ${i} ${flowSize} ${delay} ${execCost} > outSG.log 2>&1
  awk -f simgrid/parseSG.awk -v nbFlows=${i} latency=${latVal}ms flowSize=${flowSize} execCost=${execCost} outSG.log >> ${resFile}

  for s in ${seed};
  do
    ./ns-3/wifiScenario --flowNumbers=${i} --latency=${latVal}ms --flowSize=${flowSize} --DelayBetweenFlows=0 --Seed=${s} > outNS.log 2>&1
    awk -f ns-3/parseNS.awk -v nbFlows=${i} latency=${latVal}ms flowSize=${flowSize} execCost=${execCost} seed=${s} outNS.log >> ${resFile}
  done
done