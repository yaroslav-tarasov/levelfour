project(universalGraph)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# universalGraph
set( universalGraph_h ${universalGraph_h}
			universalGraph.h
			universalGraphPlugin.h
			)

set( universalGraph_moc ${universalGraph_moc}	
			universalGraph.h
			universalGraphPlugin.h
			)
			
set( universalGraph_src ${universalGraph_src}	
			universalGraph.cpp
			universalGraphPlugin.cpp
			)

set( universalGraph_res ${universalGraph_res}	
			universalgraph.xml
			)

# Create moc files		   
qt4_wrap_cpp(universalGraph_cxx ${universalGraph_moc})

# Create source groups
source_group("Moc Files" FILES ${universalGraph_cxx})
source_group("Generated Files" FILES ${universalGraph_cxx} ${universalGraph_ui_h})
source_group("Header Files" FILES ${universalGraph_h})
source_group("Header Files" FILES ${universalGraph_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(universalGraph_src ${universalGraph_src} ${universalGraph_h} ${universalGraph_res})
ENDIF(WIN32)

# Create static library
add_library(universalGraph SHARED ${universalGraph_src} ${universalGraph_cxx} )

# Add library dependencies
target_link_libraries(universalGraph
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized frappergui debug frappergui_d
			)

# Install files
install( FILES ${universalGraph_res} DESTINATION ${WIDGET_PLUGIN_INSTALL} )
install( TARGETS universalGraph RUNTIME DESTINATION ${WIDGET_PLUGIN_INSTALL} )
