/* 
   menuitem.h
   Copyright (c) 1999-2001 Perry Rapp
   Created: 1999/03 for private build of LifeLines
   Brought into repository: 2001/01/28

   Permission is hereby granted, free of charge, to any person
   obtaining a copy of this software and associated documentation
   files (the "Software"), to deal in the Software without
   restriction, including without limitation the rights to use, copy,
   modify, merge, publish, distribute, sublicense, and/or sell copies
   of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be
   included in all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
   NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
   BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
   ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
   CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
*/
/*
 menuitem.h - header file for menu items
 Copyright (c) 1999-2001 by Perry Rapp; all rights reserved
  Created in 1999/03 private build of LifeLines
  Added to repository during 3.0.6 development
 
 Menu layout code
 This is a reimplementation of the menus to move from
 fixed-size static menus, to paging, resizable menus.
*/

#ifndef _MENUITEM_H
#define _MENUITEM_H

#define MAIN_SCREEN        1
#define ONE_PER_SCREEN     2
#define ONE_FAM_SCREEN     3
#define TWO_PER_SCREEN     4
#define TWO_FAM_SCREEN     5
#define LIST_SCREEN        7
#define AUX_SCREEN         8
/* must be at bottom of list */
#define MAX_SCREEN         8


/*
  Each menu item has display text and selection character(s)
  If Choices is 0 (as it is for all as of 2002.01), then the
  first characters of Display up to whitespace are used for the
  choice characters, eg, "f  Browse to fathers"
  NB: The direct-to-child item is specially coded to use digits 1-9,
  and does not use the Choices string.
*/
typedef struct MenuItem_s {
	STRING Display;
	STRING Choices;
	INT Command;
	STRING LocalizedDisplay;
} MenuItem;
/*
A LongDisplay could be added (for, eg, status bar,
or for some type of extended command info display).
- Perry Rapp, 2001/02/10
*/
/*
typedef struct MenuItemOption_struct {
	STRING Display1;
	STRING Display2;
	STRING Choices;
	INT Command;
} MenuItemOption;
*/

/* special menu items added on the fly */
extern MenuItem g_MenuItemOther, g_MenuItemQuit;
/* this is for navigating top & bottom simultaneously
in tandem screens, and is not implemented in this version! */
extern MenuItem f_MenuItemSyncMoves;

typedef struct CmdItem_s * CMDITEM;
typedef struct CmdArray_s * CMDARRAY;

/* each screen has a lot of menu information */
/* This menu system is only used by browse screens,
and not the list screens nor the main menus */
typedef struct ScreenInfo_struct {
	STRING Title;    /* string at bottom of screen */
	INT MenuRows;    /* height of menu (at start) */
	INT MenuCols;    /* columns in this menu (3 for big, 1 for list) */
	INT MenuSize;    /* total #items in this menu */
	INT MenuPage;    /* which page of menu currently displayed */
	CMDARRAY Commands;
	MenuItem ** Menu;  /* array of pointers to items */
} ScreenInfo;

/*
global array of menu information, produced by menuitem.c
and used by both screen.c and menuitem.c
*/
extern ScreenInfo g_ScreenInfo[];


enum { 
	CMD_NONE /* unrecognized or unimplemented */
	, CMD_PARTIAL /* part of a multichar sequence */
	, CMD_QUIT, CMD_MENU_MORE, CMD_MENU_TOGGLE
	, CMD_MENU_GROW, CMD_MENU_SHRINK, CMD_MENU_MORECOLS, CMD_MENU_LESSCOLS
	, CMD_EDIT, CMD_PERSON, CMD_FATHER
	, CMD_MOTHER, CMD_SPOUSE, CMD_CHILDREN, CMD_UPSIB, CMD_DOWNSIB
	, CMD_FAMILY, CMD_PARENTS, CMD_BROWSE
	, CMD_TOP, CMD_BOTTOM, CMD_ADDASSPOUSE, CMD_ADDASCHILD
	, CMD_ADDSPOUSE, CMD_ADDCHILD, CMD_ADDFAMILY, CMD_PEDIGREE
	, CMD_SWAPFAMILIES, CMD_SWAPCHILDREN, CMD_SWAPTOPBOTTOM
	, CMD_REORDERCHILD
	, CMD_NEWPERSON, CMD_NEWFAMILY, CMD_TANDEM
	, CMD_REMOVEASSPOUSE, CMD_REMOVEASCHILD, CMD_REMOVESPOUSE, CMD_REMOVECHILD
	, CMD_SOURCES, CMD_NOTES, CMD_POINTERS
/*	, CMD_SHOWSOURCES, CMD_HIDESOURCES*/
	, CMD_SCROLL_UP, CMD_SCROLL_DOWN, CMD_DEPTH_UP, CMD_DEPTH_DOWN
	, CMD_NEXT, CMD_PREV, CMD_BROWSE_ZIP_INDI, CMD_BROWSE_ZIP_ANY
	, CMD_ADVANCED
	, CMD_SCROLL_TOP_UP, CMD_SCROLL_TOP_DOWN
	, CMD_SCROLL_BOTTOM_UP , CMD_SCROLL_BOTTOM_DOWN
	, CMD_SCROLL_BOTH_UP, CMD_SCROLL_BOTH_DOWN
	, CMD_MODE_GEDCOM, CMD_MODE_GEDCOMX, CMD_MODE_GEDCOMT
	, CMD_MODE_ANCESTORS , CMD_MODE_DESCENDANTS
	, CMD_MODE_NORMAL, CMD_MODE_CYCLE
	, CMD_MODE_PEDIGREE
	, CMD_TANDEM_CHILDREN, CMD_TANDEM_FATHERS, CMD_TANDEM_MOTHERS
	, CMD_TANDEM_SPOUSES, CMD_TANDEM_FAMILIES, CMD_TANDEM_PARENTS
	, CMD_TOGGLE_CHILDNUMS, CMD_TOGGLE_PEDTYPE
	, CMD_BROWSE_INDI, CMD_BROWSE_FAM
	/* reserve range for direct to children */
	, CMD_CHILD_DIRECT0, CMD_CHILD_DIRECT9=CMD_CHILD_DIRECT0+9
	, CMD_JUMP_HOOK
	, CMD_COPY_TOP_TO_BOTTOM, CMD_MERGE_BOTTOM_TO_TOP
	, CMD_BOTH_FATHERS, CMD_BOTH_MOTHERS
	, CMD_HISTORY_BACK, CMD_HISTORY_FWD, CMD_HISTORY_LIST, CMD_HISTORY_CLEAR
	, CMD_ADD_SOUR, CMD_ADD_EVEN, CMD_ADD_OTHR
	/* for hardware keys */
	, CMD_KY_UP=500, CMD_KY_DN, CMD_KY_PGUP, CMD_KY_PGDN
	, CMD_KY_HOME, CMD_KY_END
	, CMD_KY_ENTER
};

void menuitem_initialize(INT cols);
void menuitem_terminate(void);
INT menuitem_check_cmd(INT screen, STRING cmd);


#endif /* _MENUITEM_H */
