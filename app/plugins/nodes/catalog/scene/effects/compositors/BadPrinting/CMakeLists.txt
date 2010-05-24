project(BadPrinting)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# BadPrinting
set( BadPrinting_h ${BadPrinting_h}		
			BadPrintingNode.h
			BadPrintingNodePlugin.h
			)

set( BadPrinting_moc ${BadPrinting_moc}	
			BadPrintingNode.h
			BadPrintingNodePlugin.h
			)

set( BadPrinting_src ${BadPrinting_src}	
			BadPrintingNode.cpp
			BadPrintingNodePlugin.cpp
			)

set( BadPrinting_res ${BadPrinting_res}	
			BadPrinting.xml
			)

# Create moc files		   
qt4_wrap_cpp(BadPrinting_cxx ${BadPrinting_moc})

# Create source groups
source_group("Moc Files" FILES ${BadPrinting_cxx})
source_group("Header Files" FILES ${BadPrinting_h})
source_group("Resources" FILES ${BadPrinting_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(BadPrinting_src ${BadPrinting_src} ${BadPrinting_h} ${BadPrinting_res})
ENDIF(WIN32)

# Create static library
add_library(BadPrinting SHARED ${BadPrinting_src} ${BadPrinting_cxx})

# Add library dependencies
target_link_libraries(BadPrinting
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${BadPrinting_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS BadPrinting RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
