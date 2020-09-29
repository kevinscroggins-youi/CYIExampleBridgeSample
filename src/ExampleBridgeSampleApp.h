// © You i Labs Inc. 2000-2020. All rights reserved.

#ifndef _TEMPLATE_PROJECT_APP_H_
#define _TEMPLATE_PROJECT_APP_H_

#include <framework/YiApp.h>

class ExampleBridgeSampleApp : public CYIApp
{
public:
    ExampleBridgeSampleApp();
    virtual ~ExampleBridgeSampleApp();

    virtual bool UserInit() override;
    virtual bool UserStart() override;
    virtual void UserUpdate() override;
};

#endif // _TEMPLATE_PROJECT_APP_H_
