#!/usr/bin/awk -f

BEGIN {
}

/Communication time start:/ {
  startFlow=$7
  endFlow=$9
  print("simgrid,net,"nbFlows","execCost","latency","flowSize","startFlow","endFlow",0,src,dst")
}

/Execution finished start/ {
  startExec=$7
  endExec=$9
  print("simgrid,exec,"nbFlows","execCost","latency","flowSize","startExec","endExec",0,NA,NA")
}