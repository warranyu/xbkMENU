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
                {
                        menu->parent_menu->init();
                        return menu->parent_menu;
                }
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
                {
                        menu->submenu[submenu_select_index]->init();
                        return menu->submenu[submenu_select_index];
                }
                else
                        return menu;
        default:
                menu->update();
                return menu;
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
