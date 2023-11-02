#!/usr/bin/awk -f

BEGIN {
}

/Communication time start:/ {
  startFlow=$7
  endFlow=$9
  print("simgrid,net,"nbFlows","execCost","lat1","lat2","lat3","lat4","bw1","bw2","bw3","bw4","flowSize","startFlow","endFlow",0,src,dst")
}

/Execution finished start/ {
  startExec=$7
  endExec=$9
  print("simgrid,exec,"nbFlows","execCost","lat1","lat2","lat3","lat4","bw1","bw2","bw3","bw4","flowSize","startExec","endExec",0,NA,NA")
}