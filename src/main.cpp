#include <wx/wx.h>
#include <wx/listctrl.h>

#include <string>
#include <vector>
#include <unordered_set>

#include "itemdata.h"
#include "simplevirtuallistcontrol.h"

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
    SimpleVirtualListControl *basicListView;
    void populateListView();
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
    basicListView = new SimpleVirtualListControl(this, wxID_ANY, wxDefaultPosition, wxSize(800, 500));

    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(basicListView, 1, wxALL | wxEXPAND, 0);

    this->SetSizerAndFit(sizer);

    populateListView();
}

void MyFrame::populateListView()
{
    basicListView->items.push_back({123, "Some Item", "This is an item"});
    basicListView->items.push_back({456, "Other Item", "A different item"});
    basicListView->items.push_back({102, "Another Item", "The best one!"});

    basicListView->RefreshAfterUpdate();
}
