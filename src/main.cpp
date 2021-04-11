#include <wx/wx.h>
#include <wx/listctrl.h>

#include <string>
#include <vector>
#include <unordered_set>

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    wxListView *basicListView;
    void populateListView();
    void addSingleItem(int id, const std::string &name, const std::string &description);
    void sortByColumn(int column);

    int sortDirection = 1;

    struct ItemData
    {
        int id;
        std::string name;
        std::string description;
    };

    std::unordered_set<std::unique_ptr<ItemData>> itemDataBag;

    // helper methods for sorting
    static int compareInts(int i1, int i2, int direction);
    static int compareStrings(const std::string &s1, const std::string &s2, int direction);

    static int idSortCallback(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
    static int nameSortCallback(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
    static int descriptionSortCallback(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData);
};

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Hello World", wxPoint(50, 50), wxSize(800, 600));
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    basicListView = new wxListView(this, wxID_ANY, wxDefaultPosition, wxSize(800, 500));
    basicListView->AppendColumn("ID");
    basicListView->AppendColumn("Name");
    basicListView->AppendColumn("Description");
    basicListView->SetColumnWidth(0, 80);
    basicListView->SetColumnWidth(1, 120);
    basicListView->SetColumnWidth(2, 600);

    basicListView->Bind(wxEVT_LIST_COL_CLICK, [this](wxListEvent &evt) {
        this->sortByColumn(evt.GetColumn());
    });

    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(basicListView, 1, wxALL | wxEXPAND, 0);

    this->SetSizerAndFit(sizer);

    populateListView();
}

void MyFrame::populateListView()
{
    addSingleItem(123, "Some Item", "This is an item");
    addSingleItem(456, "Other Item", "A different item");
    addSingleItem(102, "Another Item", "The best one!");
}

void MyFrame::addSingleItem(int id, const std::string &name, const std::string &description)
{
    int index = basicListView->GetItemCount();

    basicListView->InsertItem(index, std::to_string(id));
    basicListView->SetItem(index, 1, name);
    basicListView->SetItem(index, 2, description);

    // for sorting using the SortItems method
    ItemData data{id, name, description};
    auto dataPtr = std::make_unique<ItemData>(data);
    basicListView->SetItemData(index, reinterpret_cast<wxIntPtr>(dataPtr.get()));

    // add to the set to ensure correct object ownership and release
    // we don't care about the order in the underlying container because
    // in case of non-virtual ListViews, the ordering is handled by the ListView
    itemDataBag.insert(std::move(dataPtr));
}

void MyFrame::sortByColumn(int column)
{
    switch (column)
    {
    case 0:
        basicListView->SortItems(idSortCallback, this->sortDirection);
        break;
    case 1:
        basicListView->SortItems(nameSortCallback, this->sortDirection);
        break;
    case 2:
        basicListView->SortItems(descriptionSortCallback, this->sortDirection);
        break;
    default:
        return;
    }

    basicListView->Refresh(); // wxWidgets MacOSX bug: list not reloading properly after scrolling a list with some items selected

    this->sortDirection = -this->sortDirection;
}

int MyFrame::compareInts(int i1, int i2, int direction)
{
    return i1 == i2 ? 0 : (i1 < i2 ? -direction : direction);
}

int MyFrame::compareStrings(const std::string &s1, const std::string &s2, int direction)
{
    return s1.compare(s2) * direction;
}

int MyFrame::idSortCallback(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    return compareInts(reinterpret_cast<ItemData *>(item1)->id, reinterpret_cast<ItemData *>(item2)->id, static_cast<int>(sortData));
}

int MyFrame::nameSortCallback(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    return compareStrings(reinterpret_cast<ItemData *>(item1)->name, reinterpret_cast<ItemData *>(item2)->name, static_cast<int>(sortData));
}

int MyFrame::descriptionSortCallback(wxIntPtr item1, wxIntPtr item2, wxIntPtr sortData)
{
    return compareStrings(reinterpret_cast<ItemData *>(item1)->description, reinterpret_cast<ItemData *>(item2)->description, static_cast<int>(sortData));
}
