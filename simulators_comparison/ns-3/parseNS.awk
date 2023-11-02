#!/usr/bin/awk -f

BEGIN {
}

/Sink received first pckt/ {
  startFlow=$6
  endFlow=$10
  print("ns3,net,"nbFlows","execCost","latency","flowSize","startFlow","endFlow","seed",src,dst")
}
