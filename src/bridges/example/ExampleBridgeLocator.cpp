#include "ExampleBridgeLocator.h"

#include "ExampleBridge.h"

#include <framework/YiPredef.h>

#define LOG_TAG "ExampleBridgeLocator"

#if defined(YI_TIZEN_NACL)
#    include "ExampleBridge_TizenNaCl.h"
#endif

static std::shared_ptr<ExampleBridge> s_pCachedExampleBridge = nullptr;

std::shared_ptr<ExampleBridge> ExampleBridgeLocator::GetExampleBridge()
{
    if (!s_pCachedExampleBridge)
    {
#if defined(YI_TIZEN_NACL)
        s_pCachedExampleBridge = std::make_shared<ExampleBridge_TizenNaCl>();
#endif

        if(s_pCachedExampleBridge)
        {
            s_pCachedExampleBridge->Init();
        }
    }

    return s_pCachedExampleBridge;
}
