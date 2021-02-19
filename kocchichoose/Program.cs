using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Forms;

namespace kocchichoose
{
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main()
        {
            Ambiesoft.CppUtils.AmbSetProcessDPIAware();

            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            (new FormMain()).ShowDialog();
        }
    }
}
