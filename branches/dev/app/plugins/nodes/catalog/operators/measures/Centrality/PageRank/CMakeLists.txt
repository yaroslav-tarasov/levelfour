project(pageRankCentrality)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# pageRankCentrality
set( pageRankCentrality_h ${pageRankCentrality_h}
			pageRankCentralityNode.h
			pageRankCentralityNodePlugin.h
			)

set( pageRankCentrality_moc ${pageRankCentrality_moc}	
			pageRankCentralityNode.h
			pageRankCentralityNodePlugin.h
			)

set( pageRankCentrality_src ${pageRankCentrality_src}	
			pageRankCentralityNode.cpp
			pageRankCentralityNodePlugin.cpp
			)

set( pageRankCentrality_res ${pageRankCentrality_res}	
			pageRankCentrality.xml
			)

# Create moc files		   
qt4_wrap_cpp(pageRankCentrality_cxx ${pageRankCentrality_moc})

# Create source groups
source_group("Moc Files" FILES ${pageRankCentrality_cxx})
source_group("Header Files" FILES ${pageRankCentrality_h})
source_group("Resources" FILES ${pageRankCentrality_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(pageRankCentrality_src ${pageRankCentrality_src} ${pageRankCentrality_h} ${pageRankCentrality_res})
ENDIF(WIN32)

# Create static library
add_library(pageRankCentrality SHARED ${pageRankCentrality_src} ${pageRankCentrality_cxx})

# Add library dependencies
target_link_libraries(pageRankCentrality
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${pageRankCentrality_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS pageRankCentrality RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
