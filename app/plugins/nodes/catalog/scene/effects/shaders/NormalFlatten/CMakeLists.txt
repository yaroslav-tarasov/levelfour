project(normalflatten)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

link_directories(
			${frappercore_BINARY_DIR}
			${QT_LIBRARY_DIR}
			$ENV{OGRE_HOME}/lib
			)

# normalflatten
set( normalflatten_h ${normalflatten_h}		
			NormalFlattenNode.h
			NormalFlattenNodePlugin.h
			)

set( normalflatten_moc ${normalflatten_moc}	
			NormalFlattenNode.h
			NormalFlattenNodePlugin.h
			)

set( normalflatten_src ${normalflatten_src}	
			NormalFlattenNode.cpp
			NormalFlattenNodePlugin.cpp
			)

set( normalflatten_res ${normalflatten_res}	
			normalflatten.xml
			)

# Create moc files		   
qt4_wrap_cpp(normalflatten_cxx ${normalflatten_moc})

# Create source groups
source_group("Moc Files" FILES ${normalflatten_cxx})
source_group("Header Files" FILES ${normalflatten_h})
source_group("Resources" FILES ${normalflatten_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(normalflatten_src ${normalflatten_src} ${normalflatten_h} ${normalflatten_res})
ENDIF(WIN32)

# Create static library
add_library(normalflatten SHARED ${normalflatten_src} ${normalflatten_cxx})

# Add library dependencies
target_link_libraries(normalflatten
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${normalflatten_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS normalflatten RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
