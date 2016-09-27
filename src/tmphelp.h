BOOL GetFileData(LPCTSTR pFile, WIN32_FIND_DATA* pData);
BOOL GetFileLastWriteTime(LPCTSTR pFile, FILETIME* pFT);
BOOL AddTrayIcon(HWND hWnd, UINT dwIDandCallbackMessage, HICON hIcon, LPCTSTR pszTip);
BOOL RemoveTrayIcon(HWND hWnd, UINT dwIDandCallbackMessage);
void OpenRecent();
BOOL SaveRecent(LPCTSTR pApp, LPCTSTR pFile);