/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Utils/Utils.h>

#include <AzToolsFramework/API/EntityCompositionRequestBus.h>
#include <AzToolsFramework/Component/EditorComponentAPIBus.h>
#include <AzToolsFramework/Entity/EditorEntityAPIBus.h>

#include <QCheckBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QIcon>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

#include <ShapeExampleWidget.h>

namespace ShapeExample
{
    ShapeExampleWidget::ShapeExampleWidget(QWidget* parent)
        : QWidget(parent)
    {
        setWindowTitle(QObject::tr("ShapeExample"));

        QVBoxLayout* mainLayout = new QVBoxLayout(this);

        QWidget* entityNameWidget = new QWidget(this);
        QFormLayout* formLayout = new QFormLayout();

        m_nameInput = new QLineEdit(this);
        m_nameInput->setPlaceholderText(QObject::tr("Set custom Entity name here..."));
        m_nameInput->setClearButtonEnabled(true);

        m_addShapeNameSuffix = new QCheckBox(this);
        m_addShapeNameSuffix->setDisabled(true);

        // Example of listening to signals using a lambda as the handler
        // In this case, we will only enable the checkbox if the user has typed in
        // a custom name for the entity that will be created
        QObject::connect(m_nameInput, &QLineEdit::textChanged, [this](const QString& text) {
            m_addShapeNameSuffix->setEnabled(!text.isEmpty());
        });

        formLayout->addRow(QObject::tr("Entity name"), m_nameInput);
        formLayout->addRow(QObject::tr("Add shape name suffix"), m_addShapeNameSuffix);

        entityNameWidget->setLayout(formLayout);
        mainLayout->addWidget(entityNameWidget);

        QWidget* shapeButtons = new QWidget(this);
        QGridLayout* gridLayout = new QGridLayout();

        // We want to find every component that provides the ShapeService
        AzToolsFramework::EntityCompositionRequests::ComponentServicesList providedServices;
        providedServices.push_back(AZ_CRC_CE("ShapeService"));

        AZ::ComponentTypeList typeIds;
        AzToolsFramework::EditorComponentAPIBus::BroadcastResult(
            typeIds,
            &AzToolsFramework::EditorComponentAPIRequests::FindComponentTypeIdsByService,
            providedServices,
            AzToolsFramework::EntityCompositionRequests::ComponentServicesList()
        );

        // After getting all of the types for the shape components, query to find
        // their names so we can show them
        AZStd::vector<AZStd::string> componentNames;
        AzToolsFramework::EditorComponentAPIBus::BroadcastResult(
            componentNames,
            &AzToolsFramework::EditorComponentAPIRequests::FindComponentTypeNames,
            typeIds
        );

        AZ_Assert(typeIds.size() == componentNames.size(), "Unable to find component names for all types");

        const int maxColumnCount = 3;
        int row = 0, column = 0;

        for (int i = 0; i < componentNames.size(); ++i)
        {
            AZStd::string name = componentNames[i];
            AZ::TypeId typeId = typeIds[i];

            // Find the icon registered for this component by its type id
            AZStd::string editorIconPath;
            AzToolsFramework::EditorRequestBus::BroadcastResult(editorIconPath, &AzToolsFramework::EditorRequestBus::Events::GetComponentEditorIcon, typeId, nullptr);
            QString iconPath = QString::fromUtf8(editorIconPath.c_str());

            // Create a button with the shape components name and icon
            QPushButton* shapeButton = new QPushButton(QIcon(iconPath), QString::fromUtf8(name.c_str()), this);
            shapeButton->setMinimumHeight(40);

            // Store the type id as a property on the button so we can use it later when
            // handling the button press
            shapeButton->setProperty("typeId", typeId.ToString<AZStd::string>().c_str());

            // Example of listening to signals using a slot as the handler
            QObject::connect(shapeButton, &QPushButton::clicked, this, &ShapeExampleWidget::OnShapeButtonClicked);

            // Place our shape button in the grid layout
            gridLayout->addWidget(shapeButton, row, column);

            ++column;
            if (column >= maxColumnCount)
            {
                column = 0;
                ++row;
            }
        }

        shapeButtons->setLayout(gridLayout);
        mainLayout->addWidget(shapeButtons);

        setLayout(mainLayout);
    }

    void ShapeExampleWidget::OnShapeButtonClicked()
    {
        using namespace AzToolsFramework;

        // We know the signal connected to this slot has come from one of our shape QPushButton's
        auto shapeButton = qobject_cast<QPushButton*>(sender());
        if (!shapeButton)
        {
            return;
        }

        // Retrieve the type id for the shape component we want to add to this new entity
        AZ::TypeId typeId = AZ::Uuid(shapeButton->property("typeId").toString().toUtf8().constData());

        // Create a new entity
        AZ::EntityId newEntityId;
        EditorRequestBus::BroadcastResult(newEntityId, &EditorRequests::CreateNewEntity, AZ::EntityId());

        // If the user has input a custom name and/or checked the box to append the
        // component suffix, then set the new entity name here
        if (!m_nameInput->text().isEmpty())
        {
            QString entityName = m_nameInput->text();

            if (m_addShapeNameSuffix->isChecked())
            {
                AZStd::vector<AZStd::string> componentNames;
                AzToolsFramework::EditorComponentAPIBus::BroadcastResult(
                    componentNames,
                    &AzToolsFramework::EditorComponentAPIRequests::FindComponentTypeNames,
                    AZ::ComponentTypeList{ typeId }
                );

                AZ_Assert(componentNames.size() == 1, "Unable to find component name");

                QString shapeName(componentNames[0].c_str());
                shapeName.replace(" ", "");

                entityName = QString("%1_%2").arg(entityName).arg(shapeName);
            }

            // Set new name on the entity we created
            EditorEntityAPIBus::Event(newEntityId, &EditorEntityAPIRequests::SetName, entityName.toUtf8().constData());
        }

        // Add the corresponding shape component for the button we pressed to the newly created entity
        EntityCompositionRequestBus::Broadcast(&EntityCompositionRequests::AddComponentsToEntities, EntityIdList{ newEntityId }, AZ::ComponentTypeList{ typeId });
    }
}

#include <moc_ShapeExampleWidget.cpp>
