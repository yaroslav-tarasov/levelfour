//!
//! \file "ShapeMapParameter.h"
//! \brief Header file for ShapeMapParameter class.
//!
//! \version    1.0
//! \date       19.04.2010 (last updated)
//!

#ifndef ShapeMapParameter_H
#define ShapeMapParameter_H

#include "FrapperPrerequisites.h"
#include "AbstractMapParameter.h"
#include "MaterialMapParameter.h"
#include "OgreEntity.h"

//!
//! Class representing parameters for vtk table values.
//!
class FRAPPER_CORE_EXPORT ShapeMapParameter : public AbstractMapParameter 
{
public: // nested enumerations

    //!
    //! Nested enumeration for the different alignement types.
    //!
    static enum Alignement {
        Unknown = -1,
        TopLeft,
        TopCenter,
		TopRight,
		MiddleLeft,
		MiddleCenter,
		MiddleRight,
		BottomLeft,
		BottomCenter,
		BottomRight
	};

    //!
    //! Type of shapes (geo derived, primitive meshes, areas) 
    //!
    static enum ShapeType {
        Un_known = -1,
        GEO,
        PRIMITIVE,
        AREA
	};

public: // constructors and destructors

    //!
    //! Constructor of the ShapeMapParameter class.
    //!
    //! \param name The name of the parameter.
    //!
    ShapeMapParameter ( const QString &name );

	//!
    //! Destructor of the ShapeMapParameter class.
    //!
    virtual ~ShapeMapParameter ();

public: // functions

    //!
    //! Returns the entity associate with the id.
    //!
    //! \return The ogre entity.
    //!
	Ogre::Entity * getShape (const vtkIdType id) const;

    //!
    //! Sets the shape associated to the id.
    //!
    //! \param id The id.
    //! \param entity The ogre entity representing the shape.
    //!
	void setShape ( const vtkIdType id, Ogre::Entity * entity);

    //!
    //! Returns the entity name associated with the id.
    //!
    //! \return The ogre entity name.
    //!
	QString getShapeName (const vtkIdType id) const;

    //!
    //! Sets the shape name associated to the id.
    //!
    //! \param id The id.
    //! \param name The ogre entity name representing the shape.
    //!
	void setShapeName ( const vtkIdType id, QString name);

    //!
    //! Returns the alignement associate with the id.
    //!
    //! \return The alignement string.
    //!
	Alignement getAlignement (const vtkIdType id) const;

    //!
    //! Sets the alignement associate with the id.
    //!
    //! \param id The id.
    //! \param entity The alignement.
    //!
	void setAlignement (const vtkIdType id, Alignement alignement);

    //!
    //! Returns the type of shapes associated with this mapper.
    //!
    //! \return The shape type.
    //!
	ShapeType  getShapeType () const;

    //!
    //! Sets the type of shapes associated with this mapper.
    //! \param type The shape type.
    //!
	void setShapeType (const ShapeType  type);

    //!
    //! Returns the centroid associated with this shape id.
    //!
    //! \return The centroid of the shape.
    //!
	double * getCentroid (const vtkIdType id) const;

    //!
    //! Returns true if the shape parameter has centroids
    //!
    //! \return bool True if the shape has centroids.
    //!
	bool hasCentroids () const;

    //!
    //! Sets true if the shape parameter has centroids
    //!
	void setHasCentroids (const bool hasCentroids);

    //!
    //! Copy the shape parameter into this
    //!
	void copyShapeParameter(ShapeMapParameter * shapeParam);

	//!
	//! Returns the Material associate with the id.
	//!
	//! \return The item Material.
	//!
	Ogre::String getMaterial (const vtkIdType id) const;

	//!
	//! Set the Material map input.
	//!
	void setMaterialMapParameter (MaterialMapParameter * materialMap);

	//!
	//! Get the Material map input.
	//!
	MaterialMapParameter * getMaterialMapParameter () const;

private:// data

	// describe the type of shapes (geo, areas, primitives..)
	ShapeType m_shapeType;

	// true if the shape map parameter has centroids
	bool m_hasCentroids;

	MaterialMapParameter * m_materialMap;
};


#endif
