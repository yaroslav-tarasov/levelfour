project(GeoDemLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# GeoDemLayouter
set( GeoDemLayouter_h ${GeoDemLayouter_h}
			GeoDemLayouterNode.h
			GeoDemLayouterNodePlugin.h
			)

set( GeoDemLayouter_moc ${GeoDemLayouter_moc}	
			GeoDemLayouterNode.h
			GeoDemLayouterNodePlugin.h
			)

set( GeoDemLayouter_src ${GeoDemLayouter_src}	
			GeoDemLayouterNode.cpp
			GeoDemLayouterNodePlugin.cpp
			)

set( GeoDemLayouter_res ${GeoDemLayouter_res}	
			GeoDemLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(GeoDemLayouter_cxx ${GeoDemLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${GeoDemLayouter_cxx})
source_group("Header Files" FILES ${GeoDemLayouter_h})
source_group("Resources" FILES ${GeoDemLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(GeoDemLayouter_src ${GeoDemLayouter_src} ${GeoDemLayouter_h} ${GeoDemLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(GeoDemLayouter SHARED ${GeoDemLayouter_src} ${GeoDemLayouter_cxx})

# Add library dependencies
target_link_libraries(GeoDemLayouter
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
install( FILES ${GeoDemLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS GeoDemLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
