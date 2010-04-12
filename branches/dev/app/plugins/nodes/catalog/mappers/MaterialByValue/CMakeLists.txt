project(materialByValue)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# materialByValue
set( materialByValue_h ${materialByValue_h}
			materialByValueNode.h
			materialByValueNodePlugin.h
			)

set( materialByValue_moc ${materialByValue_moc}	
			materialByValueNode.h
			materialByValueNodePlugin.h
			)

set( materialByValue_src ${materialByValue_src}	
			materialByValueNode.cpp
			materialByValueNodePlugin.cpp
			)

set( materialByValue_res ${materialByValue_res}	
			materialByValue.xml
			)

# Create moc files		   
qt4_wrap_cpp(materialByValue_cxx ${materialByValue_moc})

# Create source groups
source_group("Moc Files" FILES ${materialByValue_cxx})
source_group("Header Files" FILES ${materialByValue_h})
source_group("Resources" FILES ${materialByValue_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(materialByValue_src ${materialByValue_src} ${materialByValue_h} ${materialByValue_res})
ENDIF(WIN32)

# Create static library
add_library(materialByValue SHARED ${materialByValue_src} ${materialByValue_cxx})

# Add library dependencies
target_link_libraries(materialByValue
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${materialByValue_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS materialByValue RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
