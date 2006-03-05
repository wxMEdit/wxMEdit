///////////////////////////////////////////////////////////////////////////////
// Name:        MadCommand.h
// Description: for building MenuBar & ToolBar & KeyBinding
// Author:      madedit@gmail.com
// Licence:     GPL
///////////////////////////////////////////////////////////////////////////////

#ifndef _MADCOMMAND_H_
#define _MADCOMMAND_H_

#include "MadEdit/MadEditCommand.h"

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
