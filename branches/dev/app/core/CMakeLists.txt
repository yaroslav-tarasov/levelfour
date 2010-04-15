project(frappercore)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "" FORCE) 
SET(CMAKE_DEBUG_POSTFIX _d)

include_directories(
			${CMAKE_CURRENT_SOURCE_DIR}
			)

# frappercore
set( frappercore_h ${frappercore_h}		
			AbstractParameter.h
			BaseRectItem.h
			CompositorNode.h
			Connection.h
			ConnectionGraphicsItem.h
			CurveEditorDataNode.h
			EnumerationParameter.h
			FilenameParameter.h
			FlagGraphicsItem.h
			FrapperPlatform.h
			FrapperPrerequisites.h
			GeometryNode.h
			GeometryRenderNode.h
			Helper.h
			ImageNode.h
			InstanceCounterMacros.h
			Key.h
			Log.h
			MotionDataNode.h
			Node.h
			NodeFactory.h
			NodeGraphicsItem.h
			NodeModel.h
			NodeType.h
			NodeTypeInterface.h
			NumberParameter.h
			OgreContainer.h
			OgreManager.h
			OgreTools.h
			Parameter.h
			ParameterAction.h
			ParameterGroup.h
			ParameterPlugin.h
			ParameterTypeIcon.h
			PinGraphicsItem.h
			RenderNode.h
			SceneModel.h
			ViewFlagGraphicsItem.h
			ViewingParameters.h
			ViewNode.h
			VTKTableNode.h
			)

set( frappercore_moc ${frappercore_moc}	
			 AbstractParameter.h
			 CompositorNode.h
			 Connection.h
			 ConnectionGraphicsItem.h
			 CurveEditorDataNode.h
			 FlagGraphicsItem.h
			 GeometryNode.h
			 GeometryRenderNode.h
			 ImageNode.h
			 MotionDataNode.h
			 Node.h	
			 NodeGraphicsItem.h
			 NodeModel.h
			 NodeTypeInterface.h
			 OgreContainer.h
			 Parameter.h
			 ParameterPlugin.h
			 PinGraphicsItem.h
			 RenderNode.h
			 SceneModel.h
			 SceneNodeParameter.h
			 ViewFlagGraphicsItem.h
			 ViewNode.h
			 VTKTableNode.h
			 )

set( frappercore_src ${frappercore_src}	
			AbstractParameter.cpp			
			BaseRectItem.cpp
			CompositorNode.cpp
			Connection.cpp
			ConnectionGraphicsItem.cpp
			CurveEditorDataNode.cpp
			EnumerationParameter.cpp
			FilenameParameter.cpp
			FlagGraphicsItem.cpp
			GeometryNode.cpp
			GeometryRenderNode.cpp
			ImageNode.cpp
			Key.cpp
			Log.cpp
			MotionDataNode.cpp
			Node.cpp
			NodeFactory.cpp
			NodeGraphicsItem.cpp
			NodeModel.cpp
			NodeType.cpp
			NumberParameter.cpp
			OgreContainer.cpp
			OgreManager.cpp
			OgreTools.cpp
			Parameter.cpp
			ParameterAction.cpp
			ParameterGroup.cpp
			ParameterPlugin.cpp
			ParameterTypeIcon.cpp
			PinGraphicsItem.cpp
			RenderNode.cpp
			SceneModel.cpp
			SceneNodeParameter.cpp
			ViewFlagGraphicsItem.cpp
			ViewingParameters.cpp
			ViewNode.cpp
			VTKTableNode.cpp
			)

# Create moc files		   
qt4_wrap_cpp(frappercore_cxx ${frappercore_moc})

# Create source groups
source_group("Moc Files" FILES ${frappercore_cxx})
source_group("Header Files" FILES ${frappercore_h})

# Add header files to sources to make headers visible in Visual Studio
set(frappercore_src ${frappercore_src} ${frappercore_h})

# Create static library
add_library(frappercore SHARED ${frappercore_src} ${frappercore_cxx})

# Add custom build step - copy dlls
IF (WIN32)
	add_custom_command(TARGET frappercore POST_BUILD COMMAND copy \"${frappercore_BINARY_DIR}/$(ConfigurationName)\\*.dll\" \"${frappercore_SOURCE_DIR}/../bin/win32\")
ENDIF(WIN32)

# Add library dependencies
target_link_libraries(frappercore 
			optimized QtCore4 debug QtCored4
			optimized QtGui4 debug QtGuid4
			optimized QtXml4 debug QtXmld4
			optimized OgreMain debug OgreMain_d
			optimized libcollada15dom21 debug libcollada15dom21-d
			optimized vtkFiltering debug vtkFiltering
			optimized vtkInfovis debug vtkInfovis
			)

# Install files
install( TARGETS frappercore RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX} )
