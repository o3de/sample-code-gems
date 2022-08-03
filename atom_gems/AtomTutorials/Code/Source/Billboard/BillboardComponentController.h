/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#include <Atom/RPI.Public/SceneBus.h>
#include <AtomTutorials/Billboard/BillboardComponentBus.h>
#include <AtomTutorials/Billboard/BillboardComponentConfig.h>
#include <AzCore/Component/Component.h>
#include <AzCore/Component/TransformBus.h>
#include <AzFramework/Components/CameraBus.h>
#include <Atom/Feature/Mesh/MeshFeatureProcessor.h>
#include <Atom/RPI.Reflect/Asset/AssetUtils.h>
#include <AzFramework/Render/GeometryIntersectionBus.h>
#include <AzFramework/Visibility/BoundsBus.h>

namespace AZ
{
    namespace Render
    {
        //! Controls behavior and rendering of a wireframe Billboard
        class BillboardComponentController final
            : public BillboardComponentRequestBus::Handler
            , public AZ::TransformNotificationBus::Handler
            , public AZ::RPI::SceneNotificationBus::Handler
            , public AzFramework::BoundsRequestBus::Handler
            , public AzFramework::RenderGeometry::IntersectionRequestBus::Handler
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

            void Activate(const AZ::EntityComponentIdPair& entityComponentIdPair);
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

            // BoundsRequestBus and MeshComponentRequestBus overrides ...
            AZ::Aabb GetWorldBounds() override;
            AZ::Aabb GetLocalBounds() override;

            // IntersectionRequestBus overrides ...
            AzFramework::RenderGeometry::RayResult RenderGeometryIntersect(const AzFramework::RenderGeometry::RayRequest& ray) override;

            void BuildGrid();

            BillboardComponentConfig m_configuration;
            AZStd::vector<AZ::Vector3> m_axisGridPoints;
            bool m_dirty = true; // must be set to true for any configuration change that rebuilds the grid

            //! Cached bus to use to notify RenderGeometry::Intersector the entity/component has changed.
            AzFramework::RenderGeometry::IntersectionNotificationBus::BusPtr m_intersectionNotificationBus;

            AZ::Render::MeshFeatureProcessorInterface* m_meshFeatureProcessor;
            TransformInterface* m_transformInterface = nullptr;

            AZ::Data::Asset<AZ::RPI::ModelAsset> m_modelAsset;
            AZ::Render::MeshFeatureProcessorInterface::MeshHandle m_meshHandle;
            AZ::Data::Instance<AZ::RPI::Material> m_material;

            AZ::EntityComponentIdPair m_entityComponentIdPair;
        };
    } // namespace Render
} // namespace AZ
