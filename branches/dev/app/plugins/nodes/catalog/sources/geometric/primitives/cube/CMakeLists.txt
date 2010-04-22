project(CubeSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# CubeSource
set( CubeSource_h ${CubeSource_h}
			CubeSourceNode.h
			CubeSourceNodePlugin.h
			)

set( CubeSource_moc ${CubeSource_moc}	
			CubeSourceNode.h
			CubeSourceNodePlugin.h
			)

set( CubeSource_src ${CubeSource_src}	
			CubeSourceNode.cpp
			CubeSourceNodePlugin.cpp
			)

set( CubeSource_res ${CubeSource_res}	
			CubeSource.xml
			)

# Create moc files		   
qt4_wrap_cpp(CubeSource_cxx ${CubeSource_moc})

# Create source groups
source_group("Moc Files" FILES ${CubeSource_cxx})
source_group("Header Files" FILES ${CubeSource_h})
source_group("Resources" FILES ${CubeSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(CubeSource_src ${CubeSource_src} ${CubeSource_h} ${CubeSource_res})
ENDIF(WIN32)

# Create static library
add_library(CubeSource SHARED ${CubeSource_src} ${CubeSource_cxx})

# Add library dependencies
target_link_libraries(CubeSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${CubeSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS CubeSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
