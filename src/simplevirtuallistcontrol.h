#pragma once

#include <wx/wx.h>
#include <wx/listctrl.h>

#include <vector>
#include <numeric>

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

        this->Bind(wxEVT_LIST_COL_CLICK, [this](wxListEvent &evt) {
            auto selectedListIndex = getFirstSelectedIndex();
            long selectedDataIndex;

            if (selectedListIndex != -1)
            {
                selectedDataIndex = this->orderedIndices[selectedListIndex];

                // deselecting old index
                this->SetItemState(selectedListIndex, 0, wxLIST_STATE_SELECTED);
            }

            this->sortByColumn(evt.GetColumn());
            this->RefreshAfterUpdate();

            if (selectedListIndex != -1)
            {
                auto listIndexToSelect = findIndexOfDataIndex(selectedDataIndex);
                this->SetItemState(listIndexToSelect, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
                this->EnsureVisible(listIndexToSelect);
            }

            this->sortAscending = !this->sortAscending;
        });
    }

    virtual wxString OnGetItemText(long index, long column) const wxOVERRIDE
    {
        ItemData item = items[orderedIndices[index]];

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
        this->SetItemCount(orderedIndices.size());
        this->Refresh();
    }

    void setItems(std::vector<ItemData> itemsToSet)
    {
        this->items = itemsToSet;

        this->orderedIndices = std::vector<long>(items.size());
        std::iota(orderedIndices.begin(), orderedIndices.end(), 0);
    }

private:
    bool sortAscending = true;

    std::vector<ItemData> items;
    std::vector<long> orderedIndices;

    void sortByColumn(int column)
    {
        // C++14 generic lambda
        static auto genericCompare = [](auto i1, auto i2, bool ascending) {
            return ascending ? i1 < i2 : i1 > i2;
        };

        bool ascending = this->sortAscending;

        std::sort(orderedIndices.begin(), orderedIndices.end(), [this, column, ascending](long index1, long index2) {
            auto i1 = this->items[index1];
            auto i2 = this->items[index2];

            switch (column)
            {
            case 0:
                return genericCompare(i1.date, i2.date, ascending);
            case 1:
                return genericCompare(i1.low, i2.low, ascending);
            case 2:
                return genericCompare(i1.high, i2.high, ascending);
            case 3:
                return genericCompare(i1.open, i2.open, ascending);
            case 4:
                return genericCompare(i1.close, i2.close, ascending);
            case 5:
                return genericCompare(i1.volume, i2.volume, ascending);
            case 6:
                return genericCompare(i1.ivr, i2.ivr, ascending);
            default:
                return false;
            }
        });
    }

    long getFirstSelectedIndex()
    {
        return GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    }

    long findIndexOfDataIndex(long dataIndex)
    {
        return std::find(orderedIndices.begin(), orderedIndices.end(), dataIndex) - orderedIndices.begin();
    }
};