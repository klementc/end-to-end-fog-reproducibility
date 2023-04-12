

# Search for SimGrid
include_directories("${SimGrid_INCLUDE_DIR}" SYSTEM)

add_library(Elasticity ElasticTask.cpp DataSource.cpp ElasticPolicyCPUThreshold.cpp ElasticPolicyHybrid1.cpp ElasticPolicyReactive1.cpp TaskInstance.cpp)

target_include_directories(Elasticity PRIVATE)
target_link_libraries(Elasticity SimGrid::SimGrid)
TARGET_LINK_LIBRARIES(Elasticity ${EXTRA_LIBS})
