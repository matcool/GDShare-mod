#include <Windows.h>
#include <matdash/console.hpp>
#include <matdash/boilerplate.hpp>
#include <MinHook.h>

#include "layers/EditLevelLayer.hpp"
#include "layers/ExportSettingsLayer.hpp"
#include "layers/LevelBrowserLayer.hpp"
#include "layers/LevelInfoLayer.hpp"

void mod_main(HMODULE) {
    MH_Initialize();
    // matdash::create_console();
    // std::cout << "god i hate gdmake" << std::endl;
    
	EditLevelLayer::loadHook();
    LevelBrowserLayer::loadHook();
    LevelInfoLayer::loadHook();
	
    MH_EnableHook(MH_ALL_HOOKS);
}