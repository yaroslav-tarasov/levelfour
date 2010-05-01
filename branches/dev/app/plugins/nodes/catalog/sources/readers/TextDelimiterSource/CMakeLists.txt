project(TextDelimiterSource)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# TextDelimiterSource
set( TextDelimiterSource_h ${TextDelimiterSource_h}		
			TextDelimiterSourceNode.h
			TextDelimiterSourceNodePlugin.h
			)

set( TextDelimiterSource_moc ${TextDelimiterSource_moc}	
			TextDelimiterSourceNode.h
			TextDelimiterSourceNodePlugin.h
			)

set( TextDelimiterSource_src ${TextDelimiterSource_src}	
			TextDelimiterSourceNode.cpp
			TextDelimiterSourceNodePlugin.cpp
			)

set( TextDelimiterSource_res ${TextDelimiterSource_res}	
			TextDelimiterSource.xml
			)

# Create moc files		   
qt4_wrap_cpp(TextDelimiterSource_cxx ${TextDelimiterSource_moc})

# Create source groups
source_group("Moc Files" FILES ${TextDelimiterSource_cxx})
source_group("Header Files" FILES ${TextDelimiterSource_h})
source_group("Resources" FILES ${TextDelimiterSource_res})

# Add header files to sources to make headers visible in Visual Studio
set(TextDelimiterSource_src ${TextDelimiterSource_src} ${TextDelimiterSource_h} ${TextDelimiterSource_res})

# Create static library
add_library(TextDelimiterSource SHARED ${TextDelimiterSource_src} ${TextDelimiterSource_cxx})

# Add library dependencies
target_link_libraries(TextDelimiterSource
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkFiltering debug vtkFiltering
			optimized vtkInfovis debug vtkInfovis
			)

# Install files
install( FILES ${TextDelimiterSource_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS TextDelimiterSource RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
