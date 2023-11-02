
#!/usr/bin/awk -f

BEGIN {
}

#/Device send up tupleid:/ {
#  SENT[$8]=$2
#}

/Device received tupleid:/ {
  # store timestampend, timestampstart, src, dst
  print("IFogSim,net,"nbFlows","execCost","lat1","lat2","lat3","lat4","bw1","bw2","bw3","bw4","flowSize","$19","$2",0,"$16","$18)
}

/Completed execution of tuple/ {
  # store timestampend, timestampstart, src, dst
  print("IFogSim,exec,"nbFlows","execCost","lat1","lat2","lat3","lat4","bw1","bw2","bw3","bw4","flowSize","$11","$13",0,NA,NA")
}

END {
}