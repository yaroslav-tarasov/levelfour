project(ThreeDCirclesLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ThreeDCirclesLayouter
set( ThreeDCirclesLayouter_h ${ThreeDCirclesLayouter_h}
			ThreeDCirclesLayouterNode.h
			ThreeDCirclesLayouterNodePlugin.h
			)

set( ThreeDCirclesLayouter_moc ${ThreeDCirclesLayouter_moc}	
			ThreeDCirclesLayouterNode.h
			ThreeDCirclesLayouterNodePlugin.h
			)

set( ThreeDCirclesLayouter_src ${ThreeDCirclesLayouter_src}	
			ThreeDCirclesLayouterNode.cpp
			ThreeDCirclesLayouterNodePlugin.cpp
			)

set( ThreeDCirclesLayouter_res ${ThreeDCirclesLayouter_res}	
			ThreeDCirclesLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(ThreeDCirclesLayouter_cxx ${ThreeDCirclesLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${ThreeDCirclesLayouter_cxx})
source_group("Header Files" FILES ${ThreeDCirclesLayouter_h})
source_group("Resources" FILES ${ThreeDCirclesLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ThreeDCirclesLayouter_src ${ThreeDCirclesLayouter_src} ${ThreeDCirclesLayouter_h} ${ThreeDCirclesLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(ThreeDCirclesLayouter SHARED ${ThreeDCirclesLayouter_src} ${ThreeDCirclesLayouter_cxx})

# Add library dependencies
target_link_libraries(ThreeDCirclesLayouter
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
install( FILES ${ThreeDCirclesLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ThreeDCirclesLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
