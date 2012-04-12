struct threadData {
	HWND hWnd_;
	HANDLE hDie_;
	FILETIME ftWork_;
	LPCTSTR pWorkingFile_;
	
};

unsigned  __stdcall kanshi(void * p);
