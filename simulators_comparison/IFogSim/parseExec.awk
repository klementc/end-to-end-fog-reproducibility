
#!/usr/bin/awk -f

BEGIN {
}

/Completed execution of tuple/ {
  FINISH[$7]=$2
}

/Executing tuple/ {
  START[$5]=$2
}

END {
  for (i in FINISH)
    print("IFogSim,"nbFlows","step","nbCore","execSize","START[i]","FINISH[i])
}