import logging
import time
import enoslib as en

en.init_logging(level=logging.INFO)
en.check()

conf = (
    en.G5kConf.from_settings(job_type=[], walltime="01:00:00")
    .add_machine(
        roles=["paris"],
        cluster="paravance",
        nodes=1,
    )
    .add_machine(
        roles=["berlin"],
        cluster="paravance",
        nodes=1,
    )
)

provider = en.G5k(conf)
roles, networks = provider.init()
roles = en.sync_info(roles, networks)
# Building the network constraints

netem = en.NetemHTB()
(
    netem.add_constraints(
        src=roles["paris"],
        dest=roles["berlin"],
        delay="0ms",
        rate="50mbit",
        symmetric=True,
    )
)
netem.deploy()
netem.validate()

target = roles["berlin"][0]
source = roles["paris"][0]

print(f"{target.address}")
print(f"{source.address}")
#resultsc = en.run_command(f"sleep 10; python /home/ccourageuxsudan/client.py {target.address}", roles=roles["paris"])
#resultss = en.run_command(f"python /home/ccourageuxsudan/server.py", roles=roles["berlin"])
time.sleep(10)

#print(f"{resultss}")
#print(f"{resultsc}")
#print(f"{resultc.stdout}")


#netem.destroy()
#provider.destroy()