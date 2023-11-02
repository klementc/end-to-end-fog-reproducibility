#!/usr/bin/awk -f

BEGIN {
}

/start:/ {
  startexec=$2
  endexec=$4
  print("realworld,exec,"nbFlows","execCost","latency","flowSize","startexec","endexec","seed",src,dst")
}
