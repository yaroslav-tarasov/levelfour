project(geometryblur)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# geometryblur
set( geometryblur_h ${geometryblur_h}		
			GeometryBlurNode.h
			GeometryBlurNodePlugin.h
			)

set( geometryblur_moc ${geometryblur_moc}	
			GeometryBlurNode.h
			GeometryBlurNodePlugin.h
			)

set( geometryblur_src ${geometryblur_src}	
			GeometryBlurNode.cpp
			GeometryBlurNodePlugin.cpp
			)

set( geometryblur_res ${geometryblur_res}	
			geometryblur.xml
			)

# Create moc files		   
qt4_wrap_cpp(geometryblur_cxx ${geometryblur_moc})

# Create source groups
source_group("Moc Files" FILES ${geometryblur_cxx})
source_group("Header Files" FILES ${geometryblur_h})
source_group("Resources" FILES ${geometryblur_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(geometryblur_src ${geometryblur_src} ${geometryblur_h} ${geometryblur_res})
ENDIF(WIN32)

# Create static library
add_library(geometryblur SHARED ${geometryblur_src} ${geometryblur_cxx})

# Add library dependencies
target_link_libraries(geometryblur
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${geometryblur_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS geometryblur RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
