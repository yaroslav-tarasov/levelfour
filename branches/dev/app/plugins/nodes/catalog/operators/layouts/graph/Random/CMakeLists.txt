project(RandomLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# RandomLayouter
set( RandomLayouter_h ${RandomLayouter_h}
			RandomLayouterNode.h
			RandomLayouterNodePlugin.h
			)

set( RandomLayouter_moc ${RandomLayouter_moc}	
			RandomLayouterNode.h
			RandomLayouterNodePlugin.h
			)

set( RandomLayouter_src ${RandomLayouter_src}	
			RandomLayouterNode.cpp
			RandomLayouterNodePlugin.cpp
			)

set( RandomLayouter_res ${RandomLayouter_res}	
			RandomLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(RandomLayouter_cxx ${RandomLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${RandomLayouter_cxx})
source_group("Header Files" FILES ${RandomLayouter_h})
source_group("Resources" FILES ${RandomLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(RandomLayouter_src ${RandomLayouter_src} ${RandomLayouter_h} ${RandomLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(RandomLayouter SHARED ${RandomLayouter_src} ${RandomLayouter_cxx})

# Add library dependencies
target_link_libraries(RandomLayouter
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
install( FILES ${RandomLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS RandomLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
