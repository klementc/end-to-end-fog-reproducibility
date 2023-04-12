
# Search for SimGrid
include_directories("${SimGrid_INCLUDE_DIR}" SYSTEM)

INCLUDE_DIRECTORIES(../Elasticity)


FILE(COPY ${CMAKE_CURRENT_SOURCE_DIR}/../Elasticity/config.yml
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

ADD_EXECUTABLE( 14APPS_video_cloud 14APPS_video_cloud.cpp )
TARGET_LINK_LIBRARIES(14APPS_video_cloud Elasticity simgrid ${EXTRA_LIBS})

ADD_EXECUTABLE( 14APPS_video_fog 14APPS_video_fog.cpp )
TARGET_LINK_LIBRARIES(14APPS_video_fog Elasticity simgrid ${EXTRA_LIBS})

ADD_EXECUTABLE( 14APPS_video_cloud_coloc 14APPS_video_cloud_coloc.cpp )
TARGET_LINK_LIBRARIES(14APPS_video_cloud_coloc Elasticity simgrid ${EXTRA_LIBS})

ADD_EXECUTABLE( 14APPS_video_fog_coloc 14APPS_video_fog_coloc.cpp )
TARGET_LINK_LIBRARIES(14APPS_video_fog_coloc Elasticity simgrid ${EXTRA_LIBS})
