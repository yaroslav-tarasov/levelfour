project(TableToTree)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

link_directories(
			${TableToGraph_BINARY_DIR}
			)

# TableToTree
set( TableToTree_h ${TableToTree_h}
			TableToTreeNode.h
			TableToTreeNodePlugin.h
			${CMAKE_CURRENT_SOURCE_DIR}/../TableToGraph/TableToGraphNode.h
			)

set( TableToTree_moc ${TableToTree_moc}	
			TableToTreeNode.h
			TableToTreeNodePlugin.h
			${CMAKE_CURRENT_SOURCE_DIR}/../TableToGraph/TableToGraphNode.h
			)

set( TableToTree_src ${TableToTree_src}	
			TableToTreeNode.cpp
			TableToTreeNodePlugin.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/../TableToGraph/TableToGraphNode.cpp
			)

set( TableToTree_res ${TableToTree_res}	
			TableToTree.xml
			)

# Create moc files		   
qt4_wrap_cpp(TableToTree_cxx ${TableToTree_moc})

# Create source groups
source_group("Moc Files" FILES ${TableToTree_cxx})
source_group("Header Files" FILES ${TableToTree_h})
source_group("Resources" FILES ${TableToTree_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(TableToTree_src ${TableToTree_src} ${TableToTree_h} ${TableToTree_res})
ENDIF(WIN32)

# Create static library
add_library(TableToTree SHARED ${TableToTree_src} ${TableToTree_cxx})

# Add library dependencies
target_link_libraries(TableToTree
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized vtkFiltering debug vtkFiltering
			optimized vtkInfovis debug vtkInfovis
			optimized vtkCommon debug vtkCommon
			optimized tableToGraph debug TableToGraph_d.lib
			)

# Install files
install( FILES ${TableToTree_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS TableToTree 
	RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} 
)
