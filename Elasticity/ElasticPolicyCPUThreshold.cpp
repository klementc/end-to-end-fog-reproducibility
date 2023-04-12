
#include <vector>
#include <iostream>
#include <numeric>
#include <simgrid/s4u/Actor.hpp>
#include <simgrid/s4u/Engine.hpp>
#include <simgrid/s4u/Host.hpp>

#include "ElasticPolicy.hpp"
#include "ElasticTask.hpp"

// using namespace simgrid;
// using namespace s4u;
namespace sg_microserv {

XBT_LOG_NEW_DEFAULT_CATEGORY(elasticPolicyCPU, "Elastic tasks policy manager");

ElasticPolicyCPUThreshold::ElasticPolicyCPUThreshold(double interval, double uCPUT, double lCPUT)
  : ElasticPolicy(interval), upperCPUThresh_(uCPUT), lowCPUThresh_(lCPUT)
{}


void ElasticPolicyCPUThreshold::run() {
  int instanceToStartIndex = 0;

  XBT_INFO("ElasticPolicyCPUThreshold activated");
  XBT_INFO("Autoscaler CPU: time %lf nb instances: %d actor %s",simgrid::s4u::Engine::get_clock(), getTasks().at(0)->getInstanceAmount(), simgrid::s4u::this_actor::get_name().c_str());
  while (isActive()) {
    // wait until next scaling
    simgrid::s4u::this_actor::sleep_for(getUpdateInterval());

    // make your decision (here only one group should be in the taskgroup)
    xbt_assert(getTasks().size() == 1, "CPU threshold policy operates on single groups");

    ElasticTaskManager* etm = getTasks().at(0);
    //XBT_INFO("name: %s",etm->getServiceName().c_str());
    std::vector<double> lv = etm->getCPULoads();
    double avgLoad = std::accumulate(lv.begin(), lv.end(), 0.0) / lv.size();
    std::string s = "";
    for (auto v : lv) s += std::to_string(v)+" " ;

    /*int execInSlot = etm->getCounterExecSlot();
    XBT_INFO("%f %d %d %d %d %f stats", avgLoad, etm->getInstanceAmount(), etm->getAmountOfWaitingRequests(),
      etm->getAmountOfExecutingRequests(), execInSlot, etm->reqPerSec()*getUpdateInterval());

    etm->resetCounterExecSlot();*/

    if (avgLoad > upperCPUThresh_) {
      // if available hosts, add one
      // XBT_DEBUG("add host %d", instanceToStartIndex);
      // etm->addHost(hostPool_.at(instanceToStartIndex));
      // instanceToStartIndex = (instanceToStartIndex+1) % hostPool_.size();
      if (hostPool_.size() > 0) {
        if(!hostPool_.at(0)->is_on())
          hostPool_.at(0)->turn_on();
        //hostPool_.at(0)->set_pstate(0);
        etm->addHost(hostPool_.at(0));
        hostPool_.erase(hostPool_.begin());
      } else {
        XBT_INFO("no more hosts to add to service");
      }
    } else if (avgLoad < lowCPUThresh_ && etm->getInstanceAmount() > 1) {
      // if more than one instance, remove one
      XBT_INFO("remove host");
      simgrid::s4u::Host* h = etm->removeHost(0);
      if (h) {
        hostPool_.push_back(h);
        //h->turn_off();
        //h->set_pstate(-1);
      }
    }

    XBT_INFO("Scale CPU: time %lf nb inst: %d actor %s",simgrid::s4u::Engine::get_clock(), etm->getInstanceAmount(), simgrid::s4u::this_actor::get_name().c_str());
    /*etm->resetCounterExecSlot();*/

//    XBT_INFO("%f %d %d %d %d %f stats", avgLoad, etm->getInstanceAmount(), etm->getAmountOfWaitingRequests(),
//      etm->getAmountOfExecutingRequests(), execInSlot, etm->reqPerSec()*getUpdateInterval());
  }
}

}  // namespace sg_microserv
