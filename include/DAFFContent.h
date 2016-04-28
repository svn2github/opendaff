/*
* -------------------------------------------------------------------------------------
*
*  OpenDAFF - A free, open source software package for directional audio data
*  OpenDAFF is distributed under the terms of the GNU Lesser Public License (LGPL)
*
*  Copyright (c) Institute of Technical Acoustics, RWTH Aachen University, 2009-2016
*
*  ------------------------------------------------------------------------------------
*
*/

#ifndef IW_DAFF_CONTENT
#define IW_DAFF_CONTENT

#include <DAFFDefs.h>

// Forward declarations
class DAFFProperties;
class DAFFReader;
class DAFFMetadata;

//! Common content interface
/**
 * This purely abstract class defines the common interface for DAFF content.
 * Special (non-common) attributes of content are defined in subinterfaces
 * (e.g. #DAFFContentIR or #DAFFContentMS).
 *
 * The interface covers operations on the sphere grid, like
 * nearest neighbour search, cell determination, etc. as well
 * as spherical coordinate transformations.
 * 
 * @see DAFFContent 
 * @see DAFFContentDFT
 * @see DAFFContentIR
 * @see DAFFContentMPS
 * @see DAFFContentMS
 * @see DAFFContentPS
 *
 */
class DAFF_API DAFFContent
{
public:
	inline virtual ~DAFFContent() {};

	//! Returns the parent reader
	/*
	 * @return The pointer to parent reader
	 */
	virtual DAFFReader* getParent() const=0;

	//! Returns the properties
	/*
	 * @return The const pointer to properties
	 */
	virtual const DAFFProperties* getProperties() const=0;

	//! Returns the metadata of a record
	/**
	 * Note: The method always returns a valid DAFFMetadata, even if there is
	 *       none specified in the file. Thereby we maintain a consistent 
	 *       behaviour. Metadata may therefore be empty.
	 *
	 * @param [in] iRecordIndex	The index of the record on regular spherical grid
	 *
	 * @return The const pointer to metadata
	 */
	virtual const DAFFMetadata* getRecordMetadata( int iRecordIndex ) const=0;

	//! Determines the spherical coordinates of a record (grid point on spherical regular grid)
	/**
	 * @param [in] iRecordIndex	The index of the record on regular spherical grid
	 * @param [in] iVew			The view that should be used for the given pair of angles, one of #DAFF_VIEWS
	 * @param [out] fAngle1		First angle that corresponds to this index (Phi or Alpha, depending on view)
	 * @param [out] fAngle2		Second angle that corresponds to this index (Theta or Beta, depending on view)
	 * 
	 * @return #DAFF_NO_ERROR on success, another #DAFF_ERROR otherwise
	 */
	virtual int getRecordCoords( int iRecordIndex, int iView, float& fAngle1, float& fAngle2 ) const=0;

	//! Determine the nearest neighbour record and return its index
	/**
	 * This method takes a direction in form of an angular pair and searches the grid for
	 * the nearest neighbouring record (grid point). The distance measure is the angle
	 * on the great circle (spherical law of cosines).
	 *
	 * Note: The method always returns a valid record index, even if the angular pair
	 *       points outside the spherical grid (e.g. consider boundaries). Thereby
	 *       we maintain a consistent behaviour. You can check if the point was within
	 *       the boundaries using the bOutOfBounds argument.
	 *
	 * @param [in] iVew				The view that should be used for the given pair of angles, one of #DAFF_VIEWS
	 * @param [in] fAngle1			First angle (Phi or Alpha, depending on view)
	 * @param [in] fAngle2			Second angle (Theta or Beta, depending on view)
	 * @param [out] iRecordIndex	Index that corresponds to this pair of angles	 
	 */
	virtual void getNearestNeighbour( int iView, float fAngle1, float fAngle2, int& iRecordIndex ) const=0;

	//! Determine the nearest neighbour record and return its index (with boundary validatsion)
	/**
	* This method takes a direction in form of an angular pair and searches the grid for
	* the nearest neighbouring record (grid point). The distance measure is the angle
	* on the great circle (spherical law of cosines).
	*
	* Note: The method always returns a valid record index, even if the angular pair
	*       points outside the spherical grid (e.g. consider boundaries). Thereby
	*       we maintain a consistent behaviour. You can check if the point was within
	*       the boundaries using the bOutOfBounds argument.
	*
	* @param [in] iVew				The view that should be used for the given pair of angles, one of #DAFF_VIEWS
	* @param [in] fAngle1			First angle (Phi or Alpha, depending on view)
	* @param [in] fAngle2			Second angle (Theta or Beta, depending on view)
	* @param [out] iRecordIndex		Index that corresponds to this pair of angles
	* @param [out] bbOutOfBounds	Indicator if requested direction was out of bounds (true in this case)
	*
	*/
	virtual void getNearestNeighbour( int iView, float fAngle1, float fAngle2, int& iRecordIndex, bool& bOutOfBounds ) const=0;

