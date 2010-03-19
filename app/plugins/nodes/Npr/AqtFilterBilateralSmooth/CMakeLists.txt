project(aqtfilterbilateralsmooth)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# aqtfilterbilateralsmooth
set( aqtfilterbilateralsmooth_h ${aqtfilterbilateralsmooth_h}		
			AqtFilterBilateralSmoothNode.h
			AqtFilterBilateralSmoothNodePlugin.h
			)

set( aqtfilterbilateralsmooth_moc ${aqtfilterbilateralsmooth_moc}	
			AqtFilterBilateralSmoothNode.h
			AqtFilterBilateralSmoothNodePlugin.h
			)

set( aqtfilterbilateralsmooth_src ${aqtfilterbilateralsmooth_src}	
			AqtFilterBilateralSmoothNode.cpp
			AqtFilterBilateralSmoothNodePlugin.cpp
			)

set( aqtfilterbilateralsmooth_res ${aqtfilterbilateralsmooth_res}	
			aqtfilterbilateralsmooth.xml
			)

# Create moc files		   
qt4_wrap_cpp(aqtfilterbilateralsmooth_cxx ${aqtfilterbilateralsmooth_moc})

# Create source groups
source_group("Moc Files" FILES ${aqtfilterbilateralsmooth_cxx})
source_group("Header Files" FILES ${aqtfilterbilateralsmooth_h})
source_group("Resources" FILES ${aqtfilterbilateralsmooth_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(aqtfilterbilateralsmooth_src ${aqtfilterbilateralsmooth_src} ${aqtfilterbilateralsmooth_h} ${aqtfilterbilateralsmooth_res})
ENDIF(WIN32)

# Create static library
add_library(aqtfilterbilateralsmooth SHARED ${aqtfilterbilateralsmooth_src} ${aqtfilterbilateralsmooth_cxx})

# Add library dependencies
target_link_libraries(aqtfilterbilateralsmooth
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${aqtfilterbilateralsmooth_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS aqtfilterbilateralsmooth RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
