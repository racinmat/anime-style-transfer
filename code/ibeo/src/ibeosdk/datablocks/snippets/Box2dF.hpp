//======================================================================
/*! \file Box2dF.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date May 30, 2012
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_BOX2DF_HPP_SEEN
#define IBEOSDK_BOX2DF_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <ibeosdk/datablocks/snippets/Point2dFloat.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class Box2dF : public Snippet {
public:
	static std::streamsize getSerializedSize_static()
	{
		return std::streamsize(2*Point2dFloat::getSerializedSize_static())
		     + std::streamsize(sizeof(float));
	}

public:
	Box2dF();

	Box2dF(const Point2dFloat& center,
	       const Point2dFloat& size,
	       const float rotation = 0.0);

public: // Snippet interface
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	//========================================
	/*!\brief Returns the center point of this Box
	 *///-------------------------------------
	const Point2dFloat& getCenter() const { return m_center; }

	//========================================
	/*!\brief Returns the size of this Box
	 *
	 * The returned size denotes the size of the box in x-direction
	 * point2Dfloat::getX() and y-direction point2Dfloat::getY(), where the
	 * x-direction is rotated against the original x-axis by
	 * getRotation().
	 *///-------------------------------------
	const Point2dFloat& getSize() const { return m_size; }

	//========================================
	/*!\brief Returns the rotation angle of this Box2D in [radians],
	 *        counter clock wise.
	 *
	 * The rotation of the box' coordinate system around its center
	 * point. Must be in the interval [-pi,pi]
	 *///-------------------------------------
	float getRotation() const { return m_rotation; }


	void setCenter(const Point2dFloat& newCenter) { this->m_center = newCenter; }
	void setSize(const Point2dFloat& newSize) { this->m_size = newSize; }
	void SetRotation(const float newRotation) { this->m_rotation = newRotation; }


	//========================================
	/*!\brief Returns boundary angles for this box.
	 *
	 * This function calculates a low and a high boundary angle for
	 * all edges of the given (rotated) Box2D. The returned FloatPair
	 * has the component "first" for the lower bounding angle, and
	 * "second" for the upper bounding angle.
	 *
	 * (Note: This ordering is swapped compared to the scan point
	 * ordering!)
	 *///-------------------------------------
	std::pair<float, float> getBoundingAngles() const;

	bool containsPoint(const Point2dFloat& point) const;
	std::vector<Point2dFloat> getCorners() const;


protected:
	Point2dFloat m_center;
	Point2dFloat m_size;
	float m_rotation;
}; // Box2dF

//======================================================================

bool operator==(const Box2dF& lhs, const Box2dF& rhs);
bool operator!=(const Box2dF& lhs, const Box2dF& rhs);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_BOX2DF_HPP_SEEN

//======================================================================