	//! Determines the cell of a given direction on the sphere grid and delivers its surrounding record indices
	/**
	 * This method takes a direction in form of an angular pair and searches for the valid
	 * surrounding grid points which are related to a record. In a full covered sphere, this means
	 * a rectangular cell at any point except of the area near the poles, where we obtain triangular cells.
	 * Nevertheless, the method will return four valid record indices in any given case. It makes heavy use of
	 * the getNearestNeighbour method (which does not make it that fast) and is invented for custom
	 * interpolation purposes.
	 *
	 * Sequence convention
	 * 
	 * Imagine you are sitting at the origin of the sphere and you point in a random direction. What you
	 * prickle will be a cell similar to this one:
	 *
	 * (3)-----(2)
	 *  |		|
	 *  |		|	   ^
	 * (4)-----(1)	   | (direction beta increases)
	 * 
	 *            <--
	 *   (direction alpha increases)
	 *
	 * The first index will always be the lower grid point of both given angles, the second is the first point
	 * increased by the beta resolution at beta angle. The third point is represented by both the increase of
	 * the alpha and beta angle each with their resolution steps. The fourth point is the first point only with
	 * an increase by alpha resolution of the alpha angle.
	 * 
	 * Let's discuss an example:
	 * Requested direction: alpha =  2�, beta = 2�	object view coordinate system (front direction would be 0�,0�)
	 * Resolution:			alpha = 10�, beta = 5�
	 * Index1 -> alpha =  0�, beta = 0� (object view)
	 * Index2 -> alpha =  0�, beta = 5� (object view)
	 * Index3 -> alpha = 10�, beta = 5� (object view)
	 * Index4 -> alpha = 10�, beta = 0� (object view)
	 *
	 * Special cases
	 *
	 *  -	if a pole is directly hitten and the full sphere is covered, it will return 4 times the
	 *		pole's record index. 
	 *  -	if the requested direction is near a pole or out of boundaries, the method will simply search
	 *		for the nearest neighbouring directions for each of the four cell points as if the cell would 
	 *		exists on a regular sphere grid. This will result in the return of at least two indices 
	 *		indicating the same record. You most certainly will discover something like that near north pole
	 *
	 *   (3,2)
	 *   /	 \
	 *  /	  \
	 * (4)----(1)
	 *
	 *		or that near south pole
	 *
	 * (3)----(2)
	 *  \	  /
	 *	 \	 /
	 *	 (4,1)
	 *
	 *		or, if out of bounds, something similar to
	 * 
	 * (3,4)-----(1,2)
	 *
	 *		or
	 *
	 *	   (3,2)
	 *	     |
	 *       |
	 *     (4,1)
	 *
	 *		Anyway, using getCell out of the boundaries does not make that much sense. In case you run
	 *		out of bounds onsider using getNearestNeighbour with boundary flag instead.
	 */
	virtual void getCell(int iView, const float fAngle1, const float fAngle2, DAFFQuad& qIndices) const=0;

	// --= Coordinate transformations =--

	//! Transforms data spherical coordinates into object spherical coordinates
	/**
	 * This methods takes an angular pair (alpha, beta) in data spherical coordinates (DSC)
	 * and transforms it into the equivalent direction expressed in object spherical coordinates (OSC)
	 * as an angular pair (azimuth, elevation).
	 *
	 * \param fAlpha		Alpha angle in the DSC [degrees] (input)
	 * \param fBeta			Beta angle in the DSC [degrees] (input)
	 * \param fAzimuth		Azimuthal angle in the OSC [degrees] (output)
	 * \param fElevation	Elevation angle in the OSC [degrees] (output)
	 */
	virtual void transformAnglesD2O(const float fAlpha,
		                            const float fBeta,
						            float& fAzimuth,
							        float& fElevation) const=0;

	//! Transforms object spherical coordinates into data spherical coordinates
	/**
	 * This methods takes an angular pair (alpha, beta) in object spherical coordinates (OSC)
	 * and transforms it into the equivalent direction expressed in data spherical coordinates (DSC)
	 * as an angular pair (azimuth, elevation).
	 *
	 * \param fAzimuth		Azimuthal angle in the OSC [degrees] (input)
	 * \param fElevation	Elevation angle in the OSC [degrees] (input)
	 * \param fAlpha		Alpha angle in the DSC [degrees] (output)
	 * \param fBeta			Beta angle in the DSC [degrees] (output)
	 */
	virtual void transformAnglesO2D(const float fAzimuth,
		                            const float fElevation,
						            float& fAlpha,
							        float& fBeta) const=0;
};

#endif // IW_DAFF_CONTENT
