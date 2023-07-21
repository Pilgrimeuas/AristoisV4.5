#include "Notifications.h"

Notifications::Notifications() : IModule(0, Category::VISUAL, "Displays notifications") {
	mode = SettingEnum(this)
    .addEntry(EnumEntry("Solaris", 0))
    .addEntry(EnumEntry("Zephyr", 1));
	registerEnumSetting("Theme", &mode, 0);
	registerBoolSetting("ShowToggle", &showToggle, showToggle);
	registerBoolSetting("Color", &color, color);
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	shouldHide = true;
}

const char* Notifications::getModuleName() {
	return ("Notifications");
}
