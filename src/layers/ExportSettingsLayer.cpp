#include "ExportSettingsLayer.hpp"
#include <direct.h>
#include <shlobj_core.h>
#include "../utils/gdshare.hpp"
#include "../offsets.hpp"
#include <nfd.h>
#include <string_view>

bool ends_with(std::string_view str, std::string_view part) {
    if (str.size() < part.size()) return false;
    auto sub = str.substr(str.size() - part.size());
    return sub == part;
}

void ExportSettingsLayer::onClose(cocos2d::CCObject* pSender) {
    BrownAlertDelegate::onClose(pSender);
}

void ExportSettingsLayer::onInfo(cocos2d::CCObject*) {
    gd::FLAlertLayer::create(
        nullptr,
        "Info", "OK", nullptr,
        360.0f,
        "You can export levels for multiple different level sharing programs.\n"
        "Integrated GDShare can read all formats.\n\n"
        "<cy>.gmd</c> is for <co>GDShare</c> and is awesome simple\n"
        "<cy>.gmd2</c> is for <cg>when</c> you want to include song ig\n"
        "<cy>.lvl</c> is for <cr>not real</c> and should not be used\n"
    )->show();
}

void ExportSettingsLayer::onExport(cocos2d::CCObject*) {
    nfdchar_t* path = nullptr;
    if (NFD_SaveDialog("gmd;gmd2", nullptr, &path) != NFD_OKAY) {
        this->onClose(nullptr);
        return;
    }
    std::string outputPath = path;
    free(path);
    auto format = ExportFormat::gmd;
    if (ends_with(outputPath, ".gmd")) {
        format = ExportFormat::gmd;
    } else if (ends_with(outputPath, ".gmd2")) {
        format = ExportFormat::gmd2;
    } else {
        gd::FLAlertLayer::create(nullptr, "error", "OK", nullptr, "invalid file extension")->show();
        return;
    }

    int flag = gdshare::EF_None;

    if (this->m_pIncludeSong->getToggle()->isOn() && format == ExportFormat::gmd2)
        flag |= gdshare::EF_IncludeSong;

    auto res = gdshare::exportLevel( this->m_pLevel, outputPath, format, flag );
    if (!res.size())
        gd::FLAlertLayer::create(
            new ExportResultHandler(outputPath),
            "Success",
            "OK", "Show File",
            "Succesfully exported to <co>"_s + outputPath + "</c>!"_s
        )->show();
    else
        gd::FLAlertLayer::create(
            nullptr,
            "Error",
            "OK", nullptr,
            "<cr>Error</c>: " + res
        )->show();
    
    this->onClose(nullptr);
}

ExportResultHandler::ExportResultHandler(std::string const& _str) {
    this->m_sPath = _str;
}

void ExportResultHandler::FLAlert_Clicked(gd::FLAlertLayer*, bool _btn2) {
    if (_btn2) {
        ITEMIDLIST *pidl = ILCreateFromPathA(this->m_sPath.c_str());

        if (pidl) {
            SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
            ILFree(pidl);
        }
    }
}

void ExportSettingsLayer::setup() {
    auto winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();    

    auto pTypeLabel = cocos2d::CCLabelBMFont::create("Format:", "bigFont.fnt");
    pTypeLabel->setPosition(winSize.width / 2, winSize.height / 2 + 78.0f);
    pTypeLabel->setScale(.5f);
    this->m_pLayer->addChild(pTypeLabel);

    auto pTypeInfoSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    pTypeInfoSpr->setScale(.7f);

    auto pTypeInfo = gd::CCMenuItemSpriteExtra::create(
        pTypeInfoSpr,
        this,
        (cocos2d::SEL_MenuHandler)&ExportSettingsLayer::onInfo
    );
    pTypeInfo->setPosition(
        pTypeLabel->getScaledContentSize().width / 2 + 3.0f + pTypeInfoSpr->getScaledContentSize().width / 2,
        pTypeLabel->getPositionY() - winSize.height / 2 - 1.5f
    );
    this->m_pButtonMenu->addChild(pTypeInfo);
    
    // tysm camden <3
    this->registerWithTouchDispatcher();
    cocos2d::CCDirector::sharedDirector()->getTouchDispatcher()->incrementForcePrio(2);

    this->setTouchEnabled(true);
    this->setKeypadEnabled(true);

    auto pExportButton = gd::CCMenuItemSpriteExtra::create(
        gd::ButtonSprite::create(
            "Export", 0, 0, "goldFont.fnt", "GJ_button_01.png", 0, .8f
        ),
        this,
        (cocos2d::SEL_MenuHandler)&ExportSettingsLayer::onExport
    );
    pExportButton->setPosition(
        0.0f,
        - this->m_pLrSize.height / 2 + pExportButton->getScaledContentSize().height / 2 + 12.5f
    );

    this->m_pButtonMenu->addChild(pExportButton);

    auto pToggleOnSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    auto pToggleOffSpr = cocos2d::CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");

    pToggleOnSpr->setScale(.8f);
    pToggleOffSpr->setScale(.8f);

    this->m_pIncludeSong = BGCheckbox::create("Include song");
    this->m_pIncludeSong->setScale(.75f);
    this->m_pIncludeSong->setPosition(
        0.0f, - 60.0f
    );

    this->m_pButtonMenu->addChild(this->m_pIncludeSong);
}

ExportSettingsLayer* ExportSettingsLayer::create(gd::GJGameLevel* _lvl) {
    auto pRet = new ExportSettingsLayer();

    // this is what beautiful code looks like
    if (pRet) {
        pRet->m_pLevel = _lvl;

        if (pRet->init(
            320.0f, 260.0f,
            "GJ_square01.png",
            ("Export "_s + _lvl->m_sLevelName).c_str()
        )) {
            pRet->autorelease();
            return pRet;
        }
    }

    CC_SAFE_DELETE(pRet);
    return nullptr;
}
