project(CirclePackingLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# CirclePackingLayouter
set( CirclePackingLayouter_h ${CirclePackingLayouter_h}
			CirclePackingLayouterNode.h
			CirclePackingLayouterNodePlugin.h
			)

set( CirclePackingLayouter_moc ${CirclePackingLayouter_moc}	
			CirclePackingLayouterNode.h
			CirclePackingLayouterNodePlugin.h
			)

set( CirclePackingLayouter_src ${CirclePackingLayouter_src}	
			CirclePackingLayouterNode.cpp
			CirclePackingLayouterNodePlugin.cpp
			)

set( CirclePackingLayouter_res ${CirclePackingLayouter_res}	
			CirclePackingLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(CirclePackingLayouter_cxx ${CirclePackingLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${CirclePackingLayouter_cxx})
source_group("Header Files" FILES ${CirclePackingLayouter_h})
source_group("Resources" FILES ${CirclePackingLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(CirclePackingLayouter_src ${CirclePackingLayouter_src} ${CirclePackingLayouter_h} ${CirclePackingLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(CirclePackingLayouter SHARED ${CirclePackingLayouter_src} ${CirclePackingLayouter_cxx})

# Add library dependencies
target_link_libraries(CirclePackingLayouter
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
install( FILES ${CirclePackingLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS CirclePackingLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
