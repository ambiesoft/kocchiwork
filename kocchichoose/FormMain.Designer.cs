namespace kocchichoose
{
    partial class FormMain
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FormMain));
            this.lblFind = new System.Windows.Forms.Label();
            this.txtFind = new System.Windows.Forms.TextBox();
            this.listRecents = new System.Windows.Forms.ListView();
            this.chFile = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.chStatus = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.btnOK = new System.Windows.Forms.Button();
            this.btnCancel = new System.Windows.Forms.Button();
            this.ilExe = new System.Windows.Forms.ImageList(this.components);
            this.btnClearFind = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lblFind
            // 
            this.lblFind.AutoSize = true;
            this.lblFind.Location = new System.Drawing.Point(12, 11);
            this.lblFind.Name = "lblFind";
            this.lblFind.Size = new System.Drawing.Size(29, 12);
            this.lblFind.TabIndex = 100;
            this.lblFind.Text = "&Find:";
            // 
            // txtFind
            // 
            this.txtFind.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.txtFind.Location = new System.Drawing.Point(48, 8);
            this.txtFind.Name = "txtFind";
            this.txtFind.Size = new System.Drawing.Size(597, 19);
            this.txtFind.TabIndex = 200;
            this.txtFind.TextChanged += new System.EventHandler(this.txtFind_TextChanged);
            // 
            // listRecents
            // 
            this.listRecents.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.listRecents.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.chFile,
            this.chStatus});
            this.listRecents.Location = new System.Drawing.Point(48, 32);
            this.listRecents.Name = "listRecents";
            this.listRecents.Size = new System.Drawing.Size(678, 299);
            this.listRecents.TabIndex = 400;
            this.listRecents.UseCompatibleStateImageBehavior = false;
            this.listRecents.View = System.Windows.Forms.View.Details;
            this.listRecents.ItemActivate += new System.EventHandler(this.listRecents_ItemActivate);
            // 
            // chFile
            // 
            this.chFile.Text = "File";
            this.chFile.Width = 546;
            // 
            // chStatus
            // 
            this.chStatus.Text = "status";
            // 
            // btnOK
            // 
            this.btnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Location = new System.Drawing.Point(518, 345);
            this.btnOK.Name = "btnOK";
            this.btnOK.Size = new System.Drawing.Size(101, 21);
            this.btnOK.TabIndex = 500;
            this.btnOK.Text = "&OK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnCancel
            // 
            this.btnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Location = new System.Drawing.Point(625, 345);
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.Size = new System.Drawing.Size(101, 21);
            this.btnCancel.TabIndex = 600;
            this.btnCancel.Text = "&Cancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // ilExe
            // 
            this.ilExe.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            this.ilExe.ImageSize = new System.Drawing.Size(16, 16);
            this.ilExe.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // btnClearFind
            // 
            this.btnClearFind.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.btnClearFind.Location = new System.Drawing.Point(651, 6);
            this.btnClearFind.Name = "btnClearFind";
            this.btnClearFind.Size = new System.Drawing.Size(75, 23);
            this.btnClearFind.TabIndex = 300;
            this.btnClearFind.Text = "C&lear";
            this.btnClearFind.UseVisualStyleBackColor = true;
            this.btnClearFind.Click += new System.EventHandler(this.btnClearFind_Click);
            // 
            // FormMain
            // 
            this.AcceptButton = this.btnOK;
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 12F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.ClientSize = new System.Drawing.Size(738, 378);
            this.Controls.Add(this.btnClearFind);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.listRecents);
            this.Controls.Add(this.txtFind);
            this.Controls.Add(this.lblFind);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MinimumSize = new System.Drawing.Size(563, 285);
            this.Name = "FormMain";
            this.Text = "kocchiwork chooser";
            this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.FormMain_FormClosed);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label lblFind;
        private System.Windows.Forms.TextBox txtFind;
        private System.Windows.Forms.ListView listRecents;
        private System.Windows.Forms.ColumnHeader chFile;
        private System.Windows.Forms.ColumnHeader chStatus;
        private System.Windows.Forms.Button btnOK;
        private System.Windows.Forms.Button btnCancel;
        private System.Windows.Forms.ImageList ilExe;
        private System.Windows.Forms.Button btnClearFind;
    }
}

