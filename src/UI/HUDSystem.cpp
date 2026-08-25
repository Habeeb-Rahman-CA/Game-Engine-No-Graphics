#include "UI/HUDSystem.hpp"
#include "Debug/Logger.hpp"

namespace Engine {
namespace UISystem {

UIElement::UIElement(const std::string& id, Vec2 pos, Vec2 size, UIAnchor anchor)
    : m_id(id), m_position(pos), m_size(size), m_anchor(anchor), m_visible(true) {
}

Vec2 UIElement::getAnchoredPosition(int screenWidth, int screenHeight) const {
    Vec2 pos = m_position;

    switch (m_anchor) {
        case UIAnchor::TopLeft:
            break;
        case UIAnchor::TopCenter:
            pos.x += screenWidth * 0.5;
            break;
        case UIAnchor::TopRight:
            pos.x = screenWidth - m_position.x - m_size.x;
            break;
        case UIAnchor::BottomLeft:
            pos.y = screenHeight - m_position.y - m_size.y;
            break;
        case UIAnchor::BottomCenter:
            pos.x += screenWidth * 0.5;
            pos.y = screenHeight - m_position.y - m_size.y;
            break;
        case UIAnchor::BottomRight:
            pos.x = screenWidth - m_position.x - m_size.x;
            pos.y = screenHeight - m_position.y - m_size.y;
            break;
        case UIAnchor::Center:
            pos.x += (screenWidth - m_size.x) * 0.5;
            pos.y += (screenHeight - m_size.y) * 0.5;
            break;
    }

    return pos;
}

// 1. UIText Implementation
UIText::UIText(const std::string& id, const std::string& text, Vec2 pos, Color col, UIAnchor anchor)
    : UIElement(id, pos, Vec2(static_cast<double>(text.size() * 8), 16.0), anchor), m_text(text), m_color(col) {
}

void UIText::render(RenderSystem::GPU2DRenderer& renderer, int screenWidth, int screenHeight) {
    if (!m_visible) return;
    Vec2 p = getAnchoredPosition(screenWidth, screenHeight);
    renderer.draw_text(m_text, p, m_color);
}

// 2. UIButton Implementation
UIButton::UIButton(const std::string& id, const std::string& label, Vec2 pos, Vec2 size, std::function<void()> onClick, UIAnchor anchor)
    : UIElement(id, pos, size, anchor), m_label(label), m_onClick(onClick) {
}

void UIButton::render(RenderSystem::GPU2DRenderer& renderer, int screenWidth, int screenHeight) {
    if (!m_visible) return;
    Vec2 p = getAnchoredPosition(screenWidth, screenHeight);

    Color bg = m_isHovered ? Color(52, 152, 219) : Color(40, 45, 60);
    renderer.draw_rect(AABB::fromCenterSize(Vec2(p.x + m_size.x * 0.5, p.y + m_size.y * 0.5), m_size), bg, true);
    renderer.draw_rect(AABB::fromCenterSize(Vec2(p.x + m_size.x * 0.5, p.y + m_size.y * 0.5), m_size), Color::White, false);

    renderer.draw_text(m_label, Vec2(p.x + 8, p.y + (m_size.y - 12) * 0.5), Color::White);
}

bool UIButton::handleMouseClick(int mx, int my) {
    (void)mx;
    (void)my;
    if (!m_visible) return false;
    // Simple click bounds check
    if (m_onClick) {
        m_onClick();
        return true;
    }
    return false;
}

// 3. UIProgressBar Implementation
UIProgressBar::UIProgressBar(const std::string& id, Vec2 pos, Vec2 size, float value, Color fillColor, UIAnchor anchor)
    : UIElement(id, pos, size, anchor), m_progress(value), m_fillColor(fillColor) {
}

void UIProgressBar::render(RenderSystem::GPU2DRenderer& renderer, int screenWidth, int screenHeight) {
    if (!m_visible) return;
    Vec2 p = getAnchoredPosition(screenWidth, screenHeight);

    // Background Frame
    renderer.draw_rect(AABB::fromCenterSize(Vec2(p.x + m_size.x * 0.5, p.y + m_size.y * 0.5), m_size), Color(25, 30, 40), true);

    // Fill Bar
    double fillW = m_size.x * m_progress;
    if (fillW > 0.0) {
        renderer.draw_rect(AABB::fromCenterSize(Vec2(p.x + fillW * 0.5, p.y + m_size.y * 0.5), Vec2(fillW, m_size.y - 4)), m_fillColor, true);
    }

    // Border Outline
    renderer.draw_rect(AABB::fromCenterSize(Vec2(p.x + m_size.x * 0.5, p.y + m_size.y * 0.5), m_size), Color(180, 190, 200), false);
}

// 4. HUDSystem Implementation
HUDSystem::HUDSystem() {
    // Default In-Game HUD Elements
    auto healthBar = std::make_shared<UIProgressBar>("hud_health_bar", Vec2(16, 16), Vec2(180, 20), 1.0f, Color(46, 204, 113), UIAnchor::TopLeft);
    auto healthText = std::make_shared<UIText>("hud_health_text", "HP: 100/100", Vec2(24, 18), Color::White, UIAnchor::TopLeft);

    auto scoreText = std::make_shared<UIText>("hud_score_text", "SCORE: 1,250", Vec2(16, 42), Color(241, 196, 15), UIAnchor::TopLeft);
    auto coinsText = std::make_shared<UIText>("hud_coins_text", "COINS: 45", Vec2(160, 42), Color(241, 196, 15), UIAnchor::TopLeft);

    auto hotbarText = std::make_shared<UIText>("hud_hotbar", "[1: ATTACK] [2: HEAL] [3: DASH]", Vec2(-100, 20), Color(200, 220, 255), UIAnchor::BottomCenter);

    addElement(healthBar);
    addElement(healthText);
    addElement(scoreText);
    addElement(coinsText);
    addElement(hotbarText);
}

void HUDSystem::addElement(std::shared_ptr<UIElement> element) {
    if (element) {
        m_elements.push_back(element);
    }
}

std::shared_ptr<UIElement> HUDSystem::getElement(const std::string& id) {
    for (auto& el : m_elements) {
        if (el->getId() == id) return el;
    }
    return nullptr;
}

bool HUDSystem::handleMouseClick(int mx, int my) {
    for (auto& el : m_elements) {
        if (el->handleMouseClick(mx, my)) return true;
    }
    return false;
}

void HUDSystem::updatePlayerHUD(int currentHp, int maxHp, int score, int coins, const std::string& weapon) {
    (void)weapon;
    m_playerScore = score;
    m_playerCoins = coins;

    auto bar = std::dynamic_pointer_cast<UIProgressBar>(getElement("hud_health_bar"));
    if (bar) {
        float ratio = (maxHp > 0) ? (static_cast<float>(currentHp) / static_cast<float>(maxHp)) : 0.0f;
        bar->setProgress(ratio);
        if (ratio < 0.3f) bar->setFillColor(Color(231, 76, 60));
        else if (ratio < 0.6f) bar->setFillColor(Color(241, 196, 15));
        else bar->setFillColor(Color(46, 204, 113));
    }

    auto hpTxt = std::dynamic_pointer_cast<UIText>(getElement("hud_health_text"));
    if (hpTxt) {
        hpTxt->setText("HP: " + std::to_string(currentHp) + "/" + std::to_string(maxHp));
    }

    auto scoreTxt = std::dynamic_pointer_cast<UIText>(getElement("hud_score_text"));
    if (scoreTxt) {
        scoreTxt->setText("SCORE: " + std::to_string(score));
    }

    auto coinsTxt = std::dynamic_pointer_cast<UIText>(getElement("hud_coins_text"));
    if (coinsTxt) {
        coinsTxt->setText("COINS: " + std::to_string(coins));
    }
}

void HUDSystem::update(WorldSystem::World& world, double dt) {
    (void)dt;
    // Extract player stats from ECS World
    for (WorldSystem::Entity e : world.get_entities()) {
        std::string name = world.get_name(e);
        if (name.find("Player") != std::string::npos) {
            auto* h = world.get_health(e);
            if (h) {
                updatePlayerHUD(h->value, h->maxHp, m_playerScore, m_playerCoins, m_weaponName);
            }
            break;
        }
    }
}

void HUDSystem::renderHUD(RenderSystem::GPU2DRenderer& renderer, int screenWidth, int screenHeight) {
    for (auto& el : m_elements) {
        el->render(renderer, screenWidth, screenHeight);
    }
}

} // namespace UISystem
} // namespace Engine
