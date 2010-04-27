project(singleCycleLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# singleCycleLayouter
set( singleCycleLayouter_h ${singleCycleLayouter_h}
			singleCycleLayouterNode.h
			singleCycleLayouterNodePlugin.h
			)

set( singleCycleLayouter_moc ${singleCycleLayouter_moc}	
			singleCycleLayouterNode.h
			singleCycleLayouterNodePlugin.h
			)

set( singleCycleLayouter_src ${singleCycleLayouter_src}	
			singleCycleLayouterNode.cpp
			singleCycleLayouterNodePlugin.cpp
			)

set( singleCycleLayouter_res ${singleCycleLayouter_res}	
			singleCycleLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(singleCycleLayouter_cxx ${singleCycleLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${singleCycleLayouter_cxx})
source_group("Header Files" FILES ${singleCycleLayouter_h})
source_group("Resources" FILES ${singleCycleLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(singleCycleLayouter_src ${singleCycleLayouter_src} ${singleCycleLayouter_h} ${singleCycleLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(singleCycleLayouter SHARED ${singleCycleLayouter_src} ${singleCycleLayouter_cxx})

# Add library dependencies
target_link_libraries(singleCycleLayouter
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
install( FILES ${singleCycleLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS singleCycleLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
