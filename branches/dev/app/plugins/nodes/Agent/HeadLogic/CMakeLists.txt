project(headlogic)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# headlogic
set( headlogic_h ${headlogic_h}		
			HeadLogicNode.h
			HeadLogicNodePlugin.h
			)

set( headlogic_moc ${headlogic_moc}	
			HeadLogicNode.h
			HeadLogicNodePlugin.h
			)

set( headlogic_src ${headlogic_src}	
			HeadLogicNode.cpp
			HeadLogicNodePlugin.cpp
			)

set( headlogic_res ${headlogic_res}	
			headlogic.xml
			)

# Create moc files		   
qt4_wrap_cpp(headlogic_cxx ${headlogic_moc})

# Create source groups
source_group("Moc Files" FILES ${headlogic_cxx})
source_group("Header Files" FILES ${headlogic_h})
source_group("Header Files" FILES ${headlogic_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(headlogic_src ${headlogic_src} ${headlogic_h} ${headlogic_res})
ENDIF(WIN32)

# Create static library
add_library(headlogic SHARED ${headlogic_src} ${headlogic_cxx})

# Add library dependencies
target_link_libraries(headlogic
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${headlogic_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS headlogic RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )