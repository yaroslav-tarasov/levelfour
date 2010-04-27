project(PositionMapper)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# PositionMapper
set( PositionMapper_h ${PositionMapper_h}
			PositionMapperNode.h
			PositionMapperNodePlugin.h
			)

set( PositionMapper_moc ${PositionMapper_moc}	
			PositionMapperNode.h
			PositionMapperNodePlugin.h
			)

set( PositionMapper_src ${PositionMapper_src}	
			PositionMapperNode.cpp
			PositionMapperNodePlugin.cpp
			)

set( PositionMapper_res ${PositionMapper_res}	
			PositionMapper.xml
			)

# Create moc files		   
qt4_wrap_cpp(PositionMapper_cxx ${PositionMapper_moc})

# Create source groups
source_group("Moc Files" FILES ${PositionMapper_cxx})
source_group("Header Files" FILES ${PositionMapper_h})
source_group("Resources" FILES ${PositionMapper_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(PositionMapper_src ${PositionMapper_src} ${PositionMapper_h} ${PositionMapper_res})
ENDIF(WIN32)

# Create static library
add_library(PositionMapper SHARED ${PositionMapper_src} ${PositionMapper_cxx})

# Add library dependencies
target_link_libraries(PositionMapper
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
install( FILES ${PositionMapper_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS PositionMapper RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
