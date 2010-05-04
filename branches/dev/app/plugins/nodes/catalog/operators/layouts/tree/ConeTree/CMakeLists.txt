project(ConeTreeLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ConeTreeLayouter
set( ConeTreeLayouter_h ${ConeTreeLayouter_h}
			ConeTreeLayouterNode.h
			ConeTreeLayouterNodePlugin.h
			)

set( ConeTreeLayouter_moc ${ConeTreeLayouter_moc}	
			ConeTreeLayouterNode.h
			ConeTreeLayouterNodePlugin.h
			)

set( ConeTreeLayouter_src ${ConeTreeLayouter_src}	
			ConeTreeLayouterNode.cpp
			ConeTreeLayouterNodePlugin.cpp
			)

set( ConeTreeLayouter_res ${ConeTreeLayouter_res}	
			ConeTreeLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(ConeTreeLayouter_cxx ${ConeTreeLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${ConeTreeLayouter_cxx})
source_group("Header Files" FILES ${ConeTreeLayouter_h})
source_group("Resources" FILES ${ConeTreeLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ConeTreeLayouter_src ${ConeTreeLayouter_src} ${ConeTreeLayouter_h} ${ConeTreeLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(ConeTreeLayouter SHARED ${ConeTreeLayouter_src} ${ConeTreeLayouter_cxx})

# Add library dependencies
target_link_libraries(ConeTreeLayouter
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
install( FILES ${ConeTreeLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ConeTreeLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
