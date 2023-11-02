nbClients=$(seq 1 1 10)
sizeFlowBytes=12500000
nbRetry=$(seq 1 1 15)
execCost=500
dst=$1
resFile="res_net_RW.csv"

echo "Before launching this file, run iperf -p 12345 -s on the destination node"

for j in ${nbRetry};
do
  valRW=` echo "${sizeFlowBytes}/1000000.0" | bc -l`
  echo "Run for sample: ${j}"
  for i in ${nbClients};
  do
    echo "send to ${dst}"
    iperf -Z cubic -n ${valRW}M -c ${dst} -P ${i} -p 12345 -y C > res_seed${i}_${j}.log
  done
done

sleep 10
echo "Launch analysis now"
for j in ${nbRetry};
do
  echo "Run for sample: ${j}"
  for i in ${nbClients};
  do
    awk -F",|-" -f parse_rw.awk -v nbFlows=${i} latency=0ms flowSize=${sizeFlowBytes} seed=${j} execCost=${execCost} res_seed${i}_${j}.log >> ${resFile}
  done
done

