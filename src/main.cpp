#include <wx/wx.h>
#include <wx/listctrl.h>

#include <string>
#include <vector>
#include <unordered_set>

#include <fstream>
#include <filesystem>

#include "itemdata.h"
#include "simplevirtuallistcontrol.h"

// Quick and dirty way to localize the CSV file using a variable set by CMake.
// Do *NOT* use this method to locate application files in a production release,
// use proper resource management instead (see wxStandardPaths)
#ifndef CSV_DIR
#define CSV_DIR "."
#endif

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
    std::ifstream stream(std::string(CSV_DIR) + "/aapl-1d.csv"); // should be copied by CMake to the bin directory

    // ignore header
    stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (stream)
    {
        auto item = ItemData::fromCsvLine(stream);
        if (!item.date.empty())
        {
            basicListView->items.push_back(item);
        }
    }

    basicListView->RefreshAfterUpdate();
}
