
#include <simgrid/s4u/Actor.hpp>
#include <simgrid/s4u/Host.hpp>
#include <simgrid/s4u/Mailbox.hpp>
#include <simgrid/s4u/Engine.hpp>
#include <simgrid/s4u/Comm.hpp>
#include <simgrid/plugins/energy.h>
#include <simgrid/plugins/load.h>

#include "ElasticPolicy.hpp"
#include "ElasticTask.hpp"
#include "DataSource.hpp"
#include <memory>

XBT_LOG_NEW_DEFAULT_CATEGORY(run_log, "logs of the experiment");

double execR = 1;
double sizeR = 10000;
int totNbReq = 0;

int outSizes(RequestType, std::string);
/* RETURN FUNCTIONS, AUTO GENERATED CODE, MODIFY IF YOU KNOW WHAT YOU WANT */
    void return_0_MOTION_DETECT(sg_microserv::TaskDescription* td) {
      switch (td->requestType)
      {
case RequestType::APP_0:
      if (td->nbHops == 1) {
        XBT_DEBUG("Output Function for APP_0, put to 0_OBJECT_DETECT");
        s4u_Mailbox* m_0_OBJECT_DETECT = s4u_Mailbox::by_name("0_OBJECT_DETECT");
        m_0_OBJECT_DETECT->put(td, outSizes(td->requestType, "0_OBJECT_DETECT"));


break;
}
	}
}
    void return_0_OBJECT_DETECT(sg_microserv::TaskDescription* td) {
      switch (td->requestType)
      {
case RequestType::APP_0:
      if (td->nbHops == 2) {
        XBT_DEBUG("Output Function for OBJECT_DETECT (PARALLEL CHILDS)");
        std::vector<simgrid::s4u::CommPtr> comV;
        simgrid::s4u::CommPtr com;

          XBT_DEBUG("Put to 0_OBJECT_TRACKER");
          s4u_Mailbox* m_0_OBJECT_TRACKER = s4u_Mailbox::by_name("0_OBJECT_TRACKER");
          sg_microserv::TaskDescription* td_0 = new sg_microserv::TaskDescription(*td);
          td_0->requestType = RequestType::APP_0_0;
          com = m_0_OBJECT_TRACKER->put_async(td_0, outSizes(td_0->requestType, "0_OBJECT_TRACKER"));
          comV.push_back(com);

          XBT_DEBUG("Put to 0_USER_INTERFACE");
          s4u_Mailbox* m_0_USER_INTERFACE = s4u_Mailbox::by_name("0_USER_INTERFACE");
          sg_microserv::TaskDescription* td_1 = new sg_microserv::TaskDescription(*td);
          td_1->requestType = RequestType::APP_0_1;
          com = m_0_USER_INTERFACE->put_async(td_1, outSizes(td_1->requestType, "0_USER_INTERFACE"));
          comV.push_back(com);

simgrid::s4u::Comm::wait_all(comV); // wait for communications to finish
break;
}
	}
}
    void return_0_OBJECT_TRACKER(sg_microserv::TaskDescription* td) {
      switch (td->requestType)
      {

case RequestType::APP_0_0:
    if (td->nbHops == 3) {
      totNbReq++;XBT_INFO("Finished request exec on %s at %lf", simgrid::s4u::this_actor::get_host()->get_cname(), simgrid::s4u::Engine::get_clock());
      XBT_INFO("Request end-to-end sent: %lf finished: %lf duration: %lf %s", td->firstArrivalDate, simgrid::s4u::Engine::get_clock(), simgrid::s4u::Engine::get_clock()-td->firstArrivalDate, simgrid::s4u::this_actor::get_host()->get_cname());
    }

break;
	}
}
    void return_0_USER_INTERFACE(sg_microserv::TaskDescription* td) {
      switch (td->requestType)
      {

case RequestType::APP_0_1:
    if (td->nbHops == 3) {
      XBT_DEBUG("Output Function 0_USER_INTERFACE (FINAL NODE, NO CHILD!)");
    }

break;
	}
}
/* PR FUNCTIONS, AUTO GENERATED CODE, MODIFY IF YOU KNOW WHAT YOU WANT */
    double pr_0_MOTION_DETECT(sg_microserv::TaskDescription* td) {
      switch (td->requestType) {
case RequestType::APP_0:
      if (td->nbHops == 1) {return 500000000/execR;}
      	}//it should never end up here
return -1;
}
    double pr_0_OBJECT_DETECT(sg_microserv::TaskDescription* td) {
      switch (td->requestType) {
case RequestType::APP_0:
      if (td->nbHops == 2) {return 250000000/execR;
}
break;

      	}//it should never end up here
return -1;
}
    double pr_0_OBJECT_TRACKER(sg_microserv::TaskDescription* td) {
      switch (td->requestType) {

case RequestType::APP_0_0:
    if (td->nbHops == 3) { return 500000000/execR;}

break;
	}//it should never end up here
return -1;
}
    double pr_0_USER_INTERFACE(sg_microserv::TaskDescription* td) {
      switch (td->requestType) {

case RequestType::APP_0_1:
    if (td->nbHops == 3) {return 250000000/execR;}

break;
	}//it should never end up here
return -1;
}
int outSizes(RequestType t, std::string nextServ) {
    switch (t) {
}return sizeR; // WE SHOULD NEVER GET TO THAT POINT
}

std::string reqTypeToStr(RequestType t) {
    switch (t) {
	case RequestType::APP_0 : return "APP_0";break;
	case RequestType::APP_0_0 : return "APP_0_0";break;
	case RequestType::APP_0_1 : return "APP_0_1";break;
}return "WTF";
}

  void run(std::map<std::string, std::vector<std::string>> configServices, std::map<std::string, std::vector<std::string>> elasticPools, std::vector<std::string> used_hosts) {
    XBT_INFO("Starting run()");
    std::map<std::string, std::shared_ptr<sg_microserv::ElasticTaskManager>> serviceToETM;

    // power off all unused hosts
    auto hList = simgrid::s4u::Engine::get_instance()->get_all_hosts();
      for(auto h : hList) {
        if (h->get_name().find("STA") != std::string::npos || h->get_name().find("manager") != std::string::npos || h->get_name().find("WIFIrouter") != std::string::npos)
          continue;
        if(std::find(used_hosts.begin(), used_hosts.end(), h->get_name()) == used_hosts.end()) {
          XBT_INFO("Power off node %s", h->get_cname());
          h->turn_off();
          //h->set_pstate(-1);
        }
    }

    // create ETM for service 0_MOTION_DETECT
    std::vector<std::string> v_serv_0_MOTION_DETECT = std::vector<std::string>();
    v_serv_0_MOTION_DETECT.push_back("0_MOTION_DETECT");
    std::shared_ptr<sg_microserv::ElasticTaskManager> serv_0_MOTION_DETECT = std::make_shared<sg_microserv::ElasticTaskManager>("0_MOTION_DETECT",v_serv_0_MOTION_DETECT);
    serviceToETM["0_MOTION_DETECT"] = std::shared_ptr<sg_microserv::ElasticTaskManager>(serv_0_MOTION_DETECT);
    serv_0_MOTION_DETECT->setBootDuration(0);
    // serv_0_MOTION_DETECT->setDataSizeRatio(1); NOT USED ANYMORE, KEEP AS COMMENT IN CASE
    // modify this value in the configuration file (parDeg)
    serv_0_MOTION_DETECT->setParallelTasksPerInst(std::stoi(configServices.find("0_MOTION_DETECT")->second.at(2)));
    serv_0_MOTION_DETECT->setParallelRecPerETM(32);
    // modify this value in the  configuration file (execRatio)
    serv_0_MOTION_DETECT->setExecRatio(std::stod(configServices.find("0_MOTION_DETECT")->second.at(1)));
    serv_0_MOTION_DETECT->setOutputFunction(return_0_MOTION_DETECT);
    serv_0_MOTION_DETECT->setExecAmountFunc(pr_0_MOTION_DETECT);
    serv_0_MOTION_DETECT->setReqNames(reqTypeToStr);
    // set the host location in the config file (location)
    serv_0_MOTION_DETECT->addHost(simgrid::s4u::Host::by_name(configServices.find("0_MOTION_DETECT")->second.at(0)));
    simgrid::s4u::Actor::create("etm_0_MOTION_DETECT", simgrid::s4u::Host::by_name(configServices.find("0_MOTION_DETECT")->second.at(0)), [serv_0_MOTION_DETECT] { serv_0_MOTION_DETECT->run(); });


    // create ETM for service 0_OBJECT_DETECT
    std::vector<std::string> v_serv_0_OBJECT_DETECT = std::vector<std::string>();
    v_serv_0_OBJECT_DETECT.push_back("0_OBJECT_DETECT");
    std::shared_ptr<sg_microserv::ElasticTaskManager> serv_0_OBJECT_DETECT = std::make_shared<sg_microserv::ElasticTaskManager>("0_OBJECT_DETECT",v_serv_0_OBJECT_DETECT);
    serviceToETM["0_OBJECT_DETECT"] = std::shared_ptr<sg_microserv::ElasticTaskManager>(serv_0_OBJECT_DETECT);
    serv_0_OBJECT_DETECT->setBootDuration(0);
    // serv_0_OBJECT_DETECT->setDataSizeRatio(1); NOT USED ANYMORE, KEEP AS COMMENT IN CASE
    // modify this value in the configuration file (parDeg)
    serv_0_OBJECT_DETECT->setParallelTasksPerInst(std::stoi(configServices.find("0_OBJECT_DETECT")->second.at(2)));
    serv_0_OBJECT_DETECT->setParallelRecPerETM(32);
    // modify this value in the  configuration file (execRatio)
    serv_0_OBJECT_DETECT->setExecRatio(std::stod(configServices.find("0_OBJECT_DETECT")->second.at(1)));
    serv_0_OBJECT_DETECT->setOutputFunction(return_0_OBJECT_DETECT);
    serv_0_OBJECT_DETECT->setExecAmountFunc(pr_0_OBJECT_DETECT);
    serv_0_OBJECT_DETECT->setReqNames(reqTypeToStr);
    // set the host location in the config file (location)
    serv_0_OBJECT_DETECT->addHost(simgrid::s4u::Host::by_name(configServices.find("0_OBJECT_DETECT")->second.at(0)));
    simgrid::s4u::Actor::create("etm_0_OBJECT_DETECT", simgrid::s4u::Host::by_name(configServices.find("0_OBJECT_DETECT")->second.at(0)), [serv_0_OBJECT_DETECT] { serv_0_OBJECT_DETECT->run(); });


    // create ETM for service 0_OBJECT_TRACKER
    std::vector<std::string> v_serv_0_OBJECT_TRACKER = std::vector<std::string>();
    v_serv_0_OBJECT_TRACKER.push_back("0_OBJECT_TRACKER");
    std::shared_ptr<sg_microserv::ElasticTaskManager> serv_0_OBJECT_TRACKER = std::make_shared<sg_microserv::ElasticTaskManager>("0_OBJECT_TRACKER",v_serv_0_OBJECT_TRACKER);
    serviceToETM["0_OBJECT_TRACKER"] = std::shared_ptr<sg_microserv::ElasticTaskManager>(serv_0_OBJECT_TRACKER);
    serv_0_OBJECT_TRACKER->setBootDuration(0);
    // serv_0_OBJECT_TRACKER->setDataSizeRatio(1); NOT USED ANYMORE, KEEP AS COMMENT IN CASE
    // modify this value in the configuration file (parDeg)
    serv_0_OBJECT_TRACKER->setParallelTasksPerInst(std::stoi(configServices.find("0_OBJECT_TRACKER")->second.at(2)));
    serv_0_OBJECT_TRACKER->setParallelRecPerETM(32);
    // modify this value in the  configuration file (execRatio)
    serv_0_OBJECT_TRACKER->setExecRatio(std::stod(configServices.find("0_OBJECT_TRACKER")->second.at(1)));
    serv_0_OBJECT_TRACKER->setOutputFunction(return_0_OBJECT_TRACKER);
    serv_0_OBJECT_TRACKER->setExecAmountFunc(pr_0_OBJECT_TRACKER);
    serv_0_OBJECT_TRACKER->setReqNames(reqTypeToStr);
    // set the host location in the config file (location)
    serv_0_OBJECT_TRACKER->addHost(simgrid::s4u::Host::by_name(configServices.find("0_OBJECT_TRACKER")->second.at(0)));
    simgrid::s4u::Actor::create("etm_0_OBJECT_TRACKER", simgrid::s4u::Host::by_name(configServices.find("0_OBJECT_TRACKER")->second.at(0)), [serv_0_OBJECT_TRACKER] { serv_0_OBJECT_TRACKER->run(); });


    // create ETM for service 0_USER_INTERFACE
    std::vector<std::string> v_serv_0_USER_INTERFACE = std::vector<std::string>();
    v_serv_0_USER_INTERFACE.push_back("0_USER_INTERFACE");
    std::shared_ptr<sg_microserv::ElasticTaskManager> serv_0_USER_INTERFACE = std::make_shared<sg_microserv::ElasticTaskManager>("0_USER_INTERFACE",v_serv_0_USER_INTERFACE);
    serviceToETM["0_USER_INTERFACE"] = std::shared_ptr<sg_microserv::ElasticTaskManager>(serv_0_USER_INTERFACE);
    serv_0_USER_INTERFACE->setBootDuration(0);
    // serv_0_USER_INTERFACE->setDataSizeRatio(1); NOT USED ANYMORE, KEEP AS COMMENT IN CASE
    // modify this value in the configuration file (parDeg)
    serv_0_USER_INTERFACE->setParallelTasksPerInst(std::stoi(configServices.find("0_USER_INTERFACE")->second.at(2)));
    serv_0_USER_INTERFACE->setParallelRecPerETM(32);
    // modify this value in the  configuration file (execRatio)
    serv_0_USER_INTERFACE->setExecRatio(std::stod(configServices.find("0_USER_INTERFACE")->second.at(1)));
    serv_0_USER_INTERFACE->setOutputFunction(return_0_USER_INTERFACE);
    serv_0_USER_INTERFACE->setExecAmountFunc(pr_0_USER_INTERFACE);
    serv_0_USER_INTERFACE->setReqNames(reqTypeToStr);
    // set the host location in the config file (location)
    serv_0_USER_INTERFACE->addHost(simgrid::s4u::Host::by_name(configServices.find("0_USER_INTERFACE")->second.at(0)));
    simgrid::s4u::Actor::create("etm_0_USER_INTERFACE", simgrid::s4u::Host::by_name(configServices.find("0_USER_INTERFACE")->second.at(0)), [serv_0_USER_INTERFACE] { serv_0_USER_INTERFACE->run(); });


  /* Elastic policy management */
  XBT_INFO("Elastic policy time");
  std::vector<sg_microserv::ElasticPolicyCPUThreshold*> elasticPolicies;
  for (auto elastoc : elasticPools) {
    XBT_INFO("Setup elasticity for service %s", elastoc.first.c_str());
    sg_microserv::ElasticPolicyCPUThreshold* pol = new sg_microserv::ElasticPolicyCPUThreshold(5,0.7,0.1);
    elasticPolicies.push_back(pol);
    for(auto h : elastoc.second)
      pol->addHost(simgrid::s4u::Host::by_name(h));
    XBT_INFO("ETM name for cpul: %s",elastoc.first.c_str());
    pol->addElasticTaskManager( serviceToETM[elastoc.first.c_str()].get());
    simgrid::s4u::Actor::create("POLICY_"+elastoc.first, simgrid::s4u::Host::by_name("manager"), [pol] { pol->run(); });
  }



  /* ADD DATASOURCES MANUALLY HERE, SET THE END TIMER AS YOU WISH, AND LAUNCH YOUR SIMULATOR*/
   std::vector<simgrid::s4u::ActorPtr> placedDS;

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_0"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_1"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_2"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_3"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_4"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_5"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_6"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_7"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_8"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_9"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_10"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_11"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_12"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_13"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_14"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_15"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_16"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_17"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_18"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_19"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_20"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_21"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_22"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_23"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_24"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_25"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_26"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_27"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_28"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_29"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_30"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_31"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_32"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_33"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_34"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_35"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_36"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_37"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_38"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_39"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_40"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_41"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_42"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_43"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_44"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_45"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_46"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_47"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_48"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_49"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_50"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_51"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_52"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_53"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_54"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_55"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_56"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_57"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_58"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_59"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_60"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_61"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_62"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_63"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_64"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_65"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_66"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_67"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_68"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_69"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_70"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_71"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_72"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_73"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_74"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_75"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_76"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_77"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_78"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_79"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_80"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_81"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_82"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_83"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_84"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_85"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_86"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_87"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_88"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_89"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_90"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_91"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_92"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_93"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_94"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_95"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_96"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_97"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_98"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_99"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_100"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_101"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_102"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_103"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_104"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_105"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_106"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_107"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_108"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_109"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_110"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_111"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_112"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_113"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_114"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_115"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_116"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_117"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_118"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_119"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_120"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_121"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_122"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_123"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_124"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_125"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_126"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_127"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_128"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_129"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_130"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_131"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_132"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_133"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_134"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_135"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_136"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_137"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_138"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_139"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_140"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_141"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_142"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_143"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_144"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_145"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_146"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_147"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_148"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_149"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_150"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_151"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_152"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_153"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_154"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_155"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_156"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_157"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_158"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_159"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_160"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_161"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_162"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_163"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_164"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_165"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_166"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_167"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_168"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_169"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_170"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_171"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_172"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_173"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_174"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_175"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_176"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_177"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_178"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_179"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_180"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_181"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_182"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_183"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_184"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_185"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_186"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_187"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_188"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_189"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_190"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_191"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_192"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_193"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_194"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_195"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_196"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_197"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_198"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_199"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_200"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_201"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_202"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_203"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_204"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_205"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_206"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_207"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_208"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_209"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_210"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_211"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_212"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_213"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_214"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./4, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_215"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_216"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./3, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_217"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_218"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_219"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_220"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./1, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_221"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./5, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_222"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

      {sg_microserv::DataSourceFixedInterval* dsf = new sg_microserv::DataSourceFixedInterval("0_MOTION_DETECT",RequestType::APP_0, 1./2, sizeR);
      simgrid::s4u::ActorPtr dataS = simgrid::s4u::Actor::create("snd", simgrid::s4u::Host::by_name("STA_223"), [&]{dsf->run();});
      placedDS.push_back(dataS);}

  // kill policies and ETMs
  simgrid::s4u::this_actor::sleep_for(230); /*set it according to your needs*/
  XBT_INFO("Done. Killing policies and etms");
  for (auto p : elasticPolicies) {
    p->kill();
  }
  for(auto ds : placedDS) {
    ds->kill();
  }
  simgrid::s4u::this_actor::sleep_for(20);
  simgrid::s4u::Actor::create("k", simgrid::s4u::Host::by_name("manager"), [&] {serv_0_MOTION_DETECT->kill();});
  simgrid::s4u::Actor::create("k", simgrid::s4u::Host::by_name("manager"), [&] {serv_0_OBJECT_DETECT->kill();});
  simgrid::s4u::Actor::create("k", simgrid::s4u::Host::by_name("manager"), [&] {serv_0_OBJECT_TRACKER->kill();});
  simgrid::s4u::Actor::create("k", simgrid::s4u::Host::by_name("manager"), [&] {serv_0_USER_INTERFACE->kill();});
  simgrid::s4u::this_actor::sleep_for(20);
  XBT_INFO("Tot nb req: %d req size: %lf", totNbReq, sizeR);
  simgrid::s4u::Actor::kill_all();
}

