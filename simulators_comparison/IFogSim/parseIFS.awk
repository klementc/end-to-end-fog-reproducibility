
#!/usr/bin/awk -f

BEGIN {
}

#/Device send up tupleid:/ {
#  SENT[$8]=$2
#}

/Device received tupleid:/ {
  # store timestampend, timestampstart, src, dst
  print("IFogSim,net,"nbFlows","execCost","latency","flowSize","$19","$2",0,"$16","$18","nbCore)
}

/Completed execution of tuple/ {
  # store timestampend, timestampstart, src, dst
  print("IFogSim,exec,"nbFlows","execCost","latency","flowSize","$11","$13",0,NA,NA"","nbCore)
}

END {
}