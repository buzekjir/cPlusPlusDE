#pragma once
#define rootDir "records"
#define shadeDir "records\\shade"
class DirHandler
{
public:
	static void CreateDirectories();
	static bool DeleteDirectory(LPCTSTR lpszDir, bool noRecycleBin = true);
};

