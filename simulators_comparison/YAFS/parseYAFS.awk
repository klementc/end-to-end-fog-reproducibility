#!/usr/bin/awk -f

BEGIN {
}

/ServiceA,M.A/ {
  startFlow=$14-85
  endFlow=$15-85
  startExec=$12-85
  endExec=$13-85
  if (NR>1){print("YAFS,net,"nbFlows","execCost","latency","flowSize","startFlow","endFlow",0,src,dst")}
  if (NR>1){print("YAFS,exec,"nbFlows","execCost","latency","flowSize","startExec","endExec",0,src,dst")}
}
