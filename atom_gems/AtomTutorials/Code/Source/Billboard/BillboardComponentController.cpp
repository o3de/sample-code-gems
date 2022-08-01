/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <Atom/RPI.Public/AuxGeom/AuxGeomDraw.h>
#include <Atom/RPI.Public/AuxGeom/AuxGeomFeatureProcessorInterface.h>
#include <Atom/RPI.Public/Scene.h>
#include <Atom/Utils/Utils.h>
#include <Atom/RPI.Public/DynamicDraw/DynamicDrawInterface.h>
#include <AtomTutorials/Billboard/BillboardComponentConstants.h>
#include <AzCore/Component/TransformBus.h>
#include <AzCore/RTTI/BehaviorContext.h>
#include <AzCore/Serialization/SerializeContext.h>
#include <Billboard/BillboardComponentController.h>

namespace AZ
{
    namespace Render
    {
        void BillboardComponentController::Reflect(ReflectContext* context)
        {
            BillboardComponentConfig::Reflect(context);

            if (SerializeContext* serializeContext = azrtti_cast<SerializeContext*>(context))
            {
                serializeContext->Class<BillboardComponentController>()
                    ->Version(0)
                    ->Field("Configuration", &BillboardComponentController::m_configuration)
                    ;
            }

            if (AZ::BehaviorContext* behaviorContext = azrtti_cast<AZ::BehaviorContext*>(context))
            {
                behaviorContext->EBus<BillboardComponentRequestBus>("BillboardComponentRequestBus")
                    ->Event("GetSize", &BillboardComponentRequestBus::Events::GetSize)
                    ->Event("SetSize", &BillboardComponentRequestBus::Events::SetSize)
                    ->Event("GetAxisColor", &BillboardComponentRequestBus::Events::GetAxisColor)
                    ->Event("SetAxisColor", &BillboardComponentRequestBus::Events::SetAxisColor)
                    ->Event("GetPrimaryColor", &BillboardComponentRequestBus::Events::GetPrimaryColor)
                    ->Event("SetPrimaryColor", &BillboardComponentRequestBus::Events::SetPrimaryColor)
                    ->Event("GetPrimarySpacing", &BillboardComponentRequestBus::Events::GetPrimarySpacing)
                    ->Event("SetPrimarySpacing", &BillboardComponentRequestBus::Events::SetPrimarySpacing)
                    ->Event("GetSecondaryColor", &BillboardComponentRequestBus::Events::GetSecondaryColor)
                    ->Event("SetSecondaryColor", &BillboardComponentRequestBus::Events::SetSecondaryColor)
                    ->Event("GetSecondarySpacing", &BillboardComponentRequestBus::Events::GetSecondarySpacing)
                    ->Event("SetSecondarySpacing", &BillboardComponentRequestBus::Events::SetSecondarySpacing)
                    ->VirtualProperty("Size", "GetSize", "SetSize")
                    ->VirtualProperty("AxisColor", "GetAxisColor", "SetAxisColor")
                    ->VirtualProperty("PrimaryColor", "GetPrimaryColor", "SetPrimaryColor")
                    ->VirtualProperty("PrimarySpacing", "GetPrimarySpacing", "SetPrimarySpacing")
                    ->VirtualProperty("SecondaryColor", "GetSecondaryColor", "SetSecondaryColor")
                    ->VirtualProperty("SecondarySpacing", "GetSecondarySpacing", "SetSecondarySpacing")
                    ;
            }
        }

        void BillboardComponentController::GetProvidedServices(ComponentDescriptor::DependencyArrayType& provided)
        {
            provided.push_back(AZ_CRC_CE("BillboardService"));
        }

        void BillboardComponentController::GetIncompatibleServices(ComponentDescriptor::DependencyArrayType& incompatible)
        {
            incompatible.push_back(AZ_CRC_CE("BillboardService"));
        }

        BillboardComponentController::BillboardComponentController(const BillboardComponentConfig& config)
            : m_configuration(config)
        {
        }

