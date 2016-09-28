using System;
using System.Collections.Generic;
using System.Windows.Forms;

namespace KWOpenRecent
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            FormMain form = new FormMain();
            form.ShowDialog();
        }
    }
}