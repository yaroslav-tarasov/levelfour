project(multiplier)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# multiplier
set( multiplier_h ${multiplier_h}		
			MultiplierNode.h
			MultiplierNodePlugin.h
			)

set( multiplier_moc ${multiplier_moc}	
			MultiplierNode.h
			MultiplierNodePlugin.h
			)

set( multiplier_src ${multiplier_src}	
			MultiplierNode.cpp
			MultiplierNodePlugin.cpp
			)

set( multiplier_res ${multiplier_res}	
			multiplier.xml
			)

# Create moc files		   
qt4_wrap_cpp(multiplier_cxx ${multiplier_moc})

# Create source groups
source_group("Moc Files" FILES ${multiplier_cxx})
source_group("Header Files" FILES ${multiplier_h})
source_group("Resources" FILES ${multiplier_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(multiplier_src ${multiplier_src} ${multiplier_h} ${multiplier_res})
ENDIF(WIN32)

# Create static library
add_library(multiplier SHARED ${multiplier_src} ${multiplier_cxx})

# Add library dependencies
target_link_libraries(multiplier
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${multiplier_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS multiplier RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
