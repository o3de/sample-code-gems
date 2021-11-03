/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#include <AzCore/Component/TransformBus.h>
#include <AzCore/Utils/Utils.h>

#include <AzToolsFramework/API/EntityCompositionRequestBus.h>
#include <AzToolsFramework/Component/EditorComponentAPIBus.h>
#include <AzToolsFramework/Entity/EditorEntityAPIBus.h>

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleValidator>
#include <QFormLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
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
        mainLayout->setSpacing(20);

        // Introduction text explaining the example
        QLabel* introText = new QLabel(QObject::tr("Welcome to the Python Shape Example tool. This tool demonstrates an example of creating an entity with a shape component in the editor. It also has other functional samples for you to play with."), this);
        introText->setWordWrap(true);
        mainLayout->addWidget(introText);

        // Show link to the actual tutorial for this example
        QLabel* tutorialLink = new QLabel(QObject::tr("You can learn how to build this example on <a href=\"https://o3de.org/docs/learning-guide/tutorials/custom-tools/shape-example-cpp/\">O3DE Learn.</a>"), this);
        tutorialLink->setTextFormat(Qt::RichText);
        tutorialLink->setOpenExternalLinks(true);
        mainLayout->addWidget(tutorialLink);

        QGroupBox* entityNameWidget = new QGroupBox(QObject::tr("Name your entity (Line Edit)"), this);
        QFormLayout* formLayout = new QFormLayout();

        // Line edit to (optionally) rename the entity that gets created
        m_nameInput = new QLineEdit(this);
        m_nameInput->setPlaceholderText(QObject::tr("Optional"));
        m_nameInput->setClearButtonEnabled(true);

        // Check box used to toggle appending a suffix of the shape name when renaming the entity
        m_addShapeNameSuffix = new QCheckBox(this);
        m_addShapeNameSuffix->setDisabled(true);
        m_addShapeNameSuffix->setToolTip("e.g. Entity2_BoxShape");

        // Example of listening to signals using a slot as the handler
        QObject::connect(m_nameInput, &QLineEdit::textChanged, this, &ShapeExampleWidget::OnNameInputTextChanged);

        formLayout->addRow(QObject::tr("Entity name"), m_nameInput);
        formLayout->addRow(QObject::tr("Add shape name suffix"), m_addShapeNameSuffix);

        entityNameWidget->setLayout(formLayout);
        mainLayout->addWidget(entityNameWidget);

        QGroupBox* comboBoxGroup = new QGroupBox("Choose your scale (Combo Box)", this);
        QVBoxLayout* comboBoxLayout = new QVBoxLayout();

        // Combo box with preset scales to be set on the created entity
        // The user can also manually enter a scale into the combo box,
        // which is validated to constrain to a double value between 0 and 100 with 3 decimals allowed
        QStringList scaleValues = {
            "1.0",
            "1.5",
            "2.0",
            "5.0",
            "10.0"
        };
        m_scaleInput = new QComboBox(this);
        m_scaleInput->setEditable(true);
        m_scaleInput->setValidator(new QDoubleValidator(0.0, 100.0, 3, this));
        m_scaleInput->addItems(scaleValues);
        comboBoxLayout->addWidget(m_scaleInput);

        comboBoxGroup->setLayout(comboBoxLayout);
        mainLayout->addWidget(comboBoxGroup);

        QGroupBox* shapeButtons = new QGroupBox(QObject::tr("Choose your shape (Button)"), this);
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
        for (int i = 0; i < componentNames.size(); ++i)
        {
            AZStd::string name = componentNames[i];
            AZ::TypeId typeId = typeIds[i];

            // Create a button with the shape components name
            QPushButton* shapeButton = new QPushButton(QString::fromUtf8(name.c_str()), this);

            // Example of listening to signals using a lambda as the handler
            QObject::connect(shapeButton, &QPushButton::clicked, this, [this, typeId]() {
                CreateEntityWithShapeComponent(typeId);
            });

            // Place our shape button in the grid layout
            int row = i / maxColumnCount;
            int column = i % maxColumnCount;
            gridLayout->addWidget(shapeButton, row, column);
        }

        shapeButtons->setLayout(gridLayout);
        mainLayout->addWidget(shapeButtons);

        QLabel* warningLabel = new QLabel(QObject::tr("<i>(Make sure a level is loaded before choosing your shape)</i>"), this);
        warningLabel->setTextFormat(Qt::RichText);
        mainLayout->addWidget(warningLabel);

        // Add stretch at bottom of the layout to fill any expanded space larger than what is needed,
        // so that if our tool is resized large our content will stay together
        mainLayout->addStretch();

        setLayout(mainLayout);
    }

    void ShapeExampleWidget::CreateEntityWithShapeComponent(const AZ::TypeId& typeId)
    {
        using namespace AzToolsFramework;

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
                EditorComponentAPIBus::BroadcastResult(
                    componentNames,
                    &EditorComponentAPIRequests::FindComponentTypeNames,
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

        // Set the scale on our new entity
        bool validFloat = false;
        float scale = m_scaleInput->currentText().toFloat(&validFloat);
        if (validFloat)
        {
            AZ::TransformBus::Event(newEntityId, &AZ::TransformInterface::SetLocalUniformScale, scale);
        }

        // Add the corresponding shape component for the button we pressed to the newly created entity
        EditorComponentAPIBus::Broadcast(&EditorComponentAPIRequests::AddComponentsOfType, newEntityId, AZ::ComponentTypeList{ typeId });
    }

    void ShapeExampleWidget::OnNameInputTextChanged(const QString& text)
    {
        // We will only enable the checkbox if the user has typed in
        // a custom name for the entity that will be created
        m_addShapeNameSuffix->setEnabled(!text.isEmpty());
    }
}

#include <moc_ShapeExampleWidget.cpp>
