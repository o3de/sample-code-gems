
#pragma once

#if !defined(Q_MOC_RUN)
#include <AzToolsFramework/API/ToolsApplicationAPI.h>

#include <QWidget>
#endif

namespace AtomTutorials
{
    class AtomTutorialsWidget
        : public QWidget
    {
        Q_OBJECT
    public:
        explicit AtomTutorialsWidget(QWidget* parent = nullptr);
    };
} 
