#include <wx/wx.h>
#include <wx/listctrl.h>

#include <string>
#include <vector>

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
    void sortById(wxCommandEvent &e);

    int sortDirection = 1;
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

    auto button = new wxButton(this, wxID_ANY, "Sort by ID");
    button->Bind(wxEVT_BUTTON, &MyFrame::sortById, this);

    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(basicListView, 1, wxALL | wxEXPAND, 0);
    sizer->Add(button, 0, wxALIGN_LEFT | wxTOP | wxLEFT | wxBOTTOM, 5);

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
    basicListView->SetItemData(index, id);
}

void MyFrame::sortById(wxCommandEvent &e)
{
    basicListView->SortItems(
        [](wxIntPtr item1, wxIntPtr item2, wxIntPtr direction) {
            if (item1 == item2)
            {
                return 0;
            }
            else if (item1 < item2)
            {
                return -(int)direction;
            }
            else
            {
                return (int)direction;
            }
        },
        this->sortDirection);

    basicListView->Refresh(); // wxWidgets bug: list not reloading properly after scrolling a list with some items selected

    this->sortDirection = -this->sortDirection;
}
