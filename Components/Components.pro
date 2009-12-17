TEMPLATE    = subdirs
CONFIG     += ordered
TARGET      = Components

SUBDIRS     = LevelFour \
		  VisSystemCanvas \
              VisSystemNodeClassExplorer \
		  VTKFullscreen \
              VTKSystem \
              TemplateManager \
              PropertyEditor \
              Utils \
              ProjectManager \
              MessageLog \
              ScriptEditor \
              MainWindow \
              HelpSystem \
		  M2D \
		  M2M \
		  SceneView \
		  TableView \
		  TreeView \
		  ScenegraphManager \
		  Clipboard
              
           
# Some components are under development, but the ideas and implementation
# is not yet clear for public view/modification. Until that time, 
# we bracket the code like this.
# exists(VInfoSpace/VInfoSpace.pro)
# {
# SUBDIRS    += VInfoSpace
# }

SUBDIRS += DummyCodeGen

