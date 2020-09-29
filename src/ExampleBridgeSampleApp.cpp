// © You i Labs Inc. 2000-2020. All rights reserved.

#include "ExampleBridgeSampleApp.h"

#include "bridges/example/ExampleBridgeLocator.h"

#include <framework/YiFramework.h>

#define LOG_TAG "ExampleBridgeSampleApp"

ExampleBridgeSampleApp::ExampleBridgeSampleApp() = default;

ExampleBridgeSampleApp::~ExampleBridgeSampleApp() = default;

bool ExampleBridgeSampleApp::UserInit()
{
    auto pExampleBridge = ExampleBridgeLocator::GetExampleBridge();

    if(pExampleBridge)
    {
        pExampleBridge->SequentialNumber.Connect([](int32_t sequentialNumber){
            YI_LOGI(LOG_TAG, "Sequential Number Event #%d!", sequentialNumber);
        });

        CYIString nickname(pExampleBridge->GetNickname());

        YI_LOGI(LOG_TAG, "Default Nickname: '%s'.", nickname.GetData());

        pExampleBridge->SetNickname("Awesome Bridge");

        CYIString newNickname(pExampleBridge->GetNickname());

        YI_LOGI(LOG_TAG, "New Nickname: '%s'.", newNickname.GetData());

        CYIString ipAddress(pExampleBridge->GetIPAddress());

        YI_LOGI(LOG_TAG, "IP Address: '%s'.", ipAddress.GetData());

        std::vector<CYIString> loadedScripts = pExampleBridge->GetLoadedScripts();

        YI_LOGI(LOG_TAG, "%d Scripts Loaded:", loadedScripts.size());

        for(size_t i = 0; i < loadedScripts.size(); i++)
        {
            YI_LOGI(LOG_TAG, "%d. %s", i + 1, loadedScripts[i].GetData());
        }
    }
    else
    {
        YI_LOGW(LOG_TAG, "Example bridge not available for current platform!");
    }

    return true;
}

bool ExampleBridgeSampleApp::UserStart()
{
    return true;
}

void ExampleBridgeSampleApp::UserUpdate()
{
}
