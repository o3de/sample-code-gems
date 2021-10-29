"""
Copyright (c) Contributors to the Open 3D Engine Project.
For complete copyright and license terms please see the LICENSE at the root of this distribution.

SPDX-License-Identifier: Apache-2.0 OR MIT
"""
# -------------------------------------------------------------------------
"""PyShapeExample\\editor\\scripts\\PyShapeExample_dialog.py
Generated from O3DE PythonToolGem Template"""

import azlmbr.bus as bus
import azlmbr.editor as editor
import azlmbr.entity as entity
import azlmbr.math as math
from PySide2.QtGui import QIcon
from PySide2.QtWidgets import QCheckBox, QDialog, QFormLayout, QGridLayout, QLineEdit, QPushButton, QVBoxLayout, QWidget

class PyShapeExampleDialog(QDialog):
    def on_name_input_text_changed(self, text):
        # We will only enable the checkbox if the user has typed in
        # a custom name for the entity that will be created
        self.add_shape_name_suffix.setEnabled(len(text))

    def create_entity_with_shape_component(self, type_id):
        # Create a new entity
        new_entity_id = editor.ToolsApplicationRequestBus(bus.Broadcast, 'CreateNewEntity', entity.EntityId())

        # If the user has input a custom name and/or checked the box to append the
        # component suffix, then set the new entity name here
        if self.name_input.text():
            entity_name = self.name_input.text()

            if self.add_shape_name_suffix.isChecked():
                component_names = editor.EditorComponentAPIBus(bus.Broadcast, 'FindComponentTypeNames', [type_id])

                shape_name = component_names[0]
                shape_name = shape_name.replace(" ", "")

                entity_name = f"{entity_name}_{shape_name}"

            # Set new name on the entity we created
            editor.EditorEntityAPIBus(bus.Event, "SetName", new_entity_id, entity_name)

        # Add the corresponding shape component for the button we pressed to the newly created entity
        editor.EditorComponentAPIBus(bus.Broadcast, "AddComponentsOfType", new_entity_id, [type_id])

    def __init__(self, parent=None):
        super(PyShapeExampleDialog, self).__init__(parent)

        main_layout = QVBoxLayout()

        entity_name_widget = QWidget(self)
        form_layout = QFormLayout()

        self.name_input = QLineEdit(self)
        self.name_input.setPlaceholderText("Set custom Entity name here...")
        self.name_input.setClearButtonEnabled(True)

        self.add_shape_name_suffix = QCheckBox(self)
        self.add_shape_name_suffix.setDisabled(True)

        # Example of listening to signals using a slot as the handler
        self.name_input.textChanged.connect(self.on_name_input_text_changed)

        form_layout.addRow("Entity name", self.name_input)
        form_layout.addRow("Add shape name suffix", self.add_shape_name_suffix)

        entity_name_widget.setLayout(form_layout)
        main_layout.addWidget(entity_name_widget)

        shape_buttons = QWidget(self)
        grid_layout = QGridLayout()

        # We want to find every component that provides the ShapeService
        shape_service = math.Crc32_CreateCrc32("ShapeService")
        provided_services = [shape_service.value]
        type_ids = editor.EditorComponentAPIBus(bus.Broadcast, 'FindComponentTypeIdsByService', provided_services, [])

        # After getting all of the types for the shape components, query to find
        # their names so we can show them
        component_names = editor.EditorComponentAPIBus(bus.Broadcast, 'FindComponentTypeNames', type_ids)

        max_column_count = 3
        for i, name in enumerate(component_names):
            type_id = type_ids[i]

            # Find the icon registered for this component by its type id
            icon_path = editor.EditorRequestBus(bus.Broadcast, 'GetComponentTypeEditorIcon', type_id)

            # Create a button with the shape components name and icon
            shape_button = QPushButton(QIcon(icon_path), name, self)
            shape_button.setMinimumHeight(40)

            # Example of listening to signals using a lambda as the handler
            #shape_button.clicked.connect(lambda type_id: self.create_entity_with_shape_component(type_id))
            shape_button.clicked.connect(lambda checked=False, type_id=type_id: self.create_entity_with_shape_component(type_id))

            # Place our shape button in the grid layout
            row = i / max_column_count
            column = i % max_column_count
            grid_layout.addWidget(shape_button, row, column)

        shape_buttons.setLayout(grid_layout)
        main_layout.addWidget(shape_buttons)

        # Add stretch at bottom of the layout to fill any expanded space larger than what is needed,
        # so that if our tool is resized large our content will stay together
        main_layout.addStretch()

        self.setLayout(main_layout)
