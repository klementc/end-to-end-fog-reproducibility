set -x

nbCore=$(seq 1 1 3)
reqCost=$(seq 500 500 3000)
overlap=$(seq 0 1 2)
nbExec=$(seq 1 1 3)
SGPATH="~/Code/framagit.org/klement/simgrid_clean/build/lib"
NSPATH="~/Code/gitlab.com/nsnam/ns-allinone-3.39/ns-3.39/build/lib"
resFile="resExecDelay.csv"


export LD_LIBRARY_PATH=~/Code/gitlab.com/nsnam/ns-allinone-3.39/ns-3.39/build/lib:~/Code/framagit.org/klement/simgrid_clean/build/lib

echo "simulator,nbFlows,step,nbCore,execSize,start,end" > ${resFile}
for i in ${nbCore};
do
  for j in ${reqCost};
  do
    for k in ${nbExec};
    do
      ./simgrid/execScenario ./simgrid/platformWired_"${i}"core.xml ${k} ${j} 1 ${i} > outSG.log 2>&1
      awk -f simgrid/parseExec.awk -v nbFlows=${k} nbCore=${i} step=1 execSize=${j} outSG.log >> ${resFile}
      valIFS=` echo "${k}/1000.0" | bc -l`
      java -jar IFogSim/iFogSimExec.jar ${k} ${j} 1 ${i} > outIFS.log 2>&1
      awk -f IFogSim/parseExec.awk -v nbFlows=${k} nbCore=${i} step=1 execSize=${j} outIFS.log >> ${resFile}
    done
  done
done