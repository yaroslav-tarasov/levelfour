# Directory containing class headers.
SET(VTK_IO_HEADER_DIR "${VTK_INSTALL_PREFIX}/include/vtk-5.5")

# Classes in vtkIO.
SET(VTK_IO_CLASSES
  "vtkAbstractParticleWriter"
  "vtkAVSucdReader"
  "vtkBMPReader"
  "vtkBMPWriter"
  "vtkBYUReader"
  "vtkBYUWriter"
  "vtkBase64InputStream"
  "vtkBase64OutputStream"
  "vtkBase64Utilities"
  "vtkCGMWriter"
  "vtkChacoReader"
  "vtkCosmoReader"
  "vtkDEMReader"
  "vtkDICOMImageReader"
  "vtkDataCompressor"
  "vtkDataObjectReader"
  "vtkDataObjectWriter"
  "vtkDataReader"
  "vtkDataSetReader"
  "vtkDataSetWriter"
  "vtkDataWriter"
  "vtkDelimitedTextWriter"
  "vtkEnSight6BinaryReader"
  "vtkEnSight6Reader"
  "vtkEnSightGoldBinaryReader"
  "vtkEnSightGoldReader"
  "vtkEnSightMasterServerReader"
  "vtkEnSightReader"
  "vtkFLUENTReader"
  "vtkFacetWriter"
  "vtkGenericDataObjectReader"
  "vtkGenericDataObjectWriter"
  "vtkGESignaReader"
  "vtkGAMBITReader"
  "vtkGaussianCubeReader"
  "vtkGenericEnSightReader"
  "vtkGenericMovieWriter"
  "vtkGlobFileNames"
  "vtkGraphReader"
  "vtkGraphWriter"
  "vtkIVWriter"
  "vtkImageReader"
  "vtkImageReader2"
  "vtkImageReader2Collection"
  "vtkImageReader2Factory"
  "vtkImageWriter"
  "vtkInputStream"
  "vtkJavaScriptDataWriter"
  "vtkJPEGReader"
  "vtkJPEGWriter"
  "vtkMFIXReader"
  "vtkMaterialLibrary"
  "vtkMCubesReader"
  "vtkMCubesWriter"
  "vtkMedicalImageProperties"
  "vtkMedicalImageReader2"
  "vtkMetaImageReader"
  "vtkMetaImageWriter"
  "vtkMINCImageAttributes"
  "vtkMINCImageReader"
  "vtkMINCImageWriter"
  "vtkMultiBlockPLOT3DReader"
  "vtkMoleculeReaderBase"
  "vtkNetCDFCOARDSReader"
  "vtkNetCDFReader"
  "vtkOBJReader"
  "vtkOutputStream"
  "vtkOpenFOAMReader"
  "vtkPDBReader"
  "vtkPLOT3DReader"
  "vtkPLY"
  "vtkPLYReader"
  "vtkPLYWriter"
  "vtkPNGReader"
  "vtkPNGWriter"
  "vtkPNMReader"
  "vtkPNMWriter"
  "vtkParticleReader"
  "vtkPolyDataReader"
  "vtkPolyDataWriter"
  "vtkPostScriptWriter"
  "vtkRectilinearGridReader"
  "vtkRectilinearGridWriter"
  "vtkRTXMLPolyDataReader"
  "vtkRowQuery"
  "vtkSESAMEReader"
  "vtkShaderCodeLibrary"
  "vtkSLACParticleReader"
  "vtkSLACReader"
  "vtkSQLDatabase"
  "vtkSQLDatabaseSchema"
  "vtkSQLiteDatabase"
  "vtkSQLiteQuery"
  "vtkSQLQuery"
  "vtkSLCReader"
  "vtkSTLReader"
  "vtkSTLWriter"
  "vtkSimplePointsReader"
  "vtkSortFileNames"
  "vtkStructuredGridReader"
  "vtkStructuredGridWriter"
  "vtkStructuredPointsReader"
  "vtkStructuredPointsWriter"
  "vtkTableReader"
  "vtkTableWriter"
  "vtkTecplotReader"
  "vtkTIFFReader"
  "vtkTIFFWriter"
  "vtkTreeReader"
  "vtkTreeWriter"
  "vtkUGFacetReader"
  "vtkUnstructuredGridReader"
  "vtkUnstructuredGridWriter"
  "vtkVolume16Reader"
  "vtkVolumeReader"
  "vtkWriter"
  "vtkXMLCompositeDataReader"
  "vtkXMLDataParser"
  "vtkXMLDataReader"
  "vtkXMLDataSetWriter"
  "vtkXMLFileReadTester"
  "vtkXMLHierarchicalBoxDataReader"
  "vtkXMLHierarchicalDataReader"
  "vtkXMLImageDataReader"
  "vtkXMLImageDataWriter"
  "vtkXMLHyperOctreeReader"
  "vtkXMLHyperOctreeWriter"
  "vtkXMLMaterial"
  "vtkXMLMaterialParser"
  "vtkXMLMaterialReader"
  "vtkXMLMultiBlockDataReader"
  "vtkXMLMultiGroupDataReader"
  "vtkXMLCompositeDataWriter"
  "vtkXMLMultiBlockDataWriter"
  "vtkXMLHierarchicalBoxDataWriter"
  "vtkXMLPDataReader"
  "vtkXMLPDataSetWriter"
  "vtkXMLPDataWriter"
  "vtkXMLPImageDataReader"
  "vtkXMLPImageDataWriter"
  "vtkXMLPPolyDataReader"
  "vtkXMLPPolyDataWriter"
  "vtkXMLPRectilinearGridReader"
  "vtkXMLPRectilinearGridWriter"
  "vtkXMLPStructuredDataReader"
  "vtkXMLPStructuredDataWriter"
  "vtkXMLPStructuredGridReader"
  "vtkXMLPStructuredGridWriter"
  "vtkXMLPUnstructuredDataReader"
  "vtkXMLPUnstructuredDataWriter"
  "vtkXMLPUnstructuredGridReader"
  "vtkXMLPUnstructuredGridWriter"
  "vtkXMLParser"
  "vtkXMLPolyDataReader"
  "vtkXMLPolyDataWriter"
  "vtkXMLReader"
  "vtkXMLRectilinearGridReader"
  "vtkXMLRectilinearGridWriter"
  "vtkXMLShader"
  "vtkXMLStructuredDataReader"
  "vtkXMLStructuredDataWriter"
  "vtkXMLStructuredGridReader"
  "vtkXMLStructuredGridWriter"
  "vtkXMLUnstructuredDataReader"
  "vtkXMLUnstructuredDataWriter"
  "vtkXMLUnstructuredGridReader"
  "vtkXMLUnstructuredGridWriter"
  "vtkXMLUtilities"
  "vtkXMLWriter"
  "vtkXMLWriterC"
  "vtkXYZMolReader"
  "vtkZLibDataCompressor"
  "vtkAVIWriter"
  "vtkRowQueryToTable"
  "vtkArrayReader"
  "vtkArrayWriter")

