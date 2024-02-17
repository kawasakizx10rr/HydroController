namespace generateFonts
{
    partial class editor
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(editor));
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.optionsToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.autoCropToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.saveToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.exitToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.resizeToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addColumnLeftToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addRowTopToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addColumnRightToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.addRowBottomToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.menuStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox1.BackColor = System.Drawing.Color.White;
            this.pictureBox1.ErrorImage = null;
            this.pictureBox1.InitialImage = null;
            this.pictureBox1.Location = new System.Drawing.Point(2, 29);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(245, 273);
            this.pictureBox1.TabIndex = 3;
            this.pictureBox1.TabStop = false;
            this.pictureBox1.Click += new System.EventHandler(this.pictureBox1_Click);
            // 
            // menuStrip1
            // 
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.optionsToolStripMenuItem,
            this.resizeToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(0, 0);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(248, 24);
            this.menuStrip1.TabIndex = 4;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // optionsToolStripMenuItem
            // 
            this.optionsToolStripMenuItem.BackColor = System.Drawing.SystemColors.ControlLight;
            this.optionsToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.autoCropToolStripMenuItem,
            this.saveToolStripMenuItem,
            this.exitToolStripMenuItem});
            this.optionsToolStripMenuItem.Name = "optionsToolStripMenuItem";
            this.optionsToolStripMenuItem.Size = new System.Drawing.Size(61, 20);
            this.optionsToolStripMenuItem.Text = "Options";
            // 
            // autoCropToolStripMenuItem
            // 
            this.autoCropToolStripMenuItem.Name = "autoCropToolStripMenuItem";
            this.autoCropToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.autoCropToolStripMenuItem.Text = "Auto crop";
            this.autoCropToolStripMenuItem.Click += new System.EventHandler(this.autoCropToolStripMenuItem_Click);
            // 
            // saveToolStripMenuItem
            // 
            this.saveToolStripMenuItem.Name = "saveToolStripMenuItem";
            this.saveToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.saveToolStripMenuItem.Text = "Save";
            this.saveToolStripMenuItem.Click += new System.EventHandler(this.saveToolStripMenuItem_Click);
            // 
            // exitToolStripMenuItem
            // 
            this.exitToolStripMenuItem.Name = "exitToolStripMenuItem";
            this.exitToolStripMenuItem.Size = new System.Drawing.Size(180, 22);
            this.exitToolStripMenuItem.Text = "Exit Editor";
            this.exitToolStripMenuItem.Click += new System.EventHandler(this.exitToolStripMenuItem_Click);
            // 
            // resizeToolStripMenuItem
            // 
            this.resizeToolStripMenuItem.BackColor = System.Drawing.SystemColors.ControlLight;
            this.resizeToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.addColumnLeftToolStripMenuItem,
            this.addRowTopToolStripMenuItem,
            this.addColumnRightToolStripMenuItem,
            this.addRowBottomToolStripMenuItem});
            this.resizeToolStripMenuItem.Margin = new System.Windows.Forms.Padding(5, 0, 0, 0);
            this.resizeToolStripMenuItem.Name = "resizeToolStripMenuItem";
            this.resizeToolStripMenuItem.Size = new System.Drawing.Size(51, 20);
            this.resizeToolStripMenuItem.Text = "Resize";
            // 
            // addColumnLeftToolStripMenuItem
            // 
            this.addColumnLeftToolStripMenuItem.Name = "addColumnLeftToolStripMenuItem";
            this.addColumnLeftToolStripMenuItem.Size = new System.Drawing.Size(168, 22);
            this.addColumnLeftToolStripMenuItem.Text = "Add column left";
            this.addColumnLeftToolStripMenuItem.Click += new System.EventHandler(this.addColumnLeftToolStripMenuItem_Click);
            // 
            // addRowTopToolStripMenuItem
            // 
            this.addRowTopToolStripMenuItem.Name = "addRowTopToolStripMenuItem";
            this.addRowTopToolStripMenuItem.Size = new System.Drawing.Size(168, 22);
            this.addRowTopToolStripMenuItem.Text = "Add row top";
            this.addRowTopToolStripMenuItem.Click += new System.EventHandler(this.addRowTopToolStripMenuItem_Click);
            // 
            // addColumnRightToolStripMenuItem
            // 
            this.addColumnRightToolStripMenuItem.Name = "addColumnRightToolStripMenuItem";
            this.addColumnRightToolStripMenuItem.Size = new System.Drawing.Size(168, 22);
            this.addColumnRightToolStripMenuItem.Text = "Add column right";
            this.addColumnRightToolStripMenuItem.Click += new System.EventHandler(this.addColumnRightToolStripMenuItem_Click);
            // 
            // addRowBottomToolStripMenuItem
            // 
            this.addRowBottomToolStripMenuItem.Name = "addRowBottomToolStripMenuItem";
            this.addRowBottomToolStripMenuItem.Size = new System.Drawing.Size(168, 22);
            this.addRowBottomToolStripMenuItem.Text = "Add row bottom";
            this.addRowBottomToolStripMenuItem.Click += new System.EventHandler(this.addRowBottomToolStripMenuItem_Click);
            // 
            // editor
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(248, 306);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.menuStrip1);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MainMenuStrip = this.menuStrip1;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "editor";
            this.Text = "Font Generator  Editor";
            this.Load += new System.EventHandler(this.editor_Load);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem optionsToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem autoCropToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem saveToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem exitToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem resizeToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addColumnLeftToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addRowTopToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addColumnRightToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem addRowBottomToolStripMenuItem;
    }
}