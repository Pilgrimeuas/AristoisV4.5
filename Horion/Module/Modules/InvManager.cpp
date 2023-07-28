#include "InvManager.h"
#include "../ModuleManager.h"

class ArmorStruct {
public:
	ArmorStruct(C_ItemStack* item, C_ArmorItem* yot, int slot) {
		armor = yot;
		m_slot = slot;
		m_item = item;
	}
	bool isEqual(ArmorStruct& src) {
		if (this->m_item->getArmorValueWithEnchants() == src.m_item->getArmorValueWithEnchants())
			return true;
		else
			return false;
	}

	bool operator()(ArmorStruct lhs, ArmorStruct rhs) {
		return (lhs.m_item->getArmorValueWithEnchants() > rhs.m_item->getArmorValueWithEnchants());
	}
	C_ArmorItem* armor = nullptr;
	C_ItemStack* m_item = nullptr;
	int m_slot = 0;
};

using namespace std;
InvManager::InvManager() : IModule(0, Category::PLAYER, "Manages your inventory") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry(EnumEntry("Normal", 0));
	mode.addEntry(EnumEntry("Inventory", 1));
	// registerBoolSetting("AutoDisable", &autoDisable, autoDisable);
	registerBoolSetting("Clean", &clean, clean);
	registerBoolSetting("Sort", &autoSort, autoSort);
	registerBoolSetting("Swing", &swing, swing);
	registerIntSetting("Sword", &swordSlot, swordSlot, 1, 9);
	registerIntSetting("Pickaxe", &pickSlot, pickSlot, 1, 9);
	registerIntSetting("Axe", &axeSlot, axeSlot, 1, 9);
	registerIntSetting("Blocks", &blockSlot, blockSlot, 1, 9);
	registerIntSetting("Crystal", &crystalSlot, crystalSlot, 1, 9);
	registerIntSetting("Delay", &delay, delay, 0, 30);
}
const char* InvManager::getRawModuleName() {
	return "InvManager";
}

const char* InvManager::getModuleName() {
	name = string("InvManager ") + string(GRAY) + to_string((int)delay);
	return name.c_str();
}

void InvManager::onLevelRender() {
	// if we ever need
}

void InvManager::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (!player->canOpenContainerScreen() && mode.getSelectedValue() == 0) return;
	if (player->canOpenContainerScreen() && mode.getSelectedValue() == 1) return;

	C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;

	static C_ItemStack* emptyItemStack = nullptr;
	C_InventoryAction* first = nullptr;
	C_InventoryAction* second = nullptr;

	// Drop items
	if (clean) {
		dropSlots = findUselessItems();
		Odelay++;
		if (Odelay > delay) {
			if (!dropSlots.empty()) {
				for (int i : dropSlots) {
					player->getSupplies()->inventory->dropSlot(i);
					dropSlots.push_back(i);
				}
			}
		}
	}

	if (autoSort) {
		float SwordDamage = 0;
		float PickaxeDamage = 0;
		float AxeDamage = 0;
		int BlockCount = 0;

		int Sword = swordSlot - 1;
		int Pickaxe = pickSlot - 1;
		int Axe = axeSlot - 1;
		int Block = blockSlot - 1;
		int Crystal = crystalSlot - 1;

		for (int n = 0; n < 36; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			C_ItemStack* CAtest = inv->getItemStack(Crystal);
			if (stack->item != nullptr) {
				float currentDamage = stack->getAttackingDamageWithEnchants();
				float blockCount = stack->count;
				if (stack->getItem()->isSword() && currentDamage > SwordDamage) {
					SwordDamage = currentDamage;
					Sword = n;
				}
				if (stack->getItem()->isPickaxe() && currentDamage > PickaxeDamage) {
					PickaxeDamage = currentDamage;
					Pickaxe = n;
				}
				if (stack->getItem()->isAxe() && currentDamage > AxeDamage) {
					AxeDamage = currentDamage;
					Axe = n;
				}
				if (stack->getItem()->isBlock() && blockCount > BlockCount) {
					BlockCount = blockCount;
					Block = n;
				}
				if (stack->getItem()->itemId == 637) {
					Crystal = n;
				}
			}
		}

		if (Sword != swordSlot - 1) inv->swapSlots(Sword, swordSlot - 1);
		if (Pickaxe != pickSlot - 1) inv->swapSlots(Pickaxe, pickSlot - 1);
		if (Axe != axeSlot - 1) inv->swapSlots(Axe, axeSlot - 1);
		if (Block != blockSlot - 1) inv->swapSlots(Block, blockSlot - 1);
		if (Crystal != crystalSlot - 1) inv->swapSlots(Crystal, crystalSlot - 1);
	}

	if (autoDisable && g_Data.getLocalPlayer() == nullptr) {
		// auto notification = g_Data.addNotification("InvManager:", "Disabled");
		// notification->duration = 3;
		setEnabled(false);
	}
}

