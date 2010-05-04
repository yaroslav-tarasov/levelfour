project(SpringEadesLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SpringEadesLayouter
set( SpringEadesLayouter_h ${SpringEadesLayouter_h}
			SpringEadesLayouterNode.h
			SpringEadesLayouterNodePlugin.h
			)

set( SpringEadesLayouter_moc ${SpringEadesLayouter_moc}	
			SpringEadesLayouterNode.h
			SpringEadesLayouterNodePlugin.h
			)

set( SpringEadesLayouter_src ${SpringEadesLayouter_src}	
			SpringEadesLayouterNode.cpp
			SpringEadesLayouterNodePlugin.cpp
			)

set( SpringEadesLayouter_res ${SpringEadesLayouter_res}	
			SpringEadesLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(SpringEadesLayouter_cxx ${SpringEadesLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${SpringEadesLayouter_cxx})
source_group("Header Files" FILES ${SpringEadesLayouter_h})
source_group("Resources" FILES ${SpringEadesLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SpringEadesLayouter_src ${SpringEadesLayouter_src} ${SpringEadesLayouter_h} ${SpringEadesLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(SpringEadesLayouter SHARED ${SpringEadesLayouter_src} ${SpringEadesLayouter_cxx})

# Add library dependencies
target_link_libraries(SpringEadesLayouter
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
install( FILES ${SpringEadesLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SpringEadesLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
