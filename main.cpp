#include <Geode/Geode.hpp>
#include <Geode/modify/Slider.hpp>
#include <Geode/binding/SliderThumb.hpp>
#include <Geode/binding/SliderTouchLogic.hpp>
#include "UnlimitedInput.hpp"

using namespace geode::prelude;

// FASE 1 (implementada): todo Slider do jogo (não só do editor, mas é onde
// aparecem quase todos) ganha uma caixinha de texto do lado, sincronizada
// com a posição 0..1 do thumb. Isso já tira o limite de "só dá pra arrastar
// com o dedo, com a precisão que o dedo permitir" — agora dá pra digitar
// qualquer valor entre 0 e 1 com quantas casas decimais quiser.
//
// FASE 2 (não implementada aqui, ver comentário no fim): cada popup
// (EditTriggerPopup, ColorSelectPopup, etc.) pega esse 0..1 e multiplica
// por um min/max PRÓPRIO dele (ex: duração 0 a 10s). Pra ter valor
// REALMENTE sem teto (tipo digitar 99999), é PRECISO hookar o popup
// específico e sobrescrever o valor final que ele calcula, não só o
// slider. Isso é por trigger, um de cada vez.

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
            // trava só no range físico do widget (0..1) — não é um "teto"
            // de conteúdo, é o range do componente em si.
            v = std::clamp(v, 0.f, 1.f);
            if (m_touchLogic && m_touchLogic->m_slider) {
                m_touchLogic->m_slider->setValue(v);
            }
            // dispara o mesmo callback que o arraste manual dispararia,
            // pra qualquer popup que esteja ouvindo esse slider reagir
            // normalmente à mudança feita por texto.
            if (m_target && m_handler) {
                (m_target->*m_handler)(this);
            }
        });

        if (m_fields->input) {
            // substitui visualmente o slider: ele continua existindo (o
            // resto do jogo ainda espera um Slider real nesse lugar da
            // árvore), só fica invisível e sem toque, e a textbox fica
            // exatamente na posição/centro onde o slider estava.
            this->setPosition(this->getPosition());
            m_fields->input->setPosition({this->getContentSize().width / 2.f, 0.f});
            this->addChild(m_fields->input, 100);

            this->setTouchEnabled(false); // slider não reage mais a arrastar
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
