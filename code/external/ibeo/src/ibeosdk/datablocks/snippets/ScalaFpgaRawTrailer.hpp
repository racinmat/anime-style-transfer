//======================================================================
/*! \file ScalaFpgaRawTrailer.hpp
 *
 * \copydoc Copyright
 * \author kd
 * \date Sep 17, 2015
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_SCALAFPGARAWTRAILER_HPP_SEEN
#define IBEOSDK_SCALAFPGARAWTRAILER_HPP_SEEN

//======================================================================

#include <ibeosdk/misc/WinCompatibility.hpp>

#include <ibeosdk/datablocks/snippets/Snippet.hpp>
#include <ibeosdk/inttypes.hpp>

#include <ibeosdk/misc/deprecatedwarning.hpp>

#include <istream>
#include <ostream>

//======================================================================

namespace ibeosdk {

//======================================================================

class ScalaFpgaRawTrailer : public Snippet {
public: // public types
	enum ConfigurationRegisterFlags {
		CRF_MirrorSide        = 0x1,
		CRF_ReducedApdVoltage = 0x2
	};

	enum MirrorSide {
		MirrorSide0 = 0,
		MirrorSide1 = 1
	};

public: // static methods
	static std::streamsize getSerializedSize_static() { return 16; }

public: // constructor/destructor
	ScalaFpgaRawTrailer();
	virtual ~ScalaFpgaRawTrailer();

public: // operators
	//! Equality predicate
	bool operator==(const ScalaFpgaRawTrailer& other) const;
	bool operator!=(const ScalaFpgaRawTrailer& other) const;

public: // Snippet interface
	virtual std::streamsize getSerializedSize() const { return getSerializedSize_static(); }
	virtual bool deserialize(std::istream& is);
	virtual bool serialize(std::ostream& os) const;

public: // getter
	uint16_t getOperatingRegister() const { return m_operatingRegister; }
	uint16_t getWarning() const { return m_warning; }
	uint16_t getError() const { return m_error; }
	uint16_t getScanPointCounter() const { return m_scanPointCounter; }
	uint16_t getConfigurationRegister() const { return m_configurationRegister; }
	uint16_t getReservedTrailer7() const { return m_reservedTrailer7; }

public: // derived getter
	MirrorSide getMirrorSide() const { return ((m_configurationRegister & CRF_MirrorSide) == CRF_MirrorSide) ? MirrorSide1 : MirrorSide0; }
	bool isApdVoltageReduced() const { return ((m_configurationRegister & CRF_ReducedApdVoltage) == CRF_ReducedApdVoltage); }

public: // setter
	void setOperatingRegister(const uint16_t reg) { m_operatingRegister = reg; }
	void setWarning(const uint16_t warning) { m_warning = warning; }
	void setError(const uint16_t err) { m_error = err; }
	void setScanPointCounter(const uint16_t cnt) { m_scanPointCounter = cnt; }
	void setConfigurationRegister(const uint16_t side) { m_configurationRegister = side; }

public: // derived setter
	void setMirrorSide(const MirrorSide mirrorSide) { modifyConfigRegisterFlag(mirrorSide == MirrorSide1, CRF_MirrorSide); }
	void setApdVoltageReduced(const bool isReduced) { modifyConfigRegisterFlag(isReduced, CRF_ReducedApdVoltage); }

protected:
	void modifyConfigRegisterFlag(const bool flagValue, const ConfigurationRegisterFlags bit)
	{
		if (flagValue) {
			m_configurationRegister = uint16_t(m_configurationRegister | bit);
		}
		else {
			m_configurationRegister = uint16_t(m_configurationRegister & ~bit);
		}
	}

public: // public static const attributes
	static const uint16_t blockId;

protected: // protected attributes
	uint16_t m_operatingRegister;
	uint16_t m_warning;
	uint16_t m_error;
	uint16_t m_reservedTrailer4;
	uint16_t m_scanPointCounter;
	uint16_t m_configurationRegister;
	uint16_t m_reservedTrailer7;
}; // ScalaFpgaRawTrailer

//======================================================================

} // namespace ibeosdk

//======================================================================

#endif // IBEOSDK_SCALAFPGARAWTRAILER_HPP_SEEN

//======================================================================

