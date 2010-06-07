project(ShpSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# ShpSource
set( ShpSource_h ${ShpSource_h}
			ShpSourceNode.h
			ShpSourceNodePlugin.h
			vtkShapefileReader.h
			)

set( ShpSource_moc ${ShpSource_moc}	
			ShpSourceNode.h
			ShpSourceNodePlugin.h
			)

set( ShpSource_src ${ShpSource_src}	
			ShpSourceNode.cpp
			ShpSourceNodePlugin.cpp
			vtkShapefileReader.cpp
			)

set( ShpSource_res ${ShpSource_res}	
			shpSource.xml
			)

# Create moc files		   
qt4_wrap_cpp(ShpSource_cxx ${ShpSource_moc})

# Create source groups
source_group("Moc Files" FILES ${ShpSource_cxx})
source_group("Header Files" FILES ${ShpSource_h})
source_group("Resources" FILES ${ShpSource_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(ShpSource_src ${ShpSource_src} ${ShpSource_h} ${ShpSource_res})
ENDIF(WIN32)

# Create static library
add_library(ShpSource SHARED ${ShpSource_src} ${ShpSource_cxx})

# Add library dependencies
target_link_libraries(ShpSource
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
install( FILES ${ShpSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS ShpSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
