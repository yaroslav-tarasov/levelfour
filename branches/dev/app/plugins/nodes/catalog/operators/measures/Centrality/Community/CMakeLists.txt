project(communityCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# communityCentrality
set( communityCentrality_h ${communityCentrality_h}
			communityCentralityNode.h
			communityCentralityNodePlugin.h
			)

set( communityCentrality_moc ${communityCentrality_moc}	
			communityCentralityNode.h
			communityCentralityNodePlugin.h
			)

set( communityCentrality_src ${communityCentrality_src}	
			communityCentralityNode.cpp
			communityCentralityNodePlugin.cpp
			)

set( communityCentrality_res ${communityCentrality_res}	
			communityCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(communityCentrality_cxx ${communityCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${communityCentrality_cxx})
source_group("Header Files" FILES ${communityCentrality_h})
source_group("Resources" FILES ${communityCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(communityCentrality_src ${communityCentrality_src} ${communityCentrality_h} ${communityCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(communityCentrality SHARED ${communityCentrality_src} ${communityCentrality_cxx})

# Add library dependencies
target_link_libraries(communityCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${communityCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS communityCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
