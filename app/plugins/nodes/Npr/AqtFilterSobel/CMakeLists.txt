project(aqtfiltersobel)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# aqtfiltersobel
set( aqtfiltersobel_h ${aqtfiltersobel_h}
			AqtFilterSobelNode.h
			AqtFilterSobelNodePlugin.h
			)

set( aqtfiltersobel_moc ${aqtfiltersobel_moc}
			AqtFilterSobelNode.h
			AqtFilterSobelNodePlugin.h
			)

set( aqtfiltersobel_src ${aqtfiltersobel_src}	
			AqtFilterSobelNode.cpp
			AqtFilterSobelNodePlugin.cpp
			)

set( aqtfiltersobel_res ${aqtfiltersobel_res}	
			aqtfiltersobel.xml
			)

# Create moc files		   
qt4_wrap_cpp(aqtfiltersobel_cxx ${aqtfiltersobel_moc})

# Create source groups
source_group("Moc Files" FILES ${aqtfiltersobel_cxx})
source_group("Header Files" FILES ${aqtfiltersobel_h})
source_group("Resources" FILES ${aqtfiltersobel_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(aqtfiltersobel_src ${aqtfiltersobel_src} ${aqtfiltersobel_h} ${aqtfiltersobel_res})
ENDIF(WIN32)

# Create static library
add_library(aqtfiltersobel SHARED ${aqtfiltersobel_src} ${aqtfiltersobel_cxx})

# Add library dependencies
target_link_libraries(aqtfiltersobel
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${aqtfiltersobel_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS aqtfiltersobel RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
