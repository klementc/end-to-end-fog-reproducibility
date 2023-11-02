EXEC=wiredScenario
EXEC2=execScenario
SG_PATH=~/Code/framagit.org/klement/simgrid_clean/
SG_ARGS= -L ${SG_PATH}/build/lib -lsimgrid -I ${SG_PATH}/build/include -I ${SG_PATH}/include -I ${SG_PATH}/ -I ${SG_PATH}/build/

all: bench_1hop bench_dogbone bench_wifi #$(EXEC) $(EXEC2)

#$(EXEC): wiredScenario.cpp
#	g++ $(SG_ARGS) $^ -o $@
#$(EXEC2): execScenario.cpp
#	g++ $(SG_ARGS) $^ -o $@

bench_1hop: bench1Hop.cpp
	g++ $(SG_ARGS) $^ -o $@

bench_dogbone: benchdogbone.cpp
	g++ $(SG_ARGS) $^ -o $@

bench_wifi: benchwifi.cpp
	g++ $(SG_ARGS) $^ -o $@