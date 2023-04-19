#!/usr/bin/awk -f

BEGIN{
  cloudHostsEnergy=0
  edgeHostsEnergy=0
  staHostsEnergy=0
  wifiLinkEnergy=0
  cloudLinkEnergy=0
  edgeLinkEnergy=0
  backboneLinkEnergy=0
  collectorLinkEnergy=0

  coreLinkEnergy=0
  wifiRouterEnergy=0
  coreRouterEnergy=0
  print("cloudHostsEnergy,edgeHostsEnergy,staHostsEnergy,wifiLinkEnergy,cloudLinkEnergy,edgeLinkEnergy,backboneLinkEnergy,collectorLinkEnergy,coreLinkEnergy,wifiRouterEnergy,coreRouterEnergy")
}

/Energy consumption of link 'cluster-cloud/ {
  cloudLinkEnergy+=$8
}

/Energy consumption of link 'cluster-edge/ {
  edgeLinkEnergy+=$8
}

/Energy consumption of link 'Backbone/ {
  backboneLinkEnergy+=$8
}

/Energy consumption of link 'Collector/ {
  collectorLinkEnergy+=$8
}

/Energy consumption of link 'Core/ {
  collectorLinkEnergy+=$8
}

/ destroyed, consumed:/ {
  wifiLinkEnergy+=$7
}

/Energy consumption of host cloud/ {
  cloudHostsEnergy+=$8
}

/Energy consumption of host edge/ {
  edgeHostsEnergy+=$8
}

/Energy consumption of host STA/ {
  staHostsEnergy+=$8
}

/Energy consumption of host WIFIrouter/ {
  wifiRouterEnergy+=$8
}

/Energy consumption of host core_router_/ {
  coreRouterEnergy+=$8
}

END {
  print(cloudHostsEnergy","edgeHostsEnergy","staHostsEnergy","wifiLinkEnergy","cloudLinkEnergy","edgeLinkEnergy","backboneLinkEnergy","collectorLinkEnergy","coreLinkEnergy","wifiRouterEnergy","coreRouterEnergy)
}