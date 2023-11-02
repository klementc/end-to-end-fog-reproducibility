nbClients=$(seq 1 1 10)
sizeFlowBytes=12500000
nbRetry=$(seq 1 1 15)
execCost=500
dst=$1
resFileSC="res_exec_RW_sc.csv"
resFileDC="res_exec_RW_dc.csv"
rm -rf outF
mkdir outF

for j in ${nbRetry};
do
  echo "Run for sample: ${j}"
  for i in ${nbClients};
  do
    param=`seq -s ' ' 1 ${i}`
    echo "Execute with ${i} clients single core"
    parallel taskset 0x1 python loop.py ">>" outF/exec_sc.log ::: ${param}
    awk -f parse_exec.awk -v nbFlows=${i} latency=0ms flowSize=${sizeFlowBytes} seed=${j} execCost=${execCost} outF/exec_sc.log >> ${resFileSC}
    rm outF/exec_sc.log

    echo "Execute with ${i} clients dual core"
    parallel taskset 0x3 python loop.py ">>" outF/exec_dc.log ::: ${param}
    awk -f parse_exec.awk -v nbFlows=${i} latency=0ms flowSize=${sizeFlowBytes} seed=${j} execCost=${execCost} outF/exec_dc.log >> ${resFileDC}
    rm outF/exec_dc.log
  done
done
