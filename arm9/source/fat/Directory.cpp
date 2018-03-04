#include "vram.h"
#include "vramheap.h"
#include "string.h"
#include "DirectoryEnumerator.h"
#include "File.h"
#include "Directory.h"

PUT_IN_VRAM DirectoryEnumerator* Directory::GetEnumerator()
{
	return new DirectoryEnumerator(this);
}

PUT_IN_VRAM DirectoryEntry* Directory::GetEntryByPath(const char* path)
{
	if (path[0] == '/')
		path++;
	Directory* d = this;
	while (true)
	{
		DirectoryEnumerator* enumerator = d->GetEnumerator();
		DirectoryEntry* entry;
		while ((entry = enumerator->GetNext()) != NULL)
		{
			int diff = strcasecmp(path, entry->GetName());
			if(diff == 0)
			{
				delete enumerator;
				if (d != this)
					delete d;
				return entry;
			}
			else if(diff == '/')
			{
				delete enumerator;
				if (d != this)
					delete d;
				if(!entry->GetIsDirectory())
				{
					delete entry;
					return NULL;
				}
				d = (Directory*)entry;
				break;
			}
			delete entry;
		}
		if (entry == NULL)
			break;
	}
	return NULL;
}