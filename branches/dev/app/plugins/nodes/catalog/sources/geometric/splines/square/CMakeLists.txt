project(SquareSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SquareSource
set( SquareSource_h ${SquareSource_h}
			SquareSourceNode.h
			SquareSourceNodePlugin.h
			)

set( SquareSource_moc ${SquareSource_moc}	
			SquareSourceNode.h
			SquareSourceNodePlugin.h
			)

set( SquareSource_src ${SquareSource_src}	
			SquareSourceNode.cpp
			SquareSourceNodePlugin.cpp
			)

set( SquareSource_res ${SquareSource_res}	
			SquareSource.xml
			Square.mesh
			)

# Create moc files		   
qt4_wrap_cpp(SquareSource_cxx ${SquareSource_moc})

# Create source groups
source_group("Moc Files" FILES ${SquareSource_cxx})
source_group("Header Files" FILES ${SquareSource_h})
source_group("Resources" FILES ${SquareSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SquareSource_src ${SquareSource_src} ${SquareSource_h} ${SquareSource_res})
ENDIF(WIN32)

# Create static library
add_library(SquareSource SHARED ${SquareSource_src} ${SquareSource_cxx})

# Add library dependencies
target_link_libraries(SquareSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${SquareSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SquareSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
