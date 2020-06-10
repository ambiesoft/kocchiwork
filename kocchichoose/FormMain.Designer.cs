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
            this.btnClearAll = new System.Windows.Forms.Button();
            this.btnClearError = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // lblFind
            // 
            resources.ApplyResources(this.lblFind, "lblFind");
            this.lblFind.Name = "lblFind";
            // 
            // txtFind
            // 
            resources.ApplyResources(this.txtFind, "txtFind");
            this.txtFind.Name = "txtFind";
            this.txtFind.TextChanged += new System.EventHandler(this.txtFind_TextChanged);
            // 
            // listRecents
            // 
            resources.ApplyResources(this.listRecents, "listRecents");
            this.listRecents.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.chFile,
            this.chStatus});
            this.listRecents.Name = "listRecents";
            this.listRecents.UseCompatibleStateImageBehavior = false;
            this.listRecents.View = System.Windows.Forms.View.Details;
            this.listRecents.ItemActivate += new System.EventHandler(this.listRecents_ItemActivate);
            // 
            // chFile
            // 
            resources.ApplyResources(this.chFile, "chFile");
            // 
            // chStatus
            // 
            resources.ApplyResources(this.chStatus, "chStatus");
            // 
            // btnOK
            // 
            resources.ApplyResources(this.btnOK, "btnOK");
            this.btnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.btnOK.Name = "btnOK";
            this.btnOK.UseVisualStyleBackColor = true;
            this.btnOK.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // btnCancel
            // 
            resources.ApplyResources(this.btnCancel, "btnCancel");
            this.btnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
            this.btnCancel.Name = "btnCancel";
            this.btnCancel.UseVisualStyleBackColor = true;
            // 
            // ilExe
            // 
            this.ilExe.ColorDepth = System.Windows.Forms.ColorDepth.Depth8Bit;
            resources.ApplyResources(this.ilExe, "ilExe");
            this.ilExe.TransparentColor = System.Drawing.Color.Transparent;
            // 
            // btnClearFind
            // 
            resources.ApplyResources(this.btnClearFind, "btnClearFind");
            this.btnClearFind.Name = "btnClearFind";
            this.btnClearFind.UseVisualStyleBackColor = true;
            this.btnClearFind.Click += new System.EventHandler(this.btnClearFind_Click);
            // 
            // btnClearAll
            // 
            resources.ApplyResources(this.btnClearAll, "btnClearAll");
            this.btnClearAll.Name = "btnClearAll";
            this.btnClearAll.UseVisualStyleBackColor = true;
            this.btnClearAll.Click += new System.EventHandler(this.btnClearAll_Click);
            // 
            // btnClearError
            // 
            resources.ApplyResources(this.btnClearError, "btnClearError");
            this.btnClearError.Name = "btnClearError";
            this.btnClearError.UseVisualStyleBackColor = true;
            this.btnClearError.Click += new System.EventHandler(this.btnClearError_Click);
            // 
            // FormMain
            // 
            this.AcceptButton = this.btnOK;
            resources.ApplyResources(this, "$this");
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.CancelButton = this.btnCancel;
            this.Controls.Add(this.btnClearError);
            this.Controls.Add(this.btnClearAll);
            this.Controls.Add(this.btnClearFind);
            this.Controls.Add(this.btnCancel);
            this.Controls.Add(this.btnOK);
            this.Controls.Add(this.listRecents);
            this.Controls.Add(this.txtFind);
            this.Controls.Add(this.lblFind);
            this.Name = "FormMain";
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
        private System.Windows.Forms.Button btnClearAll;
        private System.Windows.Forms.Button btnClearError;
    }
}

