#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/SliderThumb.hpp>
#include <Geode/binding/SliderTouchLogic.hpp>

using namespace geode::prelude;

class UnlimitedInput : public CCNode, public TextInputDelegate {
protected:
    CCTextInputNode* m_input = nullptr;
    std::function<void(float)> m_onChange;

    bool init(float initialValue, std::function<void(float)> onChange) {
        if (!CCNode::init()) return false;
        m_onChange = onChange;

        auto bg = CCScale9Sprite::create("square02_001.png");
        bg->setContentSize({70.f, 34.f});
        bg->setColor({0, 0, 0});
        bg->setOpacity(180);
        this->addChild(bg);

        m_input = CCTextInputNode::create(60.f, 30.f, "0", "bigFont.fnt");
        m_input->setString(numToString(initialValue));
        m_input->setDelegate(this);
        this->addChild(m_input, 1);
        this->setContentSize({70.f, 34.f});

        return true;
    }

public:
    static UnlimitedInput* create(float initialValue, std::function<void(float)> onChange) {
        auto ret = new UnlimitedInput();
        if (ret->init(initialValue, onChange)) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr;
    }

    void textChanged(CCTextInputNode* input) override {
        std::string str = input->getString();
        char* end = nullptr;
        float value = std::strtof(str.c_str(), &end);
        if (end != str.c_str() && m_onChange) {
            m_onChange(value);
        }
    }

    void setValue(float value) {
        if (m_input) m_input->setString(numToString(value));
    }
};
