/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <ShapeExampleModuleInterface.h>
#include <ShapeExampleSystemComponent.h>

namespace ShapeExample
{
    class ShapeExampleModule
        : public ShapeExampleModuleInterface
    {
    public:
        AZ_RTTI(ShapeExampleModule, "{c967d26f-d78b-4900-b9cc-a60ac8216d1a}", ShapeExampleModuleInterface);
        AZ_CLASS_ALLOCATOR(ShapeExampleModule, AZ::SystemAllocator, 0);
    };
}// namespace ShapeExample

AZ_DECLARE_MODULE_CLASS(Gem_ShapeExample, ShapeExample::ShapeExampleModule)
