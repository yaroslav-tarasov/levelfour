project(SizeMapper)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# SizeMapper
set( SizeMapper_h ${SizeMapper_h}
			SizeMapperNode.h
			SizeMapperNodePlugin.h
			)

set( SizeMapper_moc ${SizeMapper_moc}	
			SizeMapperNode.h
			SizeMapperNodePlugin.h
			)

set( SizeMapper_src ${SizeMapper_src}	
			SizeMapperNode.cpp
			SizeMapperNodePlugin.cpp
			)

set( SizeMapper_res ${SizeMapper_res}	
			SizeMapper.xml
			)

# Create moc files		   
qt4_wrap_cpp(SizeMapper_cxx ${SizeMapper_moc})

# Create source groups
source_group("Moc Files" FILES ${SizeMapper_cxx})
source_group("Header Files" FILES ${SizeMapper_h})
source_group("Resources" FILES ${SizeMapper_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(SizeMapper_src ${SizeMapper_src} ${SizeMapper_h} ${SizeMapper_res})
ENDIF(WIN32)

# Create static library
add_library(SizeMapper SHARED ${SizeMapper_src} ${SizeMapper_cxx})

# Add library dependencies
target_link_libraries(SizeMapper
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${SizeMapper_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS SizeMapper RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
