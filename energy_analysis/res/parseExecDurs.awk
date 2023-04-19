#!/usr/bin/awk -f

BEGIN{
  print("service,dur,start,end")
}

/exec dur:/ {
  print($5","$16","$17","$18)
}
