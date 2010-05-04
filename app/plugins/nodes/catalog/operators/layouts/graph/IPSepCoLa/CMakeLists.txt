project(IPSepCoLaLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# IPSepCoLaLayouter
set( IPSepCoLaLayouter_h ${IPSepCoLaLayouter_h}
			IPSepCoLaLayouterNode.h
			IPSepCoLaLayouterNodePlugin.h
			)

set( IPSepCoLaLayouter_moc ${IPSepCoLaLayouter_moc}	
			IPSepCoLaLayouterNode.h
			IPSepCoLaLayouterNodePlugin.h
			)

set( IPSepCoLaLayouter_src ${IPSepCoLaLayouter_src}	
			IPSepCoLaLayouterNode.cpp
			IPSepCoLaLayouterNodePlugin.cpp
			)

set( IPSepCoLaLayouter_res ${IPSepCoLaLayouter_res}	
			IPSepCoLaLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(IPSepCoLaLayouter_cxx ${IPSepCoLaLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${IPSepCoLaLayouter_cxx})
source_group("Header Files" FILES ${IPSepCoLaLayouter_h})
source_group("Resources" FILES ${IPSepCoLaLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(IPSepCoLaLayouter_src ${IPSepCoLaLayouter_src} ${IPSepCoLaLayouter_h} ${IPSepCoLaLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(IPSepCoLaLayouter SHARED ${IPSepCoLaLayouter_src} ${IPSepCoLaLayouter_cxx})

# Add library dependencies
target_link_libraries(IPSepCoLaLayouter
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkFiltering debug vtkFiltering
			optimized vtkInfovis debug vtkInfovis
			optimized vtkCommon debug vtkCommon
			)

# Install files
install( FILES ${IPSepCoLaLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS IPSepCoLaLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
