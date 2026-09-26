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
              const char* groove, const char* thumb, const char* thumbSel,
              float scale) {
        if (!Slider::init(target, handler, bar, groove, thumb, thumbSel, scale)) {
            return false;
        }

        auto thumbNode = m_touchLogic ? m_touchLogic->m_slider : nullptr;
        float initial = thumbNode ? thumbNode->getValue() : 0.f;

        m_fields->input = UnlimitedInput::create(initial, [this](float v) {
            v = std::clamp(v, 0.f, 1.f);
            this->setValue(v);
            this->sliderEnded();
        });

        if (m_fields->input) {
            m_fields->input->setPosition({this->getContentSize().width / 2.f, 0.f});
            this->addChild(m_fields->input, 100);

            this->setTouchEnabled(false);
            if (m_touchLogic) {
                m_touchLogic->setVisible(false);
                m_touchLogic->setTouchEnabled(false);
            }
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
