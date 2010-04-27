project(OrthogonalLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# OrthogonalLayouter
set( OrthogonalLayouter_h ${OrthogonalLayouter_h}
			OrthogonalLayouterNode.h
			OrthogonalLayouterNodePlugin.h
			)

set( OrthogonalLayouter_moc ${OrthogonalLayouter_moc}	
			OrthogonalLayouterNode.h
			OrthogonalLayouterNodePlugin.h
			)

set( OrthogonalLayouter_src ${OrthogonalLayouter_src}	
			OrthogonalLayouterNode.cpp
			OrthogonalLayouterNodePlugin.cpp
			)

set( OrthogonalLayouter_res ${OrthogonalLayouter_res}	
			OrthogonalLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(OrthogonalLayouter_cxx ${OrthogonalLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${OrthogonalLayouter_cxx})
source_group("Header Files" FILES ${OrthogonalLayouter_h})
source_group("Resources" FILES ${OrthogonalLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(OrthogonalLayouter_src ${OrthogonalLayouter_src} ${OrthogonalLayouter_h} ${OrthogonalLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(OrthogonalLayouter SHARED ${OrthogonalLayouter_src} ${OrthogonalLayouter_cxx})

# Add library dependencies
target_link_libraries(OrthogonalLayouter
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
install( FILES ${OrthogonalLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS OrthogonalLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
