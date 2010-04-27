project(CartogramLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# CartogramLayouter
set( CartogramLayouter_h ${CartogramLayouter_h}
			CartogramLayouterNode.h
			CartogramLayouterNodePlugin.h
			)

set( CartogramLayouter_moc ${CartogramLayouter_moc}	
			CartogramLayouterNode.h
			CartogramLayouterNodePlugin.h
			)

set( CartogramLayouter_src ${CartogramLayouter_src}	
			CartogramLayouterNode.cpp
			CartogramLayouterNodePlugin.cpp
			)

set( CartogramLayouter_res ${CartogramLayouter_res}	
			CartogramLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(CartogramLayouter_cxx ${CartogramLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${CartogramLayouter_cxx})
source_group("Header Files" FILES ${CartogramLayouter_h})
source_group("Resources" FILES ${CartogramLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(CartogramLayouter_src ${CartogramLayouter_src} ${CartogramLayouter_h} ${CartogramLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(CartogramLayouter SHARED ${CartogramLayouter_src} ${CartogramLayouter_cxx})

# Add library dependencies
target_link_libraries(CartogramLayouter
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkFiltering debug vtkFiltering
			optimized vtkInfovis debug vtkInfovis
			optimized vtkCommon debug vtkCommon
			)

# Install files
install( FILES ${CartogramLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS CartogramLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
