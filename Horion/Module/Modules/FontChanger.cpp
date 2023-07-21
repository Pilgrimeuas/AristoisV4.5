#include "FontChanger.h"
FontChanger::FontChanger() : IModule(0, Category::HUD, "Font Changer") {
	registerEnumSetting("Fonts", &Fonts, 0);
	Fonts = SettingEnum(this)
				.addEntry(EnumEntry("NotoSans", 0))
				.addEntry(EnumEntry("Mojangles", 1))
	.addEntry(EnumEntry("Ten", 2))
		.addEntry(EnumEntry("UniCode", 3));
}
FontChanger::~FontChanger() {
}
const char* FontChanger::getModuleName() {
	return ("CustomFont");
}
