#ifndef _XMIDICTRLMENU_H_
#define _XMIDICTRLMENU_H_

//#include <optional>
#include "XPLMUtilities.h"
#include "XPLMMenus.h"

class XMidiCtrlMenu {
public:
    XMidiCtrlMenu();

    void createMenu();
    void deleteMenu();

private:
    static void MenuHandler(void* in_menu_ref, void* in_item_ref);

    //std::optional<Menu> midictrl_menu;
    //std::optional<MenuItem> about_item;

    //std::optional<MenuItem> reload_plugins_on_modification_item;
    int m_menu_container;
    XPLMMenuID m_menu_id;
};

#endif // _XMIDICTRLMENU_H_