using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using Ambiesoft;
namespace kocchichoose
{
    public partial class FormMain : Form
    {
#if DEBUG
        static readonly string KOCCHIWORK_INI = "kocchiworkD.ini";
        static readonly string KOCCHIWORK_EXE = "kocchiworkD.exe";
#else
        static readonly string KOCCHIWORK_INI = "kocchiwork.ini";
        static readonly string KOCCHIWORK_EXE = "kocchiwork.exe";
#endif
        static readonly string SECTION_LOCATION = "Location";
        static readonly string SECTION_OPTION = "Option";
        
        static readonly string KEY_COLUMNWIDTH = "ColumnWidth";

        List<string> _allRecents = new List<string>();

        string IniPath
        {
            get
            {
                return Path.Combine(Path.GetDirectoryName(Application.ExecutablePath),
                    KOCCHIWORK_INI
                    );
            }
        }
        public FormMain()
        {
            InitializeComponent();

            
            HashIni ini = Profile.ReadAll(IniPath);
            AmbLib.LoadFormXYWH(this, SECTION_LOCATION, ini);
            AmbLib.LoadListViewColumnWidth(listRecents, SECTION_OPTION, KEY_COLUMNWIDTH, ini);

            
            int count;
            Profile.GetInt("recents", "count", 0, out count, ini);
            for (int i = 0; i < count; ++i)
            {
                string sec = "recent_" + i;
                string tout;

                if (Profile.GetString("recents", sec, string.Empty, out tout, ini))
                {
                    if (!string.IsNullOrEmpty(tout))
                    {
                        _allRecents.Add(tout);
                    }
                }
            }

            listRecents.SmallImageList = ilExe;
            UpdateRecent();
        }

        void UpdateRecent()
        {
            listRecents.Items.Clear();

            // https://docs.microsoft.com/en-us/dotnet/framework/winforms/advanced/how-to-extract-the-icon-associated-with-a-file-in-windows-forms
            foreach (string filename in _allRecents)
            {
                if (!(string.IsNullOrEmpty(txtFind.Text) || filename.ToLower().IndexOf(txtFind.Text) >= 0))
                    continue;

                // Set a default icon for the file.
                Icon iconForFile = SystemIcons.WinLogo;

                try
                {
                    FileInfo file = new FileInfo(filename);
                    var item = new ListViewItem(file.FullName, 1);

                    // Check to see if the image collection contains an image
                    // for this extension, using the extension as a key.
                    if (!ilExe.Images.ContainsKey(file.Extension))
                    {
                        // If not, add the image to the image list.
                        // This fails on UNC
                        // iconForFile = Icon.ExtractAssociatedIcon(file.FullName);
                        iconForFile = Etier.IconHelper.IconReader.GetFileIcon(file.FullName,
                            Etier.IconHelper.IconReader.IconSize.Small, false);
                        ilExe.Images.Add(file.Extension, iconForFile);
                    }
                    item.ImageKey = file.Extension;
                    item.SubItems.Add("OK");
                    listRecents.Items.Add(item);
                }
                catch (Exception ex)
                {
                    var item = new ListViewItem(filename, 1);
                    item.SubItems.Add(ex.Message);
                    listRecents.Items.Add(item);
                }
            }
        }
        private void listRecents_ItemActivate(object sender, EventArgs e)
        {
            if (!doOpen())
                return;
            Close();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.None;
            if (listRecents.SelectedItems.Count == 0)
            {
                CppUtils.Alert(Properties.Resources.STR_NOFILESELECTED);
                return;
            }

            if (!doOpen())
                return;

            DialogResult = btnOK.DialogResult;
        }

        bool doOpen()
        {
            if (listRecents.SelectedItems.Count == 0)
                return false;

            string kocchiwork = Path.Combine(
                Path.GetDirectoryName(Application.ExecutablePath),
                KOCCHIWORK_EXE);
            foreach(ListViewItem item in listRecents.Items)
            {
                try
                {
                    FileInfo fi = new FileInfo(item.Text);
                    System.Diagnostics.Process.Start(kocchiwork, AmbLib.doubleQuoteIfSpace(fi.FullName));
                }
                catch(Exception ex)
                {
                    CppUtils.Alert(ex);
                    return false;
                }
            }
            return true;
        }

        private void FormMain_FormClosed(object sender, FormClosedEventArgs e)
        {
            HashIni ini = Profile.ReadAll(IniPath);
            AmbLib.SaveFormXYWH(this, SECTION_LOCATION, ini);
            AmbLib.SaveListViewColumnWidth(listRecents, SECTION_OPTION, KEY_COLUMNWIDTH, ini);
            if(!Profile.WriteAll(ini,IniPath))
            {
                CppUtils.Alert(Properties.Resources.STR_FAILEDTOSAVEINI);
            }
        }

        private void btnClearFind_Click(object sender, EventArgs e)
        {
            txtFind.Clear();
        }

        private void txtFind_TextChanged(object sender, EventArgs e)
        {
            UpdateRecent();
        }
    }
}
