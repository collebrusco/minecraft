#include "Inventory.h"

void SteveInventory::add(itemID item, uint8_t ct) {
    for (size_t i = 0; i < nslots; i++) {
        if (inv[i].item == item && !inv[i].full()) {
            inv[i].count++;
            return;
        }
    }
    for (size_t i = 0; i < nslots; i++) {
        if (inv[i].empty()) {
            inv[i].item = item;
            inv[i].count++;
            return;
        }
    }
}
