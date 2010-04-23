project(XmlTreeReader)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# XmlTreeReader
set( XmlTreeReader_h ${XmlTreeReader_h}
			XmlTreeReaderNode.h
			XmlTreeReaderNodePlugin.h
			)

set( XmlTreeReader_moc ${XmlTreeReader_moc}	
			XmlTreeReaderNode.h
			XmlTreeReaderNodePlugin.h
			)

set( XmlTreeReader_src ${XmlTreeReader_src}	
			XmlTreeReaderNode.cpp
			XmlTreeReaderNodePlugin.cpp
			)

set( XmlTreeReader_res ${XmlTreeReader_res}	
			XmlTreeReader.xml
			)

# Create moc files		   
qt4_wrap_cpp(XmlTreeReader_cxx ${XmlTreeReader_moc})

# Create source groups
source_group("Moc Files" FILES ${XmlTreeReader_cxx})
source_group("Header Files" FILES ${XmlTreeReader_h})
source_group("Resources" FILES ${XmlTreeReader_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(XmlTreeReader_src ${XmlTreeReader_src} ${XmlTreeReader_h} ${XmlTreeReader_res})
ENDIF(WIN32)

# Create static library
add_library(XmlTreeReader SHARED ${XmlTreeReader_src} ${XmlTreeReader_cxx})

# Add library dependencies
target_link_libraries(XmlTreeReader
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${XmlTreeReader_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS XmlTreeReader RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
