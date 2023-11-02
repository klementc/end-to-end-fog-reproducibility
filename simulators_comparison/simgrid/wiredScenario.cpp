#include <simgrid/s4u.hpp>
namespace sg4 = simgrid::s4u;

XBT_LOG_NEW_DEFAULT_CATEGORY(s4u_app_pingpong, "Messages specific for this s4u example");

static void pinger(sg4::Mailbox* mailbox_in, sg4::Mailbox* mailbox_out, uint64_t msg_size, double delay)
{
  sg4::this_actor::sleep_until(15.0+delay);
  XBT_INFO("Send message from mailbox %s to mailbox %s", mailbox_in->get_name().c_str(), mailbox_out->get_name().c_str());
  auto* payload = new double(sg4::Engine::get_clock());
  mailbox_out->put(payload, msg_size);
}

static void ponger(sg4::Mailbox* mailbox_in, sg4::Mailbox* mailbox_out, double execCost)
{
  XBT_INFO("Receive message from mailbox %s", mailbox_in->get_name().c_str());

  /* - Receive the (small) ping first ....*/
  auto sender_time          = mailbox_in->get_unique<double>();
  double communication_time = sg4::Engine::get_clock() - *sender_time;
  XBT_INFO("Communication time start: %f end: %f", *sender_time, sg4::Engine::get_clock());
  double preExec = sg4::Engine::get_clock();
  sg4::this_actor::execute(execCost);
  XBT_INFO("Execution finished start: %f end: %f cost: %f", preExec, sg4::Engine::get_clock(), execCost);

}

int main(int argc, char* argv[])
{
  sg4::Engine e(&argc, argv);
  e.load_platform(argv[1]);

  int nbFlows = std::stoi(argv[2]);
  std::string latency = (argv[3]);
  double flowSize = std::stod(argv[4]);
  double delay = std::stod(argv[5]);
  double execCost = std::stod(argv[6])*1e6;

  // set latency of the link
  sg4::Link::by_name("link0")->set_latency(latency);
  sg4::Link::by_name("link1")->set_latency(latency);

  sg4::Mailbox* mb1 = e.mailbox_by_name_or_create("Mailbox 1");
  sg4::Mailbox* mb2 = e.mailbox_by_name_or_create("Mailbox 2");

  for (int i=0; i<nbFlows; i++) {
    sg4::Actor::create("sender_"+std::to_string(i), e.host_by_name("device1"), pinger, mb1, mb2, flowSize, delay*i);
    sg4::Actor::create("receiver_"+std::to_string(i), e.host_by_name("device2"), ponger, mb2, mb1, execCost);
  }

  e.run();

  XBT_INFO("Total simulation time: %.3f", sg4::Engine::get_clock());

  return 0;
}