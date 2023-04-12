#!/usr/bin/python

import sys, os
import random

wai=os.path.dirname(os.path.abspath(__file__))

def pwrite(text):
    print(text+"\n")

#servList = ["MOTION_OBJECT_DETECT","OBJECT_TRACKER","USER_INTERFACE"]
servList = ["MOTION_DETECT", "OBJECT_DETECT","OBJECT_TRACKER","USER_INTERFACE"]
if len(sys.argv) < 4:
  print("Usage: ./"+os.path.basename(__file__)+" <nAPPs> <nRepPerAPP> <from>")

nAPPs = int(sys.argv[1])
nRepPerAPP = int(sys.argv[2])
startFrom = int(sys.argv[3])

print("serviceName nodename")
for app in range(nAPPs):
  nbPlaced = startFrom
  for serv in servList:
    for nRep in range(nRepPerAPP):
      print("{}_{} cloud_{}".format(app, serv, nbPlaced))
      nbPlaced += 1