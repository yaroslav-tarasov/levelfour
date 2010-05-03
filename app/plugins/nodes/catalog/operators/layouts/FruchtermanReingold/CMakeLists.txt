project(FruchtermanReingoldLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# FruchtermanReingoldLayouter
set( FruchtermanReingoldLayouter_h ${FruchtermanReingoldLayouter_h}
			FruchtermanReingoldLayouterNode.h
			FruchtermanReingoldLayouterNodePlugin.h
			)

set( FruchtermanReingoldLayouter_moc ${FruchtermanReingoldLayouter_moc}	
			FruchtermanReingoldLayouterNode.h
			FruchtermanReingoldLayouterNodePlugin.h
			)

set( FruchtermanReingoldLayouter_src ${FruchtermanReingoldLayouter_src}	
			FruchtermanReingoldLayouterNode.cpp
			FruchtermanReingoldLayouterNodePlugin.cpp
			)

set( FruchtermanReingoldLayouter_res ${FruchtermanReingoldLayouter_res}	
			FruchtermanReingoldLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(FruchtermanReingoldLayouter_cxx ${FruchtermanReingoldLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${FruchtermanReingoldLayouter_cxx})
source_group("Header Files" FILES ${FruchtermanReingoldLayouter_h})
source_group("Resources" FILES ${FruchtermanReingoldLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(FruchtermanReingoldLayouter_src ${FruchtermanReingoldLayouter_src} ${FruchtermanReingoldLayouter_h} ${FruchtermanReingoldLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(FruchtermanReingoldLayouter SHARED ${FruchtermanReingoldLayouter_src} ${FruchtermanReingoldLayouter_cxx})

# Add library dependencies
target_link_libraries(FruchtermanReingoldLayouter
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
install( FILES ${FruchtermanReingoldLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS FruchtermanReingoldLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
