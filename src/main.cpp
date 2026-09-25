#include <Geode/Geode.hpp>
#include <Geode/modify/Slider.hpp>
#include <Geode/binding/SliderThumb.hpp>
#include <Geode/binding/SliderTouchLogic.hpp>
#include "UnlimitedInput.hpp"

using namespace geode::prelude;

class $modify(UnlimitedSlider, Slider) {
    struct Fields {
        UnlimitedInput* input = nullptr;
    };

    bool init(CCNode* target, SEL_MenuHandler handler, const char* bar,
              const char* thumbSprite, const char* thumbSpriteSelected,
              const char* barCircleSprite) {
        if (!Slider::init(target, handler, bar, thumbSprite,
                           thumbSpriteSelected, barCircleSprite)) {
            return false;
        }

        auto thumb = m_touchLogic ? m_touchLogic->m_slider : nullptr;
        float initial = thumb ? thumb->getValue() : 0.f;

        m_fields->input = UnlimitedInput::create(initial, [this](float v) {
            v = std::clamp(v, 0.f, 1.f);
            if (m_touchLogic && m_touchLogic->m_slider) {
                m_touchLogic->m_slider->setValue(v);
            }
            if (m_target && m_handler) {
                (m_target->*m_handler)(this);
            }
        });

        if (m_fields->input) {
            this->setPosition(this->getPosition());
            m_fields->input->setPosition({this->getContentSize().width / 2.f, 0.f});
            this->addChild(m_fields->input, 100);

            this->setTouchEnabled(false);
            if (m_touchLogic) m_touchLogic->setVisible(false);
            if (m_touchLogic && m_touchLogic->m_slider) {
                m_touchLogic->m_slider->setVisible(false);
            }
            for (auto child : CCArrayExt<CCNode*>(this->getChildren())) {
                if (child != m_fields->input) child->setVisible(false);
            }
        }

        return true;
    }
};
