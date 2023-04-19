#!/usr/bin/awk -f

BEGIN{
  print("host,ts,nbInst")
}

/Scale CPU:/ {
  print($12","$7","$10)
}
