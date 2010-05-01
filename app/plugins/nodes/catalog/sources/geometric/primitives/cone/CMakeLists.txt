project(ConeSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ConeSource
set( ConeSource_h ${ConeSource_h}
			ConeSourceNode.h
			ConeSourceNodePlugin.h
			)

set( ConeSource_moc ${ConeSource_moc}	
			ConeSourceNode.h
			ConeSourceNodePlugin.h
			)

set( ConeSource_src ${ConeSource_src}	
			ConeSourceNode.cpp
			ConeSourceNodePlugin.cpp
			)

set( ConeSource_res ${ConeSource_res}	
			ConeSource.xml
			Cone.mesh
			)

# Create moc files		   
qt4_wrap_cpp(ConeSource_cxx ${ConeSource_moc})

# Create source groups
source_group("Moc Files" FILES ${ConeSource_cxx})
source_group("Header Files" FILES ${ConeSource_h})
source_group("Resources" FILES ${ConeSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ConeSource_src ${ConeSource_src} ${ConeSource_h} ${ConeSource_res})
ENDIF(WIN32)

# Create static library
add_library(ConeSource SHARED ${ConeSource_src} ${ConeSource_cxx})

# Add library dependencies
target_link_libraries(ConeSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${ConeSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ConeSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
