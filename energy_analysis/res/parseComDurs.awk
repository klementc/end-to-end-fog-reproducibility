#!/usr/bin/awk -f

BEGIN{
  print("dst,dur,start,end")
}

/comm TI dur:/ {
  print("TI"$5","$13","$14","$15)
}

/comm ETM dur:/ {
  print("ETM"$5","$13","$14","$15)
}
