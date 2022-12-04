#pragma once

#include <GDMake.h>

template<typename T>
static T getChild(cocos2d::CCNode* x, int i) {
    return static_cast<T>(x->getChildren()->objectAtIndex(i));
}

inline std::string operator"" _s (const char* txt, size_t) {
    return std::string(txt);
}

inline cocos2d::CCSprite* makeSpriteOrFallback(const char* spr, const char* alt) {
    auto res = cocos2d::CCSprite::create(spr);
    if (!res)
        res = cocos2d::CCSprite::createWithSpriteFrameName(alt);
    return res;
}

// typedef const char* nullstr_t;
// static constexpr nullstr_t nullstr = "";

