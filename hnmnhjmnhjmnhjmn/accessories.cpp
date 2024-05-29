#include "Item.h"


Accessory::Accessory() : Item() {
	this->itemType = SpecialisedType::ACCESSORY;
}

ArmorPiece::ArmorPiece() : Accessory() {

}