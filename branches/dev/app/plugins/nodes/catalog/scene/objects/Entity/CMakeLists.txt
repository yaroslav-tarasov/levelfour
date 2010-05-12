project(Entity)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# Entity
set( Entity_h ${Entity_h}		
			EntityNode.h
			EntityNodePlugin.h
			)

set( Entity_moc ${Entity_moc}	
			EntityNode.h
			EntityNodePlugin.h
			)

set( Entity_src ${Entity_src}	
			EntityNode.cpp
			EntityNodePlugin.cpp
			)

set( Entity_res ${Entity_res}	
			Entity.xml
			)

# Create moc files		   
qt4_wrap_cpp(Entity_cxx ${Entity_moc})

# Create source groups
source_group("Moc Files" FILES ${Entity_cxx})
source_group("Header Files" FILES ${Entity_h})
source_group("Resources" FILES ${Entity_res})

# Add header files to sources to make headers visible in Visual Studio
set(Entity_src ${Entity_src} ${Entity_h} ${Entity_res})

# Create static library
add_library(Entity SHARED ${Entity_src} ${Entity_cxx})

# Add library dependencies
target_link_libraries(Entity
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
install( FILES ${Entity_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS Entity RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )