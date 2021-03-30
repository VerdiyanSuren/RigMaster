#include <vufMayaGlobalIncludes.h>
#include <iostream>

//overloaded version to handle all those special std::endl and others...
std::ostream& operator,(std::ostream& out, std::ostream& (*f)(std::ostream&))
{
	out << f;
	return out;
}
/*
#pragma comment (lib, "OpenMaya.lib")
#pragma comment (lib, "OpenMayaAnim.lib")
#pragma comment (lib, "OpenMayaRender.lib")
#pragma comment (lib, "OpenMayaUI.lib")
#pragma comment (lib, "Foundation.lib")
*/

uint64_t vufRM::g_unique_id = 0;

