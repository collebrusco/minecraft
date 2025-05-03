#include "UI.h"
LOG_MODULE(ui);
using namespace glm;

bool UIbbox::inside(glm::vec2 mpos) const {
    return mpos.x >= _min.x && mpos.x <= _max.x &&
           mpos.y >= _min.y && mpos.y <= _max.y;
}

void UIbbox::merge(UIbbox const& other) {
    if (other.degen()) return;
    if (degen()) {
        _min = other._min;
        _max = other._max;
    } else {
        _min = glm::min(_min, other._min);
        _max = glm::max(_max, other._max);
    }
}

bool UIbbox::isect(UIbbox const& other) {
    return !(other._max.x < _min.x || other._min.x > _max.x ||
             other._max.y < _min.y || other._min.y > _max.y);
}

bool UIbbox::contains(UIbbox const& other) {
    return _min.x <= other._min.x && _max.x >= other._max.x &&
           _min.y <= other._min.y && _max.y >= other._max.y;
}

bool UIbbox::degen() const {
    return _min.x > _max.x || _min.y > _max.y;
}

glm::vec2 UIbbox::size() const {
    return _max - _min;
}

glm::vec2 UIbbox::middle() const {
    return (_min + _max) * 0.5f;
}

glm::vec2 const& UIbbox::min() const {
    return _min;
}

glm::vec2 const& UIbbox::max() const {
    return _max;
}

UIbbox UIbbox::from_merge(UIbbox const& a, UIbbox const& b) {
    if (a.degen()) return b;
    if (b.degen()) return a;
    return UIbbox(glm::min(a._min, b._min), glm::max(a._max, b._max));
}

UIbbox UIbbox::from_isect(UIbbox const& a, UIbbox const& b) {
    glm::vec2 new_min = glm::max(a._min, b._min);
    glm::vec2 new_max = glm::min(a._max, b._max);
    return UIbbox(new_min, new_max);
}

UIbbox UIbbox::from_minmax(glm::vec2 min, glm::vec2 max) {
    return UIbbox(min, max);
}

UIbbox UIbbox::from_minsize(glm::vec2 min, glm::vec2 size) {
    return UIbbox(min, min + size);
}









uint8_t UI::guiscale;

UIelement::UIelement(UIbbox const &uv) : disabled(false), render(true) {
    size = uv.size() * 1024.f * UI::get_guiscalef();
    uvbbox = uv;
}

UIelement::UIelement() : disabled(true), render(false) {}

UIelement::UIelement(glm::vec2 sz) : disabled(false), render(false), size(sz * UI::get_guiscalef()) {}

void UIelement::onUpdate(float) {}
void UIelement::onMousePress(Mouse const&) {}
void UIelement::onMouseRelease(Mouse const&) {}
void UIelement::onMouseHoverEnter(Mouse const&) {}
void UIelement::onMouseHoverExit(Mouse const&) {}
void UIelement::draw() const {
}

glm::vec2 UIelement::get_absolute_pos() const {
    return parent ? parent->get_absolute_pos() + offset : offset;
}

UIelement* UIelement::hitTest(glm::vec2 mpos) {
    for (UIelement* child : children) {
        if (UIelement* hit = child->hitTest(mpos)) return hit;
    }
    if (bbox.inside(mpos)) return this;
    return 0;
}

void UIelement::updateSubtreeBBox() {
    if (disabled) return;
    glm::vec2 abs_pos = get_absolute_pos();
    glm::vec2 half = size * 0.5f;
    bbox = UIbbox::from_minmax(abs_pos - half, abs_pos + half);

    subtree_bbox = bbox;
    for (UIelement* c : children) {
        c->updateSubtreeBBox();
        subtree_bbox.merge(c->subtree_bbox);
    }
}

void UIelement::addChild(UIelement *child) {
    this->children.push_back(child);
    child->parent = this;
}

void UIelement::offsetScaled(glm::vec2 ofs) {
    this->offset += (ofs * UI::get_guiscalef());
}

