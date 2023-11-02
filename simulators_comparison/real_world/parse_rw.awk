#!/usr/bin/awk -f

BEGIN {
}

// {
  startFlow=$7
  endFlow=$9
  print("realworld,net,"nbFlows","execCost","latency","flowSize","$7+15","$8+15","seed",src,dst")
}
