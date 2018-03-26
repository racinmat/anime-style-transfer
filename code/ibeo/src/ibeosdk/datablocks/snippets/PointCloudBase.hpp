//======================================================================
/*! \file PointCloudBase.hpp
 *
 * \copydoc Copyright
 * \author Kristian Bischoff (kb)
 * \date Mar 15, 2016
 *///-------------------------------------------------------------------

//======================================================================

#ifndef IBEOSDK_POINTCLOUDBASE_HPP_SEEN
#define IBEOSDK_POINTCLOUDBASE_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/datablocks/snippets/ReferencePlane.hpp>

#include <boost/iterator/iterator_facade.hpp>

//======================================================================

namespace ibeosdk {

//======================================================================

namespace pointType{
enum PointType {
	PointTypePoint           = 0x0000,
	PointTypeWithEpw         = 0x0001,
	PointTypeWithFlags       = 0x0002,
	PointTypeWithEpwAndFlags = 0x0003
};
} // namespace pointType

//==============================================================================

namespace pointKind{
enum PointKind
{
	PointKindUndefined    = 0x0000,
	PointKindScanPoint    = 0x0001,
	PointKindLanePoint    = 0x0002
};
} // namespace pointKind

//==============================================================================

namespace pointFlags{
enum PointFlag{
	POINTFLAG_ROADMARKING =  (1  <<  0),
	POINTFLAG_OFFROAD     =  (1  <<  1),
	PointFlagReserved_02  =  (1  <<  2),
	PointFlagReserved_03  =  (1  <<  3),
	PointFlagReserved_04  =  (1  <<  4),
	PointFlagReserved_05  =  (1  <<  5),
	PointFlagReserved_06  =  (1  <<  6),
	PointFlagReserved_07  =  (1  <<  7),
	PointFlagReserved_08  =  (1  <<  8),
	PointFlagReserved_09  =  (1  <<  9),
	PointFlagReserved_10  =  (1  << 10),
	PointFlagReserved_11  =  (1  << 11),
	PointFlagReserved_12  =  (1  << 12),
	PointFlagReserved_13  =  (1  << 13),
	PointFlagReserved_14  =  (1  << 14),
	PointFlagReserved_15  =  (1  << 15),
	PointFlagReserved_16  =  (1  << 16),
	PointFlagReserved_17  =  (1  << 17),
	PointFlagReserved_18  =  (1  << 18),
	PointFlagReserved_19  =  (1  << 19),
	PointFlagReserved_20  =  (1  << 20),
	PointFlagReserved_21  =  (1  << 21),
	PointFlagReserved_22  =  (1  << 22),
	PointFlagReserved_23  =  (1  << 23),
	PointFlagReserved_24  =  (1  << 24),
	PointFlagReserved_25  =  (1  << 25),
	PointFlagReserved_26  =  (1  << 26),
	PointFlagReserved_27  =  (1  << 27),
	PointFlagReserved_28  =  (1  << 28),
	PointFlagReserved_29  =  (1  << 29),
	PointFlagReserved_30  =  (1  << 30),
	PointFlagReserved_31  =  (1  << 31)
}; // PointFlag
} // namespace pointFlags

//======================================================================
//======================================================================

class PointCloudBase: public Snippet {
public:
	PointCloudBase();
	virtual ~PointCloudBase() {}

public:
	bool operator==(const PointCloudBase& other) const;
	virtual bool empty() const = 0;

public:
	virtual std::streamsize getSerializedSize() const;
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public:
	static std::string kindToString(const pointKind::PointKind kind);
	static pointKind::PointKind stringToKind(const std::string& kind);

public:
	virtual pointKind::PointKind getKind() const { return m_kind; }
	virtual pointType::PointType getType() const { return m_type; }

	virtual void setKind(const pointKind::PointKind kind) { m_kind = kind; }
	virtual void setType(const pointType::PointType type) { m_type = type; }

	virtual ReferencePlane& referencePlane() { return m_refPlane; }
	virtual const ReferencePlane& getReferencePlane() const { return m_refPlane; }
	virtual void setReferencePlane(const ReferencePlane& plane) { m_refPlane = plane; }

public:
	//========================================
	//  ITERATOR IMPLEMENTATION
	//========================================
	// Value refers to the Point
	// PointIterator refers to the std::vector::iterator or const_iterator
	template <class PointType, class PointIterator, class ProxyType, class ProxyTypeNonConst, class CloudType, class CloudTypeNonConst>
	class CustomIterator : public boost::iterator_facade<CustomIterator<PointType,
	                                                                    PointIterator,
	                                                                    ProxyType,
	                                                                    ProxyTypeNonConst,
	                                                                    CloudType,
	                                                                    CloudTypeNonConst>,
	                                                     ProxyType,
	                                                     boost::bidirectional_traversal_tag> {
	public:
		CustomIterator()
		{
			updateCurrent();
		}

		CustomIterator(PointIterator it, CloudType* cloud ) : m_it_point(it), m_cloud(cloud), m_currentProxy(createProxy())
		{
			updateCurrent();
		}

		// copy constructor to be able to create a const_iterator from a non const iterator
		template <class OtherIter, class OtherPointIterator, class OtherProxyType, class OtherProxyTypeNonConst, class OtherCloudType, class OtherCloudTypeNonConst>
		CustomIterator(CustomIterator<OtherIter, OtherPointIterator, OtherProxyType, OtherProxyTypeNonConst, OtherCloudType, OtherCloudTypeNonConst> const& other) : m_it_point(other.m_it_point), m_cloud(other.m_cloud){}

	private:
		// friends required for construction and equality check
		friend class boost::iterator_core_access;
		template <class, class, class, class, class, class> friend class CustomIterator;

		void increment() { ++m_it_point; updateCurrent(); }
		void decrement() { --m_it_point; updateCurrent(); }

		void updateCurrent()
		{
			m_currentProxy = ProxyType( const_cast<PointType*>(&(*m_it_point)), const_cast<CloudTypeNonConst* >(m_cloud) );
		}

		ProxyType& createProxy() { updateCurrent(); return m_currentProxy; }

		// equality should be checkable also between const and non-const iterators
		template <class OtherIter, class OtherPointIterator, class OtherProxyType, class OtherProxyTypeNonConst, class OtherCloudType, class OtherCloudTypeNonConst>
		bool equal(CustomIterator<OtherIter, OtherPointIterator, OtherProxyType, OtherProxyTypeNonConst, OtherCloudType, OtherCloudTypeNonConst> const& other) const
		{
			return this->m_it_point == other.m_it_point;
		}

		// dereferencing
		ProxyType& dereference() const { return const_cast<ProxyType&>(m_currentProxy); }

	private:
		PointIterator      m_it_point;
		CloudType*         m_cloud;
		ProxyTypeNonConst  m_currentProxy;
	}; // CustomIterator

protected:
	//========================================
	pointKind::PointKind m_kind;
	pointType::PointType m_type;

	ReferencePlane m_refPlane;
}; // PointCloudBase

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_POINTCLOUDBASE_HPP_SEEN

//======================================================================
