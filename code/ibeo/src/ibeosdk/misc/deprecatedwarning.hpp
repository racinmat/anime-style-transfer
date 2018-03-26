//======================================================================
/*! \file deprecatedwarning.hpp
 *
 * \copydoc Copyright
 * \author Jan Christian Dittmer (jcd)
 * \date Mar 7, 2014
 *///-------------------------------------------------------------------

#ifndef IBEOSDK_DEPRECATEDWARNING_HPP_SEEN
#define IBEOSDK_DEPRECATEDWARNING_HPP_SEEN

//======================================================================

#ifdef __GNUC__
#  ifndef __GNUC_PREREQ
/* Mingw has gcc but not this particular version check (huh?!?) */
#    define __GNUC_PREREQ(a,b) 1
#  endif
#  if __GNUC_PREREQ(3, 0)
/* The "deprecated" attribute is only available in gcc >= 3.0.x */
#    define IBEOSDK_DEPRECATED __attribute__((deprecated))
#  else
#    define IBEOSDK_DEPRECATED
#  endif
#else /* __GNUC__ */
/* Non-gcc compiler */
#  ifdef _MSC_VER
#    define IBEOSDK_DEPRECATED __declspec(deprecated)
#  else
#    define IBEOSDK_DEPRECATED
#  endif
#endif /* __GNUC__ */

//======================================================================

#endif // IBEOSDK_DEPRECATEDWARNING_HPP_SEEN

//======================================================================
