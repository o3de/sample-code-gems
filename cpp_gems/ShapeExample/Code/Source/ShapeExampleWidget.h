/*
 * Copyright (c) Contributors to the Open 3D Engine Project.
 * For complete copyright and license terms please see the LICENSE at the root of this distribution.
 *
 * SPDX-License-Identifier: Apache-2.0 OR MIT
 *
 */

#pragma once

#if !defined(Q_MOC_RUN)
#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <QWidget>
#endif

class QCheckBox;
class QLineEdit;

namespace ShapeExample
{
    class ShapeExampleWidget
        : public QWidget
    {
        Q_OBJECT
    public:
        explicit ShapeExampleWidget(QWidget* parent = nullptr);

    private Q_SLOTS:
        void OnShapeButtonClicked();

    private:
        QLineEdit* m_nameInput = nullptr;
        QCheckBox* m_addShapeNameSuffix = nullptr;
    };
} 
