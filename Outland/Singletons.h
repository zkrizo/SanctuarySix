#ifndef Singletons_H
#define Singletons_H

#include <Outland\UI Core\GUI_Loading.h>
#include <Outland\UI Core\UICore.h>
#include <Outland\Graphics Core\GraphicsCore.h>
#include <Outland\Player Core\GameplayCore.h>
#include <Outland\Logger.h>

#define logger Logger::Instance()
#define GUILoad GUI_Loading::Instance()
#define uiCore UserInputCore::Instance()
#define grCore GraphicsCore::Instance()
#define gameCore GameplayCore::Instance()

#endif