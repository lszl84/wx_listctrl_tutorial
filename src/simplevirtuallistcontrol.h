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
        this->AppendColumn("Date");
        this->AppendColumn("Low");
        this->AppendColumn("High");
        this->AppendColumn("Open");
        this->AppendColumn("Close");
        this->AppendColumn("Volume");
        this->AppendColumn("IVR");

        this->SetColumnWidth(0, 180);
        this->SetColumnWidth(5, 100);
    }

    virtual wxString OnGetItemText(long index, long column) const wxOVERRIDE
    {
        ItemData item = items[index];

        switch (column)
        {
        case 0:
            return item.date;
        case 1:
            return wxString::Format("%.2f", item.low);
        case 2:
            return wxString::Format("%.2f", item.high);
        case 3:
            return wxString::Format("%.2f", item.open);
        case 4:
            return wxString::Format("%.2f", item.close);
        case 5:
            return wxString::Format("%.0f", item.volume);
        case 6:
            return wxString::Format("%d", item.ivr);
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