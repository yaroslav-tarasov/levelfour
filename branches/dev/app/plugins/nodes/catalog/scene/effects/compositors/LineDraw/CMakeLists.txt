project(LineDraw)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# LineDraw
set( LineDraw_h ${LineDraw_h}		
			LineDrawNode.h
			LineDrawNodePlugin.h
			)

set( LineDraw_moc ${LineDraw_moc}	
			LineDrawNode.h
			LineDrawNodePlugin.h
			)

set( LineDraw_src ${LineDraw_src}	
			LineDrawNode.cpp
			LineDrawNodePlugin.cpp
			)

set( LineDraw_res ${LineDraw_res}	
			LineDraw.xml
			)

# Create moc files		   
qt4_wrap_cpp(LineDraw_cxx ${LineDraw_moc})

# Create source groups
source_group("Moc Files" FILES ${LineDraw_cxx})
source_group("Header Files" FILES ${LineDraw_h})
source_group("Resources" FILES ${LineDraw_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(LineDraw_src ${LineDraw_src} ${LineDraw_h} ${LineDraw_res})
ENDIF(WIN32)

# Create static library
add_library(LineDraw SHARED ${LineDraw_src} ${LineDraw_cxx})

# Add library dependencies
target_link_libraries(LineDraw
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${LineDraw_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS LineDraw RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