UI::UI()
: roots{
    UIRootElement(PIN_TOPLEFT),
    UIRootElement(PIN_TOPCENTER),
    UIRootElement(PIN_TOPRIGHT),
    UIRootElement(PIN_CENTERLEFT),
    UIRootElement(PIN_CENTERCENTER),
    UIRootElement(PIN_CENTERRIGHT),
    UIRootElement(PIN_BOTLEFT),
    UIRootElement(PIN_BOTCENTER),
    UIRootElement(PIN_BOTRIGHT)
} 
{
    guiscale = 3;
}
void UI::tick(float dt, Mouse const& mouse) {
    for (UIRootElement& r : roots) {
        r.onUpdate(dt);
        r.updateSubtreeBBox();
        std::vector<UIelement*> stack{&r};
        while (!stack.empty()) {
            UIelement* e = stack.back(); stack.pop_back();
            if (e->disabled) continue;
            e->onUpdate(dt);
            for (UIelement* c : e->children)
                stack.push_back(c);
        }
    }
    

    // Update hover state
    glm::vec2 flipped_mouse_pos = { mouse.pos.x, window.frame.y - mouse.pos.y };
    UIelement* now_hovered = hitTest(flipped_mouse_pos);
    if (hovered != now_hovered) {
        if (hovered) hovered->onMouseHoverExit(mouse);
        if (now_hovered) now_hovered->onMouseHoverEnter(mouse);
        hovered = now_hovered;
    }

    // Handle mouse press/release
    if (mouse.left().pressed && hovered)  hovered->onMousePress(mouse);
    if (mouse.left().released && hovered) hovered->onMouseRelease(mouse);
}

void UI::draw() const {
    for (const UIRootElement& r : roots) {
        std::vector<UIelement const*> stack{&r};
        while (!stack.empty()) {
            UIelement const* e = stack.back(); stack.pop_back();
            e->draw();
            for (auto* c : e->children) stack.push_back(c);
        }
    }
}

UIelement* UI::hitTest(glm::vec2 mpos) const {
    for (const UIRootElement& root : roots) {
        if (UIelement* hit = const_cast<UIRootElement&>(root).hitTest(mpos)) return hit;
    }
    return nullptr;
}

UI::UIRootElement::UIRootElement(pin_e p) : pin(p) {
    assert(p >= 0 && p < PIN_LAST);
    render = false; disabled = false;
    parent = nullptr;
    offset = glm::vec2(0.f);
    bbox = UIbbox::from_minmax(offset, offset);
    subtree_bbox = bbox;
}

void UI::UIRootElement::onUpdate(float) {
    glm::vec2 win = glm::vec2(window.frame);
    switch (pin) {
        case PIN_TOPLEFT:       offset = {0.f, win.y}; break;
        case PIN_TOPCENTER:     offset = {win.x * 0.5f, win.y}; break;
        case PIN_TOPRIGHT:      offset = {win.x, win.y}; break;
        case PIN_CENTERLEFT:    offset = {0.f, win.y * 0.5f}; break;
        case PIN_CENTERCENTER:  offset = {win.x * 0.5f, win.y * 0.5f}; break;
        case PIN_CENTERRIGHT:   offset = {win.x, win.y * 0.5f}; break;
        case PIN_BOTLEFT:       offset = {0.f, 0.f}; break;
        case PIN_BOTCENTER:     offset = {win.x * 0.5f, 0.f}; break;
        case PIN_BOTRIGHT:      offset = {win.x, 0.f}; break;
        default: break;
    }
}

VertexArray UIRenderer::vao;
VertexBuffer<Vertex_2f> UIRenderer::vbo;
ElementBuffer UIRenderer::ibo;
InstanceBuffer<iAttr_2v4> UIRenderer::instance;
size_t UIRenderer::n_instances;

Shader UIRenderer::shader;
Texture UIRenderer::texture;

void UIRenderer::init(const char* texname) {
    const Vertex_2f verts[] = {
        {0.f,0.f},
        {0.f,1.f},
        {1.f,1.f},
        {1.f,0.f}
    };
    const uint32_t elems[] = {
        0,2,1,
        0,3,2
    };
    shader = Shader::from_source("ui_vert", "ui_frag");
    shader.unbind();
    if (texname) {
        texture = Texture::from_file(texname);
        texture.pixelate();
    }
    vao.create_bind();
    vbo.create_bind();
    vao.attach(vbo);
    vbo.buffer_data(4, verts);
    ibo.create_bind();
    ibo.buffer_data(6, elems);
    instance.create_bind();
    vao.attach(instance);
    vao.unbind();
    vbo.unbind();
    ibo.unbind();
    instance.unbind();
}

