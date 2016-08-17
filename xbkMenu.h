/**
 *
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

typedef enum xbkMenuFieldType
{
        XBK_FIELD_INTEGER,
        XBK_FIELD_STRING
}

tyoedef enum xbkMenuEvent
{
        XBKMENU_UP,
        XBKMENU_DOWN,
        XBKMENU_LEFT,
        XBKMENU_RIGHT,
        XBKMENU_ENTER,
        XBKMENU_NONE
}

typedef struct xbkMenu
{
        char name[20];                  // name of the menu
        xbkMenu * parent_menu;          // pointer to the perent xbkMenu
        xbkMenu ** submenus;            // pointer to array of xbkMenu
        uint8_t number_of_submenues;    // total number of submenus
        uint8_t submenu_select_index;   // which submenu is currently selected
        xbkMenuMode mode;               //
        (void)* init;                   // pointer to the menu's init function
        (void)* update;                 // pointer to the menu's update function
        uint8_t has_field;              // does this menu have a field value
        xbkMenuFieldType field_type     // the field type
        char field_value[20];           // menu field value, if it exists
} xbkMenu;

#endif
