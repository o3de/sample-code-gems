"""
Copyright (c) Contributors to the Open 3D Engine Project.
For complete copyright and license terms please see the LICENSE at the root of this distribution.

SPDX-License-Identifier: Apache-2.0 OR MIT
"""
# -------------------------------------------------------------------------
"""PyShapeExample\\editor\\scripts\\boostrap.py
Generated from O3DE PythonToolGem Template"""

import az_qt_helpers
import azlmbr.editor as editor
from pyshapeexample_dialog import PyShapeExampleDialog

if __name__ == "__main__":
    print("PyShapeExample.boostrap, Generated from O3DE PythonToolGem Template")

    # Register our custom widget as a dockable tool with the Editor under an Examples sub-menu
    options = editor.ViewPaneOptions()
    options.showOnToolsToolbar = True
    options.toolbarIcon = ":/PyShapeExample/toolbar_icon.svg"
    az_qt_helpers.register_view_pane('Shape Example (Python)', PyShapeExampleDialog, category="Examples", options=options)
