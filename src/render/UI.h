/** 
 * UI.h 
 * created 04/05/25 by frank collebrusco
 */
#ifndef UI_H
#define UI_H
#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/glm.h>
#include <flgl/allocators.h>
#include <vector>

struct UIbbox {
    bool inside(glm::vec2 mpos) const;
    void merge(UIbbox const& other);
    bool isect(UIbbox const& other);
    bool contains(UIbbox const& other);
    bool degen() const;
    glm::vec2 size() const;
    glm::vec2 middle() const;
    glm::vec2 const& min() const;
    glm::vec2 const& max() const;
    static UIbbox from_merge(UIbbox const& a, UIbbox const& b);
    static UIbbox from_isect(UIbbox const& a, UIbbox const& b);
    static UIbbox from_minmax(glm::vec2 min, glm::vec2 max);
    static UIbbox from_minsize(glm::vec2 min, glm::vec2 size);
    static inline UIbbox null() { return UIbbox(); }
    UIbbox() : _min(1e30f), _max(-1e30f) {}
private:
    glm::vec2 _min, _max;
    UIbbox(glm::vec2 m, glm::vec2 M) : _min(m), _max(M) {}
};


struct UIelement {
    UIelement(UIbbox const& uv); /** uv bbox (element renders as textured quad w these uvs) */
    UIelement(glm::vec2 size); /** size only (element doesn't render, but can hit-detect w mouse) */
    UIelement(); /** default, disabled */
    virtual ~UIelement() = default;

    UIelement* parent = nullptr;
    glm::vec2 offset = glm::vec2(0.f); /** from parent */

    glm::vec2 size;
    UIbbox uvbbox;
    UIbbox bbox;    /** in global (screen) space, used by renderer */
    UIbbox subtree_bbox; 

    std::vector<UIelement*> children;

    bool disabled; /** disables this node and subtree entirely */
    bool render; /** disables from renderer, still processes node and subtree */

    // Must be implemented by derived elements
    virtual void onUpdate(float dt);
    virtual void onMousePress(Mouse const& mouse);
    virtual void onMouseRelease(Mouse const& mouse);
    virtual void onMouseHoverEnter(Mouse const& mouse);
    virtual void onMouseHoverExit(Mouse const& mouse);
    virtual void draw() const; /** not used anymore */

    // Utilities (non-virtual)
    glm::vec2 get_absolute_pos() const;
    UIelement* hitTest(glm::vec2 mpos);
    void updateSubtreeBBox();
    void addChild(UIelement* child);
    void offsetScaled(glm::vec2 ofs);
    void updateSizeFromUV();
};



typedef abstract_linear_objpool<UIelement, 8192> uialloc_t;

struct UI {
    uialloc_t alloc; /** use to alloc UIelements, do SubType* ptr = alloc.push<SubType>(constructor args...); */

    enum pin_e {
        PIN_TOPLEFT,
        PIN_TOPCENTER,
        PIN_TOPRIGHT,
        PIN_CENTERLEFT,
        PIN_CENTERCENTER,
        PIN_CENTERRIGHT,
        PIN_BOTLEFT,
        PIN_BOTCENTER,
        PIN_BOTRIGHT,
        PIN_LAST
    };

    struct UIRootElement : public UIelement {
        UIRootElement(pin_e pin);
        virtual void onUpdate(float dt) override final;
        inline pin_e getpin() const {return pin;}
    private:
        const pin_e pin;
    };

    UIRootElement roots[PIN_LAST];

    UI();

    void tick(float dt, Mouse const& mouse);
    void draw() const; /** not used */

    inline static void set_guiscale(uint8_t s) {guiscale = s;}
    inline static uint8_t get_guiscale() {return guiscale;}
    inline static float get_guiscalef() {return (float)guiscale;}
private:
    static uint8_t guiscale;
    UIelement* hovered = nullptr;
    UIelement* held = nullptr;

    UIelement* hitTest(glm::vec2 mpos) const;
};

struct UIRenderer {
    static VertexArray vao;
    static VertexBuffer<Vertex_2f> vbo;
    static ElementBuffer ibo;
    static InstanceBuffer<iAttr_2v4> instance;

    static Shader shader;
    static Texture texture;

    static void init(const char* texname);
    static void destroy();
    static void prepare(UI& ui);
    static void render();
private:
    static void prepare_recurse(UIelement& root, UI const& ui, std::vector<iAttr_2v4>& data);
    static size_t n_instances;
};


extern const UIbbox uibb_inventory;
extern const UIbbox uibb_heart;
extern const UIbbox uibb_food;

extern const UIbbox uibb_halfheart;
extern const UIbbox uibb_halffood;
extern const UIbbox uibb_hotbar;
extern const UIbbox uibb_hotbar_select;
extern const UIbbox uibb_butpress;
extern const UIbbox uibb_button;
extern const UIbbox uibb_buthover;
extern const UIbbox uibb_craft;
#include "standard.h"
#include "data/Inventory.h"


struct DispInventorySlot : public UIelement {
    ItemStack stored;
    ItemStack* grabbed;
    DispInventorySlot();
    virtual void draw() const override final;
    virtual void onUpdate(float dt) override;
};

struct InventorySlot : public DispInventorySlot {
    ItemStack* grabbed;
    InventorySlot();
    void tell_grabbed(ItemStack& grabbed);
    virtual void onMousePress(Mouse const& mouse) override final;
    virtual void onMouseRelease(Mouse const& mouse) override final;
    virtual void onMouseHoverEnter(Mouse const& mouse) override final;
    virtual void onMouseHoverExit(Mouse const& mouse) override final;
private:
};

struct InventoryUIelement : public UIelement {


    InventoryUIelement(UI& home);

    InventorySlot slots[SteveInventory::nslots];
    ItemStack grabbed;
    UIelement disp_grabbed;

    void read_inv(SteveInventory& inv);
    void write_inv(SteveInventory& inv);

    virtual void onUpdate(float dt) override final;
    virtual void onMousePress(Mouse const& mouse) override final;
    virtual void onMouseRelease(Mouse const& mouse) override final;
    virtual void onMouseHoverEnter(Mouse const& mouse) override final;
    virtual void onMouseHoverExit(Mouse const& mouse) override final;
};

struct HotbarUIelement : public UIelement {
    DispInventorySlot slots[9];
    UIelement selected; int iselected;
    HotbarUIelement();
    void read_inv(SteveInventory& inv);
    void write_inv(SteveInventory& inv);
    virtual void onUpdate(float dt) override final;
};

struct MinecraftUI : public UI {
    InventoryUIelement inventory;
    HotbarUIelement hotbar;
    MinecraftUI();
    static TextRenderer tr;
    static void init();
    static void destroy();
};

#endif /* UI_H */
