#!/usr/bin/python

import sys, os
import random

wai=os.path.dirname(os.path.abspath(__file__))

def pwrite(text):
    print(text+"\n")


if len(sys.argv) < 5:
  print("Usage: ./"+os.path.basename(__file__)+" <nAPPs> <mbName> <nSTAPerApp> <reqSize> <freqMin> <freqMax>")

nApps = int(sys.argv[1])
mbName = sys.argv[2]
nSTAPerApp = int(sys.argv[3])
reqSize = int(sys.argv[4])
freqMin = int(sys.argv[5])
freqMax = int(sys.argv[6])

print("   std::vector<simgrid::s4u::ActorPtr> placedDS;")

placedDS = 0
for i in range(nApps):
  mbApp = "{}_{}".format(i, mbName)
  for j in range(nSTAPerApp):
    freq = random.randint(freqMin, freqMax)
    print("""
      {{sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("{}",RequestType::{}, 1./{}, {});
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_{}"), [&]{{dsf->run();}});
      placedDS.push_back(dataS);}}\
      """.format(str(i)+"_"+mbName, "APP_"+str(i), freq, reqSize, placedDS))
    placedDS += 1