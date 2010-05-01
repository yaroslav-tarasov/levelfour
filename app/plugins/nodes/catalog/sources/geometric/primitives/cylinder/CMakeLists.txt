project(CylinderSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# CylinderSource
set( CylinderSource_h ${CylinderSource_h}
			CylinderSourceNode.h
			CylinderSourceNodePlugin.h
			)

set( CylinderSource_moc ${CylinderSource_moc}	
			CylinderSourceNode.h
			CylinderSourceNodePlugin.h
			)

set( CylinderSource_src ${CylinderSource_src}	
			CylinderSourceNode.cpp
			CylinderSourceNodePlugin.cpp
			)

set( CylinderSource_res ${CylinderSource_res}	
			CylinderSource.xml
			Cylinder.mesh
			)

# Create moc files		   
qt4_wrap_cpp(CylinderSource_cxx ${CylinderSource_moc})

# Create source groups
source_group("Moc Files" FILES ${CylinderSource_cxx})
source_group("Header Files" FILES ${CylinderSource_h})
source_group("Resources" FILES ${CylinderSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(CylinderSource_src ${CylinderSource_src} ${CylinderSource_h} ${CylinderSource_res})
ENDIF(WIN32)

# Create static library
add_library(CylinderSource SHARED ${CylinderSource_src} ${CylinderSource_cxx})

# Add library dependencies
target_link_libraries(CylinderSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${CylinderSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS CylinderSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
