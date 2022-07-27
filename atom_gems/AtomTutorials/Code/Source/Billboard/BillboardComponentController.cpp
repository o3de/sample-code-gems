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

                AZ::RPI::AuxGeomDraw::AuxGeomDynamicDrawArguments drawArgs;
                drawArgs.m_verts = m_secondaryGridPoints.data();
                drawArgs.m_vertCount = aznumeric_cast<uint32_t>(m_secondaryGridPoints.size());
                drawArgs.m_colors = &m_configuration.m_secondaryColor;
                drawArgs.m_colorCount = 1;
                auxGeom->DrawLines(drawArgs);

                drawArgs.m_verts = m_primaryGridPoints.data();
                drawArgs.m_vertCount = aznumeric_cast<uint32_t>(m_primaryGridPoints.size());
                drawArgs.m_colors = &m_configuration.m_primaryColor;
                auxGeom->DrawLines(drawArgs);

                drawArgs.m_verts = m_axisGridPoints.data();
                drawArgs.m_vertCount = aznumeric_cast<uint32_t>(m_axisGridPoints.size());
                drawArgs.m_colors = &m_configuration.m_axisColor;
                auxGeom->DrawLines(drawArgs);
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
            if (m_dirty)
            {
                m_dirty = false;

                AZ::Transform transform;
                AZ::TransformBus::EventResult(transform, m_entityId, &AZ::TransformBus::Events::GetWorldTM);

                const float halfLength = m_configuration.m_gridSize / 2.0f;

                m_axisGridPoints.clear();
                m_axisGridPoints.reserve(4);
                m_axisGridPoints.push_back(transform.TransformPoint(AZ::Vector3(-halfLength, 0, 0.0f)));
                m_axisGridPoints.push_back(transform.TransformPoint(AZ::Vector3(halfLength, 0, 0.0f)));
                m_axisGridPoints.push_back(transform.TransformPoint(AZ::Vector3(0, -halfLength, 0.0f)));
                m_axisGridPoints.push_back(transform.TransformPoint(AZ::Vector3(0, halfLength, 0.0f)));

                m_primaryGridPoints.clear();
                m_primaryGridPoints.reserve(aznumeric_cast<size_t>(4.0f * m_configuration.m_gridSize / m_configuration.m_primarySpacing));
                for (float position = m_configuration.m_primarySpacing; position <= halfLength;
                     position += m_configuration.m_primarySpacing)
                {
                    m_primaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(-halfLength, -position, 0.0f)));
                    m_primaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(halfLength, -position, 0.0f)));
                    m_primaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(-halfLength, position, 0.0f)));
                    m_primaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(halfLength, position, 0.0f)));
                    m_primaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(-position, -halfLength, 0.0f)));
                    m_primaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(-position, halfLength, 0.0f)));
                    m_primaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(position, -halfLength, 0.0f)));
                    m_primaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(position, halfLength, 0.0f)));
                }

                m_secondaryGridPoints.clear();
                m_secondaryGridPoints.reserve(aznumeric_cast<size_t>(4.0f * m_configuration.m_gridSize / m_configuration.m_secondarySpacing));
                for (float position = m_configuration.m_secondarySpacing; position <= halfLength;
                     position += m_configuration.m_secondarySpacing)
                {
                    m_secondaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(-halfLength, -position, 0.0f)));
                    m_secondaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(halfLength, -position, 0.0f)));
                    m_secondaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(-halfLength, position, 0.0f)));
                    m_secondaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(halfLength, position, 0.0f)));
                    m_secondaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(-position, -halfLength, 0.0f)));
                    m_secondaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(-position, halfLength, 0.0f)));
                    m_secondaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(position, -halfLength, 0.0f)));
                    m_secondaryGridPoints.push_back(transform.TransformPoint(AZ::Vector3(position, halfLength, 0.0f)));
                }

                BillboardComponentNotificationBus::Event(m_entityId, &BillboardComponentNotificationBus::Events::OnGridChanged);
            }
        }
    } // namespace Render
} // namespace AZ
