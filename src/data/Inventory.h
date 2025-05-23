/** 
 * Inventory.h 
 * minecraft
 * created 05/03/25 by frank collebrusco
 */
#ifndef INVENTORY_H
#define INVENTORY_H
#include "standard.h"

typedef blockID itemID;

struct ItemStack {
    itemID item{0};
    uint8_t count{0};
    inline bool null() const {return item == 0 || count == 0;}
    inline bool empty() const {return count == 0;}
    inline bool clear() {return count = 0;}
    inline bool full() const {return count == 64;}
};

struct SteveInventory {
    static const size_t nslots = 9*(3+1);
    ItemStack inv[nslots];
    uint8_t selected;
    void add(itemID item, uint8_t ct = 1);
};

typedef SteveInventory c_PlayerInventory;


#endif /* INVENTORY_H */
