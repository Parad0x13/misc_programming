#ifndef _RESOURCESTREAM_
#define _RESOURCESTREAM_

#include <wx/mstream.h>
#include <windows.h>

wxMemoryInputStream *GetResourceInputStream(wxString resource_name, wxString resource_type);
bool HasResource(wxString resource_name, wxString resource_type);

#endif