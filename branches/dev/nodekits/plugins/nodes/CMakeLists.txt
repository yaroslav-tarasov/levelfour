project(nodes)

if (FRAPPER_BUILD_NODES_3D)
add_subdirectory(3d)
endif (FRAPPER_BUILD_NODES_3D)

if (FRAPPER_BUILD_NODES_AGENT)
add_subdirectory(Agent)
endif (FRAPPER_BUILD_NODES_AGENT)

if (FRAPPER_BUILD_NODES_COMPOSITING)
add_subdirectory(Compositing)
endif (FRAPPER_BUILD_NODES_COMPOSITING)

if (FRAPPER_BUILD_NODES_INPUT)
add_subdirectory(Input)
endif (FRAPPER_BUILD_NODES_INPUT)

if (FRAPPER_BUILD_NODES_MISC)
add_subdirectory(Misc)
endif (FRAPPER_BUILD_NODES_MISC)

if (FRAPPER_BUILD_NODES_NPR)
add_subdirectory(Npr)
endif (FRAPPER_BUILD_NODES_NPR)

if (FRAPPER_BUILD_NODES_SHADING)
add_subdirectory(Shading)
endif (FRAPPER_BUILD_NODES_SHADING)