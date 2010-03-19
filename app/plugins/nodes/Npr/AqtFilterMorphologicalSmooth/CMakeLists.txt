project(aqtfiltermorphologicalsmooth)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# aqtfiltermorphologicalsmooth
set( aqtfiltermorphologicalsmooth_h ${aqtfiltermorphologicalsmooth_h}		
			AqtFilterMorphologicalSmoothNode.h
			AqtFilterMorphologicalSmoothNodePlugin.h
			)

set( aqtfiltermorphologicalsmooth_moc ${aqtfiltermorphologicalsmooth_moc}	
			AqtFilterMorphologicalSmoothNode.h
			AqtFilterMorphologicalSmoothNodePlugin.h
			)

set( aqtfiltermorphologicalsmooth_src ${aqtfiltermorphologicalsmooth_src}	
			AqtFilterMorphologicalSmoothNode.cpp
			AqtFilterMorphologicalSmoothNodePlugin.cpp
			)

set( aqtfiltermorphologicalsmooth_res ${aqtfiltermorphologicalsmooth_res}	
			aqtfiltermorphologicalsmooth.xml
			)

# Create moc files		   
qt4_wrap_cpp(aqtfiltermorphologicalsmooth_cxx ${aqtfiltermorphologicalsmooth_moc})

# Create source groups
source_group("Moc Files" FILES ${aqtfiltermorphologicalsmooth_cxx})
source_group("Header Files" FILES ${aqtfiltermorphologicalsmooth_h})
source_group("Resources" FILES ${aqtfiltermorphologicalsmooth_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(aqtfiltermorphologicalsmooth_src ${aqtfiltermorphologicalsmooth_src} ${aqtfiltermorphologicalsmooth_h} ${aqtfiltermorphologicalsmooth_res})
ENDIF(WIN32)

# Create static library
add_library(aqtfiltermorphologicalsmooth SHARED ${aqtfiltermorphologicalsmooth_src} ${aqtfiltermorphologicalsmooth_cxx})

# Add library dependencies
target_link_libraries(aqtfiltermorphologicalsmooth
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${aqtfiltermorphologicalsmooth_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS aqtfiltermorphologicalsmooth RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
