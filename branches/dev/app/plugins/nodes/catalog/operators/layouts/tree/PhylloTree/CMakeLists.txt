project(PhylloTreeLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# PhylloTreeLayouter
set( PhylloTreeLayouter_h ${PhylloTreeLayouter_h}
			PhylloTreeLayouterNode.h
			PhylloTreeLayouterNodePlugin.h
			)

set( PhylloTreeLayouter_moc ${PhylloTreeLayouter_moc}	
			PhylloTreeLayouterNode.h
			PhylloTreeLayouterNodePlugin.h
			)

set( PhylloTreeLayouter_src ${PhylloTreeLayouter_src}	
			PhylloTreeLayouterNode.cpp
			PhylloTreeLayouterNodePlugin.cpp
			)

set( PhylloTreeLayouter_res ${PhylloTreeLayouter_res}	
			PhylloTreeLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(PhylloTreeLayouter_cxx ${PhylloTreeLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${PhylloTreeLayouter_cxx})
source_group("Header Files" FILES ${PhylloTreeLayouter_h})
source_group("Resources" FILES ${PhylloTreeLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(PhylloTreeLayouter_src ${PhylloTreeLayouter_src} ${PhylloTreeLayouter_h} ${PhylloTreeLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(PhylloTreeLayouter SHARED ${PhylloTreeLayouter_src} ${PhylloTreeLayouter_cxx})

# Add library dependencies
target_link_libraries(PhylloTreeLayouter
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
install( FILES ${PhylloTreeLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS PhylloTreeLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
