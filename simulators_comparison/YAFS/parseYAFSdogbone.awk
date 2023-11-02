#!/usr/bin/awk -f

BEGIN {
}

/ServiceA,M.A/ {
  startFlow=$14-85
  endFlow=$15-85
  startExec=$12-85
  endExec=$13-85
  if (NR>1){print("YAFS,net,"nbFlows","execCost","lat1","lat2","lat3","lat4","bw1","bw2","bw3","bw4","flowSize","startFlow","endFlow",0,src,dst")}
  if (NR>1){print("YAFS,exec,"nbFlows","execCost","lat1","lat2","lat3","lat4","bw1","bw2","bw3","bw4","flowSize","startExec","endExec",0,src,dst")}
}
