
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
    wxPanel *panel = new wxPanel(this);
    basicListView = new wxListView(panel);
    basicListView->AppendColumn("ID");
    basicListView->AppendColumn("Name");
    basicListView->AppendColumn("Description");
    basicListView->SetColumnWidth(0, 80);
    basicListView->SetColumnWidth(1, 120);
    basicListView->SetColumnWidth(2, 600);

    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(basicListView, 1, wxALL | wxEXPAND, 0);
    panel->SetSizerAndFit(sizer);

    populateListView();
}

void MyFrame::populateListView()
{
    basicListView->InsertItem(0, "123");
    basicListView->SetItem(0, 1, "Some Item");
    basicListView->SetItem(0, 2, "This is an item");

    basicListView->InsertItem(1, "456");
    basicListView->SetItem(1, 1, "Other Item");
    basicListView->SetItem(1, 2, "A different item");

    basicListView->InsertItem(2, "102");
    basicListView->SetItem(2, 1, "Another Item");
    basicListView->SetItem(2, 2, "The best one!");
}