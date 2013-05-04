///////////////////////////////////////////////////////////////////////////////
// Name:        wxm_command.h
// Description: for building MenuBar & ToolBar & KeyBinding
// Author:      madedit@gmail.com  (creator)
//              wxmedit@gmail.com  (current maintainer)
// Licence:     GPL
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
