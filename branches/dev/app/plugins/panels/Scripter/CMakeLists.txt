project(Scripter)
find_package(QT4 REQUIRED)
include(${QT_USE_FILE})
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE)
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(  
		    ${Scripter_INCLUDEPATH}
		    ${Scripter_BINARY_DIR}
		    
			.
			../../../core
			../../../gui
			
			$ENV{OGRE_HOME}/include
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/collada/include
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/collada/include/1.5
		    )

link_directories(
			${QT_LIBRARY_DIR}
			$ENV{OGRE_HOME}/lib
			${CMAKE_SOURCE_DIR}/thirdparty_libraries/collada/lib	
			)

# Scripter
set( Scripter_h ${Scripter_h}
			Scripter.h
			ScripterPlugin.h
			)

set( Scripter_moc ${Scripter_moc}	
			Scripter.h
			ScripterPlugin.h
			)
			
set( Scripter_src ${Scripter_src}	
			Scripter.cpp
			ScripterPlugin.cpp
			)

# Create moc files		   
qt4_wrap_cpp(Scripter_cxx ${Scripter_moc})

# Create source groups
source_group("Moc Files" FILES ${Scripter_cxx})
source_group("Generated Files" FILES ${Scripter_cxx} ${Scripter_ui_h})
source_group("Header Files" FILES ${Scripter_h})

# Add header files to sources to make headers visible in Visual Studio
IF (WIN32)
  set(Scripter_src ${Scripter_src} ${Scripter_h} )
ENDIF(WIN32)

# Create static library
add_library(Scripter SHARED ${Scripter_src} ${Scripter_cxx} )

# Add custom build step - copy dlls
IF (WIN32)
	add_custom_command(TARGET Scripter POST_BUILD COMMAND copy \"${Scripter_BINARY_DIR}/$(ConfigurationName)\\*.dll\" \"${Scripter_SOURCE_DIR}/../../../bin/win32/plugins/panels\" COMMAND copy \"${Scripter_SOURCE_DIR}\\*.xml\" \"${CMAKE_SOURCE_DIR}/bin/win32/plugins/panels\")
ENDIF(WIN32)

add_dependencies(Scripter frappercore frappergui)

# Add library dependencies
target_link_libraries(Scripter 
			#${QT_LIBRARIES}
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized frappercore debug frappercore_d
			optimized frappergui debug frappergui_d
			)
