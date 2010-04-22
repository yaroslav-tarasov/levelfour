project(Model2Scene)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# model2scene
set( Model2Scene_h ${Model2Scene_h}		
			Model2SceneNode.h
			Model2SceneNodePlugin.h
			)

set( Model2Scene_moc ${Model2Scene_moc}	
			Model2SceneNode.h
			Model2SceneNodePlugin.h
			)

set( Model2Scene_src ${Model2Scene_src}	
			Model2SceneNode.cpp
			Model2SceneNodePlugin.cpp
			)

set( Model2Scene_res ${Model2Scene_res}	
			Model2Scene.xml
			)

# Create moc files		   
qt4_wrap_cpp(Model2Scene_cxx ${Model2Scene_moc})

# Create source groups
source_group("Moc Files" FILES ${Model2Scene_cxx})
source_group("Header Files" FILES ${Model2Scene_h})
source_group("Resources" FILES ${Model2Scene_res})

# Add header files to sources to make headers visible in Visual Studio
set(Model2Scene_src ${Model2Scene_src} ${Model2Scene_h} ${Model2Scene_res})

# Create static library
add_library(Model2Scene SHARED ${Model2Scene_src} ${Model2Scene_cxx})

# Add library dependencies
target_link_libraries(Model2Scene
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
install( FILES ${Model2Scene_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS Model2Scene RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )