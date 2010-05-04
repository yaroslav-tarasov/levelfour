project(CommunityLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# CommunityLayouter
set( CommunityLayouter_h ${CommunityLayouter_h}
			CommunityLayouterNode.h
			CommunityLayouterNodePlugin.h
			)

set( CommunityLayouter_moc ${CommunityLayouter_moc}	
			CommunityLayouterNode.h
			CommunityLayouterNodePlugin.h
			)

set( CommunityLayouter_src ${CommunityLayouter_src}	
			CommunityLayouterNode.cpp
			CommunityLayouterNodePlugin.cpp
			)

set( CommunityLayouter_res ${CommunityLayouter_res}	
			CommunityLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(CommunityLayouter_cxx ${CommunityLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${CommunityLayouter_cxx})
source_group("Header Files" FILES ${CommunityLayouter_h})
source_group("Resources" FILES ${CommunityLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(CommunityLayouter_src ${CommunityLayouter_src} ${CommunityLayouter_h} ${CommunityLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(CommunityLayouter SHARED ${CommunityLayouter_src} ${CommunityLayouter_cxx})

# Add library dependencies
target_link_libraries(CommunityLayouter
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
install( FILES ${CommunityLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS CommunityLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
