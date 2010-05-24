project(Bloom)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# Bloom
set( Bloom_h ${Bloom_h}		
			BloomNode.h
			BloomNodePlugin.h
			)

set( Bloom_moc ${Bloom_moc}	
			BloomNode.h
			BloomNodePlugin.h
			)

set( Bloom_src ${Bloom_src}	
			BloomNode.cpp
			BloomNodePlugin.cpp
			)

set( Bloom_res ${Bloom_res}	
			Bloom.xml
			)

# Create moc files		   
qt4_wrap_cpp(Bloom_cxx ${Bloom_moc})

# Create source groups
source_group("Moc Files" FILES ${Bloom_cxx})
source_group("Header Files" FILES ${Bloom_h})
source_group("Resources" FILES ${Bloom_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(Bloom_src ${Bloom_src} ${Bloom_h} ${Bloom_res})
ENDIF(WIN32)

# Create static library
add_library(Bloom SHARED ${Bloom_src} ${Bloom_cxx})

# Add library dependencies
target_link_libraries(Bloom
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${Bloom_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS Bloom RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
