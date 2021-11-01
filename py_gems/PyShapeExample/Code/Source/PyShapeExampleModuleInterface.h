
#include <AzCore/Memory/SystemAllocator.h>
#include <AzCore/Module/Module.h>

namespace PyShapeExample
{
    class PyShapeExampleModuleInterface
        : public AZ::Module
    {
    public:
        AZ_RTTI(PyShapeExampleModuleInterface, "{f8731810-a6e3-403d-af72-ee81424ef59c}", AZ::Module);
        AZ_CLASS_ALLOCATOR(PyShapeExampleModuleInterface, AZ::SystemAllocator, 0);

        PyShapeExampleModuleInterface()
        {
        }

        /**
         * Add required SystemComponents to the SystemEntity.
         */
        AZ::ComponentTypeList GetRequiredSystemComponents() const override
        {
            return AZ::ComponentTypeList{
            };
        }
    };
}// namespace PyShapeExample
