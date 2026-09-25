#pragma once

#include <Geode/Geode.hpp>
#include <Geode/binding/Slider.hpp>
#include <Geode/binding/SliderThumb.hpp>
#include <Geode/binding/SliderTouchLogic.hpp>

using namespace geode::prelude;

// Caixa de texto numérica que fica ao lado de um Slider nativo do GD.
// Não tem min/max: o valor digitado vai direto pro callback, sem clamp.
// O componente NÃO decide sozinho o que "valor sem limite" significa pra
// cada trigger — quem usa esse helper (o hook de cada popup) é quem decide
// se vai só espelhar a % do slider (0..1, sem limite de casas decimais) ou
// se vai sobrescrever o valor final que o popup calcula (aí sim "sem teto").
class UnlimitedInput : public CCNode {
protected:
    CCTextInputNode* m_input = nullptr;
    std::function<void(float)> m_onChange;

    bool init(float initialValue, std::function<void(float)> onChange) {
        if (!CCNode::init()) return false;
        m_onChange = onChange;

        // 60px de largura, sem filtro de caracteres além do que
        // definimos manualmente no callback (aceita "-", ".", dígitos).
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

    // Chamado pelo CCTextInputNode (via CCTextInputDelegate) a cada edição.
    // Sem clamp: se o texto não for um número válido, simplesmente ignora
    // em vez de forçar um valor "seguro" — é isso que torna o campo
    // "sem limites" de verdade (não trava em nenhum teto arbitrário).
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
