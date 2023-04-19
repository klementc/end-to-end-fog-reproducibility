#!/usr/bin/awk -f

BEGIN{
  print("service,start,end,dur")
}

/Request end-to-end/ {
  print($12","$7","$9","$11)
}
