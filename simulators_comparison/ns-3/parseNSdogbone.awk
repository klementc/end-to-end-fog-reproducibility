#!/usr/bin/awk -f

BEGIN {
}

/Sink received first pckt/ {
  startFlow=$6
  endFlow=$10
  print("ns3,net,"nbFlows","execCost","lat1","lat2","lat3","lat4","bw1","bw2","bw3","bw4","flowSize","startFlow","endFlow","seed",src,dst")
}
