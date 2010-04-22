project(MaterialMapper)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# MaterialMapper
set( MaterialMapper_h ${MaterialMapper_h}
			MaterialMapperNode.h
			MaterialMapperNodePlugin.h
			)

set( MaterialMapper_moc ${MaterialMapper_moc}	
			MaterialMapperNode.h
			MaterialMapperNodePlugin.h
			)

set( MaterialMapper_src ${MaterialMapper_src}	
			MaterialMapperNode.cpp
			MaterialMapperNodePlugin.cpp
			)

set( MaterialMapper_res ${MaterialMapper_res}	
			MaterialMapper.xml
			)

# Create moc files		   
qt4_wrap_cpp(MaterialMapper_cxx ${MaterialMapper_moc})

# Create source groups
source_group("Moc Files" FILES ${MaterialMapper_cxx})
source_group("Header Files" FILES ${MaterialMapper_h})
source_group("Resources" FILES ${MaterialMapper_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(MaterialMapper_src ${MaterialMapper_src} ${MaterialMapper_h} ${MaterialMapper_res})
ENDIF(WIN32)

# Create static library
add_library(MaterialMapper SHARED ${MaterialMapper_src} ${MaterialMapper_cxx})

# Add library dependencies
target_link_libraries(MaterialMapper
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${MaterialMapper_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS MaterialMapper RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
