project(noteNode)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# note
set( note_h ${note_h}
			noteNode.h
			noteNodePlugin.h
			)

set( note_moc ${note_moc}	
			noteNode.h
			noteNodePlugin.h
			)

set( note_src ${note_src}	
			noteNode.cpp
			noteNodePlugin.cpp
			)

set( note_res ${note_res}	
			note.xml
			)

# Create moc files		   
qt4_wrap_cpp(note_cxx ${note_moc})

# Create source groups
source_group("Moc Files" FILES ${note_cxx})
source_group("Header Files" FILES ${note_h})
source_group("Resources" FILES ${note_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(note_src ${note_src} ${note_h} ${note_res})
ENDIF(WIN32)

# Create static library
add_library(note SHARED ${note_src} ${note_cxx})

# Add library dependencies
target_link_libraries(note
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${note_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS note RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
