#
# Copyright (c) Contributors to the Open 3D Engine Project.
# For complete copyright and license terms please see the LICENSE at the root of this distribution.
#
# SPDX-License-Identifier: Apache-2.0 OR MIT
#
#

# Currently we are in the Code folder: ${CMAKE_CURRENT_LIST_DIR}
# Get the platform specific folder ${pal_dir} for the current folder: ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME}
# Note: o3de_pal_dir will take care of the details for us, as this may be a restricted platform
#       in which case it will see if that platform is present here or in the restricted folder.
#       i.e. It could here in our gem : Gems/${Name}/Code/Platform/<platorm_name>  or
#            <restricted_folder>/<platform_name>/Gems/${Name}/Code
o3de_pal_dir(pal_dir ${CMAKE_CURRENT_LIST_DIR}/Platform/${PAL_PLATFORM_NAME} "${gem_restricted_path}" "${gem_path}" "${gem_parent_relative_path}")

# Now that we have the platform abstraction layer (PAL) folder for this folder, thats where we will find the
# traits for this platform. Traits for a platform are defines for things like whether or not something in this gem
# is supported by this platform.
include(${pal_dir}/PAL_${PAL_PLATFORM_NAME_LOWERCASE}.cmake)

# Add the ShapeExample.Static target
# Note: We include the common files and the platform specific files which are set in shapeexample_common_files.cmake
# and in ${pal_dir}/shapeexample_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
ly_add_target(
    NAME ShapeExample.Static STATIC
    NAMESPACE Gem
    FILES_CMAKE
        shapeexample_files.cmake
        ${pal_dir}/shapeexample_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
        PRIVATE
            Source
    BUILD_DEPENDENCIES
        PUBLIC
            AZ::AzCore
            AZ::AzFramework
)

# Here add ShapeExample target, it depends on the ShapeExample.Static
ly_add_target(
    NAME ShapeExample ${PAL_TRAIT_MONOLITHIC_DRIVEN_MODULE_TYPE}
    NAMESPACE Gem
    FILES_CMAKE
        shapeexample_shared_files.cmake
        ${pal_dir}/shapeexample_shared_${PAL_PLATFORM_NAME_LOWERCASE}_files.cmake
    INCLUDE_DIRECTORIES
        PUBLIC
            Include
        PRIVATE
            Source
    BUILD_DEPENDENCIES
        PRIVATE
            Gem::ShapeExample.Static
)

# By default, we will specify that the above target ShapeExample would be used by
# Client and Server type targets when this gem is enabled.  If you don't want it
# active in Clients or Servers by default, delete one of both of the following lines:
ly_create_alias(NAME ShapeExample.Clients NAMESPACE Gem TARGETS Gem::ShapeExample)
ly_create_alias(NAME ShapeExample.Servers NAMESPACE Gem TARGETS Gem::ShapeExample)

# If we are on a host platform, we want to add the host tools targets like the ShapeExample.Editor target which
# will also depend on ShapeExample.Static
if(PAL_TRAIT_BUILD_HOST_TOOLS)
    ly_add_target(
        NAME ShapeExample.Editor.Static STATIC
        NAMESPACE Gem
        AUTOMOC
        AUTORCC
        FILES_CMAKE
            shapeexample_editor_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                Source
            PUBLIC
                Include
        BUILD_DEPENDENCIES
            PUBLIC
                AZ::AzToolsFramework
                Gem::ShapeExample.Static
    )

    ly_add_target(
        NAME ShapeExample.Editor GEM_MODULE
        NAMESPACE Gem
        AUTOMOC
        FILES_CMAKE
            shapeexample_editor_shared_files.cmake
        INCLUDE_DIRECTORIES
            PRIVATE
                Source
            PUBLIC
                Include
        BUILD_DEPENDENCIES
            PUBLIC
                Gem::ShapeExample.Editor.Static
    )

    # By default, we will specify that the above target ShapeExample would be used by
    # Tool and Builder type targets when this gem is enabled.  If you don't want it
    # active in Tools or Builders by default, delete one of both of the following lines:
    ly_create_alias(NAME ShapeExample.Tools    NAMESPACE Gem TARGETS Gem::ShapeExample.Editor)
    ly_create_alias(NAME ShapeExample.Builders NAMESPACE Gem TARGETS Gem::ShapeExample.Editor)


endif()

################################################################################
# Tests
################################################################################
# See if globally, tests are supported
if(PAL_TRAIT_BUILD_TESTS_SUPPORTED)
    # We globally support tests, see if we support tests on this platform for ShapeExample.Static
    if(PAL_TRAIT_SHAPEEXAMPLE_TEST_SUPPORTED)
        # We support ShapeExample.Tests on this platform, add ShapeExample.Tests target which depends on ShapeExample.Static
        ly_add_target(
            NAME ShapeExample.Tests ${PAL_TRAIT_TEST_TARGET_TYPE}
            NAMESPACE Gem
            FILES_CMAKE
                shapeexample_files.cmake
                shapeexample_tests_files.cmake
            INCLUDE_DIRECTORIES
                PRIVATE
                    Tests
                    Source
            BUILD_DEPENDENCIES
                PRIVATE
                    AZ::AzTest
                    AZ::AzFramework
                    Gem::ShapeExample.Static
        )

        # Add ShapeExample.Tests to googletest
        ly_add_googletest(
            NAME Gem::ShapeExample.Tests
        )
    endif()

    # If we are a host platform we want to add tools test like editor tests here
    if(PAL_TRAIT_BUILD_HOST_TOOLS)
        # We are a host platform, see if Editor tests are supported on this platform
        if(PAL_TRAIT_SHAPEEXAMPLE_EDITOR_TEST_SUPPORTED)
            # We support ShapeExample.Editor.Tests on this platform, add ShapeExample.Editor.Tests target which depends on ShapeExample.Editor
            ly_add_target(
                NAME ShapeExample.Editor.Tests ${PAL_TRAIT_TEST_TARGET_TYPE}
                NAMESPACE Gem
                FILES_CMAKE
                    shapeexample_editor_tests_files.cmake
                INCLUDE_DIRECTORIES
                    PRIVATE
                        Tests
                        Source
                BUILD_DEPENDENCIES
                    PRIVATE
                        AZ::AzTest
                        Gem::ShapeExample.Editor
            )

            # Add ShapeExample.Editor.Tests to googletest
            ly_add_googletest(
                NAME Gem::ShapeExample.Editor.Tests
            )
        endif()
    endif()
endif()
