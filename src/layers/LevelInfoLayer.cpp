#include "LevelInfoLayer.hpp"
#include "EditLevelLayer.hpp"

bool __fastcall LevelInfoLayer::initHook(LevelInfoLayer* self, uintptr_t, gd::GJGameLevel* lvl) {
    if (!init(self, lvl))
        return false;

    auto exportButton = gd::CCMenuItemSpriteExtra::create(
        makeSpriteOrFallback("BE_Export_File.png", "GJ_downloadBtn_001.png"),
        self,
        (cocos2d::SEL_MenuHandler)&EditLevelLayer::onExport
    );
    exportButton->setUserData(lvl);

    auto gm = gd::GameManager::sharedState();
    auto ratePowerSeed = *as<int*>(as<uintptr_t>(gm) + 0x2d0);
    auto ratePowerRand = *as<int*>(as<uintptr_t>(gm) + 0x2d4);
    auto ratePower = ratePowerSeed - ratePowerRand;

    if (self->m_pCloneBtn) {
        self->m_pCloneBtn->getParent()->addChild(
            exportButton
        );

        exportButton->setPosition(
            self->m_pCloneBtn->getPositionX(),
            self->m_pCloneBtn->getPositionY() + (ratePower ? 100.f : 50.f)
        );
    } else if (self->m_pLikeBtn) {
        auto menu = self->m_pLikeBtn->getParent();
        auto l_ix = menu->getChildren()->indexOfObject(self->m_pLikeBtn);
        auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

        auto pos_x = - self->m_pLikeBtn->getPositionX();
        auto pos_y = 0.0f;

        for (auto ix = l_ix; ix < menu->getChildrenCount(); ix++) {
            auto obj = getChild<cocos2d::CCNode*>(menu, ix);

            if (obj->getPositionX() < 80.0f && obj->getPositionY() > pos_y)
                    pos_y = obj->getPositionY();
        }

        menu->addChild(exportButton);

        exportButton->setPosition(pos_x, pos_y + 50.f);
    } else {
        self->m_pPlayBtnMenu->addChild(exportButton);

        exportButton->setPosition(-80.0f, 0.0f);
    }

    return true;
}
