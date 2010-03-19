project(ambientocclusion)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ambientocclusion
set( ambientocclusion_h ${ambientocclusion_h}		
			AmbientOcclusionNode.h
			AmbientOcclusionNodePlugin.h
			)

set( ambientocclusion_moc ${ambientocclusion_moc}	
			AmbientOcclusionNode.h
			AmbientOcclusionNodePlugin.h
			)

set( ambientocclusion_src ${ambientocclusion_src}	
			AmbientOcclusionNode.cpp
			AmbientOcclusionNodePlugin.cpp
			)

set( ambientocclusion_res ${ambientocclusion_res}	
			ambientocclusion.xml
			)

# Create moc files		   
qt4_wrap_cpp(ambientocclusion_cxx ${ambientocclusion_moc})

# Create source groups
source_group("Moc Files" FILES ${ambientocclusion_cxx})
source_group("Header Files" FILES ${ambientocclusion_h})
source_group("Resources" FILES ${ambientocclusion_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ambientocclusion_src ${ambientocclusion_src} ${ambientocclusion_h} ${ambientocclusion_res})
ENDIF(WIN32)

# Create static library
add_library(ambientocclusion SHARED ${ambientocclusion_src} ${ambientocclusion_cxx})

# Add library dependencies
target_link_libraries(ambientocclusion
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${ambientocclusion_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ambientocclusion RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
