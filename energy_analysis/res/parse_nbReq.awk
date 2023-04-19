#!/usr/bin/awk -f

BEGIN{
  print("host,sent,finished")
}

/send req to/ {
  reqSent[$7]++
}

/Finished request exec on/ {
  reqFin[$8]++
}

END {
  for (host in reqFin)
    print(host","reqSent[host]","reqFin[host])
}