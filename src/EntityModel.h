/** 
 * EntityModel.h 
 * minecraft
 * created 04/29/25 by frank collebrusco
 */
#ifndef ENTITY_MODEL_H
#define ENTITY_MODEL_H
#include "standard.h"

struct ModelPart {
    glm::mat4 mat;
    struct {
        glm::vec2 mi;
        glm::vec2 mx;
    } uvs[ORIENTATION_LAST + 1];
};

typedef std::vector<ModelPart> Model; /** placehold with a typedef, can become a struct later if needed */

struct EntityModel {
    virtual ~EntityModel() = default; /** base classes have to have virtual destructors in order to call child destructor from base reference */
    virtual Model get() const = 0; /** = 0 means abstract, const means we won't change members and can call this method from a const& */
};

typedef glm::vec2 steve_skin_t; /** maybe this is like the bot-left of a given skin on a sheet of skins */

struct SteveModel : public EntityModel {
    inline float getT() const {return t;}
    inline void setT(float nt) {t = nt;}

    void setSkin(const steve_skin_t skin);

    virtual Model get() const override final; /** final meaning no other class will inherit and override this */
private:
    float t;
};

struct CreeperModel : public EntityModel {
    inline float getT() const {return t;}
    inline void setT(float nt) {t = nt;}

    void setSkin(const steve_skin_t skin);

    virtual Model get() const override final; /** final meaning no other class will inherit and override this */
private:
    float t;    
};

#endif /* ENTITY_MODEL_H */
