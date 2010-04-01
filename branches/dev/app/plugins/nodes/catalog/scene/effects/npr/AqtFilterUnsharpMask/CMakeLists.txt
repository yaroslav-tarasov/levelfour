project(aqtfilterunsharpmask)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# aqtfilterunsharpmask
set( aqtfilterunsharpmask_h ${aqtfilterunsharpmask_h}		
			AqtFilterUnsharpMaskNode.h
			AqtFilterUnsharpMaskNodePlugin.h
			)

set( aqtfilterunsharpmask_moc ${aqtfilterunsharpmask_moc}	
			AqtFilterUnsharpMaskNode.h
			AqtFilterUnsharpMaskNodePlugin.h
			)

set( aqtfilterunsharpmask_src ${aqtfilterunsharpmask_src}	
			AqtFilterUnsharpMaskNode.cpp
			AqtFilterUnsharpMaskNodePlugin.cpp
			)

set( aqtfilterunsharpmask_res ${aqtfilterunsharpmask_res}	
			aqtfilterunsharpmask.xml
			)

# Create moc files		   
qt4_wrap_cpp(aqtfilterunsharpmask_cxx ${aqtfilterunsharpmask_moc})

# Create source groups
source_group("Moc Files" FILES ${aqtfilterunsharpmask_cxx})
source_group("Header Files" FILES ${aqtfilterunsharpmask_h})
source_group("Resources" FILES ${aqtfilterunsharpmask_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(aqtfilterunsharpmask_src ${aqtfilterunsharpmask_src} ${aqtfilterunsharpmask_h} ${aqtfilterunsharpmask_res})
ENDIF(WIN32)

# Create static library
add_library(aqtfilterunsharpmask SHARED ${aqtfilterunsharpmask_src} ${aqtfilterunsharpmask_cxx})

# Add library dependencies
target_link_libraries(aqtfilterunsharpmask
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${aqtfilterunsharpmask_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS aqtfilterunsharpmask RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