void UIRenderer::destroy() {
    vao.destroy();
    vbo.destroy();
    ibo.destroy();
    instance.destroy();
    shader.destroy();
    texture.destroy();
}

void UIRenderer::prepare(UI& ui) {
    std::vector<iAttr_2v4> instance_data;

    for (size_t r = 0; r < UI::PIN_LAST; r++) {
        prepare_recurse(ui.roots[r], ui, instance_data);
    }

    instance.bind();
    instance.buffer(instance_data);
    n_instances = instance_data.size();
}

void UIRenderer::prepare_recurse(UIelement& root, UI const &ui, std::vector<iAttr_2v4>& data) {
    if (root.disabled) return;
    if (root.render) {
        vec2 vmin, vmax;
        vmin = root.bbox.min() / (vec2)window.frame;
        vmax = root.bbox.max() / (vec2)window.frame;
        data.push_back({{vmin.x, vmin.y, vmax.x, vmax.y},
                        {root.uvbbox.min().x, root.uvbbox.min().y, root.uvbbox.max().x, root.uvbbox.max().y}});
    }
    for (UIelement* c : root.children) {
        prepare_recurse(*c, ui, data);
    }
}

void UIRenderer::render() {
    vao.bind();
    shader.bind();
    texture.bind();
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    gl.draw_vao_ibo_instanced(ibo, n_instances);
}









static glm::vec2 touv(glm::ivec2 pix, glm::vec2 squeeze = {0.f,0.f}) {
    return (((glm::vec2)pix) + squeeze) / 1024.f;
}




const UIbbox uibb_inventory  = UIbbox::from_minmax(touv({0,858}), touv({176,1024}));

const UIbbox uibb_heart = UIbbox::from_minmax(touv({0,849}), touv({9,858}));

const UIbbox uibb_food = UIbbox::from_minmax(touv({0,839}), touv({9,848}));


const UIbbox uibb_halfheart = UIbbox::from_minmax(touv({9,849}), touv({18,858}));

const UIbbox uibb_halffood = UIbbox::from_minmax(touv({9,839}), touv({18,848}));

const UIbbox uibb_hotbar = UIbbox::from_minsize(touv({176,1002}), touv({182,22}));

const UIbbox uibb_hotbar_select = UIbbox::from_minsize(touv({177,979}), touv({22,22}));

const UIbbox uibb_butpress = UIbbox::from_minsize(touv({177,958}), touv({200,20}));

const UIbbox uibb_button = UIbbox::from_minsize(touv({177,938}), touv({200,20}));

const UIbbox uibb_buthover = UIbbox::from_minsize(touv({177,918}), touv({200,20}));

const UIbbox uibb_craft = UIbbox::from_minsize(touv({728,858}), touv({176,166}));









InventorySlot::InventorySlot() : UIelement(uibb_hotbar_select) {
    render = false;
}

void InventorySlot::onMouseHoverEnter(Mouse const& mouse) {
    render = true;
}

void InventorySlot::onMouseHoverExit(Mouse const& mouse) {
    render = false;
}



InventoryUIelement::InventoryUIelement(UI& home) : UIelement(uibb_inventory) {
    render = true;
    for (size_t j = 0; j < 3; j++) {
        for (size_t i = 0; i < 9; i++) {
            UIelement* slot = home.alloc.push<InventorySlot>();
            float x = -72.f + ((float)i * 18.f);
            slot->offsetScaled(glm::vec2(x, -9.f - ((float)j * 18.f)));
            this->addChild(slot);
        }
    }
    for (size_t i = 0; i < 9; i++) {
        UIelement* slot = home.alloc.push<InventorySlot>();
        float x = -72.f + ((float)i * 18.f);
        slot->offsetScaled(glm::vec2(x, -9.f - 4.f - (3.f * 18.f)));
        this->addChild(slot);
    }
}

void InventoryUIelement::onUpdate(float dt) {

}

void InventoryUIelement::onMousePress(Mouse const& mouse) {

}

void InventoryUIelement::onMouseRelease(Mouse const& mouse) {

}

void InventoryUIelement::onMouseHoverEnter(Mouse const& mouse) {

}

void InventoryUIelement::onMouseHoverExit(Mouse const& mouse) {

}











MinecraftUI::MinecraftUI() : UI(), inventory(*this) {
    roots[PIN_CENTERCENTER].addChild(&inventory);
}



