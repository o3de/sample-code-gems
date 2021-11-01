
#include <PyShapeExampleModuleInterface.h>
#include <PyShapeExampleEditorSystemComponent.h>

void InitPyShapeExampleResources()
{
    // We must register our Qt resources (.qrc file) since this is being loaded from a separate module (gem)
    Q_INIT_RESOURCE(PyShapeExample);
}

namespace PyShapeExample
{
    class PyShapeExampleEditorModule
        : public PyShapeExampleModuleInterface
    {
    public:
        AZ_RTTI(PyShapeExampleEditorModule, "{cebbfc02-eab9-4c61-b0de-31795b1c044a}", PyShapeExampleModuleInterface);
        AZ_CLASS_ALLOCATOR(PyShapeExampleEditorModule, AZ::SystemAllocator, 0);

        PyShapeExampleEditorModule()
        {
            InitPyShapeExampleResources();

            // Push results of [MyComponent]::CreateDescriptor() into m_descriptors here.
            // Add ALL components descriptors associated with this gem to m_descriptors.
            // This will associate the AzTypeInfo information for the components with the the SerializeContext, BehaviorContext and EditContext.
            // This happens through the [MyComponent]::Reflect() function.
            m_descriptors.insert(m_descriptors.end(), {
                PyShapeExampleEditorSystemComponent::CreateDescriptor(),
            });
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         * Non-SystemComponents should not be added here
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList {
                azrtti_typeid<PyShapeExampleEditorSystemComponent>(),
            };
        }
    };
}// namespace PyShapeExample

AZ_DECLARE_MODULE_CLASS(Gem_PyShapeExample, PyShapeExample::PyShapeExampleEditorModule)
