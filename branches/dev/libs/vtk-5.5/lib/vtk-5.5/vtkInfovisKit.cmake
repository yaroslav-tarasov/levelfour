# Directory containing class headers.
SET(VTK_INFOVIS_HEADER_DIR "${VTK_INSTALL_PREFIX}/include/vtk-5.5")

# Classes in vtkInfovis.
SET(VTK_INFOVIS_CLASSES
  "vtkAddMembershipArray"
  "vtkAppendPoints"
  "vtkApplyColors"
  "vtkApplyIcons"
  "vtkArcParallelEdgeStrategy"
  "vtkAreaLayout"
  "vtkAreaLayoutStrategy"
  "vtkAssignCoordinates"
  "vtkAssignCoordinatesLayoutStrategy"
  "vtkBivariateLinearTableThreshold"
  "vtkBivariateStatisticsAlgorithm"
  "vtkBoxLayoutStrategy"
  "vtkChacoGraphReader"
  "vtkCircularLayoutStrategy"
  "vtkClustering2DLayoutStrategy"
  "vtkCollapseGraph"
  "vtkCommunity2DLayoutStrategy"
  "vtkConeLayoutStrategy"
  "vtkConstrained2DLayoutStrategy"
  "vtkContingencyStatistics"
  "vtkCorrelativeStatistics"
  "vtkCosmicTreeLayoutStrategy"
  "vtkDataObjectToTable"
  "vtkDelimitedTextReader"
  "vtkDescriptiveStatistics"
  "vtkEdgeCenters"
  "vtkEdgeLayout"
  "vtkEdgeLayoutStrategy"
  "vtkExpandSelectedGraph"
  "vtkExtractSelectedGraph"
  "vtkExtractSelectedRows"
  "vtkFast2DLayoutStrategy"
  "vtkFixedWidthTextReader"
  "vtkForceDirectedLayoutStrategy"
  "vtkGenerateIndexArray"
  "vtkGeoEdgeStrategy"
  "vtkGeoMath"
  "vtkGraphHierarchicalBundle"
  "vtkGraphHierarchicalBundleEdges"
  "vtkGraphLayout"
  "vtkGraphLayoutStrategy"
  "vtkGroupLeafVertices"
  "vtkISIReader"
  "vtkKMeansDistanceFunctor"
  "vtkKMeansDistanceFunctorCalculator"
  "vtkKMeansStatistics"
  "vtkMergeColumns"
  "vtkMergeGraphs"
  "vtkMergeTables"
  "vtkMultiCorrelativeStatistics"
  "vtkMutableGraphHelper"
  "vtkNetworkHierarchy"
  "vtkOrderStatistics"
  "vtkPassArrays"
  "vtkPassThrough"
  "vtkPassThroughEdgeStrategy"
  "vtkPassThroughLayoutStrategy"
  "vtkPerturbCoincidentVertices"
  "vtkPCAStatistics"
  "vtkPruneTreeFilter"
  "vtkRISReader"
  "vtkRandomGraphSource"
  "vtkRandomLayoutStrategy"
  "vtkRemoveIsolatedVertices"
  "vtkRemoveHiddenData"
  "vtkSCurveSpline"
  "vtkSimple2DLayoutStrategy"
  "vtkSliceAndDiceLayoutStrategy"
  "vtkSplineGraphEdges"
  "vtkSquarifyLayoutStrategy"
  "vtkSpanTreeLayoutStrategy"
  "vtkStackedTreeLayoutStrategy"
  "vtkStatisticsAlgorithm"
  "vtkStrahlerMetric"
  "vtkStringToCategory"
  "vtkStringToNumeric"
  "vtkTableToGraph"
  "vtkTableToTreeFilter"
  "vtkThresholdTable"
  "vtkTransferAttributes"
  "vtkTreeFieldAggregator"
  "vtkTreeLayoutStrategy"
  "vtkTreeOrbitLayoutStrategy"
  "vtkTreeLevelsFilter"
  "vtkTreeMapLayout"
  "vtkTreeMapLayoutStrategy"
  "vtkTreeMapToPolyData"
  "vtkTreeRingToPolyData"
  "vtkTulipReader"
  "vtkUnivariateStatisticsAlgorithm"
  "vtkAttributeClustering2DLayoutStrategy"
  "vtkVertexDegree"
  "vtkXGMLReader"
  "vtkXMLTreeReader"
  "vtkAdjacencyMatrixToEdgeTable"
  "vtkArrayNorm"
  "vtkArrayToTable"
  "vtkComputeHistogram2DOutliers"
  "vtkDiagonalMatrixSource"
  "vtkDotProductSimilarity"
  "vtkExtractArray"
  "vtkExtractHistogram2D"
  "vtkMatricizeArray"
  "vtkNormalizeMatrixVectors"
  "vtkPairwiseExtractHistogram2D"
  "vtkSparseArrayToTable"
  "vtkTableToSparseArray"
  "vtkTransposeMatrix"
  "vtkSQLDatabaseGraphSource"
  "vtkSQLDatabaseTableSource"
  "vtkSQLGraphReader"
  "vtkStringToTimePoint"
  "vtkTimePointToString")

# Abstract classes in vtkInfovis.
SET(VTK_INFOVIS_CLASSES_ABSTRACT
  "vtkAreaLayoutStrategy"
  "vtkBivariateStatisticsAlgorithm"
  "vtkEdgeLayoutStrategy"
  "vtkGraphLayoutStrategy"
  "vtkStatisticsAlgorithm"
  "vtkTreeMapLayoutStrategy"
  "vtkUnivariateStatisticsAlgorithm")

# Wrap-exclude classes in vtkInfovis.
SET(VTK_INFOVIS_CLASSES_WRAP_EXCLUDE)

# Set convenient variables to test each class.
FOREACH(class ${VTK_INFOVIS_CLASSES})
  SET(VTK_CLASS_EXISTS_${class} 1)
ENDFOREACH(class)
FOREACH(class ${VTK_INFOVIS_CLASSES_ABSTRACT})
  SET(VTK_CLASS_ABSTRACT_${class} 1)
ENDFOREACH(class)
FOREACH(class ${VTK_INFOVIS_CLASSES_WRAP_EXCLUDE})
  SET(VTK_CLASS_WRAP_EXCLUDE_${class} 1)
ENDFOREACH(class)
