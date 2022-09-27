#include <assert.h>

#include "../json.h"
#include "reflection.h"
#include "reflection_register.h"
#include "../serializer/serializer.h"

#include "core/_generated/reflection/all_reflection.h"
#include "core/_generated/serializer/all_serializer.ipp"

namespace Mage
{
    namespace Reflection
    {
        void TypeMetaRegister::Unregister() { TypeMetaRegisterinterface::unregisterAll(); }
    } // namespace Reflection
} // namespace Piccolo
