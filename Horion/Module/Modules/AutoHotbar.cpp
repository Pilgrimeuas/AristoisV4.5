#include"AutoHotbar.h"

using namespace std;
AutoHotbar::AutoHotbar() : IModule(0, Category::PLAYER, "Auto put crystal in hotbar") {
	registerBoolSetting("Sword", &Swordhb, Swordhb);
	registerBoolSetting("Pickaxe", &Pickaxehb, Pickaxehb);
	registerBoolSetting("Obsidian", &obsidianhb, obsidianhb);
	registerBoolSetting("Anvil", &anvilhb, anvilhb);
	registerBoolSetting("Crystal", &crystalhb, crystalhb);
	registerBoolSetting("Gapple", &gapplehb, gapplehb);
	registerIntSetting("Sword slot", &SwordSlot, SwordSlot, 1, 9);
	registerIntSetting("Pickaxe slot", &PickaxeSlot, PickaxeSlot, 1, 9);
	registerIntSetting("Obsidian slot", &obsidianSlot, obsidianSlot, 1, 9);
	registerIntSetting("Anvil slot", &anvilSlot, anvilSlot, 1, 9);
	registerIntSetting("Crystal slot", &crystalSlot, crystalSlot, 1, 9);
	registerIntSetting("Gapple slot", &gappleSlot, gappleSlot, 1, 9);
	registerBoolSetting("No delay", &Nodelay, Nodelay);
}
const char* AutoHotbar::getRawModuleName() {
	return "AutoHotbar";
}

const char* AutoHotbar::getModuleName() {
	name = string("AutoHotbar");
	return name.c_str();
}

void AutoHotbar::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int realgappleSlot = gappleSlot - 1;
	int realobsidianSlot = obsidianSlot - 1;
	int realcrystalSlot = crystalSlot - 1;
	int realanvilSlot = anvilSlot - 1;
	int realswordSlot = SwordSlot - 1;
	int realpickaxeSlot = PickaxeSlot - 1;

	dasword = false;
	dapickaxe = false;
	daob = false;
	daanvil = false;
	dacry = false;
	dagapple = false;

	for (int n = 0; n < 36; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (Swordhb) {
			if (!dasword) {
				C_ItemStack* obsidianitem = inv->getItemStack(realswordSlot);
				if (obsidianitem->item != nullptr) {
					if (obsidianitem->getItem()->itemId != 316) {
						if (stack->item != nullptr) {
							if (stack->getItem()->itemId == 316) {
								if (!Nodelay) {
									inv->swapSlots(n, realswordSlot);
								} else {
									inv->setItem(realswordSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								dasword = true;
							}
						}
					}
				} else {
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 316) {
							if (!Nodelay) {
								inv->swapSlots(n, realswordSlot);
							} else {
								inv->setItem(realswordSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							dasword = true;
						}
					}
				}
			}
		}
		if (Pickaxehb) {
			if (!dapickaxe) {
				C_ItemStack* obsidianitem = inv->getItemStack(realpickaxeSlot);
				if (obsidianitem->item != nullptr) {
					if (obsidianitem->getItem()->itemId != 318) {
						if (stack->item != nullptr) {
							if (stack->getItem()->itemId == 318) {
								if (!Nodelay) {
									inv->swapSlots(n, realpickaxeSlot);
								} else {
									inv->setItem(realpickaxeSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								dapickaxe = true;
							}
						}
					}
				} else {
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 318) {
							if (!Nodelay) {
								inv->swapSlots(n, realpickaxeSlot);
							} else {
								inv->setItem(realpickaxeSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							dapickaxe = true;
						}
					}
				}
			}
		}
		if (obsidianhb) {
			if (!daob) {
				C_ItemStack* obsidianitem = inv->getItemStack(realobsidianSlot);
				if (obsidianitem->item != nullptr) {
					if (obsidianitem->getItem()->itemId != 49) {
						if (stack->item != nullptr) {
							if (stack->getItem()->itemId == 49) {
								if (!Nodelay) {
									inv->swapSlots(n, realobsidianSlot);
								} else {
									inv->setItem(realobsidianSlot, *stack);
									inv->removeItem(n, stack->count);
								}

								// inv->swapSlots(n, realobsidianSlot);
								daob = true;
							}
						}
					}
				} else {
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 49) {
							if (!Nodelay) {
								inv->swapSlots(n, realobsidianSlot);
							} else {
								inv->setItem(realobsidianSlot, *stack);
								inv->removeItem(n, stack->count);
							}

							// inv->swapSlots(n, realobsidianSlot);
							daob = true;
						}
					}
				}
			}
		}
		if (anvilhb) {
			if (!daanvil) {
				C_ItemStack* anvilitem = inv->getItemStack(realanvilSlot);
				if (anvilitem->item != nullptr) {
					if (anvilitem->getItem()->itemId != 145) {
						if (stack->item != nullptr) {
							if (stack->getItem()->itemId == 145) {
								if (!Nodelay) {
									inv->swapSlots(n, realanvilSlot);
								} else {
									inv->setItem(realanvilSlot, *stack);
									inv->removeItem(n, stack->count);
								}

								// inv->swapSlots(n, realobsidianSlot);
								daanvil = true;
							}
						}
					}
				} else {
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 145) {
							if (!Nodelay) {
								inv->swapSlots(n, realanvilSlot);
							} else {
								inv->setItem(realanvilSlot, *stack);
								inv->removeItem(n, stack->count);
							}

							// inv->swapSlots(n, realobsidianSlot);
							daanvil = true;
						}
					}
				}
			}
		}
		if (crystalhb) {
			if (!dacry) {
				C_ItemStack* crystalitem = inv->getItemStack(realcrystalSlot);
				if (crystalitem->item != nullptr) {
					if (crystalitem->getItem()->itemId != 637) {
						if (stack->item != nullptr) {
							if (stack->getItem()->itemId == 637) {
								if (!Nodelay) {
									inv->swapSlots(n, realcrystalSlot);
								} else {
									inv->setItem(realcrystalSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								dacry = true;
							}
						}
					}
				} else {
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 637) {
							if (!Nodelay) {
								inv->swapSlots(n, realcrystalSlot);
							} else {
								inv->setItem(realcrystalSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							dacry = true;
						}
					}
				}
			}
		}
		if (gapplehb) {
			if (!dagapple) {
				C_ItemStack* gappleitem = inv->getItemStack(realgappleSlot);
				if (gappleitem->item != nullptr) {
					if (gappleitem->getItem()->itemId != 259) {
						if (stack->item != nullptr) {
							if (stack->getItem()->itemId == 259) {
								if (!Nodelay) {
									inv->swapSlots(n, realgappleSlot);
								} else {
									inv->setItem(realgappleSlot, *stack);
									inv->removeItem(n, stack->count);
								}
								dagapple = true;
							}
						}
					}
				} else {
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 259) {
							if (!Nodelay) {
								inv->swapSlots(n, realgappleSlot);
							} else {
								inv->setItem(realgappleSlot, *stack);
								inv->removeItem(n, stack->count);
							}
							dagapple = true;
						}
					}
				}
			}
		}
	}
}
