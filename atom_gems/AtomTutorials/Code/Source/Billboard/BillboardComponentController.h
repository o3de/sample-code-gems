/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Atom/RPI.Public/SceneBus.h>
#include <Atom/RPI.Public/DynamicDraw/DynamicDrawContext.h>
#include <AtomTutorials/Billboard/BillboardComponentBus.h>
#include <AtomTutorials/Billboard/BillboardComponentConfig.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TransformBus.h>
#include <AzFramework/Components/CameraBus.h>
#include <Atom/Utils/AssetCollectionAsyncLoader.h>

namespace AZ
{
    namespace Render
    {
        //! Controls behavior and rendering of a wireframe Billboard
        class BillboardComponentController final
            : public BillboardComponentRequestBus::Handler
            , public AZ::TransformNotificationBus::Handler
            , public AZ::RPI::SceneNotificationBus::Handler
        {
        public:
            friend class EditorBillboardComponent;

            AZ_CLASS_ALLOCATOR(BillboardComponentController, SystemAllocator, 0)
            AZ_RTTI(AZ::Render::BillboardComponentController, "{A0288BD7-B660-47B6-88D7-57E7C4F92FEK}");

            static void Reflect(ReflectContext* context);
            static void GetProvidedServices(ComponentDescriptor::DependencyArrayType& provided);
            static void GetIncompatibleServices(ComponentDescriptor::DependencyArrayType& incompatible);

            BillboardComponentController() = default;
            BillboardComponentController(const BillboardComponentConfig& config);

            void Activate(EntityId entityId);
            void Deactivate();
            void SetConfiguration(const BillboardComponentConfig& config);
            const BillboardComponentConfig& GetConfiguration() const;

            static constexpr float MinGridSize = 0.0f;
            static constexpr float MaxGridSize = 1000000.0f;
            static constexpr float MinSpacing = 0.01f;

        private:
            AZ_DISABLE_COPY(BillboardComponentController);

            struct ExampleVertex
            {
                ExampleVertex(float position[3], float color[4])
                {
                    memcpy(m_position, position, sizeof(float) * 3);
                    memcpy(m_color, color, sizeof(float) * 4);
                }
                float m_position[3];
                float m_color[4];
            };

            //! BillboardComponentRequestBus overrides...
            void SetSize(float size) override;
            float GetSize() const override;
            void SetPrimarySpacing(float spacing) override;
            float GetPrimarySpacing() const override;
            void SetSecondarySpacing(float spacing) override;
            float GetSecondarySpacing() const override;

            void SetAxisColor(const AZ::Color& color) override;
            AZ::Color GetAxisColor() const override;
            void SetPrimaryColor(const AZ::Color& color) override;
            AZ::Color GetPrimaryColor() const override;
            void SetSecondaryColor(const AZ::Color& color) override;
            AZ::Color GetSecondaryColor() const override;

            //! AZ::TransformNotificationBus::Handler overrides ...
            void OnTransformChanged(const Transform& local, const Transform& world) override;

            // AZ::RPI::SceneNotificationBus::Handler overrides ...
            void OnBeginPrepareRender() override;

            void BuildGrid();

            EntityId m_entityId;
            BillboardComponentConfig m_configuration;
            AZStd::vector<AZ::Vector3> m_axisGridPoints;
            bool m_dirty = true; // must be set to true for any configuration change that rebuilds the grid
            AZ::RHI::Ptr<AZ::RPI::DynamicDrawContext> m_dynamicDraw;

            //! Async asset load
            AZ::AssetCollectionAsyncLoader m_assetLoadManager;
        };
    } // namespace Render
} // namespace AZ
