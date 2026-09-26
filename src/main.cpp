#include <Geode/Geode.hpp>
#include <Geode/modify/SetupTimeWarpPopup.hpp>
#include "UnlimitedInput.hpp"

using namespace geode::prelude;

class $modify(UnlimitedTimeWarp, SetupTimeWarpPopup) {
    struct Fields {
        UnlimitedInput* input = nullptr;
    };

    bool init(EffectGameObject* triggerObj, CCArray* triggers) {
        if (!SetupTimeWarpPopup::init(triggerObj, triggers)) return false;

        if (m_timeWarpSlider) {
            m_timeWarpSlider->setVisible(false);
            m_timeWarpSlider->setTouchEnabled(false);

            m_fields->input = UnlimitedInput::create(m_timeWarpMod, [this](float v) {
                m_timeWarpMod = v;
                this->updateTimeWarp();
                this->updateTimeWarpLabel();
            });

            if (m_fields->input) {
                m_fields->input->setPosition(m_timeWarpSlider->getPosition());
                m_timeWarpSlider->getParent()->addChild(m_fields->input, 100);
            }
        }

        return true;
    }
};
