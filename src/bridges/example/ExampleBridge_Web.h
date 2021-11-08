#ifndef _EXAMPLE_BRIDGE_WEB_H_
#define _EXAMPLE_BRIDGE_WEB_H_

#include "ExampleBridge.h"

#include <platform/YiWebMessagingBridge.h>
#include <platform/YiWebViewController.h>
#include <utility/YiRapidJSONUtility.h>
#include <utility/YiString.h>

#include <vector>

class CYIWebFileLoader;

class ExampleBridgeWeb : public ExampleBridge
{
public:
    ExampleBridgeWeb();
    virtual ~ExampleBridgeWeb();

    virtual void Init() override;
    virtual CYIString GetNickname() override;
    virtual bool SetNickname(const CYIString &nickname) override;
    virtual CYIString GetIPAddress() override;
    virtual std::vector<CYIString> GetLoadedScripts() override;

private:
    CYIWebMessagingBridge *GetWebMessagingBridge() const;
    CYIWebFileLoader *GetFileLoader() const;
    CYIWebMessagingBridge::FutureResponse CallStaticFunction(yi::rapidjson::Document &&message, const CYIString &functionName, yi::rapidjson::Value &&functionArgumentsValue, bool *pMessageSent);
    CYIWebMessagingBridge::FutureResponse CallInstanceFunction(yi::rapidjson::Document &&message, const CYIString &functionName, yi::rapidjson::Value &&functionArgumentsValue, bool *pMessageSent);
    uint64_t RegisterEventHandler(const CYIString &eventName, CYIWebMessagingBridge::EventCallback &&eventCallback);
    void UnregisterEventHandler(uint64_t &eventHandlerId);
    bool LoadScript();

    uint64_t m_sequentialNumberEventHandlerId;
    mutable std::unique_ptr<CYIWebViewController> m_pWebViewController;
};

#endif // _EXAMPLE_BRIDGE_WEB_H_
