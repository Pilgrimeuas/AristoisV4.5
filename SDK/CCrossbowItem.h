#pragma once
#include "CItem.h"

class C_ItemStack;
class C_Inventory;
class C_Player;

class C_CrossbowItem {
public:
	virtual void `vftable'(); private : virtual void function_1();

public:
	virtual void CrossbowItem::CrossbowItem(std::string const& a1, int a2, +13¡üo a3);
	virtual void `scalar deleting destructor'(uint a1); virtual void initServer(Json::Value & a1, SemVersion const& a2);

private:
	virtual void function_5();

public:
	virtual void getMaxUseDuration(ItemStack const* a1);
	virtual void getMaxUseDuration(ItemInstance const* a1);

private:
	virtual void function_8();
	virtual void function_9();
	virtual void function_10();
	virtual void function_11();
	virtual void function_12();
	virtual void function_13();

public:
	virtual void isCamera();

private:
	virtual void function_15();

public:
	virtual void isDamageable();

private:
	virtual void function_17();
	virtual void function_18();

public:
	virtual void getType();

private:
	virtual void function_20();

public:
	virtual void isFood();

private:
	virtual void function_22();
	virtual void function_23();
	virtual void function_24();

public:
	virtual void getCamera();
	virtual void getFood();

private:
	virtual void function_27();

public:
	virtual void setMaxStackSize(unsigned char a1);
	virtual void setStackedByData(bool a1);
	virtual void setMaxDamage(int a1);
	virtual void setHandEquipped();
	virtual void setUseAnimation(UseAnimation a1);
	virtual void setMaxUseDuration(int a1);
	virtual void setRequiresWorldBuilder(bool a1);
	virtual void setExplodable(bool a1);
	virtual void setFireResistant(bool a1);
	virtual void setIsGlint(bool a1);
	virtual void setShouldDespawn(bool a1);
	virtual void buildNetworkTag();
	virtual void initializeFromNetwork(CompoundTag const& a1);
	virtual void getResourceCount(Randomize & a1, Block const& a2, int a3);
	virtual void canBeDepleted();

private:
	virtual void function_43();
	virtual void function_44();

public:
	virtual void isStackedByData();
	virtual void getMaxDamage();

private:
	virtual void function_47();

public:
	virtual void isHandEquipped();
	virtual void isGlint(ItemStackBase const& a1);

private:
	virtual void function_50();
	virtual void function_51();
	virtual void function_52();
	virtual void function_53();
	virtual void function_54();
	virtual void function_55();
	virtual void function_56();
	virtual void function_57();
	virtual void function_58();

public:
	virtual void appendFormattedHovertext(ItemStackBase const& a1, Level& a2, std::string& a3, bool a4);

private:
	virtual void function_60();

public:
	virtual void getEnchantSlot();

private:
	virtual void function_62();
	virtual void function_63();
	virtual void function_64();
	virtual void function_65();

public:
	virtual void getDamageChance(int a1);

private:
	virtual void function_67();
	virtual void function_68();
	virtual void function_69();
	virtual void function_70();

public:
	virtual void getDustColor(Block const& a1);

private:
	virtual void function_72();
	virtual void function_73();
	virtual void function_74();
	virtual void function_75();
	virtual void function_76();

public:
	virtual void getDustColor(Block const& a1);
	virtual void getDustColor(Block const& a1);
	virtual void getActorIdentifier(ItemStack const& a1);

private:
	virtual void function_80();
	virtual void function_81();

public:
	virtual void buildIdAux(short a1, CompoundTag const* a2);

private:
	virtual void function_83();

public:
	virtual void use(ItemStack & a1, Player & a2);
	virtual void dispense(BlockSource & a1, Container & a2, int a3, Vector3<float> const& a4, unsigned char a5);
	virtual void useTimeDepleted(ItemStack & a1, Level * a2, Player * a3);
	virtual void releaseUsing(ItemStack & a1, Player * a2, int a3);

private:
	virtual void function_88();
	virtual void function_89();
	virtual void function_90();
	virtual void function_91();
	virtual void function_92();

public:
	virtual void mineBlock(ItemInstance & a1, Block const& a2, int a3, int a4, int a5, Actor* a6);
	virtual void buildDescriptionName(ItemStackBase const& a1);
	virtual void buildDescriptionId(ItemDescriptor const& a1, CompoundTag const* a2);
	virtual void buildEffectDescriptionName(ItemStackBase const& a1);
	virtual void buildCategoryDescriptionName();
	virtual void readUserData(ItemStackBase & a1, IDataInput & a2, ReadOnlyBinaryStream & a3);
	virtual void writeUserData(ItemStackBase const& a1, IDataOutput& a2);
	virtual void getMaxStackSize(ItemDescriptor const& a1);

private:
	virtual void function_101();
	virtual void function_102();

public:
	virtual void getCooldownType();
	virtual void getCooldownTime();
	virtual void fixupCommon(ItemStackBase & a1, Level & a2);
	virtual void fixupCommon(ItemStackBase & a1);
	virtual void getDamageValue(CompoundTag const* a1);
	virtual void setDamageValue(ItemStackBase & a1, short a2);
	virtual void getInHandUpdateType(Player const& a1, ItemStack const& a2, ItemStack const& a3, bool a4, bool a5);
	virtual void getInHandUpdateType(Player const& a1, ItemStack const& a2, ItemStack const& a3, bool a4, bool a5);

private:
	virtual void function_111();

public:
	virtual void isSameItem(ItemStackBase const& a1, ItemStackBase const& a2);
	virtual void initClient(Json::Value & a1, SemVersion const& a2);
	virtual void getInteractText(Player const& a1);
	virtual void getAnimationFrameFor(Mob * a1, bool a2, ItemStack const* a3, bool a4);
	virtual void isEmissive(int a1);
	virtual void getLightEmission(int a1);
	virtual void getIcon(ItemStackBase const& a1, int a2, bool a3);

private:
	virtual void function_119();

public:
	virtual void setIcon(TextureUVCoordinateSet const& a1);
	virtual void setIcon(std::string const& a1, int a2);
	virtual void setIconAtlas(std::string const& a1, int a2);

private:
	virtual void function_123();

public:
	virtual void playSoundIncrementally(ItemStack const& a1, Mob& a2);

private:
	virtual void function_125();

public:
	virtual void getFurnaceBurnIntervalMultipler(ItemStackBase const& a1);
	virtual void getFurnaceXPmultiplier(ItemStackBase const& a1);
	virtual void getAuxValuesDescription();
	virtual void _checkUseOnPermissions(Actor & a1, ItemStackBase & a2, unsigned char const& a3, Vector3<int> const& a4);

private:
	virtual void function_130();

public:
	virtual void _useOn(ItemStack & a1, Actor & a2, Vector3<int> a3, unsigned char a4, Vector3<float> const& a5);

private:
	virtual void function_132();

public:
	virtual void CrossbowItem::setIcon(std::string const& a1, int a2, +4E¡üo a3);

private:
	virtual void function_134();
}
