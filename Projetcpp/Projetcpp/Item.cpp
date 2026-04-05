#include "Item.h"
#include <iostream>

using namespace std;

void Item::display() const {
    cout << "  - " << this->m_name << " [" << this->m_type << "] +" << this->m_value
        << " HP | Quantite : " << this->m_quantity << endl;
}