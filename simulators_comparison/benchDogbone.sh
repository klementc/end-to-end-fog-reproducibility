set -x

bwsrc0="50" #Mbps
bwsrc1="50" #Mbps
bwdev="100 50 25" #Mbps
bwdst="50" #Mbps

latsrc0=0
latsrc1=0
latdev=$(seq 0 10 51)
latdst=0
flowSize="6250000 12500000 18750000" # MB
execCost=500 # MIPS
resFile="resDogbone.csv"
IFSPATH="/home/clem/Code/github.com/YAFS"

export LD_LIBRARY_PATH=~/Code/gitlab.com/nsnam/ns-allinone-3.39/ns-3.39/build/lib:~/Code/framagit.org/klement/simgrid_clean/build/lib


echo "simulator,type,nbF,execCost,lat1,lat2,lat3,lat4,bw1,bw2,bw3,bw4,flowS,start,end,seed,src,dst" > ${resFile}
for flowS in ${flowSize};
do
  for lat0 in ${latsrc0};
  do
    for lat1 in ${latsrc1};
    do
      for latd12 in ${latdev};
      do
        for latd in ${latdst};
        do
          for b1 in ${bwsrc0};
          do
            for b2 in ${bwsrc1};
            do
              for b3 in ${bwdev};
              do
                for b4 in ${bwdst};
                do
                  lat0IFS=` echo "${lat0}/1000.0" | bc -l`
                  lat1IFS=` echo "${lat1}/1000.0" | bc -l`
                  latd12IFS=` echo "${latd12}/1000.0" | bc -l`
                  latdIFS=` echo "${latd}/1000.0" | bc -l`
                  java -jar IFogSim/iFogSim2_dogbone.jar ${lat0IFS} ${lat1IFS} ${latd12IFS} ${latdIFS} ${flowS} ${execCost} ${b1} ${b2} ${b3} ${b4} > outIFS.log 2>&1
                  awk -f IFogSim/parseIFSdogbone.awk -v nbFlows=1 lat1=${lat0}ms lat2=${lat1}ms lat3=${latd12}ms lat4=${latd}ms bw1=${b1}Mbps bw2=${b2}Mbps bw3=${b3}Mbps bw4=${b4}Mbps flowSize=${flowS} execCost=${execCost} outIFS.log >> ${resFile}

                  # YAFS
                  PREVID=${PWD}
                  cd ${IFSPATH}
                  cd examples/Tutorial/
                  python main1_semi_db.py ${lat0} ${lat1} ${latd12} ${latdst} ${flowS} ${execCost} ${b1} ${b2} ${b3} ${b4}
                  cd ${PREVID}
                  awk -F ',' -f YAFS/parseYAFSdogbone.awk -v nbFlows=1  lat1=${lat0}ms lat2=${lat1}ms lat3=${latd12}ms lat4=${latd}ms bw1=${b1}Mbps bw2=${b2}Mbps bw3=${b3}Mbps bw4=${b4}Mbps flowSize=${flowS} execCost=${execCost} ~/Code/github.com/YAFS/examples/Tutorial/results/sim_trace.csv >> ${resFile}


                  ./simgrid/bench_dogbone simgrid/platform_dogbone.xml ${lat0}ms ${lat1}ms ${latd12}ms ${latdst}ms ${flowS} ${execCost} ${b1} ${b2} ${b3} ${b4} > outSG.log 2>&1
                  awk -f simgrid/parseSGdogbone.awk -v nbFlows=1  lat1=${lat0}ms lat2=${lat1}ms lat3=${latd12}ms lat4=${latd}ms bw1=${b1}Mbps bw2=${b2}Mbps bw3=${b3}Mbps bw4=${b4}Mbps flowSize=${flowS} execCost=${execCost} outSG.log >> ${resFile}

                  ./ns-3/wired_dogbone --flowNumbers=1 --latsrc0=${lat0}ms --latsrc1=${lat1}ms --latdev=${latd12}ms --latdst=${latd}ms --flowSize=${flowS} --DelayBetweenFlows=0 --Seed=1 --bwsrc0=${b1}Mbps --bwsrc1=${b2}Mbps --bwdev=${b3}Mbps --bwdst=${b4}Mbps > outNS.log 2>&1
                  awk -f ns-3/parseNSdogbone.awk -v nbFlows=1 lat1=${lat0}ms lat2=${lat1}ms lat3=${latd12}ms lat4=${latd}ms bw1=${b1}Mbps bw2=${b2}Mbps bw3=${b3}Mbps bw4=${b4}Mbps flowSize=${flowS} execCost=${execCost} seed=1 outNS.log >> ${resFile}
                done
              done
            done
          done
        done
      done
    done
  done
done
