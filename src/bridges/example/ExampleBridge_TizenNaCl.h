#ifndef _EXAMPLE_BRIDGE_TIZEN_NACL_H_
#define _EXAMPLE_BRIDGE_TIZEN_NACL_H_

#include "ExampleBridge.h"

#include <platform/YiWebMessagingBridge.h>
#include <utility/YiRapidJSONUtility.h>
#include <utility/YiString.h>

class ExampleBridge_TizenNaCl : public ExampleBridge
{
public:
    ExampleBridge_TizenNaCl();
    virtual ~ExampleBridge_TizenNaCl();

    virtual void Init() override;
    virtual CYIString GetNickname() override;
    virtual bool SetNickname(const CYIString &nickname) override;
    virtual CYIString GetIPAddress() override;
    virtual std::vector<CYIString> GetLoadedScripts() override;

private:
    uint64_t m_sequentialNumberEventHandlerId;
};

#endif // _EXAMPLE_BRIDGE_TIZEN_NACL_H_
