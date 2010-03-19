project(plugins)

add_subdirectory(nodes)

if (FRAPPER_BUILD_NODES_ADDITIONAL)
if(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/nodes_additional)
add_subdirectory(nodes_additional)
endif(IS_DIRECTORY ${CMAKE_SOURCE_DIR}/plugins/nodes_additional)
endif (FRAPPER_BUILD_NODES_ADDITIONAL)

if (FRAPPER_BUILD_PANELS)
add_subdirectory(panels)
endif (FRAPPER_BUILD_PANELS)

if (FRAPPER_BUILD_WIDGETS)
add_subdirectory(widgets)
endif (FRAPPER_BUILD_WIDGETS)