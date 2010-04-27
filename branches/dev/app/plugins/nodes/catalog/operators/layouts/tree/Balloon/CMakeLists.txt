project(BalloonLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# BalloonLayouter
set( BalloonLayouter_h ${BalloonLayouter_h}
			BalloonLayouterNode.h
			BalloonLayouterNodePlugin.h
			)

set( BalloonLayouter_moc ${BalloonLayouter_moc}	
			BalloonLayouterNode.h
			BalloonLayouterNodePlugin.h
			)

set( BalloonLayouter_src ${BalloonLayouter_src}	
			BalloonLayouterNode.cpp
			BalloonLayouterNodePlugin.cpp
			)

set( BalloonLayouter_res ${BalloonLayouter_res}	
			BalloonLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(BalloonLayouter_cxx ${BalloonLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${BalloonLayouter_cxx})
source_group("Header Files" FILES ${BalloonLayouter_h})
source_group("Resources" FILES ${BalloonLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(BalloonLayouter_src ${BalloonLayouter_src} ${BalloonLayouter_h} ${BalloonLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(BalloonLayouter SHARED ${BalloonLayouter_src} ${BalloonLayouter_cxx})

# Add library dependencies
target_link_libraries(BalloonLayouter
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
install( FILES ${BalloonLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS BalloonLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
