#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>

#include <vector>

#include "itemdata.h"

class SimpleVirtualListControl : public wxListCtrl
{
public:
    SimpleVirtualListControl(wxWindow *parent, const wxWindowID id, const wxPoint &pos, const wxSize &size)
        : wxListCtrl(parent, id, pos, size, wxLC_REPORT | wxLC_VIRTUAL)
    {
        this->AppendColumn("ID");
        this->AppendColumn("Name");
        this->AppendColumn("Description");
        this->SetColumnWidth(0, 80);
        this->SetColumnWidth(1, 120);
        this->SetColumnWidth(2, 600);
    }

    virtual wxString OnGetItemText(long index, long column) const wxOVERRIDE
    {
        ItemData item = items[index];

        switch (column)
        {
        case 0:
            return std::to_string(item.id);
        case 1:
            return item.name;
        case 2:
            return item.description;
        default:
            return "";
        }
    }

    void RefreshAfterUpdate()
    {
        this->SetItemCount(items.size());
        this->Refresh();
    }

    std::vector<ItemData> items;
};