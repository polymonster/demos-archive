namespace Collada_Converter
{
    partial class MainForm
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
            this.convertButton = new System.Windows.Forms.Button();
            this.inputList = new System.Windows.Forms.ListView();
            this.addSourceButton = new System.Windows.Forms.Button();
            this.destinationLabel = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.changeDestButton = new System.Windows.Forms.Button();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.lab = new System.Windows.Forms.ToolStripStatusLabel();
            this.responseCombo = new System.Windows.Forms.ComboBox();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // convertButton
            // 
            this.convertButton.Location = new System.Drawing.Point(213, 219);
            this.convertButton.Name = "convertButton";
            this.convertButton.Size = new System.Drawing.Size(126, 28);
            this.convertButton.TabIndex = 0;
            this.convertButton.Text = "Convert";
            this.convertButton.UseVisualStyleBackColor = true;
            this.convertButton.Click += new System.EventHandler(this.convertButton_Click);
            // 
            // inputList
            // 
            this.inputList.GridLines = true;
            this.inputList.Location = new System.Drawing.Point(9, 13);
            this.inputList.Name = "inputList";
            this.inputList.Size = new System.Drawing.Size(334, 190);
            this.inputList.TabIndex = 1;
            this.inputList.UseCompatibleStateImageBehavior = false;
            this.inputList.View = System.Windows.Forms.View.Details;
            this.inputList.SelectedIndexChanged += new System.EventHandler(this.inputList_SelectedIndexChanged);
            // 
            // addSourceButton
            // 
            this.addSourceButton.Location = new System.Drawing.Point(9, 220);
            this.addSourceButton.Name = "addSourceButton";
            this.addSourceButton.Size = new System.Drawing.Size(126, 27);
            this.addSourceButton.TabIndex = 2;
            this.addSourceButton.Text = "Add New Source";
            this.addSourceButton.UseVisualStyleBackColor = true;
            this.addSourceButton.Click += new System.EventHandler(this.addSourceButton_Click);
            // 
            // destinationLabel
            // 
            this.destinationLabel.AutoSize = true;
            this.destinationLabel.Location = new System.Drawing.Point(6, 266);
            this.destinationLabel.Name = "destinationLabel";
            this.destinationLabel.Size = new System.Drawing.Size(92, 13);
            this.destinationLabel.TabIndex = 3;
            this.destinationLabel.Text = "Destination Folder";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(6, 291);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(0, 13);
            this.label2.TabIndex = 4;
            // 
            // changeDestButton
            // 
            this.changeDestButton.Location = new System.Drawing.Point(316, 283);
            this.changeDestButton.Name = "changeDestButton";
            this.changeDestButton.Size = new System.Drawing.Size(23, 21);
            this.changeDestButton.TabIndex = 5;
            this.changeDestButton.Text = "...";
            this.changeDestButton.UseVisualStyleBackColor = true;
            this.changeDestButton.Click += new System.EventHandler(this.changeDestButton_Click);
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lab});
            this.statusStrip1.Location = new System.Drawing.Point(0, 327);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(565, 22);
            this.statusStrip1.TabIndex = 6;
            this.statusStrip1.Text = "statusStrip";
            // 
            // lab
            // 
            this.lab.Name = "lab";
            this.lab.Size = new System.Drawing.Size(0, 17);
            // 
            // responseCombo
            // 
            this.responseCombo.FormattingEnabled = true;
            this.responseCombo.Items.AddRange(new object[] {
            "All Data",
            "Animations Only"});
            this.responseCombo.Location = new System.Drawing.Point(355, 16);
            this.responseCombo.Name = "responseCombo";
            this.responseCombo.Size = new System.Drawing.Size(200, 21);
            this.responseCombo.TabIndex = 7;
            this.responseCombo.SelectedIndexChanged += new System.EventHandler(this.responseCombo_SelectedIndexChanged);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(565, 349);
            this.Controls.Add(this.responseCombo);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.changeDestButton);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.destinationLabel);
            this.Controls.Add(this.addSourceButton);
            this.Controls.Add(this.inputList);
            this.Controls.Add(this.convertButton);
            this.Name = "MainForm";
            this.Text = "Collada to PSM v1.0";
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button convertButton;
        private System.Windows.Forms.ListView inputList;
        private System.Windows.Forms.Button addSourceButton;
        private System.Windows.Forms.Label destinationLabel;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Button changeDestButton;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel lab;
        private System.Windows.Forms.ComboBox responseCombo;
    }
}