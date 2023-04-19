#!/usr/bin/awk -f

BEGIN{
  print("host,avg,start,end")
}

/load on / {
  print($6","$7","$9","$11)
}
