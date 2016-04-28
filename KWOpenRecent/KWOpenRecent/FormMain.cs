using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace KWOpenRecent
{
    public partial class FormMain : Form
    {
        public FormMain()
        {
            InitializeComponent();
        }

        private void FormMain_Load(object sender, EventArgs e)
        {
            koko
            listItems.Items.Add(@"c:\t\aaa.txt");
            listItems.Items.Add(@"c:\t\aaa222.txt");
            listItems.Items.Add(@"\\Thexp\Share\CurR\The Strategic Case for TPP.pdf");
        }

        private void Launch(string filename)
        {
            try
            {
                System.Diagnostics.Process.Start("kocchiwork.exe", "\"" + filename + "\"");
        
            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.Message,
                    Application.ProductName,
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Error);
            }
        }
        private void listItems_DoubleClick(object sender, EventArgs e)
        {
            object o = listItems.SelectedItem;
            if (o == null)
                return;

            Launch(o.ToString());
        }
    }
}