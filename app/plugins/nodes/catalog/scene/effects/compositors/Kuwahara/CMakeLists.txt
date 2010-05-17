project(Kuwahara)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# Kuwahara
set( Kuwahara_h ${Kuwahara_h}		
			KuwaharaNode.h
			KuwaharaNodePlugin.h
			)

set( Kuwahara_moc ${Kuwahara_moc}	
			KuwaharaNode.h
			KuwaharaNodePlugin.h
			)

set( Kuwahara_src ${Kuwahara_src}	
			KuwaharaNode.cpp
			KuwaharaNodePlugin.cpp
			)

set( Kuwahara_res ${Kuwahara_res}	
			Kuwahara.xml
			)

# Create moc files		   
qt4_wrap_cpp(Kuwahara_cxx ${Kuwahara_moc})

# Create source groups
source_group("Moc Files" FILES ${Kuwahara_cxx})
source_group("Header Files" FILES ${Kuwahara_h})
source_group("Resources" FILES ${Kuwahara_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(Kuwahara_src ${Kuwahara_src} ${Kuwahara_h} ${Kuwahara_res})
ENDIF(WIN32)

# Create static library
add_library(Kuwahara SHARED ${Kuwahara_src} ${Kuwahara_cxx})

# Add library dependencies
target_link_libraries(Kuwahara
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${Kuwahara_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS Kuwahara RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
