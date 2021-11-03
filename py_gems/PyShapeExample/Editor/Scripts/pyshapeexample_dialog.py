"""
Copyright (c) Contributors to the Open 3D Engine Project.
For complete copyright and license terms please see the LICENSE at the root of this distribution.

SPDX-License-Identifier: Apache-2.0 OR MIT
"""
# -------------------------------------------------------------------------
"""PyShapeExample\\editor\\scripts\\PyShapeExample_dialog.py
Generated from O3DE PythonToolGem Template"""

import azlmbr.bus as bus
import azlmbr.components as components
import azlmbr.editor as editor
import azlmbr.entity as entity
import azlmbr.math as math

from PySide2.QtCore import Qt
from PySide2.QtGui import QDoubleValidator
from PySide2.QtWidgets import QCheckBox, QComboBox, QDialog, QFormLayout, QGridLayout, QGroupBox, QLabel, QLineEdit, QPushButton, QVBoxLayout, QWidget

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

        # Set the scale on our new entity
        try:
            scale_text = self.scale_combobox.currentText()
            components.TransformBus(bus.Event, "SetLocalUniformScale", new_entity_id, float(scale_text))
        except:
            pass

        # Add the corresponding shape component for the button we pressed to the newly created entity
        editor.EditorComponentAPIBus(bus.Broadcast, "AddComponentsOfType", new_entity_id, [type_id])

    def __init__(self, parent=None):
        super(PyShapeExampleDialog, self).__init__(parent)

        main_layout = QVBoxLayout()
        main_layout.setSpacing(20)

        # Introduction text explaining the example
        intro_text = QLabel("Welcome to the Python Shape Example tool. This tool demonstrates an example of creating an entity with a shape component in the editor. It also has other functional samples for you to play with.", self)
        intro_text.setWordWrap(True)
        main_layout.addWidget(intro_text)

        # Show link to the actual tutorial for this example
        tutorial_link = QLabel('You can learn how to build this example on <a href="https://o3de.org/docs/learning-guide/tutorials/custom-tools/shape-example-py/">O3DE Learn.</a>', self)
        tutorial_link.setTextFormat(Qt.RichText)
        tutorial_link.setOpenExternalLinks(True)
        main_layout.addWidget(tutorial_link)

        entity_name_widget = QGroupBox("Name your entity (Line Edit)", self)
        form_layout = QFormLayout()

        # Line edit to (optionally) rename the entity that gets created
        self.name_input = QLineEdit(self)
        self.name_input.setPlaceholderText("Optional")
        self.name_input.setClearButtonEnabled(True)

        # Check box used to toggle appending a suffix of the shape name when renaming the entity
        self.add_shape_name_suffix = QCheckBox(self)
        self.add_shape_name_suffix.setDisabled(True)
        self.add_shape_name_suffix.setToolTip("e.g. Entity2_BoxShape")

        # Example of listening to signals using a slot as the handler
        self.name_input.textChanged.connect(self.on_name_input_text_changed)

        form_layout.addRow("Entity name", self.name_input)
        form_layout.addRow("Add shape name suffix", self.add_shape_name_suffix)

        entity_name_widget.setLayout(form_layout)
        main_layout.addWidget(entity_name_widget)

        combobox_group = QGroupBox("Choose your scale (Combo Box)", self)
        combobox_layout = QVBoxLayout()

        # Combo box with preset scales to be set on the created entity
        # The user can also manually enter a scale into the combo box,
        # which is validated to constrain to a double value between 0 and 100 with 3 decimals allowed
        scale_values = [
            "1.0",
            "1.5",
            "2.0",
            "5.0",
            "10.0"
        ]
        self.scale_combobox = QComboBox(self)
        self.scale_combobox.setEditable(True)
        self.scale_combobox.setValidator(QDoubleValidator(0.0, 100.0, 3, self))
        self.scale_combobox.addItems(scale_values)
        combobox_layout.addWidget(self.scale_combobox)

        combobox_group.setLayout(combobox_layout)
        main_layout.addWidget(combobox_group)

        shape_buttons = QGroupBox("Choose your shape (Button)", self)
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

            # Create a button with the shape components name
            shape_button = QPushButton(name, self)

            # Example of listening to signals using a lambda as the handler
            shape_button.clicked.connect(lambda checked=False, type_id=type_id: self.create_entity_with_shape_component(type_id))

            # Place our shape button in the grid layout
            row = i / max_column_count
            column = i % max_column_count
            grid_layout.addWidget(shape_button, row, column)

        shape_buttons.setLayout(grid_layout)
        main_layout.addWidget(shape_buttons)

        warning_label = QLabel("<i>(Make sure a level is loaded before choosing your shape)</i>", self)
        warning_label.setTextFormat(Qt.RichText)
        main_layout.addWidget(warning_label)

        # Add stretch at bottom of the layout to fill any expanded space larger than what is needed,
        # so that if our tool is resized large our content will stay together
        main_layout.addStretch()

        self.setLayout(main_layout)


if __name__ == "__main__":
    # Create a new instance of the tool if launched from the Python Scripts window,
    # which allows for quick iteration without having to close/re-launch the Editor
    test_dialog = PyShapeExampleDialog()
    test_dialog.setWindowTitle("Shape Example (Python)")
    test_dialog.show()
    test_dialog.adjustSize()
