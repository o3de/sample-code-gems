
#include <AtomTutorialsModuleInterface.h>
#include "AtomTutorialsSystemComponent.h"

namespace AtomTutorials
{
    class AtomTutorialsModule
        : public AtomTutorialsModuleInterface
    {
    public:
        AZ_RTTI(AtomTutorialsModule, "{C96BD29A-15A9-4C5F-8076-4C4DB113A5FE}", AtomTutorialsModuleInterface);
        AZ_CLASS_ALLOCATOR(AtomTutorialsModule, AZ::SystemAllocator, 0);
    };
}// namespace AtomTutorials

AZ_DECLARE_MODULE_CLASS(Gem_AtomTutorials, AtomTutorials::AtomTutorialsModule)
