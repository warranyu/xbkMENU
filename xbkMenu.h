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

#ifndef XBKMENU_H
#define XBKMENU_H

#include <stdint.h>

typedef enum xbkMenuMode
{
        XBK_GO_BACK,                    // This mode is used when wanting to go back to parent menu
        XBK_SUBMENU_SELECT,             // This mode is used to select submenus
        XBK_EDIT_MENU_FIELD             // This mode is used to edit a menu field
} xbkMenuMode;

tyoedef enum xbkMenuEvent
{
        XBKMENU_UP,
        XBKMENU_DOWN,
        XBKMENU_LEFT,
        XBKMENU_RIGHT,
        XBKMENU_ENTER,
        XBKMENU_NONE
}

typedef struct xbkMenu xbkMenu;

struct xbkMenu
{
        char name[20];                          // name of the menu
        xbkMenu * parent_menu;                  // pointer to the perent xbkMenu
        xbkMenu ** submenus;                    // pointer to array of xbkMenu
        uint8_t number_of_submenues;            // total number of submenus
        uint8_t submenu_select_index;           // which submenu is currently selected
        xbkMenuMode mode;                       // the current menu mode
        xbkMenu * (*init)(xbkMenu * menu);      // pointer to the menu's init function
        xbkMenu * (*update)(xbkMenu * menu);    // pointer to the menu's update function
        xbkMenu * (*execute)(xbkMenu * menu);   // pointer to the menu's execute function
        uint8_t has_field;                      // indicates if this menu has a valid field value
        int32_t field_value;                    // menu field value, if it exists
        int32_t field_min_val;                  // maximum field value
        int32_t field_max_val;                  // minimum field value
};

#endif
