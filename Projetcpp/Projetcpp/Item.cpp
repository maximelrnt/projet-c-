#include "Item.h"
#include <iostream>

using namespace std;

void Item::display() const {
    cout << "  - " << m_name << " [" << m_type << "] +" << m_value 
         << " HP | Quantite : " << m_quantity << endl;
}
