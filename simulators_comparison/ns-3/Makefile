

##### NS3 g++ Arguments
NS3_ARGS= -D NS3_LOG_ENABLE -L /home/clem/Code/gitlab.com/nsnam/ns-allinone-3.39/ns-3.39/build/lib -I/home/clem/Code/gitlab.com/nsnam/ns-allinone-3.39/ns-3.39/build/include/
NS3_ARGS+=$(addprefix -l, $(subst lib,,$(subst .so,,$(notdir $(wildcard /home/clem/Code/gitlab.com/nsnam/ns-allinone-3.39/ns-3.39/build/lib/libns3*.so)))))
NS3_VERSION="3.39"


all: wiredScenario wired_nhop wired_dogbone wifiScenario

wiredScenario: wiredScenario.cpp
	g++ -std=c++17 -fPIC -g  -D NS3_VERSION=\"${NS3_VERSION}\" $(NS3_ARGS) wiredScenario.cpp -o $@  $(NS3_ARGS)

wired_nhop: wired_nhop.cpp
	g++ -std=c++17 -fPIC -g  -D NS3_VERSION=\"${NS3_VERSION}\" $(NS3_ARGS) wired_nhop.cpp -o $@  $(NS3_ARGS)

wired_dogbone: wired_dogbone.cpp
	g++ -std=c++17 -fPIC -g  -D NS3_VERSION=\"${NS3_VERSION}\" $(NS3_ARGS) wired_dogbone.cpp -o $@  $(NS3_ARGS)

wifiScenario: wifiScenario.cpp
	g++ -std=c++17 -fPIC -g  -D NS3_VERSION=\"${NS3_VERSION}\" $(NS3_ARGS) wifiScenario.cpp -o $@  $(NS3_ARGS)

clean:
	- rm wired_nhop wiredScenario


.PHONY: clean
