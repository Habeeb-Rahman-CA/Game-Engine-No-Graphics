#ifndef UI_HUDSYSTEM_HPP
#define UI_HUDSYSTEM_HPP

#include "Math/Vec2.hpp"
#include "Math/Geometry.hpp"
#include "Renderer/GPU2DRenderer.hpp"
#include "System/ISystem.hpp"
#include "World/World.hpp"

#include <string>
#include <vector>
#include <functional>
#include <memory>

namespace Engine {
namespace UISystem {

using Vec2 = Math::Vec2;
using Color = RenderSystem::Color;
using AABB = Math::AABB;

enum class UIAnchor {
    TopLeft,
    TopCenter,
    TopRight,
    BottomLeft,
    BottomCenter,
    BottomRight,
    Center
};

class UIElement {
public:
    UIElement(const std::string& id, Vec2 pos, Vec2 size, UIAnchor anchor = UIAnchor::TopLeft);
    virtual ~UIElement() = default;

    virtual void render(RenderSystem::GPU2DRenderer& renderer, int screenWidth, int screenHeight) = 0;
    virtual bool handleMouseClick(int mx, int my) { (void)mx; (void)my; return false; }

    Vec2 getAnchoredPosition(int screenWidth, int screenHeight) const;

    const std::string& getId() const { return m_id; }
    bool isVisible() const { return m_visible; }
    void setVisible(bool visible) { m_visible = visible; }

protected:
    std::string m_id;
    Vec2 m_position;
    Vec2 m_size;
    UIAnchor m_anchor;
    bool m_visible = true;
};

// 1. Text Label
class UIText : public UIElement {
public:
    UIText(const std::string& id, const std::string& text, Vec2 pos, Color col = Color::White, UIAnchor anchor = UIAnchor::TopLeft);
    void render(RenderSystem::GPU2DRenderer& renderer, int screenWidth, int screenHeight) override;

    void setText(const std::string& text) { m_text = text; }
    void setColor(Color color) { m_color = color; }

private:
    std::string m_text;
    Color m_color;
};

// 2. Interactive Button
class UIButton : public UIElement {
public:
    UIButton(const std::string& id, const std::string& label, Vec2 pos, Vec2 size, std::function<void()> onClick, UIAnchor anchor = UIAnchor::TopLeft);
    void render(RenderSystem::GPU2DRenderer& renderer, int screenWidth, int screenHeight) override;
    bool handleMouseClick(int mx, int my) override;

    void setLabel(const std::string& label) { m_label = label; }

private:
    std::string m_label;
    std::function<void()> m_onClick;
    bool m_isHovered = false;
};

// 3. Health / Progress Bar
class UIProgressBar : public UIElement {
public:
    UIProgressBar(const std::string& id, Vec2 pos, Vec2 size, float value = 1.0f, Color fillColor = Color(46, 204, 113), UIAnchor anchor = UIAnchor::TopLeft);
    void render(RenderSystem::GPU2DRenderer& renderer, int screenWidth, int screenHeight) override;

    void setProgress(float val) { m_progress = std::max(0.0f, std::min(1.0f, val)); }
    void setFillColor(Color color) { m_fillColor = color; }

private:
    float m_progress = 1.0f;
    Color m_fillColor;
};

// 4. Central HUD Manager System
class HUDSystem : public Engine::System::ISystem {
public:
    HUDSystem();
    ~HUDSystem() = default;

    std::string getName() const override { return "HUDSystem"; }
    void update(WorldSystem::World& world, double dt) override;
    void renderHUD(RenderSystem::GPU2DRenderer& renderer, int screenWidth, int screenHeight);

    void addElement(std::shared_ptr<UIElement> element);
    std::shared_ptr<UIElement> getElement(const std::string& id);

    bool handleMouseClick(int mx, int my);

    // In-game HUD metrics
    void updatePlayerHUD(int currentHp, int maxHp, int score, int coins, const std::string& currentWeapon);

private:
    std::vector<std::shared_ptr<UIElement>> m_elements;
    int m_playerScore = 1250;
    int m_playerCoins = 45;
    std::string m_weaponName = "Iron Sword";
};

} // namespace UISystem
} // namespace Engine

#endif // UI_HUDSYSTEM_HPP
