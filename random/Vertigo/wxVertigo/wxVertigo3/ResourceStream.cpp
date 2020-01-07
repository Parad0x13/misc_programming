#include "ResourceStream.h"

wxMemoryInputStream *GetResourceInputStream(wxString resource_name, wxString resource_type)
{
	HRSRC hrsrc=FindResource(NULL, resource_name, resource_type);
	if(hrsrc==NULL) return NULL;

	HGLOBAL hglobal=LoadResource(NULL, hrsrc);
	if(hglobal==NULL) return NULL;

	void *data=LockResource(hglobal);
	if(data==NULL) return NULL;

	DWORD datalen=SizeofResource(NULL, hrsrc);
	if(datalen<1) return NULL;

	return new wxMemoryInputStream(data, datalen); 
}

bool HasResource(wxString resource_name, wxString resource_type)
{
	HRSRC hrsrc=FindResource(NULL, resource_name, resource_type);
	if(hrsrc==NULL) return false;

	HGLOBAL hglobal=LoadResource(NULL, hrsrc);
	if(hglobal==NULL) return false;

	return true;
}