project(SquarifyLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SquarifyLayouter
set( SquarifyLayouter_h ${SquarifyLayouter_h}
			SquarifyLayouterNode.h
			SquarifyLayouterNodePlugin.h
			)

set( SquarifyLayouter_moc ${SquarifyLayouter_moc}	
			SquarifyLayouterNode.h
			SquarifyLayouterNodePlugin.h
			)

set( SquarifyLayouter_src ${SquarifyLayouter_src}	
			SquarifyLayouterNode.cpp
			SquarifyLayouterNodePlugin.cpp
			)

set( SquarifyLayouter_res ${SquarifyLayouter_res}	
			SquarifyLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(SquarifyLayouter_cxx ${SquarifyLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${SquarifyLayouter_cxx})
source_group("Header Files" FILES ${SquarifyLayouter_h})
source_group("Resources" FILES ${SquarifyLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SquarifyLayouter_src ${SquarifyLayouter_src} ${SquarifyLayouter_h} ${SquarifyLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(SquarifyLayouter SHARED ${SquarifyLayouter_src} ${SquarifyLayouter_cxx})

# Add library dependencies
target_link_libraries(SquarifyLayouter
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
install( FILES ${SquarifyLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SquarifyLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
