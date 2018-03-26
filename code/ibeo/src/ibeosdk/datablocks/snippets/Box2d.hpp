//======================================================================
/*! \file Box2d.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Sep 13, 2016
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_BOX2D_HPP_SEEN
#define IBEOSDK_BOX2D_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>

#include <ibeosdk/CompressedPoint2d.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

class Box2d : public Snippet {
public:
	static std::streamsize getSerializedSize_static()
	{
		return std::streamsize(2*CompressedPoint2d::getSerializedSize_static())
		     + std::streamsize(sizeof(int16_t));
	}

public:
	Box2d();

	Box2d(const CompressedPoint2d& center,
	      const CompressedPoint2d& size,
	      const int16_t compressedRotation = 0);

public:
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	//========================================
	/*!\brief Returns the center point of this Box
	 *///-------------------------------------
	const CompressedPoint2d& getCenter() const { return m_center; }

	//========================================
	/*!\brief Returns the size of this Box
	 *
	 * The returned size denotes the size of the box in x-direction
	 * point2Dfloat::getX() and y-direction point2Dfloat::getY(), where the
	 * x-direction is rotated against the original x-axis by
	 * getRotation().
	 *///-------------------------------------
	const CompressedPoint2d& getSize() const { return m_size; }

	//========================================
	/*!\brief Returns the rotation angle of this Box2D in [radians],
	 *        counter clock wise.
	 *
	 * The rotation of the box' coordinate system around its center
	 * point. Must be in the interval [-pi,pi]
	 *///-------------------------------------
	int16_t getRotation() const { return m_compressedRotation; }


	void setCenter(const CompressedPoint2d& newCenter) { this->m_center = newCenter; }
	void setSize(const CompressedPoint2d& newSize) { this->m_size = newSize; }
	void SetRotation(const int16_t newCompressedRotation) { this->m_compressedRotation = newCompressedRotation; }

protected:
	CompressedPoint2d m_center;
	CompressedPoint2d m_size;
	int16_t m_compressedRotation;
}; // Box2d

//======================================================================

bool operator==(const Box2d& lhs, const Box2d& rhs);
bool operator!=(const Box2d& lhs, const Box2d& rhs);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_BOX2D_HPP_SEEN

//======================================================================
