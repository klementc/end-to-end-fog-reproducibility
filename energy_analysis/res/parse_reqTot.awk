#!/usr/bin/awk -f

BEGIN{
  print("nbReqTotFinish,reqSize,sizeTot")
}

/Tot nb req:/ {
  print($7","$10","$7*$10)
}
