project(ScratchedFilm)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ScratchedFilm
set( ScratchedFilm_h ${ScratchedFilm_h}		
			ScratchedFilmNode.h
			ScratchedFilmNodePlugin.h
			)

set( ScratchedFilm_moc ${ScratchedFilm_moc}	
			ScratchedFilmNode.h
			ScratchedFilmNodePlugin.h
			)

set( ScratchedFilm_src ${ScratchedFilm_src}	
			ScratchedFilmNode.cpp
			ScratchedFilmNodePlugin.cpp
			)

set( ScratchedFilm_res ${ScratchedFilm_res}	
			ScratchedFilm.xml
			)

# Create moc files		   
qt4_wrap_cpp(ScratchedFilm_cxx ${ScratchedFilm_moc})

# Create source groups
source_group("Moc Files" FILES ${ScratchedFilm_cxx})
source_group("Header Files" FILES ${ScratchedFilm_h})
source_group("Resources" FILES ${ScratchedFilm_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ScratchedFilm_src ${ScratchedFilm_src} ${ScratchedFilm_h} ${ScratchedFilm_res})
ENDIF(WIN32)

# Create static library
add_library(ScratchedFilm SHARED ${ScratchedFilm_src} ${ScratchedFilm_cxx})

# Add library dependencies
target_link_libraries(ScratchedFilm
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${ScratchedFilm_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ScratchedFilm RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
