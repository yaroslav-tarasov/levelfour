project(ShapeMapper)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ShapeMapper
set( ShapeMapper_h ${ShapeMapper_h}
			ShapeMapperNode.h
			ShapeMapperNodePlugin.h
			)

set( ShapeMapper_moc ${ShapeMapper_moc}	
			ShapeMapperNode.h
			ShapeMapperNodePlugin.h
			)

set( ShapeMapper_src ${ShapeMapper_src}	
			ShapeMapperNode.cpp
			ShapeMapperNodePlugin.cpp
			)

set( ShapeMapper_res ${ShapeMapper_res}	
			ShapeMapper.xml
			)

# Create moc files		   
qt4_wrap_cpp(ShapeMapper_cxx ${ShapeMapper_moc})

# Create source groups
source_group("Moc Files" FILES ${ShapeMapper_cxx})
source_group("Header Files" FILES ${ShapeMapper_h})
source_group("Resources" FILES ${ShapeMapper_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ShapeMapper_src ${ShapeMapper_src} ${ShapeMapper_h} ${ShapeMapper_res})
ENDIF(WIN32)

# Create static library
add_library(ShapeMapper SHARED ${ShapeMapper_src} ${ShapeMapper_cxx})

# Add library dependencies
target_link_libraries(ShapeMapper
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
install( FILES ${ShapeMapper_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ShapeMapper RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