        void BillboardComponentController::Activate(EntityId entityId)
        {
            m_entityId = entityId;
            m_dirty = true;

            RPI::Scene* scene = RPI::Scene::GetSceneForEntityId(m_entityId);
            if (scene)
            {
                AZ::RPI::SceneNotificationBus::Handler::BusConnect(scene->GetId());
            }

            BillboardComponentRequestBus::Handler::BusConnect(m_entityId);
            AZ::TransformNotificationBus::Handler::BusConnect(m_entityId);

            // Dynamic Draw
            AZStd::vector<RPI::DynamicDrawContext::VertexChannel> vertexChannels =
            {
                { "POSITION", RHI::Format::R32G32B32_FLOAT },
                { "COLOR", RHI::Format::R32G32B32A32_FLOAT }
            };

            m_dynamicDraw = RPI::DynamicDrawInterface::Get()->CreateDynamicDrawContext();
            m_dynamicDraw->InitVertexFormat(vertexChannels);
            m_dynamicDraw->AddDrawStateOptions(RPI::DynamicDrawContext::DrawStateOptions::BlendMode | RPI::DynamicDrawContext::DrawStateOptions::PrimitiveType
                | RPI::DynamicDrawContext::DrawStateOptions::DepthState | RPI::DynamicDrawContext::DrawStateOptions::FaceCullMode);
            m_dynamicDraw->SetOutputScope(scene);
            m_dynamicDraw->EndInit();

            AZ_Assert(m_dynamicDraw->IsVertexSizeValid(sizeof(ExampleVertex)), "Invalid vertex format");
        }

        void BillboardComponentController::Deactivate()
        {
            AZ::TransformNotificationBus::Handler::BusDisconnect();
            BillboardComponentRequestBus::Handler::BusDisconnect();
            AZ::RPI::SceneNotificationBus::Handler::BusDisconnect();

            m_entityId = EntityId(EntityId::InvalidEntityId);
        }

        void BillboardComponentController::SetConfiguration(const BillboardComponentConfig& config)
        {
            m_configuration = config;
            m_dirty = true;
        }

        const BillboardComponentConfig& BillboardComponentController::GetConfiguration() const
        {
            return m_configuration;
        }

        void BillboardComponentController::SetSize(float size)
        {
            m_configuration.m_gridSize = AZStd::clamp(size, MinGridSize, MaxGridSize);
            m_dirty = true;
        }

        float BillboardComponentController::GetSize() const
        {
            return m_configuration.m_gridSize;
        }

        void BillboardComponentController::SetPrimarySpacing(float spacing)
        {
            m_configuration.m_primarySpacing = AZStd::max(spacing, MinSpacing);
            m_dirty = true;
        }

        float BillboardComponentController::GetPrimarySpacing() const
        {
            return m_configuration.m_primarySpacing;
        }

        void BillboardComponentController::SetSecondarySpacing(float spacing)
        {
            m_configuration.m_secondarySpacing = AZStd::max(spacing, MinSpacing);
            m_dirty = true;
        }

        float BillboardComponentController::GetSecondarySpacing() const
        {
            return m_configuration.m_secondarySpacing;
        }

        void BillboardComponentController::SetAxisColor(const AZ::Color& color)
        {
            m_configuration.m_axisColor = color;
        }

        AZ::Color BillboardComponentController::GetAxisColor() const
        {
            return m_configuration.m_axisColor;
        }

        void BillboardComponentController::SetPrimaryColor(const AZ::Color& color)
        {
            m_configuration.m_primaryColor = color;
        }

        AZ::Color BillboardComponentController::GetPrimaryColor() const
        {
            return m_configuration.m_primaryColor;
        }

        void BillboardComponentController::SetSecondaryColor(const AZ::Color& color)
        {
            m_configuration.m_secondaryColor = color;
        }

        AZ::Color BillboardComponentController::GetSecondaryColor() const
        {
            return m_configuration.m_secondaryColor;
        }

        void BillboardComponentController::OnBeginPrepareRender()
        {
            if (m_configuration.m_gridSize <= 0.0f)
            {
                return;
            }

            auto* auxGeomFP = AZ::RPI::Scene::GetFeatureProcessorForEntity<AZ::RPI::AuxGeomFeatureProcessorInterface>(m_entityId);
            if (!auxGeomFP)
            {
                return;
            }

            if (auto auxGeom = auxGeomFP->GetDrawQueue())
            {
                BuildGrid();

                /*
                AZ::RPI::AuxGeomDraw::AuxGeomDynamicDrawArguments drawArgs;
                drawArgs.m_verts = m_axisGridPoints.data();
                drawArgs.m_vertCount = aznumeric_cast<uint32_t>(m_axisGridPoints.size());
                drawArgs.m_colors = &m_configuration.m_axisColor;
                auxGeom->DrawLines(drawArgs);*/
            }
        }