vector<int> InvManager::findStackableItems() {
	vector<int> stackableSlot;

	for (int i = 0; i < 36; i++) {
		C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemStack->item != nullptr) {
			if ((*itemStack->item)->getMaxStackSize(0) > itemStack->count) {
				for (int i2 = 0; i2 < 36; i2++) {
					if (i2 == i) continue;
					C_ItemStack* itemStack2 = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i2);
					if ((*itemStack2->item)->getMaxStackSize(0) > itemStack->count) {
						if (*itemStack->item == *itemStack2->item) {
							if ((find(stackableSlot.begin(), stackableSlot.end(), i2) == stackableSlot.end())) stackableSlot.push_back(i2);
							if ((find(stackableSlot.begin(), stackableSlot.end(), i) == stackableSlot.end())) stackableSlot.push_back(i);
						}
					}
				}
			}
		}
	}

	return stackableSlot;
}

vector<int> InvManager::findUselessItems() {
	// Filter by options

	vector<int> uselessItems;
	vector<C_ItemStack*> items;

	{
		for (int i = 0; i < 36; i++) {
			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr) {
				if (!stackIsUseful(itemStack)) {
					if (find(items.begin(), items.end(), itemStack) == items.end())
						uselessItems.push_back(i);
					else
						items.push_back(itemStack);
				} else if (find(items.begin(), items.end(), itemStack) == items.end()) {
					if ((*itemStack->item)->itemId == 261 && !isLastItem(*itemStack->item))
						uselessItems.push_back(i);
					else
						items.push_back(itemStack);
				}
			}
		}

		for (int i = 0; i < 4; i++) {
			if (g_Data.getLocalPlayer()->getArmor(i)->item != nullptr)
				items.push_back(g_Data.getLocalPlayer()->getArmor(i));
		}
	}

	// Filter Swords
	if (items.size() > 0) {
		// Filter by attack damage
		sort(items.begin(), items.end(), [](const C_ItemStack* lhs, const C_ItemStack* rhs) {
			C_ItemStack* current = const_cast<C_ItemStack*>(lhs);
			C_ItemStack* other = const_cast<C_ItemStack*>(rhs);
			return current->getAttackingDamageWithEnchants() > other->getAttackingDamageWithEnchants();
		});

		bool hadTheBestItem = false;
		C_ItemStack* bestItem = items.at(0);

		for (int i = 0; i < 36; i++) {  // 36
			if (find(uselessItems.begin(), uselessItems.end(), i) != uselessItems.end())
				continue;
			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr) {
				if (!itemStack->getItem()->isPickaxe() && !itemStack->getItem()->isTool() && !itemStack->getItem()->isFood() && !itemStack->getItem()->isAxe() && !itemStack->getItem()->isBlock() && itemStack->getAttackingDamageWithEnchants() < bestItem->getAttackingDamageWithEnchants()) {
					uselessItems.push_back(i);
				}
			}
		}
	}

	// Filter tools
	if (items.size() > 0) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		float SwordDamage = 0;
		float PickaxeDamage = 0;
		float AxeDamage = 0;

		for (int n = 0; n < 36; n++) {
			if (find(uselessItems.begin(), uselessItems.end(), n) != uselessItems.end())
				continue;
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				float currentDamage = stack->getAttackingDamageWithEnchants();
				// Sword
				if (stack->getItem()->isSword() && currentDamage > SwordDamage)
					SwordDamage = currentDamage;
				else if (stack->getItem()->isSword())
					uselessItems.push_back(n);
				// Pickaxe
				if (stack->getItem()->isPickaxe() && currentDamage > PickaxeDamage)
					PickaxeDamage = currentDamage;
				else if (stack->getItem()->isPickaxe())
					uselessItems.push_back(n);
				// Axe
				if (stack->getItem()->isAxe() && currentDamage > AxeDamage)
					AxeDamage = currentDamage;
				else if (stack->getItem()->isAxe())
					uselessItems.push_back(n);
			}
		}
	}

	// Filter armor
	{
		vector<C_ItemStack*> helmets;
		vector<C_ItemStack*> chestplates;
		vector<C_ItemStack*> leggings;
		vector<C_ItemStack*> boots;

		// Filter by armor value
		sort(items.begin(), items.end(), [](const C_ItemStack* lhs, const C_ItemStack* rhs) {
			C_ItemStack* current = const_cast<C_ItemStack*>(lhs);
			C_ItemStack* other = const_cast<C_ItemStack*>(rhs);
			return current->getArmorValueWithEnchants() > other->getArmorValueWithEnchants();
		});

		// Put armor items in their respective vectors
		for (C_ItemStack* itemsteck : items) {
			C_Item* item = itemsteck->getItem();
			if (item->isArmor()) {
				C_ArmorItem* armorItem = reinterpret_cast<C_ArmorItem*>(item);
				if (armorItem->isHelmet())
					helmets.push_back(itemsteck);
				else if (armorItem->isChestplate())
					chestplates.push_back(itemsteck);
				else if (armorItem->isLeggings())
					leggings.push_back(itemsteck);
				else if (armorItem->isBoots())
					boots.push_back(itemsteck);
			}
		}
		bool hadBest[4] = {0, 0, 0, 0};
		for (int i = 0; i < 4; i++) {
			C_ItemStack* itemsteck = g_Data.getLocalPlayer()->getArmor(i);
			C_Item** item = itemsteck->item;
			if (item != nullptr) {
				C_ArmorItem* armor = reinterpret_cast<C_ArmorItem*>(*item);

				float testArmorValue = 0;
				switch (armor->getArmorSlot()) {
				case 0:
					if (helmets.size() > 0)
						testArmorValue = helmets.at(0)->getArmorValueWithEnchants();
					break;
				case 1:
					if (chestplates.size() > 0)
						testArmorValue = chestplates.at(0)->getArmorValueWithEnchants();
					break;
				case 2:
					if (leggings.size() > 0)
						testArmorValue = leggings.at(0)->getArmorValueWithEnchants();
					break;
				case 3:
					if (boots.size() > 0)
						testArmorValue = boots.at(0)->getArmorValueWithEnchants();
					break;
				}
				if (itemsteck->getArmorValueWithEnchants() >= testArmorValue)
					hadBest[armor->getArmorValue()] = true;
			}
		}

		for (int i = 0; i < 36; i++) {  // 36
			if (find(uselessItems.begin(), uselessItems.end(), i) != uselessItems.end())
				continue;  // item already useless
			C_ItemStack* itemStack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
			if (itemStack->item != nullptr && (*itemStack->item)->getArmorValue()) {
				C_ArmorItem* armor = reinterpret_cast<C_ArmorItem*>(*itemStack->item);
				if (armor->isHelmet()) {
					if (hadBest[0] || itemStack->getArmorValueWithEnchants() < helmets.at(0)->getArmorValueWithEnchants()) {
						uselessItems.push_back(i);
					} else
						hadBest[0] = true;
				} else if (armor->isChestplate()) {
					if (hadBest[1] || itemStack->getArmorValueWithEnchants() < chestplates.at(0)->getArmorValueWithEnchants())
						uselessItems.push_back(i);
					else
						hadBest[1] = true;
				} else if (armor->isLeggings()) {
					if (hadBest[2] || itemStack->getArmorValueWithEnchants() < leggings.at(0)->getArmorValueWithEnchants())
						uselessItems.push_back(i);
					else
						hadBest[2] = true;
				} else if (armor->isBoots()) {
					if (hadBest[3] || itemStack->getArmorValueWithEnchants() < boots.at(0)->getArmorValueWithEnchants())
						uselessItems.push_back(i);
					else
						hadBest[3] = true;
				}
			}
		}
	}

	return uselessItems;
}

bool InvManager::stackIsUseful(C_ItemStack* itemStack) {
	if (itemStack->item == nullptr) return true;
	if ((*itemStack->item)->isWeapon()) return true;   // Weapon
	if ((*itemStack->item)->isPickaxe()) return true;  // dickaxe
	if ((*itemStack->item)->isArmor()) return true;    // Armor
	if ((*itemStack->item)->isTool()) return true;     // Tools
	if ((*itemStack->item)->isFood()) return true;     // Food
	if ((*itemStack->item)->isBlock()) return true;    // Block
	return false;
}

bool InvManager::isLastItem(C_Item* item) {
	vector<C_Item*> items;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (stack->item != nullptr) items.push_back((*stack->item));
	}
	int count = 0;
	for (C_Item* a : items) {
		if (a == item) count++;
	}
	if (count > 1) return false;
	return true;
}
