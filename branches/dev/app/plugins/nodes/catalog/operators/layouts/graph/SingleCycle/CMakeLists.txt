project(SingleCycleLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SingleCycleLayouter
set( SingleCycleLayouter_h ${SingleCycleLayouter_h}
			SingleCycleLayouterNode.h
			SingleCycleLayouterNodePlugin.h
			)

set( SingleCycleLayouter_moc ${SingleCycleLayouter_moc}	
			SingleCycleLayouterNode.h
			SingleCycleLayouterNodePlugin.h
			)

set( SingleCycleLayouter_src ${SingleCycleLayouter_src}	
			SingleCycleLayouterNode.cpp
			SingleCycleLayouterNodePlugin.cpp
			)

set( SingleCycleLayouter_res ${SingleCycleLayouter_res}	
			SingleCycleLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(SingleCycleLayouter_cxx ${SingleCycleLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${SingleCycleLayouter_cxx})
source_group("Header Files" FILES ${SingleCycleLayouter_h})
source_group("Resources" FILES ${SingleCycleLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SingleCycleLayouter_src ${SingleCycleLayouter_src} ${SingleCycleLayouter_h} ${SingleCycleLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(SingleCycleLayouter SHARED ${SingleCycleLayouter_src} ${SingleCycleLayouter_cxx})

# Add library dependencies
target_link_libraries(SingleCycleLayouter
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
install( FILES ${SingleCycleLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SingleCycleLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
