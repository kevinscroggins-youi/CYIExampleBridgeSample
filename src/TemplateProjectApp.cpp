// © You i Labs Inc. 2000-2020. All rights reserved.
#include "TemplateProjectApp.h"

#include <automation/YiWebDriverLocator.h>
#include <framework/YiFramework.h>
#include <scenetree/YiSceneManager.h>
#include <view/YiSceneView.h>

TemplateProjectApp::TemplateProjectApp()
{
}

TemplateProjectApp::~TemplateProjectApp()
{
}

bool TemplateProjectApp::UserInit()
{
    //! [UserInit]
    // Start the web driver for allowing the use of appium.
    CYIWebDriver *pWebDriver = CYIWebDriverLocator::GetWebDriver();
    if (pWebDriver)
    {
        pWebDriver->Start();
    }

    // Load a layout file which will be the root scene view.
    std::unique_ptr<CYISceneView> pSceneViewMain = GetSceneManager()->LoadScene("TemplateProject_MainComp.layout", CYISceneManager::ScaleType::Fit, CYISceneManager::VerticalAlignmentType::Center, CYISceneManager::HorizontalAlignmentType::Center);
    if (!pSceneViewMain)
    {
        YI_LOGE("TemplateProjectApp::UserInit", "Loading scene has failed");
        return false;
    }
    // Add the scene view to the scene manager.
    GetSceneManager()->AddScene("MainComp", std::move(pSceneViewMain), 0, CYISceneManager::LayerType::Opaque);
    GetSceneManager()->StageScene("MainComp");

    return true;

    //! [UserInit]
}

bool TemplateProjectApp::UserStart()
{
    // This is the first function called upon completion of engine and application initialization. Return false in case of failure.
    return true;
}

void TemplateProjectApp::UserUpdate()
{
    // This per-frame hook can be used to drive components of an application which rely on a time-step and are not managed by You.i Engine.
}
