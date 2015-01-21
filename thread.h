struct threadData {
	HWND hWnd_;
	HANDLE hDie_;

	FILETIME ftWork_;
	LPCTSTR pWorkingFile_;
	// BOOL bAppKanshi_;
	// HANDLE hApp_;
};

unsigned  __stdcall kanshi(void * p);
