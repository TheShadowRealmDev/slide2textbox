#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/SliderThumb.hpp>
#include <Geode/binding/SliderTouchLogic.hpp>

using namespace geode::prelude;

class UnlimitedInput : public CCNode {
protected:
    CCTextInputNode* m_input = nullptr;
    std::function<void(float)> m_onChange;

    bool init(float initialValue, std::function<void(float)> onChange) {
        if (!CCNode::init()) return false;
        m_onChange = onChange;

        m_input = CCTextInputNode::create(60.f, 30.f, "0", "chatFont.fnt");
        m_input->setString(numToString(initialValue));
        m_input->setDelegate(this);
        this->addChild(m_input);
        this->setContentSize(m_input->getContentSize());

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

    void textChanged(CCTextInputNode* input) {
        auto str = input->getString();
        char* end = nullptr;
        float value = std::strtof(str, &end);
        if (end != str && m_onChange) {
            m_onChange(value);
        }
    }

    void setValue(float value) {
        if (m_input) m_input->setString(numToString(value));
    }
};
