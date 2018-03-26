//======================================================================
/*! \file LogPolygon2dFloat.hpp
 *
 * \copydoc Copyright
 * \author Ruben Jungnickel (rju)
 * \date Sep 11, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_LOGPOLYGON2DFLOAT_HPP_SEEN
#define IBEOSDK_LOGPOLYGON2DFLOAT_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>
#include <ibeosdk/datablocks/snippets/Polygon2dFloat.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class LogPolygon2dFloat : public Snippet {
public:
	LogPolygon2dFloat();
	virtual ~LogPolygon2dFloat();

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	const std::string& getMsg() const { return m_msg; }
	const Polygon2dFloat& getPoly() const { return m_poly; }

public:
	void setMsg(const std::string& msg) { m_msg = msg; }
	void setPoly(const Polygon2dFloat& poly) { m_poly = poly; }

private:
	Polygon2dFloat m_poly;
	std::string m_msg;
}; // Polygon2dFloat

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_LOGPOLYGON2DFLOAT_HPP_SEEN

//======================================================================
