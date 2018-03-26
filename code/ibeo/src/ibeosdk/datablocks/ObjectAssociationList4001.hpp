//======================================================================
/*! \file ObjectAssociationList4001.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Apr 26, 2016
 *///-------------------------------------------------------------------

#ifndef OBJECTASSOCIATIONLIST4001_HPP_SEEN
#define OBJECTASSOCIATIONLIST4001_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/RegisteredDataBlock.hpp>
#include <ibeosdk/datablocks/snippets/ObjectAssociation4001.hpp>

#include <vector>

//======================================================================

namespace ibeosdk {

//======================================================================

class ObjectAssociationList4001 : public RegisteredDataBlock<ObjectAssociationList4001> {
public: // type declaration
	typedef std::vector<ObjectAssociation4001> ObjAssocVector;

public:
	static const int32_t nbOfReserved = 8;

public:
	ObjectAssociationList4001();

public: // getter
	UINT8 getRefObjListId() const { return m_refObjListId; }
	UINT8 getRefDevType() const { return m_refDevType; }
	UINT16 getRefDevInterfaceVersion() const { return m_refDevInterfaceVersion; }
	UINT8 getDutObjListId() const { return m_dutObjListId; }
	UINT8 getDutDevType() const { return m_dutDevType; }
	UINT16 getDutDevInterfaceVersion() const { return m_dutDevInterfaceVersion; }

	const ObjAssocVector& getObjectAssociations() const { return m_objAssocs; }
	ObjAssocVector& getObjectAssociations() { return m_objAssocs; }

	char getReserved(const int32_t r) const
	{
		assert(r>=0);
		assert(r < nbOfReserved);
		return m_reserved[r];
	}

public: // setter
	void setRefObjListId(const UINT8 newRefObjListId) { m_refObjListId = newRefObjListId; }
	void setRefDevType(const UINT8 newRefDevType) { m_refDevType = newRefDevType; }
	void setRefDevInterfaceVersion(const UINT16 newRefDevInterfaceVersion) { m_refDevInterfaceVersion = newRefDevInterfaceVersion; }
	void setDutObjListId(const UINT8 newDutObjListId) { m_dutObjListId = newDutObjListId; }
	void setDutDevType(const UINT8 newDutDevType) { m_dutDevType = newDutDevType; }
	void setDutDevInterfaceVersion(const UINT16 newDutDevInterfaceVersion) { m_dutDevInterfaceVersion = newDutDevInterfaceVersion; }

	//========================================
	/*!\brief Setter for reserved values.
	 * \attention Not part of the "public" interface For testing purpose only!
	 *///-------------------------------------
	void setReserved(const int32_t r, const char newReservedValue)
	{
		assert(r>=0);
		assert(r < nbOfReserved);
		m_reserved[r] = newReservedValue;
	}

public:
	virtual std::streamsize getSerializedSize() const;
	virtual DataTypeId getDataType() const;
	virtual bool deserialize(std::istream& is, const IbeoDataHeader& dh);
	virtual bool serialize(std::ostream& os) const;

protected:
	uint8_t m_refObjListId;
	uint8_t m_refDevType;
	uint16_t m_refDevInterfaceVersion; // 14bits are used
	uint8_t m_dutObjListId;
	uint8_t m_dutDevType;
	uint16_t m_dutDevInterfaceVersion; // 14bits are used
	// uint32_t objAssocCount
	char m_reserved[nbOfReserved];

	ObjAssocVector m_objAssocs;
}; // ObjectAssociationList4001

//======================================================================

bool operator==(const ObjectAssociationList4001& lhs, const ObjectAssociationList4001& rhs);

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // OBJECTASSOCIATIONLIST4001_HPP_SEEN

//======================================================================
