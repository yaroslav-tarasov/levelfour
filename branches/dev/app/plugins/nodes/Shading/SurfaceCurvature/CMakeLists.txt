project(surfacecurvature)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# surfacecurvature
set( surfacecurvature_h ${surfacecurvature_h}		
			SurfaceCurvatureNode.h
			SurfaceCurvatureNodePlugin.h
			)

set( surfacecurvature_moc ${surfacecurvature_moc}	
			SurfaceCurvatureNode.h
			SurfaceCurvatureNodePlugin.h
			)

set( surfacecurvature_src ${surfacecurvature_src}	
			SurfaceCurvatureNode.cpp
			SurfaceCurvatureNodePlugin.cpp
			)

set( surfacecurvature_res ${surfacecurvature_res}	
			surfacecurvature.xml
			)

# Create moc files		   
qt4_wrap_cpp(surfacecurvature_cxx ${surfacecurvature_moc})

# Create source groups
source_group("Moc Files" FILES ${surfacecurvature_cxx})
source_group("Header Files" FILES ${surfacecurvature_h})
source_group("Resources" FILES ${surfacecurvature_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(surfacecurvature_src ${surfacecurvature_src} ${surfacecurvature_h} ${surfacecurvature_res})
ENDIF(WIN32)

# Create static library
add_library(surfacecurvature SHARED ${surfacecurvature_src} ${surfacecurvature_cxx})

# Add library dependencies
target_link_libraries(surfacecurvature
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${surfacecurvature_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS surfacecurvature RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
