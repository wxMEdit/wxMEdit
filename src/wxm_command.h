///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_command.h
// Description: for Building MenuBar & ToolBar & KeyBinding
// Copyright:   2013-2019  JiaYanwei   <wxmedit@gmail.com>
//              2007-2010  Alston Chen <madedit@gmail.com>
// License:     GPLv3
///////////////////////////////////////////////////////////////////////////////

#ifndef _WXM_COMMAND_H_
#define _WXM_COMMAND_H_

#include "wxmedit/wxmedit_command.h"

struct CommandData
{
    MadEditCommand command;
    int            menu_level;
    int            menu_id;
    const wxChar   *menuid_name;
    const wxChar   *text;
    const wxChar   *key;
    wxItemKind     kind;
    int            image_idx;
    wxMenu         **menu_ptr;
    const wxChar   *hint;
};

extern CommandData CommandTable[];

#endif
