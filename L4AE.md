### Introduction ###

The main premise of the L4AE is to provide an end-user development environment for using the L4TK to create analytical scenes and to publish visual stories. A user can:

  * load data sets ranging from statistical to geometric
  * apply "encoding" methods for visual representation (e.g. graphs, charts, maps)
  * modify and annotate the scene (add textures, cameras, lights, billboards)
  * explore the analysis space with levels of detail, navigation, filtering, etc.
  * create animated sequences (fly-throughs, time series)
  * publish results in forms of publication quality stills, movies, and games
  * share the workflow for reuse and audit

### Details ###

The environment is heavily influenced by concepts of the "digital content creation" domain and relies on advances of 3d model/rendering and game engine technology. The interface is designed to provide the tools and controls needed for a complete visual analysis environment:

  * Scenegraph Manager: for organization of a scene
  * Feature Object Manager: to access all data points
  * Node-Link Modeler: to configure L4TK nodes for encoding pipelines
  * Method Inspector: to parameterize nodes of the L4TK
  * Property Inspector: for customization of all visual variables (e.g., position, shape, size, material)
  * Animation Controller: a controller and keyframe based animation editor
  * Event Manager: an interface for defining interactivity
  * Render Viewport: to render 3d scenes
  * Data Viewports: to render tabular data

L4AE is based on the excellent works of OSG (OpenSceneGraph) and VTK Designer.