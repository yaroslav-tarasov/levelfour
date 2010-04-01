project(animatablemesh)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# animatablemesh
set( animatablemesh_h ${animatablemesh_h}		
			AnimatableMeshNode.h
			AnimatableMeshNodePlugin.h
			)

set( animatablemesh_moc ${animatablemesh_moc}	
			AnimatableMeshNode.h
			AnimatableMeshNodePlugin.h
			)

set( animatablemesh_src ${animatablemesh_src}	
			AnimatableMeshNode.cpp
			AnimatableMeshNodePlugin.cpp
			)

set( animatablemesh_res ${animatablemesh_res}	
			animatablemesh.xml
			)

# Create moc files		   
qt4_wrap_cpp(animatablemesh_cxx ${animatablemesh_moc})

# Create source groups
source_group("Moc Files" FILES ${animatablemesh_cxx})
source_group("Header Files" FILES ${animatablemesh_h})
source_group("Resources" FILES ${animatablemesh_res})

# Add header files to sources to make headers visible in Visual Studio
set(animatablemesh_src ${animatablemesh_src} ${animatablemesh_h} ${animatablemesh_res})

# Create static library
add_library(animatablemesh SHARED ${animatablemesh_src} ${animatablemesh_cxx})

# Add library dependencies
target_link_libraries(animatablemesh
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${animatablemesh_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS animatablemesh RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )