#include <simgrid/s4u.hpp>
namespace sg4 = simgrid::s4u;

XBT_LOG_NEW_DEFAULT_CATEGORY(s4u_app_pingpong, "Messages specific for this s4u example");

static void executer(uint64_t execCost, double delay)
{
  sg4::this_actor::sleep_until(15.0+delay);
  XBT_INFO("Execute task with cost %d", execCost);
  sg4::this_actor::execute(execCost*1000);
  XBT_INFO("Execution finished, start: %lf end: %lf",15.0+delay, sg4::Engine::get_clock());
}


int main(int argc, char* argv[])
{
  sg4::Engine e(&argc, argv);
  e.load_platform(argv[1]);

  int nbExec = std::stoi(argv[2]);
  double execCost = std::stod(argv[3])*1000;
  double delay = std::stod(argv[4]);
  int nb_cores = std::stoi(argv[5]);

  //sg4::Host::by_name("device1")-> set_core_count(nb_cores);

  for (int i=0; i<nbExec; i++) {
    sg4::Actor::create("executer_"+std::to_string(i), e.host_by_name("device1"), executer, execCost, delay*i);
  }

  e.run();

  XBT_INFO("Total simulation time: %.3f", sg4::Engine::get_clock());

  return 0;
}