project(ColorMapper)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ColorMapper
set( ColorMapper_h ${ColorMapper_h}
			ColorMapperNode.h
			ColorMapperNodePlugin.h
			)

set( ColorMapper_moc ${ColorMapper_moc}	
			ColorMapperNode.h
			ColorMapperNodePlugin.h
			)

set( ColorMapper_src ${ColorMapper_src}	
			ColorMapperNode.cpp
			ColorMapperNodePlugin.cpp
			)

set( ColorMapper_res ${ColorMapper_res}	
			ColorMapper.xml
			)

# Create moc files		   
qt4_wrap_cpp(ColorMapper_cxx ${ColorMapper_moc})

# Create source groups
source_group("Moc Files" FILES ${ColorMapper_cxx})
source_group("Header Files" FILES ${ColorMapper_h})
source_group("Resources" FILES ${ColorMapper_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ColorMapper_src ${ColorMapper_src} ${ColorMapper_h} ${ColorMapper_res})
ENDIF(WIN32)

# Create static library
add_library(ColorMapper SHARED ${ColorMapper_src} ${ColorMapper_cxx})

# Add library dependencies
target_link_libraries(ColorMapper
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${ColorMapper_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ColorMapper RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
