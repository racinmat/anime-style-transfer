//======================================================================
/*! \file Polygon2dFloat.cpp
 *
 * \copydoc Copyright
 * \author Ruben Jungnickel (rju)
 * \date Sep 11, 2015
 *///-------------------------------------------------------------------
//======================================================================

#include <ibeosdk/datablocks/snippets/Polygon2dFloat.hpp>
#include <ibeosdk/io.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

Polygon2dFloat::Polygon2dFloat()
  : std::vector<Point2dFloat>(),
    Snippet()
{}

//======================================================================

Polygon2dFloat::Polygon2dFloat(const Point2dFloat& p1)
  : std::vector<Point2dFloat>(1, p1),
    Snippet()
{}

//======================================================================

Polygon2dFloat::Polygon2dFloat(const Point2dFloat& p1, const Point2dFloat& p2)
  : std::vector<Point2dFloat>(),
    Snippet()
{
	this->reserve(2);
	this->push_back(p1);
	this->push_back(p2);
}

//======================================================================

Polygon2dFloat::Polygon2dFloat(const Point2dFloat& p1, const Point2dFloat& p2, const Point2dFloat& p3)
  : std::vector<Point2dFloat>(),
    Snippet()
{
	this->reserve(3);
	this->push_back(p1);
	this->push_back(p2);
	this->push_back(p3);
}

//======================================================================

Polygon2dFloat::Polygon2dFloat(const Point2dFloat& p1, const Point2dFloat& p2, const Point2dFloat& p3, const Point2dFloat& p4)
  : std::vector<Point2dFloat>(),
    Snippet()
{
	this->reserve(4);
	this->push_back(p1);
	this->push_back(p2);
	this->push_back(p3);
	this->push_back(p4);
}

//======================================================================

Polygon2dFloat::~Polygon2dFloat() {}

//======================================================================

std::streamsize Polygon2dFloat::getSerializedSize() const
{
	return std::streamsize(sizeof(uint16_t)) +  std::streamsize(this->size()) * Point2dFloat::getSerializedSize_static();
}

//======================================================================

bool Polygon2dFloat::deserialize(std::istream& is)
{
	const int64_t startPos = streamposToInt64(is.tellg());

	uint16_t sz;
	readBE(is, sz);
	this->resize(sz);

	Polygon2dFloat::iterator iter = this->begin();
	for (; iter!=this->end(); ++iter) {
		iter->deserialize(is);
	}

	return !is.fail() && ((streamposToInt64(is.tellg()) - startPos) == this->getSerializedSize());
}

//======================================================================

bool Polygon2dFloat::serialize(std::ostream& os) const
{
	const int64_t startPos = streamposToInt64(os.tellp());

	const uint16_t sz = uint16_t(this->size());
	writeBE(os, sz);

	Polygon2dFloat::const_iterator iter = this->begin();
	for (; iter!=this->end(); ++iter) {
		iter->serialize(os);
	}

	return !os.fail() && ((streamposToInt64(os.tellp()) - startPos) == this->getSerializedSize());
}

//======================================================================

}// namespace ibeosdk

//======================================================================
