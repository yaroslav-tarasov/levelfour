project(CompositorBase)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# CompositorBase
set( CompositorBase_h ${CompositorBase_h}		
			CompositorBaseNode.h
			CompositorBaseNodePlugin.h
			)

set( CompositorBase_moc ${CompositorBase_moc}	
			CompositorBaseNode.h
			CompositorBaseNodePlugin.h
			)

set( CompositorBase_src ${CompositorBase_src}	
			CompositorBaseNode.cpp
			CompositorBaseNodePlugin.cpp
			)

set( CompositorBase_res ${CompositorBase_res}	
			BadPrinting.xml
			BleachBypass.xml
			Bloom.xml
			ColorClipping.xml
			ColorCurve.xml
			ColorMixer.xml
			ColorMixerMono.xml
			ColorSpin.xml
			Corona.xml
			DropShadow.xml
			EdgeDetect.xml
			Glow.xml
			Kuwahara.xml
			LineDraw.xml
			Negative.xml
			NegativeOffset.xml
			NightVision.xml
			Posterize.xml
			RadialBlur.xml
			ScratchedFilm.xml
			Sepia.xml
			Tiles.xml
			)

# Create moc files		   
qt4_wrap_cpp(CompositorBase_cxx ${CompositorBase_moc})

# Create source groups
source_group("Moc Files" FILES ${CompositorBase_cxx})
source_group("Header Files" FILES ${CompositorBase_h})
source_group("Resources" FILES ${CompositorBase_res})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(CompositorBase_src ${CompositorBase_src} ${CompositorBase_h} ${CompositorBase_res})
ENDIF(WIN32)

# Create static library
add_library(CompositorBase SHARED ${CompositorBase_src} ${CompositorBase_cxx})

# Add library dependencies
target_link_libraries(CompositorBase
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			)

# Install files
install( FILES ${CompositorBase_res} DESTINATION ${NODE_PLUGIN_INSTALL} )
install( TARGETS CompositorBase RUNTIME DESTINATION ${NODE_PLUGIN_INSTALL} )
