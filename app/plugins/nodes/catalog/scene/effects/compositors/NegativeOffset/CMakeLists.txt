project(NegativeOffset)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# NegativeOffset
set( NegativeOffset_h ${NegativeOffset_h}		
			NegativeOffsetNode.h
			NegativeOffsetNodePlugin.h
			)

set( NegativeOffset_moc ${NegativeOffset_moc}	
			NegativeOffsetNode.h
			NegativeOffsetNodePlugin.h
			)

set( NegativeOffset_src ${NegativeOffset_src}	
			NegativeOffsetNode.cpp
			NegativeOffsetNodePlugin.cpp
			)

set( NegativeOffset_res ${NegativeOffset_res}	
			NegativeOffset.xml
			)

# Create moc files		   
qt4_wrap_cpp(NegativeOffset_cxx ${NegativeOffset_moc})

# Create source groups
source_group("Moc Files" FILES ${NegativeOffset_cxx})
source_group("Header Files" FILES ${NegativeOffset_h})
source_group("Resources" FILES ${NegativeOffset_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(NegativeOffset_src ${NegativeOffset_src} ${NegativeOffset_h} ${NegativeOffset_res})
ENDIF(WIN32)

# Create static library
add_library(NegativeOffset SHARED ${NegativeOffset_src} ${NegativeOffset_cxx})

# Add library dependencies
target_link_libraries(NegativeOffset
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${NegativeOffset_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS NegativeOffset RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
