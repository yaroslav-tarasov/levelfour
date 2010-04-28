project(SugiyamaLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SugiyamaLayouter
set( SugiyamaLayouter_h ${SugiyamaLayouter_h}
			SugiyamaLayouterNode.h
			SugiyamaLayouterNodePlugin.h
			)

set( SugiyamaLayouter_moc ${SugiyamaLayouter_moc}	
			SugiyamaLayouterNode.h
			SugiyamaLayouterNodePlugin.h
			)

set( SugiyamaLayouter_src ${SugiyamaLayouter_src}	
			SugiyamaLayouterNode.cpp
			SugiyamaLayouterNodePlugin.cpp
			)

set( SugiyamaLayouter_res ${SugiyamaLayouter_res}	
			SugiyamaLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(SugiyamaLayouter_cxx ${SugiyamaLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${SugiyamaLayouter_cxx})
source_group("Header Files" FILES ${SugiyamaLayouter_h})
source_group("Resources" FILES ${SugiyamaLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SugiyamaLayouter_src ${SugiyamaLayouter_src} ${SugiyamaLayouter_h} ${SugiyamaLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(SugiyamaLayouter SHARED ${SugiyamaLayouter_src} ${SugiyamaLayouter_cxx})

# Add library dependencies
target_link_libraries(SugiyamaLayouter
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
install( FILES ${SugiyamaLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SugiyamaLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
