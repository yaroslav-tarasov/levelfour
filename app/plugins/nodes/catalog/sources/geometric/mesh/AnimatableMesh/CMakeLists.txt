project(AnimatableMesh)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# AnimatableMesh
set( AnimatableMesh_h ${AnimatableMesh_h}		
			AnimatableMeshNode.h
			AnimatableMeshNodePlugin.h
			)

set( AnimatableMesh_moc ${AnimatableMesh_moc}	
			AnimatableMeshNode.h
			AnimatableMeshNodePlugin.h
			)

set( AnimatableMesh_src ${AnimatableMesh_src}	
			AnimatableMeshNode.cpp
			AnimatableMeshNodePlugin.cpp
			)

set( AnimatableMesh_res ${AnimatableMesh_res}	
			AnimatableMesh.xml
			)

# Create moc files		   
qt4_wrap_cpp(AnimatableMesh_cxx ${AnimatableMesh_moc})

# Create source groups
source_group("Moc Files" FILES ${AnimatableMesh_cxx})
source_group("Header Files" FILES ${AnimatableMesh_h})
source_group("Resources" FILES ${AnimatableMesh_res})

# Add header files to sources to make headers visible in Visual Studio
set(AnimatableMesh_src ${AnimatableMesh_src} ${AnimatableMesh_h} ${AnimatableMesh_res})

# Create static library
add_library(AnimatableMesh SHARED ${AnimatableMesh_src} ${AnimatableMesh_cxx})

# Add library dependencies
target_link_libraries(AnimatableMesh
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${AnimatableMesh_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS AnimatableMesh RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )