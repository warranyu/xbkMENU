/*
 * This file provides all the logic to navigate a menu system and edit menu
 * field values using a five control buttons (up, down, left, right, enter).
 * The user can instantiate a menu hierarchy and connect up display and other
 * functionality to the menu's init() and update() functions. Typically the
 * init() and update() functions will return a pointer to its own menu, but
 * sometimes, for example in a case of a menu that toggles a gpio, the init()
 * function will toggle the gpio and immediately return a pointer to the parent
 * menu.
 *
 * The menu system is broken down into three modes:
 *      "Go Back"         - This mode merely asks for the user to confirm that
 *                          they want to go back to the parent menu.
 *      "Submenu Select"  - This mode changes the selected submenu and will
 *                          navigate to selected submenu when enter is pressed.
 *      "Edit Menu Field" - This mode allows the user to modify the menu's
 *                          field value.
 *
 * Each menu has a parent and a list of submenus along with a field value.
 * Some menus do not have a parent menu (NULL). These are the top level menus.
 * Some menus act as submenus. These typically do not have a field value.
 * Some menus do not have submenus. These will sometimes have a field value.
 */

#include "xbkMenu.h"
#include <stdlib.h>

/* Public Functions */

/**
 * Call this function periodically to service the menu system.
 * @param  menu  pointer to the current menu
 * @param  event the xbkMenuEvent
 * @return       pointer to the next menu to be processed.
 */
xbkMenu * xbkMenu_Service(xbkMenu * menu, xbkMenuEvent event)
{
        switch(menu->mode)
        {
        case XBK_GO_BACK:
                return xbkMenu_Service_GoBackMode(menu, event);
        case XBK_SUBMENU_SELECT:
                return xbkMenu_Service_SubMenuSelectMode(menu, event);
        case XBK_EDIT_MENU_FIELD
                return xbkMenu_Service_EditMenuFieldMode(menu, event);
        }
}

/* Private Functions */

/**
 * Handles the xbkMenuEvent when the menu is in "GoBack" mode
 * @param  menu  pointer to the current menu
 * @param  event the xbkMenuEvent
 * @return       pointer to the next menu to be processed
 */
xbkMenu * xbkMenu_Service_GoBackMode(xbkMenu * menu, xbkMenuEvent event)
{
        switch(event)
        {
        case XBKMENU_RIGHT:
                menu->mode = XBK_SUBMENU_SELECT;
                return menu;
        case XBKMENU_ENTER:
                if(menu->parent_menu->init != NULL)
                        return menu->parent_menu->init(menu);
                else
                        return menu;
        default:
                return menu;
        }
}

/**
 * Handles the xbkMenuEvent when the menu is in "Submenu Select" mode
 * @param  menu  pointer to the current menu
 * @param  event the xbkMenuEvent
 * @return       pointer to the next menu to be processed
 */
xbkMenu * xbkMenu_Service_SubMenuSelectMode(xbkMenu * menu, xbkMenuEvent event)
{
        switch(event)
        {
        case XBKMENU_UP:
                xbkMenu_SelectPreviousSubmenu(menu);
                return menu;
        case XBKMENU_DOWN:
                xbkMenu_SelectNextSubmenu(menu);
                return menu;
        case XBKMENU_LEFT:
                menu->mode = XBK_GO_BACK;
                return menu;
        case XBKMENU_RIGHT:
                if(menu->field_type != XBK_FIELD_NONE)
                        menu->mode = XBK_EDIT_MENU_FIELD;
                return menu;
        case XBKMENU_ENTER:
                if(menu->submenu[submenu_select_index]->init != NULL)
                        return menu->submenu[submenu_select_index]->init(menu);
                else
                        return menu;
        default:
                return menu->update(menu);
        }
}

/**
 * Calls the appropriate function to edit the menu field based on the menu field type
 * @param  menu  pointer to the current menu
 * @param  event the xbkMenuEvent
 * @return       pointer to the next menu to be processed
 */
xbkMenu * xbkMenu_Service_EditMenuFieldMode(xbkMenu * menu, xbkMenuEvent event)
{
        switch(menu->field_type)
        {
        case XBK_FIELD_INTEGER:
                return xbkMenu_Service_EditMenuFieldModeInteger(menu, event);
        case XBK_FIELD_STRING:
                return xbkMenu_Service_EditMenuFieldModeString(menu, event);
        default:
                return menu;
        }
}

/**
 * Handles the changing of integer type field value for a menu
 * @param  menu  pointer to the current menu
 * @param  event the xbkMenuEvent
 * @return       pointer to the next menu to be processed
 */
xbkMenu * xbkMenu_Service_EditMenuFieldModeInteger(xbkMenu * menu, xbkMenuEvent event)
{
        int32_t value = atoi(menu->field_value);
        switch (event)
        {
        case XBKMENU_UP:
                itoa(value++, menu->field_value, 10);
                return menu;
        case XBKMENU_DOWN:
                itoa(value--, menu->field_value, 10);
                return menu;
        case XBKMENU_LEFT:
                menu->mode = XBK_SUBMENU_SELECT;
                return menu;
        case XBKMENU_ENTER:
                menu->mode = XBK_SUBMENU_SELECT;
                return menu;
        default:
                return menu;
        }
}

/**
 * Handles the changing of string type field value for a menu
 * @param  menu  pointer to the current menu
 * @param  event the xbkMenuEvent
 * @return       pointer to the next menu to be processed
 */
xbkMenu * xbkMenu_Service_EditMenuFieldModeString(xbkMenu * menu, xbkMenuEvent event)
{
        // TODO implement menu field editing in string mode
        return menu;
}

/**
 * Selects the next submenu. Updates the submenu_select_index.
 * @param  menu pointer to xbkMenu
 * @return      updated submenu_select_index value
 */
uint8_t xbkMenu_SelectNextSubmenu(xbkMenu * menu)
{
        if(menu->submenu_select_index >= menu->number_of_submenues)
                menu->submenu_select_index = 0;
        else
                menu->submenu_select_index++;
        return menu->submenu_select_index;
}

/**
 * Selects the previous submenu. Updates the submenu_select_index.
 * @param  menu pointer to xbkMenu
 * @return      updated submenu_select_index value
 */
uint8_t xbkMenu_SelectPreviousSubmenu(xbkMenu * menu)
{
        if(menu->submenu_select_index == 0)
                menu->submenu_select_index = menu->number_of_submenues;
        else
                menu->submenu_select_index--;
        return menu->submenu_select_index;
}