# Abstract classes in vtkIO.
SET(VTK_IO_CLASSES_ABSTRACT
  "vtkAbstractParticleWriter"
  "vtkDataCompressor"
  "vtkEnSightReader"
  "vtkGenericMovieWriter"
  "vtkMoleculeReaderBase"
  "vtkRowQuery"
  "vtkSQLDatabase"
  "vtkSQLQuery"
  "vtkVolumeReader"
  "vtkWriter"
  "vtkXMLCompositeDataReader"
  "vtkXMLDataReader"
  "vtkXMLCompositeDataWriter"
  "vtkXMLPDataReader"
  "vtkXMLPDataWriter"
  "vtkXMLPStructuredDataReader"
  "vtkXMLPStructuredDataWriter"
  "vtkXMLPUnstructuredDataReader"
  "vtkXMLPUnstructuredDataWriter"
  "vtkXMLReader"
  "vtkXMLStructuredDataReader"
  "vtkXMLStructuredDataWriter"
  "vtkXMLUnstructuredDataReader"
  "vtkXMLUnstructuredDataWriter"
  "vtkXMLWriter")

# Wrap-exclude classes in vtkIO.
SET(VTK_IO_CLASSES_WRAP_EXCLUDE
  "vtkPLY"
  "vtkXMLWriterC")

# Set convenient variables to test each class.
FOREACH(class ${VTK_IO_CLASSES})
  SET(VTK_CLASS_EXISTS_${class} 1)
ENDFOREACH(class)
FOREACH(class ${VTK_IO_CLASSES_ABSTRACT})
  SET(VTK_CLASS_ABSTRACT_${class} 1)
ENDFOREACH(class)
FOREACH(class ${VTK_IO_CLASSES_WRAP_EXCLUDE})
  SET(VTK_CLASS_WRAP_EXCLUDE_${class} 1)
ENDFOREACH(class)
