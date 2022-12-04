#include "EditLevelLayer.hpp"
#include "ExportSettingsLayer.hpp"
#include "../offsets.hpp"

void EditLevelLayer::onExport(cocos2d::CCObject* pSender) {
    ExportSettingsLayer::create(
        as<gd::GJGameLevel*>(
            as<cocos2d::CCNode*>(pSender)->getUserData()
        )
    )->show();
}

bool __fastcall EditLevelLayer::initHook(EditLevelLayer* self, uintptr_t, gd::GJGameLevel* level) {
    if (!init(self, level))
        return false;

    auto exportButton = gd::CCMenuItemSpriteExtra::create(
        makeSpriteOrFallback("BE_Export_File.png", "GJ_downloadBtn_001.png"),
        self,
        (cocos2d::SEL_MenuHandler)&EditLevelLayer::onExport
    );
    exportButton->setUserData(level);
    
    auto* menu = cocos2d::CCMenu::create();
    menu->addChild(exportButton);
    self->addChild(menu);

    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
    menu->setPosition(ccp(winSize.width - 30.f, 52.f));
    menu->setZOrder(1);

    return true;
}