static void waiter()
{
  double nextTs = 5;
  while (simgrid::s4u::Engine::get_clock() < 230) {
    auto hList = simgrid::s4u::Engine::get_instance()->get_all_hosts();
    for(auto h : hList) {
      XBT_INFO("load on %s %lf between %lf and %lf", h->get_cname(), sg_host_get_avg_load(h), simgrid::s4u::Engine::get_clock()-5, simgrid::s4u::Engine::get_clock());
      //sg_host_load_reset(h);
    }
    nextTs+=5;
    simgrid::s4u::this_actor::sleep_until(nextTs-0.5);
  }
}

int main(int argc, char* argv[]) {
  if (argc <= 5) {
    std::cout << "Wrong execution line" << std::endl;
		std::cout << argv[0] << "<simgrid-platform-file.xml> <services-config-file.csv> <execR> <retSize>" << std::endl;
		exit(1);
	}


  // read the config from the file provided by the user
  std::ifstream input(argv[2]);
  std::map<std::string, std::vector<std::string>> servConfig;
  std::string v;
  std::vector<std::string> used_hosts = {};
  // pass header
  input>>v>>v>>v>>v;
  for (std::string servName, servLocation, servExecRatio, servParDeg; input >> servName >> servLocation >> servExecRatio >> servParDeg;) {
        servConfig[servName] = {servLocation, servExecRatio, servParDeg};
        used_hosts.push_back(servLocation);
  }

  std::ifstream inputE(argv[3]);
  std::map<std::string, std::vector<std::string>> elasticConf;
  // pass header
  inputE>>v>>v;
  for (std::string servName, elasticLoc; inputE >> servName >> elasticLoc;) {
    XBT_INFO("Host %s for serv %s", servName.c_str(), elasticLoc.c_str());
    if (elasticConf.count(servName)) {
      elasticConf[servName].push_back(elasticLoc);
    } else {
      elasticConf[servName] = {elasticLoc};
    }
  }
  execR = std::stod(argv[4]);
  sizeR = std::stod(argv[5]);
	simgrid::s4u::Engine* e = new simgrid::s4u::Engine(&argc, argv);

  sg_wifi_energy_plugin_init();
  sg_link_energy_plugin_init();
  sg_host_energy_plugin_init();
  sg_host_load_plugin_init();

	e->load_platform(argv[1]);
  for( auto* l : e->get_all_links()) {
    if ((l->get_name().find("Core-link") != std::string::npos) || (l->get_name().find("Backbone-link") != std::string::npos)) {
      l->set_property("wattage_range", "0.0:21.168");
    } else {
      l->set_property("wattage_range", "0.0:0.441"/*"1.12:2.002"*/);
    }
  }

  simgrid::s4u::Actor::create("wait", simgrid::s4u::Host::by_name("manager"), waiter);
	simgrid::s4u::Actor::create("main", simgrid::s4u::Host::by_name(servConfig.find("default")->second.at(0)), [&] {run(servConfig, elasticConf, used_hosts);});

    // setup WiFi bandwidths
  int staCount = 0;
  for (int i=0; i<14*4; i++) {
    std::ostringstream ss;
    ss << "AP_" << i;
    const auto* l = e->link_by_name(ss.str());
    for(int j=0; j<4; j++) {
      std::ostringstream staName;
      staName << "STA_" << staCount;
      XBT_INFO("%s %s",ss.str().c_str(), staName.str().c_str());
      l->set_host_wifi_rate(e->host_by_name(staName.str()), 0);
      staCount++;
    }
  }

	e->run();
	return 0;
}