        void BillboardComponentController::OnTransformChanged(const Transform& local, const Transform& world)
        {
            AZ_UNUSED(local);
            AZ_UNUSED(world);
            m_dirty = true;
        }

        void BillboardComponentController::BuildGrid()
        {
            EntityId cam;
            Camera::CameraSystemRequestBus::BroadcastResult(cam, &Camera::CameraSystemRequests::GetActiveCamera);

            AZ::Vector3 cameraWorldPosition;
            AZ::TransformBus::EventResult(cameraWorldPosition, cam, &AZ::TransformBus::Events::GetWorldTranslation);

            AZ::Vector3 myPosition;
            AZ::TransformBus::EventResult(myPosition, m_entityId, &AZ::TransformBus::Events::GetWorldTranslation);

            const float halfLength = m_configuration.m_gridSize / 2.0f;

            // From camera POV, the forward axis is Z positive, even though O3DE's default is Y positive for forward axis.
            AZ::Matrix3x4 viewMatrix = AZ::Matrix3x4::CreateFromTransform(AZ::Transform::CreateLookAt(cameraWorldPosition, myPosition, AZ::Transform::Axis::ZPositive));
            AZ::Vector3 cameraRight = AZ::Vector3(viewMatrix(0, 0), viewMatrix(1, 0), viewMatrix(2, 0));
            AZ::Vector3 cameraUp = AZ::Vector3(viewMatrix(0, 1), viewMatrix(1, 1), viewMatrix(2, 1));

            AZ::Vector3 p0_world = myPosition + cameraRight * halfLength + cameraUp * halfLength; // top right
            AZ::Vector3 p1_world = myPosition + cameraRight * halfLength - cameraUp * halfLength; // bottom right
            AZ::Vector3 p2_world = myPosition - cameraRight * halfLength - cameraUp * halfLength; // bottom left
            AZ::Vector3 p3_world = myPosition - cameraRight * halfLength + cameraUp * halfLength; // top left

            /*
            // Make lines
            m_axisGridPoints.clear();
            m_axisGridPoints.reserve(8);
            // Right
            m_axisGridPoints.push_back(p0_world);
            m_axisGridPoints.push_back(p1_world);
            // Bottom
            m_axisGridPoints.push_back(p1_world);
            m_axisGridPoints.push_back(p2_world);
            // Left
            m_axisGridPoints.push_back(p2_world);
            m_axisGridPoints.push_back(p3_world);
            // Top
            m_axisGridPoints.push_back(p3_world);
            m_axisGridPoints.push_back(p0_world);*/

            float positions[4][3] =
            {
                {p0_world.GetX(), p0_world.GetY(), p0_world.GetZ()},
                {p1_world.GetX(), p1_world.GetY(), p1_world.GetZ()},
                {p2_world.GetX(), p2_world.GetY(), p2_world.GetZ()},
                {p3_world.GetX(), p3_world.GetY(), p3_world.GetZ()},
            };

            float colors[4][4] = { { 1, 0, 0, 0.5f }, { 0, 1, 0, 0.5f }, { 0, 0, 1, 0.5f }, {1, 1, 0, 0.5f} };

            ExampleVertex vertices[8] = {
                ExampleVertex{positions[0], colors[0]},
                ExampleVertex{positions[1], colors[0]},
                ExampleVertex{positions[1], colors[0]},
                ExampleVertex{positions[2], colors[0]},
                ExampleVertex{positions[2], colors[0]},
                ExampleVertex{positions[3], colors[0]},
                ExampleVertex{positions[3], colors[0]},
                ExampleVertex{positions[0], colors[0]}
            };

            RHI::DepthState depthState;
            depthState.m_enable = true;
            depthState.m_writeMask = RHI::DepthWriteMask::All;
            depthState.m_func = RHI::ComparisonFunc::GreaterEqual;
            m_dynamicDraw->SetDepthState(depthState);
            // Disable blend
            RHI::TargetBlendState blendState;
            blendState.m_enable = false;
            m_dynamicDraw->SetTarget0BlendState(blendState);

            Data::Instance<RPI::ShaderResourceGroup> drawSrg = m_dynamicDraw->NewDrawSrg();
            drawSrg->Compile();

            m_dynamicDraw->SetCullMode(RHI::CullMode::None);
            m_dynamicDraw->DrawLinear(vertices, 8, drawSrg);

            BillboardComponentNotificationBus::Event(m_entityId, &BillboardComponentNotificationBus::Events::OnGridChanged);
        }
    } // namespace Render
} // namespace AZ
