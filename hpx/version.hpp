//  Copyright (c) 2007-2012 Hartmut Kaiser
//  Copyright (c)      2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#if !defined(HPX_VERSION_AUG_18_2011_0854PM)
#define HPX_VERSION_AUG_18_2011_0854PM

#include <string>

#include <boost/version.hpp>
#include <boost/config.hpp>
#include <boost/cstdint.hpp>
#include <boost/preprocessor/cat.hpp>

#include <hpx/config/export_definitions.hpp>

///////////////////////////////////////////////////////////////////////////////
//  The version of HPX
//
//  HPX_VERSION_FULL & 0x0000FF is the sub-minor version
//  HPX_VERSION_FULL & 0x00FF00 is the minor version
//  HPX_VERSION_FULL & 0xFF0000 is the major version
//
#define HPX_VERSION_FULL         0x000800

#define HPX_VERSION_MAJOR        0
#define HPX_VERSION_MINOR        8
#define HPX_VERSION_SUBMINOR     0

#define HPX_VERSION_DATE         20120316

#if !defined(HPX_AGAS_VERSION)
    #define HPX_AGAS_VERSION 0x21
#endif

#define HPX_VERSION_TAG          ""

#if !defined(HPX_SVN_REVISION)
    #define HPX_SVN_REVISION     "$Revision$"
#endif

///////////////////////////////////////////////////////////////////////////////
// The version check enforces the major and minor version numbers to match for
// every compilation unit to be compiled.
#define HPX_CHECK_VERSION                                                     \
    BOOST_PP_CAT(hpx_check_version_,                                          \
        BOOST_PP_CAT(HPX_VERSION_MAJOR,                                       \
            BOOST_PP_CAT(_, HPX_VERSION_MINOR)))                              \
    /**/

// The version check enforces the major and minor version numbers to match for
// every compilation unit to be compiled.
#define HPX_CHECK_BOOST_VERSION                                               \
    BOOST_PP_CAT(hpx_check_boost_version_, BOOST_VERSION)                     \
    /**/

///////////////////////////////////////////////////////////////////////////////
namespace hpx
{
    // Returns the major HPX version.
    HPX_EXPORT boost::uint8_t major_version();

    // Returns the minor HPX version.
    HPX_EXPORT boost::uint8_t minor_version();

    // Returns the sub-minor/patch-level HPX version.
    HPX_EXPORT boost::uint8_t subminor_version();

    // Returns the full HPX version.
    HPX_EXPORT boost::uint32_t full_version();

    // Returns the AGAS subsystem version.
    HPX_EXPORT boost::uint8_t agas_version();

    // Returns the tag.
    HPX_EXPORT std::string tag();

    // Returns the HPX version string.
    HPX_EXPORT std::string build_string();

    // Returns the Boost version string.
    HPX_EXPORT std::string boost_version();

    // Returns the copyright string.
    HPX_EXPORT std::string copyright();

    // Returns the full version string.
    HPX_EXPORT std::string complete_version();

    // Helper data structures allowing to automatically detect version problems
    // between applications and the core libraries.
    extern HPX_EXPORT char const HPX_CHECK_VERSION[];
    extern HPX_EXPORT char const HPX_CHECK_BOOST_VERSION[];
}

///////////////////////////////////////////////////////////////////////////////
#if !defined(HPX_EXPORTS)
    // This is instantiated for each translation unit outside of the HPX core
    // library, forcing to resolve the variable HPX_CHECK_VERSION.
    namespace
    {
        // Note: this function is never executed.
        char const* check_hpx_version()
        {
            char const* versions[] = {
                hpx::HPX_CHECK_VERSION, hpx::HPX_CHECK_BOOST_VERSION
            };
            return versions[0];
        }
    }
#endif

#endif

