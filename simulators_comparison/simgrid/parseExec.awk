#!/usr/bin/awk -f

BEGIN {
}

/Execution finished, start:/ {
  print("SimGrid,"nbFlows","step","nbCore","execSize","$7","$9)
}