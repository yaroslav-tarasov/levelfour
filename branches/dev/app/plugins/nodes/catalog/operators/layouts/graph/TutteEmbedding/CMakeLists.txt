project(TutteEmbeddingLayouter)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# TutteEmbeddingLayouter
set( TutteEmbeddingLayouter_h ${TutteEmbeddingLayouter_h}
			TutteEmbeddingLayouterNode.h
			TutteEmbeddingLayouterNodePlugin.h
			)

set( TutteEmbeddingLayouter_moc ${TutteEmbeddingLayouter_moc}	
			TutteEmbeddingLayouterNode.h
			TutteEmbeddingLayouterNodePlugin.h
			)

set( TutteEmbeddingLayouter_src ${TutteEmbeddingLayouter_src}	
			TutteEmbeddingLayouterNode.cpp
			TutteEmbeddingLayouterNodePlugin.cpp
			)

set( TutteEmbeddingLayouter_res ${TutteEmbeddingLayouter_res}	
			TutteEmbeddingLayouter.xml
			)

# Create moc files		   
qt4_wrap_cpp(TutteEmbeddingLayouter_cxx ${TutteEmbeddingLayouter_moc})

# Create source groups
source_group("Moc Files" FILES ${TutteEmbeddingLayouter_cxx})
source_group("Header Files" FILES ${TutteEmbeddingLayouter_h})
source_group("Resources" FILES ${TutteEmbeddingLayouter_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(TutteEmbeddingLayouter_src ${TutteEmbeddingLayouter_src} ${TutteEmbeddingLayouter_h} ${TutteEmbeddingLayouter_res})
ENDIF(WIN32)

# Create static library
add_library(TutteEmbeddingLayouter SHARED ${TutteEmbeddingLayouter_src} ${TutteEmbeddingLayouter_cxx})

# Add library dependencies
target_link_libraries(TutteEmbeddingLayouter
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
install( FILES ${TutteEmbeddingLayouter_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS TutteEmbeddingLayouter RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
