project(Negative)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# Negative
set( Negative_h ${Negative_h}		
			NegativeNode.h
			NegativeNodePlugin.h
			)

set( Negative_moc ${Negative_moc}	
			NegativeNode.h
			NegativeNodePlugin.h
			)

set( Negative_src ${Negative_src}	
			NegativeNode.cpp
			NegativeNodePlugin.cpp
			)

set( Negative_res ${Negative_res}	
			Negative.xml
			)

# Create moc files		   
qt4_wrap_cpp(Negative_cxx ${Negative_moc})

# Create source groups
source_group("Moc Files" FILES ${Negative_cxx})
source_group("Header Files" FILES ${Negative_h})
source_group("Resources" FILES ${Negative_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(Negative_src ${Negative_src} ${Negative_h} ${Negative_res})
ENDIF(WIN32)

# Create static library
add_library(Negative SHARED ${Negative_src} ${Negative_cxx})

# Add library dependencies
target_link_libraries(Negative
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${Negative_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS Negative RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
