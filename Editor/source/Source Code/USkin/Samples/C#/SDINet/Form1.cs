using System;
using System.Drawing;
using System.Collections;
using System.ComponentModel;
using System.Windows.Forms;
using System.Data;
using System.Runtime.InteropServices;
using USkin;

namespace WindowsApplication1
{
	/// <summary>
	/// Form1 ��ժҪ˵����
	/// </summary>
	public class Form1 : System.Windows.Forms.Form
	{
		private System.ComponentModel.IContainer components;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage NormalControl;
		private System.Windows.Forms.TabPage tabPage1;
		private System.Windows.Forms.TabPage tabPage2;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.Button button1;
		private System.Windows.Forms.CheckBox checkBox1;
		private System.Windows.Forms.RadioButton radioButton1;
		private System.Windows.Forms.Button button2;
		private System.Windows.Forms.Button button3;
		private System.Windows.Forms.Button button4;
		private System.Windows.Forms.GroupBox groupBox2;
		private System.Windows.Forms.ComboBox comboBox1;
		private System.Windows.Forms.ComboBox comboBox2;
		private System.Windows.Forms.ComboBox comboBox3;
		private System.Windows.Forms.ComboBox comboBox4;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.TextBox textBox1;
		private System.Windows.Forms.TextBox textBox2;
		private System.Windows.Forms.ListBox listBox1;
		private System.Windows.Forms.CheckedListBox checkedListBox1;
		private System.Windows.Forms.PrintDialog printDialog1;
		private System.Windows.Forms.ColorDialog colorDialog1;
		private System.Windows.Forms.FontDialog fontDialog1;
		private System.Windows.Forms.FolderBrowserDialog folderBrowserDialog1;
		private System.Windows.Forms.SaveFileDialog saveFileDialog1;
		private System.Windows.Forms.OpenFileDialog openFileDialog1;
		private System.Windows.Forms.Button button5;
		private System.Windows.Forms.PrintPreviewDialog printPreviewDialog1;
		private System.Windows.Forms.TabPage tabPage3;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.LinkLabel linkLabel1;
		private System.Windows.Forms.GroupBox groupBox5;
		private System.Windows.Forms.PictureBox pictureBox1;
		private System.Windows.Forms.GroupBox groupBox6;
		private System.Windows.Forms.Panel panel1;
		private System.Windows.Forms.Panel panel2;
		private System.Windows.Forms.ListView listView1;
		private System.Windows.Forms.TreeView treeView1;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.ColumnHeader columnHeader5;
		private System.Windows.Forms.DateTimePicker dateTimePicker1;
		private System.Windows.Forms.MonthCalendar monthCalendar1;
		private System.Windows.Forms.StatusBar statusBar1;
		private System.Windows.Forms.ToolBar toolBar1;
		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.MainMenu mainMenu1;
		private System.Windows.Forms.ToolBarButton btnNew;
		private System.Windows.Forms.ToolBarButton btnOpen;
		private System.Windows.Forms.ToolBarButton btnSave;
		private System.Windows.Forms.ToolBarButton btnHelp;
		private System.Windows.Forms.MenuItem menuItem1;
		private System.Windows.Forms.MenuItem menuItem2;
		private System.Windows.Forms.MenuItem menuItem3;
		private System.Windows.Forms.MenuItem menuItem4;
		private System.Windows.Forms.MenuItem menuItem5;
		private System.Windows.Forms.MenuItem menuItem6;
		private System.Windows.Forms.MenuItem menuItem7;
		private System.Windows.Forms.MenuItem menuItem8;
		private System.Windows.Forms.MenuItem menuItem9;
		private System.Windows.Forms.MenuItem menuItem10;
		private System.Windows.Forms.MenuItem menuItem11;
		private System.Windows.Forms.TabPage tabPage4;
		private System.Windows.Forms.Button button6;
		private System.Windows.Forms.LinkLabel linkLabel2;
		private System.Windows.Forms.DomainUpDown domainUpDown1;
		private System.Windows.Forms.NumericUpDown numericUpDown1;
		private System.Windows.Forms.TrackBar trackBar1;
		private System.Windows.Forms.ProgressBar progressBar1;
		private System.Windows.Forms.HScrollBar hScrollBar1;
		private System.Windows.Forms.RichTextBox richTextBox1;
		private System.Windows.Forms.VScrollBar vScrollBar1;
		private System.Windows.Forms.TrackBar trackBar2;
		private System.Windows.Forms.PageSetupDialog pageSetupDialog1;
		private System.Windows.Forms.StatusBarPanel statusBarPanel1;
        private ToolStrip toolStrip1;
        private ToolStripButton �½�NToolStripButton;
        private ToolStripButton ��OToolStripButton;
        private ToolStripButton ����SToolStripButton;
        private ToolStripButton ��ӡPToolStripButton;
        private ToolStripSeparator toolStripSeparator;
        private ToolStripButton ����UToolStripButton;
        private ToolStripButton ����CToolStripButton;
        private ToolStripButton ճ��PToolStripButton;
        private ToolStripSeparator toolStripSeparator1;
        private ToolStripButton ����LToolStripButton;
        private TextBox textBox3;
        private MenuStrip menuStrip1;
        private ToolStripMenuItem �ļ�FToolStripMenuItem;
        private ToolStripMenuItem �½�NToolStripMenuItem;
        private ToolStripMenuItem ��OToolStripMenuItem;
        private ToolStripSeparator toolStripSeparator2;
        private ToolStripMenuItem ����SToolStripMenuItem;
        private ToolStripMenuItem ���ΪAToolStripMenuItem;
        private ToolStripSeparator toolStripSeparator3;
        private ToolStripMenuItem ��ӡPToolStripMenuItem;
        private ToolStripMenuItem ��ӡԤ��VToolStripMenuItem;
        private ToolStripSeparator toolStripSeparator4;
        private ToolStripMenuItem �˳�XToolStripMenuItem;
        private ToolStripMenuItem �༭EToolStripMenuItem;
        private ToolStripMenuItem ����UToolStripMenuItem;
        private ToolStripMenuItem �ظ�RToolStripMenuItem;
        private ToolStripSeparator toolStripSeparator5;
        private ToolStripMenuItem ����TToolStripMenuItem;
        private ToolStripMenuItem ����CToolStripMenuItem;
        private ToolStripMenuItem ճ��PToolStripMenuItem;
        private ToolStripSeparator toolStripSeparator6;
        private ToolStripMenuItem ȫѡAToolStripMenuItem;
        private ToolStripMenuItem ����TToolStripMenuItem;
        private ToolStripMenuItem �Զ���CToolStripMenuItem;
        private ToolStripMenuItem ѡ��OToolStripMenuItem;
        private ToolStripMenuItem ����HToolStripMenuItem;
        private ToolStripMenuItem ����CToolStripMenuItem;
        private ToolStripMenuItem ����IToolStripMenuItem;
        private ToolStripMenuItem ����SToolStripMenuItem;
        private ToolStripSeparator toolStripSeparator7;
        private ToolStripMenuItem ����AToolStripMenuItem;
        private StatusStrip statusStrip1;
        private ToolStripProgressBar toolStripProgressBar1;
        private ToolStripStatusLabel toolStripStatusLabel1;
        private ToolStripSplitButton toolStripSplitButton1;
        private ToolStripDropDownButton toolStripDropDownButton1;
        private ToolStripSplitButton toolStripSplitButton2;
		private System.Windows.Forms.StatusBarPanel statusBarPanel2;
		
		public Form1()
		{
			//
			// Windows ���������֧���������
			//
			InitializeComponent();

			//
			// TODO: �� InitializeComponent ���ú�����κι��캯������
			//
		}

		/// <summary>
		/// ������������ʹ�õ���Դ��
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			USkinSDK.USkinExit();
			if( disposing )
			{
				if (components != null) 
				{
					components.Dispose();
				}
			}
			base.Dispose( disposing );
		}

		#region Windows ������������ɵĴ���
		/// <summary>
		/// �����֧������ķ��� - ��Ҫʹ�ô���༭���޸�
		/// �˷��������ݡ�
		/// </summary>
		private void InitializeComponent()
		{
            this.components = new System.ComponentModel.Container();
            System.Windows.Forms.TreeNode treeNode133 = new System.Windows.Forms.TreeNode("Node8");
            System.Windows.Forms.TreeNode treeNode134 = new System.Windows.Forms.TreeNode("Node7", new System.Windows.Forms.TreeNode[] {
            treeNode133});
            System.Windows.Forms.TreeNode treeNode135 = new System.Windows.Forms.TreeNode("Node6", new System.Windows.Forms.TreeNode[] {
            treeNode134});
            System.Windows.Forms.TreeNode treeNode136 = new System.Windows.Forms.TreeNode("Node0", new System.Windows.Forms.TreeNode[] {
            treeNode135});
            System.Windows.Forms.TreeNode treeNode137 = new System.Windows.Forms.TreeNode("Node11");
            System.Windows.Forms.TreeNode treeNode138 = new System.Windows.Forms.TreeNode("Node10", new System.Windows.Forms.TreeNode[] {
            treeNode137});
            System.Windows.Forms.TreeNode treeNode139 = new System.Windows.Forms.TreeNode("Node9", new System.Windows.Forms.TreeNode[] {
            treeNode138});
            System.Windows.Forms.TreeNode treeNode140 = new System.Windows.Forms.TreeNode("Node1", new System.Windows.Forms.TreeNode[] {
            treeNode139});
            System.Windows.Forms.TreeNode treeNode141 = new System.Windows.Forms.TreeNode("Node14");
            System.Windows.Forms.TreeNode treeNode142 = new System.Windows.Forms.TreeNode("Node13", new System.Windows.Forms.TreeNode[] {
            treeNode141});
            System.Windows.Forms.TreeNode treeNode143 = new System.Windows.Forms.TreeNode("Node12", new System.Windows.Forms.TreeNode[] {
            treeNode142});
            System.Windows.Forms.TreeNode treeNode144 = new System.Windows.Forms.TreeNode("Node2", new System.Windows.Forms.TreeNode[] {
            treeNode143});
            System.Windows.Forms.TreeNode treeNode145 = new System.Windows.Forms.TreeNode("Node18");
            System.Windows.Forms.TreeNode treeNode146 = new System.Windows.Forms.TreeNode("Node17", new System.Windows.Forms.TreeNode[] {
            treeNode145});
            System.Windows.Forms.TreeNode treeNode147 = new System.Windows.Forms.TreeNode("Node16", new System.Windows.Forms.TreeNode[] {
            treeNode146});
            System.Windows.Forms.TreeNode treeNode148 = new System.Windows.Forms.TreeNode("Node15", new System.Windows.Forms.TreeNode[] {
            treeNode147});
            System.Windows.Forms.TreeNode treeNode149 = new System.Windows.Forms.TreeNode("Node3", new System.Windows.Forms.TreeNode[] {
            treeNode148});
            System.Windows.Forms.TreeNode treeNode150 = new System.Windows.Forms.TreeNode("Node20");
            System.Windows.Forms.TreeNode treeNode151 = new System.Windows.Forms.TreeNode("Node19", new System.Windows.Forms.TreeNode[] {
            treeNode150});
            System.Windows.Forms.TreeNode treeNode152 = new System.Windows.Forms.TreeNode("Node4", new System.Windows.Forms.TreeNode[] {
            treeNode151});
            System.Windows.Forms.TreeNode treeNode153 = new System.Windows.Forms.TreeNode("Node21");
            System.Windows.Forms.TreeNode treeNode154 = new System.Windows.Forms.TreeNode("Node5", new System.Windows.Forms.TreeNode[] {
            treeNode153});
            System.Windows.Forms.ListViewItem listViewItem49 = new System.Windows.Forms.ListViewItem(new string[] {
            "USkinForDotNet",
            "NEEMedia",
            "Sales@NEEMedia.com",
            "www.neemedia.com",
            "USkin Is good for dot net"}, -1);
            System.Windows.Forms.ListViewItem listViewItem50 = new System.Windows.Forms.ListViewItem(new string[] {
            "USkinForDotNet",
            "NEEMedia",
            "Sales@NEEMedia.com",
            "www.neemedia.com",
            "USkin Is good for dot net"}, -1);
            System.Windows.Forms.ListViewItem listViewItem51 = new System.Windows.Forms.ListViewItem(new string[] {
            "USkinForDotNet",
            "NEEMedia",
            "Sales@NEEMedia.com",
            "www.neemedia.com",
            "USkin Is good for dot net"}, -1);
            System.Windows.Forms.ListViewItem listViewItem52 = new System.Windows.Forms.ListViewItem(new string[] {
            "USkinForDotNet",
            "NEEMedia",
            "Sales@NEEMedia.com",
            "www.neemedia.com",
            "USkin Is good for dot net"}, -1);
            System.Windows.Forms.ListViewItem listViewItem53 = new System.Windows.Forms.ListViewItem(new string[] {
            "USkinForDotNet",
            "NEEMedia",
            "Sales@NEEMedia.com",
            "www.neemedia.com",
            "USkin Is good for dot net"}, -1);
            System.Windows.Forms.ListViewItem listViewItem54 = new System.Windows.Forms.ListViewItem(new string[] {
            "USkinForDotNet",
            "NEEMedia",
            "Sales@NEEMedia.com",
            "www.neemedia.com",
            "USkin Is good for dot net"}, -1);
            System.Windows.Forms.ListViewItem listViewItem55 = new System.Windows.Forms.ListViewItem(new string[] {
            "USkinForDotNet",
            "NEEMedia",
            "Sales@NEEMedia.com",
            "www.neemedia.com",
            "USkin Is good for dot net"}, -1);
            System.Windows.Forms.ListViewItem listViewItem56 = new System.Windows.Forms.ListViewItem(new string[] {
            "USkinForDotNet",
            "NEEMedia",
            "Sales@NEEMedia.com",
            "www.neemedia.com",
            "USkin Is good for dot net"}, -1);
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.NormalControl = new System.Windows.Forms.TabPage();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.checkedListBox1 = new System.Windows.Forms.CheckedListBox();
            this.listBox1 = new System.Windows.Forms.ListBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.comboBox4 = new System.Windows.Forms.ComboBox();
            this.comboBox3 = new System.Windows.Forms.ComboBox();
            this.comboBox2 = new System.Windows.Forms.ComboBox();
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.button5 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.button3 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.checkBox1 = new System.Windows.Forms.CheckBox();
            this.button1 = new System.Windows.Forms.Button();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.groupBox6 = new System.Windows.Forms.GroupBox();
            this.panel2 = new System.Windows.Forms.Panel();
            this.panel1 = new System.Windows.Forms.Panel();
            this.groupBox5 = new System.Windows.Forms.GroupBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.linkLabel1 = new System.Windows.Forms.LinkLabel();
            this.label1 = new System.Windows.Forms.Label();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.treeView1 = new System.Windows.Forms.TreeView();
            this.listView1 = new System.Windows.Forms.ListView();
            this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
            this.columnHeader5 = new System.Windows.Forms.ColumnHeader();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.monthCalendar1 = new System.Windows.Forms.MonthCalendar();
            this.dateTimePicker1 = new System.Windows.Forms.DateTimePicker();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.trackBar2 = new System.Windows.Forms.TrackBar();
            this.vScrollBar1 = new System.Windows.Forms.VScrollBar();
            this.richTextBox1 = new System.Windows.Forms.RichTextBox();
            this.hScrollBar1 = new System.Windows.Forms.HScrollBar();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.domainUpDown1 = new System.Windows.Forms.DomainUpDown();
            this.printDialog1 = new System.Windows.Forms.PrintDialog();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.fontDialog1 = new System.Windows.Forms.FontDialog();
            this.folderBrowserDialog1 = new System.Windows.Forms.FolderBrowserDialog();
            this.saveFileDialog1 = new System.Windows.Forms.SaveFileDialog();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.printPreviewDialog1 = new System.Windows.Forms.PrintPreviewDialog();
            this.statusBar1 = new System.Windows.Forms.StatusBar();
            this.statusBarPanel1 = new System.Windows.Forms.StatusBarPanel();
            this.statusBarPanel2 = new System.Windows.Forms.StatusBarPanel();
            this.toolBar1 = new System.Windows.Forms.ToolBar();
            this.btnNew = new System.Windows.Forms.ToolBarButton();
            this.btnOpen = new System.Windows.Forms.ToolBarButton();
            this.btnSave = new System.Windows.Forms.ToolBarButton();
            this.btnHelp = new System.Windows.Forms.ToolBarButton();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.mainMenu1 = new System.Windows.Forms.MainMenu(this.components);
            this.menuItem1 = new System.Windows.Forms.MenuItem();
            this.menuItem2 = new System.Windows.Forms.MenuItem();
            this.menuItem3 = new System.Windows.Forms.MenuItem();
            this.menuItem4 = new System.Windows.Forms.MenuItem();
            this.menuItem5 = new System.Windows.Forms.MenuItem();
            this.menuItem6 = new System.Windows.Forms.MenuItem();
            this.menuItem7 = new System.Windows.Forms.MenuItem();
            this.menuItem8 = new System.Windows.Forms.MenuItem();
            this.menuItem9 = new System.Windows.Forms.MenuItem();
            this.menuItem10 = new System.Windows.Forms.MenuItem();
            this.menuItem11 = new System.Windows.Forms.MenuItem();
            this.button6 = new System.Windows.Forms.Button();
            this.linkLabel2 = new System.Windows.Forms.LinkLabel();
            this.pageSetupDialog1 = new System.Windows.Forms.PageSetupDialog();
            this.toolStrip1 = new System.Windows.Forms.ToolStrip();
            this.�½�NToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.��OToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.����SToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.��ӡPToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator = new System.Windows.Forms.ToolStripSeparator();
            this.����UToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.����CToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.ճ��PToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
            this.����LToolStripButton = new System.Windows.Forms.ToolStripButton();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.�ļ�FToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.�½�NToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.��OToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
            this.����SToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.���ΪAToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator3 = new System.Windows.Forms.ToolStripSeparator();
            this.��ӡPToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.��ӡԤ��VToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator4 = new System.Windows.Forms.ToolStripSeparator();
            this.�˳�XToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.�༭EToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.����UToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.�ظ�RToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator5 = new System.Windows.Forms.ToolStripSeparator();
            this.����TToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.����CToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ճ��PToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator6 = new System.Windows.Forms.ToolStripSeparator();
            this.ȫѡAToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.����TToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.�Զ���CToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.ѡ��OToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.����HToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.����CToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.����IToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.����SToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripSeparator7 = new System.Windows.Forms.ToolStripSeparator();
            this.����AToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.toolStripProgressBar1 = new System.Windows.Forms.ToolStripProgressBar();
            this.toolStripStatusLabel1 = new System.Windows.Forms.ToolStripStatusLabel();
            this.toolStripSplitButton1 = new System.Windows.Forms.ToolStripSplitButton();
            this.toolStripDropDownButton1 = new System.Windows.Forms.ToolStripDropDownButton();
            this.toolStripSplitButton2 = new System.Windows.Forms.ToolStripSplitButton();
            this.tabControl1.SuspendLayout();
            this.NormalControl.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox2.SuspendLayout();
            this.groupBox1.SuspendLayout();
            this.tabPage3.SuspendLayout();
            this.groupBox6.SuspendLayout();
            this.groupBox5.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.groupBox4.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.tabPage4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanel1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanel2)).BeginInit();
            this.toolStrip1.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.NormalControl);
            this.tabControl1.Controls.Add(this.tabPage3);
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Controls.Add(this.tabPage4);
            this.tabControl1.Location = new System.Drawing.Point(8, 32);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(496, 296);
            this.tabControl1.TabIndex = 0;
            // 
            // NormalControl
            // 
            this.NormalControl.Controls.Add(this.groupBox3);
            this.NormalControl.Controls.Add(this.groupBox2);
            this.NormalControl.Controls.Add(this.groupBox1);
            this.NormalControl.Location = new System.Drawing.Point(4, 21);
            this.NormalControl.Name = "NormalControl";
            this.NormalControl.Size = new System.Drawing.Size(488, 271);
            this.NormalControl.TabIndex = 0;
            this.NormalControl.Text = "NormalControl";
            // 
            // groupBox3
            // 
            this.groupBox3.Controls.Add(this.checkedListBox1);
            this.groupBox3.Controls.Add(this.listBox1);
            this.groupBox3.Controls.Add(this.textBox2);
            this.groupBox3.Controls.Add(this.textBox1);
            this.groupBox3.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox3.Location = new System.Drawing.Point(328, 8);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(152, 248);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Edit&ListBox";
            // 
            // checkedListBox1
            // 
            this.checkedListBox1.Items.AddRange(new object[] {
            "Line 1",
            "Line 2",
            "Line 3",
            "Line 4",
            "Line 5",
            "Line 6",
            "Line 7",
            "Line 8",
            "Line 9"});
            this.checkedListBox1.Location = new System.Drawing.Point(8, 160);
            this.checkedListBox1.Name = "checkedListBox1";
            this.checkedListBox1.Size = new System.Drawing.Size(136, 84);
            this.checkedListBox1.TabIndex = 3;
            // 
            // listBox1
            // 
            this.listBox1.ItemHeight = 12;
            this.listBox1.Items.AddRange(new object[] {
            "Line 1",
            "Line 2",
            "Line 3",
            "Line 4",
            "Line 5",
            "Line 6",
            "Line 7",
            "Line 8",
            "Line 9"});
            this.listBox1.Location = new System.Drawing.Point(8, 88);
            this.listBox1.Name = "listBox1";
            this.listBox1.Size = new System.Drawing.Size(136, 64);
            this.listBox1.TabIndex = 2;
            // 
            // textBox2
            // 
            this.textBox2.Enabled = false;
            this.textBox2.Location = new System.Drawing.Point(8, 56);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(136, 21);
            this.textBox2.TabIndex = 1;
            this.textBox2.Text = "textBox2";
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(8, 24);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(136, 21);
            this.textBox1.TabIndex = 0;
            this.textBox1.Text = "textBox1";
            // 
            // groupBox2
            // 
            this.groupBox2.Controls.Add(this.comboBox4);
            this.groupBox2.Controls.Add(this.comboBox3);
            this.groupBox2.Controls.Add(this.comboBox2);
            this.groupBox2.Controls.Add(this.comboBox1);
            this.groupBox2.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox2.Location = new System.Drawing.Point(160, 8);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(160, 248);
            this.groupBox2.TabIndex = 1;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "ComboBox";
            // 
            // comboBox4
            // 
            this.comboBox4.Items.AddRange(new object[] {
            "Line 1",
            "Line 2",
            "Line 3",
            "Line 4",
            "Line 5",
            "Line 6",
            "Line 7",
            "Line 8",
            "Line 9"});
            this.comboBox4.Location = new System.Drawing.Point(8, 56);
            this.comboBox4.Name = "comboBox4";
            this.comboBox4.Size = new System.Drawing.Size(144, 20);
            this.comboBox4.TabIndex = 3;
            this.comboBox4.Text = "comboBox4";
            // 
            // comboBox3
            // 
            this.comboBox3.DropDownStyle = System.Windows.Forms.ComboBoxStyle.Simple;
            this.comboBox3.Items.AddRange(new object[] {
            "Line 1",
            "Line 2",
            "Line 3",
            "Line 4",
            "Line 5",
            "Line 6",
            "Line 7",
            "Line 8",
            "Line 9"});
            this.comboBox3.Location = new System.Drawing.Point(8, 120);
            this.comboBox3.Name = "comboBox3";
            this.comboBox3.Size = new System.Drawing.Size(144, 120);
            this.comboBox3.TabIndex = 2;
            this.comboBox3.Text = "comboBox3";
            // 
            // comboBox2
            // 
            this.comboBox2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBox2.Items.AddRange(new object[] {
            "Line 1",
            "Line 2",
            "Line 3",
            "Line 4",
            "Line 5",
            "Line 6",
            "Line 7",
            "Line 8",
            "Line 9"});
            this.comboBox2.Location = new System.Drawing.Point(8, 88);
            this.comboBox2.Name = "comboBox2";
            this.comboBox2.Size = new System.Drawing.Size(144, 20);
            this.comboBox2.TabIndex = 1;
            // 
            // comboBox1
            // 
            this.comboBox1.Items.AddRange(new object[] {
            "Line 1",
            "Line 2",
            "Line 3",
            "Line 4",
            "Line 5",
            "Line 6",
            "Line 7",
            "Line 8",
            "Line 9"});
            this.comboBox1.Location = new System.Drawing.Point(8, 24);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(144, 20);
            this.comboBox1.TabIndex = 0;
            this.comboBox1.Text = "comboBox1";
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.button5);
            this.groupBox1.Controls.Add(this.button4);
            this.groupBox1.Controls.Add(this.button3);
            this.groupBox1.Controls.Add(this.button2);
            this.groupBox1.Controls.Add(this.radioButton1);
            this.groupBox1.Controls.Add(this.checkBox1);
            this.groupBox1.Controls.Add(this.button1);
            this.groupBox1.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox1.Location = new System.Drawing.Point(8, 8);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(144, 248);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Button";
            // 
            // button5
            // 
            this.button5.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.button5.Location = new System.Drawing.Point(16, 120);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(112, 24);
            this.button5.TabIndex = 6;
            this.button5.Text = "FolderDialog";
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // button4
            // 
            this.button4.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.button4.Location = new System.Drawing.Point(16, 216);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(112, 24);
            this.button4.TabIndex = 5;
            this.button4.Text = "PrintDialog";
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // button3
            // 
            this.button3.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.button3.Location = new System.Drawing.Point(16, 184);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(112, 24);
            this.button3.TabIndex = 4;
            this.button3.Text = "ColorDialog";
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button2
            // 
            this.button2.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.button2.Location = new System.Drawing.Point(16, 152);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(112, 24);
            this.button2.TabIndex = 3;
            this.button2.Text = "FileDialog";
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // radioButton1
            // 
            this.radioButton1.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.radioButton1.Location = new System.Drawing.Point(16, 88);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(112, 24);
            this.radioButton1.TabIndex = 2;
            this.radioButton1.Text = "radioButton1";
            // 
            // checkBox1
            // 
            this.checkBox1.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkBox1.Location = new System.Drawing.Point(16, 56);
            this.checkBox1.Name = "checkBox1";
            this.checkBox1.Size = new System.Drawing.Size(112, 24);
            this.checkBox1.TabIndex = 1;
            this.checkBox1.Text = "checkBox1";
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(16, 24);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(112, 24);
            this.button1.TabIndex = 0;
            this.button1.Text = "PrintPreview";
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.groupBox6);
            this.tabPage3.Controls.Add(this.groupBox5);
            this.tabPage3.Controls.Add(this.groupBox4);
            this.tabPage3.Location = new System.Drawing.Point(4, 21);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Size = new System.Drawing.Size(488, 271);
            this.tabPage3.TabIndex = 3;
            this.tabPage3.Text = "Picture&Label";
            // 
            // groupBox6
            // 
            this.groupBox6.Controls.Add(this.panel2);
            this.groupBox6.Controls.Add(this.panel1);
            this.groupBox6.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox6.Location = new System.Drawing.Point(184, 8);
            this.groupBox6.Name = "groupBox6";
            this.groupBox6.Size = new System.Drawing.Size(296, 88);
            this.groupBox6.TabIndex = 2;
            this.groupBox6.TabStop = false;
            this.groupBox6.Text = "Pan";
            // 
            // panel2
            // 
            this.panel2.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.panel2.Location = new System.Drawing.Point(152, 24);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(136, 56);
            this.panel2.TabIndex = 1;
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.OrangeRed;
            this.panel1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.panel1.Location = new System.Drawing.Point(8, 24);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(128, 56);
            this.panel1.TabIndex = 0;
            // 
            // groupBox5
            // 
            this.groupBox5.Controls.Add(this.pictureBox1);
            this.groupBox5.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox5.Location = new System.Drawing.Point(8, 104);
            this.groupBox5.Name = "groupBox5";
            this.groupBox5.Size = new System.Drawing.Size(472, 160);
            this.groupBox5.TabIndex = 1;
            this.groupBox5.TabStop = false;
            this.groupBox5.Text = "PictureBox";
            // 
            // pictureBox1
            // 
            this.pictureBox1.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
            this.pictureBox1.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox1.Image")));
            this.pictureBox1.Location = new System.Drawing.Point(8, 24);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(456, 128);
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // groupBox4
            // 
            this.groupBox4.Controls.Add(this.linkLabel1);
            this.groupBox4.Controls.Add(this.label1);
            this.groupBox4.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.groupBox4.Location = new System.Drawing.Point(8, 8);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(168, 88);
            this.groupBox4.TabIndex = 0;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Label";
            // 
            // linkLabel1
            // 
            this.linkLabel1.Location = new System.Drawing.Point(8, 56);
            this.linkLabel1.Name = "linkLabel1";
            this.linkLabel1.Size = new System.Drawing.Size(152, 24);
            this.linkLabel1.TabIndex = 1;
            this.linkLabel1.TabStop = true;
            this.linkLabel1.Text = "Http://www.neemedia.com";
            this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel1_LinkClicked);
            // 
            // label1
            // 
            this.label1.Location = new System.Drawing.Point(8, 24);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(136, 24);
            this.label1.TabIndex = 0;
            this.label1.Text = "Connect To NEEMedia";
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.treeView1);
            this.tabPage1.Controls.Add(this.listView1);
            this.tabPage1.Location = new System.Drawing.Point(4, 21);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Size = new System.Drawing.Size(488, 271);
            this.tabPage1.TabIndex = 1;
            this.tabPage1.Text = "TreeView&List";
            // 
            // treeView1
            // 
            this.treeView1.Location = new System.Drawing.Point(8, 8);
            this.treeView1.Name = "treeView1";
            treeNode133.Name = "";
            treeNode133.Text = "Node8";
            treeNode134.Name = "";
            treeNode134.Text = "Node7";
            treeNode135.Name = "";
            treeNode135.Text = "Node6";
            treeNode136.Name = "";
            treeNode136.Text = "Node0";
            treeNode137.Name = "";
            treeNode137.Text = "Node11";
            treeNode138.Name = "";
            treeNode138.Text = "Node10";
            treeNode139.Name = "";
            treeNode139.Text = "Node9";
            treeNode140.Name = "";
            treeNode140.Text = "Node1";
            treeNode141.Name = "";
            treeNode141.Text = "Node14";
            treeNode142.Name = "";
            treeNode142.Text = "Node13";
            treeNode143.Name = "";
            treeNode143.Text = "Node12";
            treeNode144.Name = "";
            treeNode144.Text = "Node2";
            treeNode145.Name = "";
            treeNode145.Text = "Node18";
            treeNode146.Name = "";
            treeNode146.Text = "Node17";
            treeNode147.Name = "";
            treeNode147.Text = "Node16";
            treeNode148.Name = "";
            treeNode148.Text = "Node15";
            treeNode149.Name = "";
            treeNode149.Text = "Node3";
            treeNode150.Name = "";
            treeNode150.Text = "Node20";
            treeNode151.Name = "";
            treeNode151.Text = "Node19";
            treeNode152.Name = "";
            treeNode152.Text = "Node4";
            treeNode153.Name = "";
            treeNode153.Text = "Node21";
            treeNode154.Name = "";
            treeNode154.Text = "Node5";
            this.treeView1.Nodes.AddRange(new System.Windows.Forms.TreeNode[] {
            treeNode136,
            treeNode140,
            treeNode144,
            treeNode149,
            treeNode152,
            treeNode154});
            this.treeView1.Size = new System.Drawing.Size(192, 256);
            this.treeView1.TabIndex = 1;
            // 
            // listView1
            // 
            this.listView1.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2,
            this.columnHeader3,
            this.columnHeader4,
            this.columnHeader5});
            this.listView1.FullRowSelect = true;
            this.listView1.GridLines = true;
            this.listView1.Items.AddRange(new System.Windows.Forms.ListViewItem[] {
            listViewItem49,
            listViewItem50,
            listViewItem51,
            listViewItem52,
            listViewItem53,
            listViewItem54,
            listViewItem55,
            listViewItem56});
            this.listView1.Location = new System.Drawing.Point(208, 8);
            this.listView1.Name = "listView1";
            this.listView1.Size = new System.Drawing.Size(272, 256);
            this.listView1.TabIndex = 0;
            this.listView1.UseCompatibleStateImageBehavior = false;
            this.listView1.View = System.Windows.Forms.View.Details;
            // 
            // columnHeader1
            // 
            this.columnHeader1.Text = "Title";
            // 
            // columnHeader2
            // 
            this.columnHeader2.Text = "Author";
            // 
            // columnHeader3
            // 
            this.columnHeader3.Text = "Email";
            // 
            // columnHeader4
            // 
            this.columnHeader4.Text = "URL";
            // 
            // columnHeader5
            // 
            this.columnHeader5.Text = "Description";
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.monthCalendar1);
            this.tabPage2.Controls.Add(this.dateTimePicker1);
            this.tabPage2.Location = new System.Drawing.Point(4, 21);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Size = new System.Drawing.Size(488, 271);
            this.tabPage2.TabIndex = 2;
            this.tabPage2.Text = "DateTime";
            // 
            // monthCalendar1
            // 
            this.monthCalendar1.Location = new System.Drawing.Point(8, 48);
            this.monthCalendar1.Name = "monthCalendar1";
            this.monthCalendar1.TabIndex = 1;
            // 
            // dateTimePicker1
            // 
            this.dateTimePicker1.Location = new System.Drawing.Point(8, 8);
            this.dateTimePicker1.Name = "dateTimePicker1";
            this.dateTimePicker1.Size = new System.Drawing.Size(272, 21);
            this.dateTimePicker1.TabIndex = 0;
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.statusStrip1);
            this.tabPage4.Controls.Add(this.textBox3);
            this.tabPage4.Controls.Add(this.trackBar2);
            this.tabPage4.Controls.Add(this.vScrollBar1);
            this.tabPage4.Controls.Add(this.richTextBox1);
            this.tabPage4.Controls.Add(this.hScrollBar1);
            this.tabPage4.Controls.Add(this.progressBar1);
            this.tabPage4.Controls.Add(this.trackBar1);
            this.tabPage4.Controls.Add(this.numericUpDown1);
            this.tabPage4.Controls.Add(this.domainUpDown1);
            this.tabPage4.Location = new System.Drawing.Point(4, 21);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Size = new System.Drawing.Size(488, 271);
            this.tabPage4.TabIndex = 4;
            this.tabPage4.Text = "CommonControls";
            // 
            // trackBar2
            // 
            this.trackBar2.Location = new System.Drawing.Point(408, 8);
            this.trackBar2.Name = "trackBar2";
            this.trackBar2.Orientation = System.Windows.Forms.Orientation.Vertical;
            this.trackBar2.Size = new System.Drawing.Size(45, 248);
            this.trackBar2.TabIndex = 7;
            this.trackBar2.TickStyle = System.Windows.Forms.TickStyle.TopLeft;
            // 
            // vScrollBar1
            // 
            this.vScrollBar1.Location = new System.Drawing.Point(464, 8);
            this.vScrollBar1.Name = "vScrollBar1";
            this.vScrollBar1.Size = new System.Drawing.Size(16, 256);
            this.vScrollBar1.TabIndex = 6;
            // 
            // richTextBox1
            // 
            this.richTextBox1.Location = new System.Drawing.Point(8, 184);
            this.richTextBox1.Name = "richTextBox1";
            this.richTextBox1.Size = new System.Drawing.Size(296, 80);
            this.richTextBox1.TabIndex = 5;
            this.richTextBox1.Text = "richTextBox1";
            // 
            // hScrollBar1
            // 
            this.hScrollBar1.Location = new System.Drawing.Point(8, 152);
            this.hScrollBar1.Name = "hScrollBar1";
            this.hScrollBar1.Size = new System.Drawing.Size(296, 24);
            this.hScrollBar1.TabIndex = 4;
            // 
            // progressBar1
            // 
            this.progressBar1.Location = new System.Drawing.Point(8, 72);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(296, 16);
            this.progressBar1.TabIndex = 3;
            // 
            // trackBar1
            // 
            this.trackBar1.Location = new System.Drawing.Point(8, 96);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(296, 45);
            this.trackBar1.TabIndex = 2;
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Location = new System.Drawing.Point(8, 40);
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(136, 21);
            this.numericUpDown1.TabIndex = 1;
            // 
            // domainUpDown1
            // 
            this.domainUpDown1.Location = new System.Drawing.Point(8, 8);
            this.domainUpDown1.Name = "domainUpDown1";
            this.domainUpDown1.Size = new System.Drawing.Size(136, 21);
            this.domainUpDown1.TabIndex = 0;
            this.domainUpDown1.Text = "domainUpDown1";
            // 
            // printDialog1
            // 
            this.printDialog1.AllowSelection = true;
            this.printDialog1.PrintToFile = true;
            // 
            // printPreviewDialog1
            // 
            this.printPreviewDialog1.AutoScrollMargin = new System.Drawing.Size(0, 0);
            this.printPreviewDialog1.AutoScrollMinSize = new System.Drawing.Size(0, 0);
            this.printPreviewDialog1.ClientSize = new System.Drawing.Size(400, 300);
            this.printPreviewDialog1.Enabled = true;
            this.printPreviewDialog1.Icon = ((System.Drawing.Icon)(resources.GetObject("printPreviewDialog1.Icon")));
            this.printPreviewDialog1.Name = "printPreviewDialog1";
            this.printPreviewDialog1.Visible = false;
            // 
            // statusBar1
            // 
            this.statusBar1.Location = new System.Drawing.Point(0, 363);
            this.statusBar1.Name = "statusBar1";
            this.statusBar1.Panels.AddRange(new System.Windows.Forms.StatusBarPanel[] {
            this.statusBarPanel1,
            this.statusBarPanel2});
            this.statusBar1.Size = new System.Drawing.Size(512, 22);
            this.statusBar1.TabIndex = 1;
            this.statusBar1.Text = "statusBar1";
            // 
            // statusBarPanel1
            // 
            this.statusBarPanel1.Name = "statusBarPanel1";
            this.statusBarPanel1.Text = "statusBarPanel1";
            // 
            // statusBarPanel2
            // 
            this.statusBarPanel2.Name = "statusBarPanel2";
            this.statusBarPanel2.Text = "statusBarPanel2";
            // 
            // toolBar1
            // 
            this.toolBar1.Buttons.AddRange(new System.Windows.Forms.ToolBarButton[] {
            this.btnNew,
            this.btnOpen,
            this.btnSave,
            this.btnHelp});
            this.toolBar1.DropDownArrows = true;
            this.toolBar1.ImageList = this.imageList1;
            this.toolBar1.Location = new System.Drawing.Point(0, 0);
            this.toolBar1.Name = "toolBar1";
            this.toolBar1.ShowToolTips = true;
            this.toolBar1.Size = new System.Drawing.Size(512, 28);
            this.toolBar1.TabIndex = 2;
            this.toolBar1.ButtonClick += new System.Windows.Forms.ToolBarButtonClickEventHandler(this.toolBar1_ButtonClick);
            // 
            // btnNew
            // 
            this.btnNew.ImageIndex = 3;
            this.btnNew.Name = "btnNew";
            // 
            // btnOpen
            // 
            this.btnOpen.ImageIndex = 4;
            this.btnOpen.Name = "btnOpen";
            // 
            // btnSave
            // 
            this.btnSave.ImageIndex = 9;
            this.btnSave.Name = "btnSave";
            // 
            // btnHelp
            // 
            this.btnHelp.ImageIndex = 11;
            this.btnHelp.Name = "btnHelp";
            this.btnHelp.Tag = "Help";
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "");
            this.imageList1.Images.SetKeyName(1, "");
            this.imageList1.Images.SetKeyName(2, "");
            this.imageList1.Images.SetKeyName(3, "");
            this.imageList1.Images.SetKeyName(4, "");
            this.imageList1.Images.SetKeyName(5, "");
            this.imageList1.Images.SetKeyName(6, "");
            this.imageList1.Images.SetKeyName(7, "");
            this.imageList1.Images.SetKeyName(8, "");
            this.imageList1.Images.SetKeyName(9, "");
            this.imageList1.Images.SetKeyName(10, "");
            this.imageList1.Images.SetKeyName(11, "");
            // 
            // mainMenu1
            // 
            this.mainMenu1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem1,
            this.menuItem6,
            this.menuItem10});
            // 
            // menuItem1
            // 
            this.menuItem1.Index = 0;
            this.menuItem1.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem2,
            this.menuItem3,
            this.menuItem4,
            this.menuItem5});
            this.menuItem1.Text = "File";
            // 
            // menuItem2
            // 
            this.menuItem2.Index = 0;
            this.menuItem2.Text = "New";
            // 
            // menuItem3
            // 
            this.menuItem3.Index = 1;
            this.menuItem3.Text = "Open";
            // 
            // menuItem4
            // 
            this.menuItem4.Index = 2;
            this.menuItem4.Text = "Save";
            // 
            // menuItem5
            // 
            this.menuItem5.Index = 3;
            this.menuItem5.Text = "Exit";
            this.menuItem5.Click += new System.EventHandler(this.menuItem5_Click);
            // 
            // menuItem6
            // 
            this.menuItem6.Index = 1;
            this.menuItem6.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem7,
            this.menuItem8,
            this.menuItem9});
            this.menuItem6.Text = "Skin";
            // 
            // menuItem7
            // 
            this.menuItem7.Index = 0;
            this.menuItem7.Text = "LoadSkin";
            this.menuItem7.Click += new System.EventHandler(this.button6_Click);
            // 
            // menuItem8
            // 
            this.menuItem8.Index = 1;
            this.menuItem8.Text = "ColorTheme";
            this.menuItem8.Click += new System.EventHandler(this.menuItem8_Click);
            // 
            // menuItem9
            // 
            this.menuItem9.Index = 2;
            this.menuItem9.Text = "SkinInfo";
            this.menuItem9.Click += new System.EventHandler(this.menuItem9_Click);
            // 
            // menuItem10
            // 
            this.menuItem10.Index = 2;
            this.menuItem10.MenuItems.AddRange(new System.Windows.Forms.MenuItem[] {
            this.menuItem11});
            this.menuItem10.Text = "Help";
            // 
            // menuItem11
            // 
            this.menuItem11.Index = 0;
            this.menuItem11.Text = "Http://www.neemedia.com";
            this.menuItem11.Click += new System.EventHandler(this.menuItem11_Click);
            // 
            // button6
            // 
            this.button6.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.button6.Location = new System.Drawing.Point(416, 336);
            this.button6.Name = "button6";
            this.button6.Size = new System.Drawing.Size(88, 24);
            this.button6.TabIndex = 3;
            this.button6.Text = "Choose Skin";
            this.button6.Click += new System.EventHandler(this.button6_Click);
            // 
            // linkLabel2
            // 
            this.linkLabel2.Location = new System.Drawing.Point(8, 336);
            this.linkLabel2.Name = "linkLabel2";
            this.linkLabel2.Size = new System.Drawing.Size(400, 24);
            this.linkLabel2.TabIndex = 4;
            this.linkLabel2.TabStop = true;
            this.linkLabel2.Text = "Http://www.neemedia.com";
            this.linkLabel2.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.linkLabel2_LinkClicked);
            // 
            // toolStrip1
            // 
            this.toolStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.�½�NToolStripButton,
            this.��OToolStripButton,
            this.����SToolStripButton,
            this.��ӡPToolStripButton,
            this.toolStripSeparator,
            this.����UToolStripButton,
            this.����CToolStripButton,
            this.ճ��PToolStripButton,
            this.toolStripSeparator1,
            this.����LToolStripButton});
            this.toolStrip1.Location = new System.Drawing.Point(112, 4);
            this.toolStrip1.Name = "toolStrip1";
            this.toolStrip1.RenderMode = System.Windows.Forms.ToolStripRenderMode.Professional;
            this.toolStrip1.Size = new System.Drawing.Size(206, 25);
            this.toolStrip1.TabIndex = 5;
            this.toolStrip1.Text = "toolStrip1";
            // 
            // �½�NToolStripButton
            // 
            this.�½�NToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.�½�NToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("�½�NToolStripButton.Image")));
            this.�½�NToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.�½�NToolStripButton.Name = "�½�NToolStripButton";
            this.�½�NToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.�½�NToolStripButton.Text = "�½�(&N)";
            // 
            // ��OToolStripButton
            // 
            this.��OToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.��OToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("��OToolStripButton.Image")));
            this.��OToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.��OToolStripButton.Name = "��OToolStripButton";
            this.��OToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.��OToolStripButton.Text = "��(&O)";
            // 
            // ����SToolStripButton
            // 
            this.����SToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.����SToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("����SToolStripButton.Image")));
            this.����SToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.����SToolStripButton.Name = "����SToolStripButton";
            this.����SToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.����SToolStripButton.Text = "����(&S)";
            // 
            // ��ӡPToolStripButton
            // 
            this.��ӡPToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.��ӡPToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("��ӡPToolStripButton.Image")));
            this.��ӡPToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.��ӡPToolStripButton.Name = "��ӡPToolStripButton";
            this.��ӡPToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.��ӡPToolStripButton.Text = "��ӡ(&P)";
            // 
            // toolStripSeparator
            // 
            this.toolStripSeparator.Name = "toolStripSeparator";
            this.toolStripSeparator.Size = new System.Drawing.Size(6, 25);
            // 
            // ����UToolStripButton
            // 
            this.����UToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.����UToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("����UToolStripButton.Image")));
            this.����UToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.����UToolStripButton.Name = "����UToolStripButton";
            this.����UToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.����UToolStripButton.Text = "����(&U)";
            // 
            // ����CToolStripButton
            // 
            this.����CToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.����CToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("����CToolStripButton.Image")));
            this.����CToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.����CToolStripButton.Name = "����CToolStripButton";
            this.����CToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.����CToolStripButton.Text = "����(&C)";
            // 
            // ճ��PToolStripButton
            // 
            this.ճ��PToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.ճ��PToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("ճ��PToolStripButton.Image")));
            this.ճ��PToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ճ��PToolStripButton.Name = "ճ��PToolStripButton";
            this.ճ��PToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.ճ��PToolStripButton.Text = "ճ��(&P)";
            // 
            // toolStripSeparator1
            // 
            this.toolStripSeparator1.Name = "toolStripSeparator1";
            this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
            // 
            // ����LToolStripButton
            // 
            this.����LToolStripButton.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.����LToolStripButton.Image = ((System.Drawing.Image)(resources.GetObject("����LToolStripButton.Image")));
            this.����LToolStripButton.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.����LToolStripButton.Name = "����LToolStripButton";
            this.����LToolStripButton.Size = new System.Drawing.Size(23, 22);
            this.����LToolStripButton.Text = "����(&L)";
            // 
            // textBox3
            // 
            this.textBox3.Location = new System.Drawing.Point(151, 8);
            this.textBox3.Multiline = true;
            this.textBox3.Name = "textBox3";
            this.textBox3.ScrollBars = System.Windows.Forms.ScrollBars.Both;
            this.textBox3.Size = new System.Drawing.Size(155, 53);
            this.textBox3.TabIndex = 8;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.�ļ�FToolStripMenuItem,
            this.�༭EToolStripMenuItem,
            this.����TToolStripMenuItem,
            this.����HToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(318, 5);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(237, 24);
            this.menuStrip1.TabIndex = 6;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // �ļ�FToolStripMenuItem
            // 
            this.�ļ�FToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.�½�NToolStripMenuItem,
            this.��OToolStripMenuItem,
            this.toolStripSeparator2,
            this.����SToolStripMenuItem,
            this.���ΪAToolStripMenuItem,
            this.toolStripSeparator3,
            this.��ӡPToolStripMenuItem,
            this.��ӡԤ��VToolStripMenuItem,
            this.toolStripSeparator4,
            this.�˳�XToolStripMenuItem});
            this.�ļ�FToolStripMenuItem.Name = "�ļ�FToolStripMenuItem";
            this.�ļ�FToolStripMenuItem.Size = new System.Drawing.Size(57, 20);
            this.�ļ�FToolStripMenuItem.Text = "�ļ�(&F)";
            // 
            // �½�NToolStripMenuItem
            // 
            this.�½�NToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("�½�NToolStripMenuItem.Image")));
            this.�½�NToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.�½�NToolStripMenuItem.Name = "�½�NToolStripMenuItem";
            this.�½�NToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.N)));
            this.�½�NToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.�½�NToolStripMenuItem.Text = "�½�(&N)";
            // 
            // ��OToolStripMenuItem
            // 
            this.��OToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("��OToolStripMenuItem.Image")));
            this.��OToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.��OToolStripMenuItem.Name = "��OToolStripMenuItem";
            this.��OToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.O)));
            this.��OToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.��OToolStripMenuItem.Text = "��(&O)";
            // 
            // toolStripSeparator2
            // 
            this.toolStripSeparator2.Name = "toolStripSeparator2";
            this.toolStripSeparator2.Size = new System.Drawing.Size(6, 6);
            // 
            // ����SToolStripMenuItem
            // 
            this.����SToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("����SToolStripMenuItem.Image")));
            this.����SToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.����SToolStripMenuItem.Name = "����SToolStripMenuItem";
            this.����SToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.S)));
            this.����SToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.����SToolStripMenuItem.Text = "����(&S)";
            // 
            // ���ΪAToolStripMenuItem
            // 
            this.���ΪAToolStripMenuItem.Name = "���ΪAToolStripMenuItem";
            this.���ΪAToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.���ΪAToolStripMenuItem.Text = "���Ϊ(&A)";
            // 
            // toolStripSeparator3
            // 
            this.toolStripSeparator3.Name = "toolStripSeparator3";
            this.toolStripSeparator3.Size = new System.Drawing.Size(6, 6);
            // 
            // ��ӡPToolStripMenuItem
            // 
            this.��ӡPToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("��ӡPToolStripMenuItem.Image")));
            this.��ӡPToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.��ӡPToolStripMenuItem.Name = "��ӡPToolStripMenuItem";
            this.��ӡPToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.P)));
            this.��ӡPToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.��ӡPToolStripMenuItem.Text = "��ӡ(&P)";
            // 
            // ��ӡԤ��VToolStripMenuItem
            // 
            this.��ӡԤ��VToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("��ӡԤ��VToolStripMenuItem.Image")));
            this.��ӡԤ��VToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.��ӡԤ��VToolStripMenuItem.Name = "��ӡԤ��VToolStripMenuItem";
            this.��ӡԤ��VToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.��ӡԤ��VToolStripMenuItem.Text = "��ӡԤ��(&V)";
            // 
            // toolStripSeparator4
            // 
            this.toolStripSeparator4.Name = "toolStripSeparator4";
            this.toolStripSeparator4.Size = new System.Drawing.Size(6, 6);
            // 
            // �˳�XToolStripMenuItem
            // 
            this.�˳�XToolStripMenuItem.Name = "�˳�XToolStripMenuItem";
            this.�˳�XToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.�˳�XToolStripMenuItem.Text = "�˳�(&X)";
            // 
            // �༭EToolStripMenuItem
            // 
            this.�༭EToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.����UToolStripMenuItem,
            this.�ظ�RToolStripMenuItem,
            this.toolStripSeparator5,
            this.����TToolStripMenuItem,
            this.����CToolStripMenuItem,
            this.ճ��PToolStripMenuItem,
            this.toolStripSeparator6,
            this.ȫѡAToolStripMenuItem});
            this.�༭EToolStripMenuItem.Name = "�༭EToolStripMenuItem";
            this.�༭EToolStripMenuItem.Size = new System.Drawing.Size(57, 20);
            this.�༭EToolStripMenuItem.Text = "�༭(&E)";
            // 
            // ����UToolStripMenuItem
            // 
            this.����UToolStripMenuItem.Name = "����UToolStripMenuItem";
            this.����UToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Z)));
            this.����UToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.����UToolStripMenuItem.Text = "����(&U)";
            // 
            // �ظ�RToolStripMenuItem
            // 
            this.�ظ�RToolStripMenuItem.Name = "�ظ�RToolStripMenuItem";
            this.�ظ�RToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.Y)));
            this.�ظ�RToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.�ظ�RToolStripMenuItem.Text = "�ظ�(&R)";
            // 
            // toolStripSeparator5
            // 
            this.toolStripSeparator5.Name = "toolStripSeparator5";
            this.toolStripSeparator5.Size = new System.Drawing.Size(6, 6);
            // 
            // ����TToolStripMenuItem
            // 
            this.����TToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("����TToolStripMenuItem.Image")));
            this.����TToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.����TToolStripMenuItem.Name = "����TToolStripMenuItem";
            this.����TToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.X)));
            this.����TToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.����TToolStripMenuItem.Text = "����(&T)";
            // 
            // ����CToolStripMenuItem
            // 
            this.����CToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("����CToolStripMenuItem.Image")));
            this.����CToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.����CToolStripMenuItem.Name = "����CToolStripMenuItem";
            this.����CToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.C)));
            this.����CToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.����CToolStripMenuItem.Text = "����(&C)";
            // 
            // ճ��PToolStripMenuItem
            // 
            this.ճ��PToolStripMenuItem.Image = ((System.Drawing.Image)(resources.GetObject("ճ��PToolStripMenuItem.Image")));
            this.ճ��PToolStripMenuItem.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.ճ��PToolStripMenuItem.Name = "ճ��PToolStripMenuItem";
            this.ճ��PToolStripMenuItem.ShortcutKeys = ((System.Windows.Forms.Keys)((System.Windows.Forms.Keys.Control | System.Windows.Forms.Keys.V)));
            this.ճ��PToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.ճ��PToolStripMenuItem.Text = "ճ��(&P)";
            // 
            // toolStripSeparator6
            // 
            this.toolStripSeparator6.Name = "toolStripSeparator6";
            this.toolStripSeparator6.Size = new System.Drawing.Size(6, 6);
            // 
            // ȫѡAToolStripMenuItem
            // 
            this.ȫѡAToolStripMenuItem.Name = "ȫѡAToolStripMenuItem";
            this.ȫѡAToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.ȫѡAToolStripMenuItem.Text = "ȫѡ(&A)";
            // 
            // ����TToolStripMenuItem
            // 
            this.����TToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.�Զ���CToolStripMenuItem,
            this.ѡ��OToolStripMenuItem});
            this.����TToolStripMenuItem.Name = "����TToolStripMenuItem";
            this.����TToolStripMenuItem.Size = new System.Drawing.Size(57, 20);
            this.����TToolStripMenuItem.Text = "����(&T)";
            // 
            // �Զ���CToolStripMenuItem
            // 
            this.�Զ���CToolStripMenuItem.Name = "�Զ���CToolStripMenuItem";
            this.�Զ���CToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.�Զ���CToolStripMenuItem.Text = "�Զ���(&C)";
            // 
            // ѡ��OToolStripMenuItem
            // 
            this.ѡ��OToolStripMenuItem.Name = "ѡ��OToolStripMenuItem";
            this.ѡ��OToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.ѡ��OToolStripMenuItem.Text = "ѡ��(&O)";
            // 
            // ����HToolStripMenuItem
            // 
            this.����HToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.����CToolStripMenuItem,
            this.����IToolStripMenuItem,
            this.����SToolStripMenuItem,
            this.toolStripSeparator7,
            this.����AToolStripMenuItem});
            this.����HToolStripMenuItem.Name = "����HToolStripMenuItem";
            this.����HToolStripMenuItem.Size = new System.Drawing.Size(58, 20);
            this.����HToolStripMenuItem.Text = "����(&H)";
            // 
            // ����CToolStripMenuItem
            // 
            this.����CToolStripMenuItem.Name = "����CToolStripMenuItem";
            this.����CToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.����CToolStripMenuItem.Text = "����(&C)";
            // 
            // ����IToolStripMenuItem
            // 
            this.����IToolStripMenuItem.Name = "����IToolStripMenuItem";
            this.����IToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.����IToolStripMenuItem.Text = "����(&I)";
            // 
            // ����SToolStripMenuItem
            // 
            this.����SToolStripMenuItem.Name = "����SToolStripMenuItem";
            this.����SToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.����SToolStripMenuItem.Text = "����(&S)";
            // 
            // toolStripSeparator7
            // 
            this.toolStripSeparator7.Name = "toolStripSeparator7";
            this.toolStripSeparator7.Size = new System.Drawing.Size(6, 6);
            // 
            // ����AToolStripMenuItem
            // 
            this.����AToolStripMenuItem.Name = "����AToolStripMenuItem";
            this.����AToolStripMenuItem.Size = new System.Drawing.Size(32, 19);
            this.����AToolStripMenuItem.Text = "����(&A)...";
            // 
            // statusStrip1
            // 
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripProgressBar1,
            this.toolStripStatusLabel1,
            this.toolStripSplitButton1,
            this.toolStripDropDownButton1,
            this.toolStripSplitButton2});
            this.statusStrip1.Location = new System.Drawing.Point(0, 249);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(488, 22);
            this.statusStrip1.TabIndex = 9;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // toolStripProgressBar1
            // 
            this.toolStripProgressBar1.Name = "toolStripProgressBar1";
            this.toolStripProgressBar1.Size = new System.Drawing.Size(100, 16);
            // 
            // toolStripStatusLabel1
            // 
            this.toolStripStatusLabel1.Name = "toolStripStatusLabel1";
            this.toolStripStatusLabel1.Size = new System.Drawing.Size(109, 17);
            this.toolStripStatusLabel1.Text = "toolStripStatusLabel1";
            // 
            // toolStripSplitButton1
            // 
            this.toolStripSplitButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripSplitButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripSplitButton1.Image")));
            this.toolStripSplitButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSplitButton1.Name = "toolStripSplitButton1";
            this.toolStripSplitButton1.Size = new System.Drawing.Size(32, 20);
            this.toolStripSplitButton1.Text = "toolStripSplitButton1";
            // 
            // toolStripDropDownButton1
            // 
            this.toolStripDropDownButton1.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripDropDownButton1.Image = ((System.Drawing.Image)(resources.GetObject("toolStripDropDownButton1.Image")));
            this.toolStripDropDownButton1.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripDropDownButton1.Name = "toolStripDropDownButton1";
            this.toolStripDropDownButton1.Size = new System.Drawing.Size(29, 20);
            this.toolStripDropDownButton1.Text = "toolStripDropDownButton1";
            // 
            // toolStripSplitButton2
            // 
            this.toolStripSplitButton2.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
            this.toolStripSplitButton2.Image = ((System.Drawing.Image)(resources.GetObject("toolStripSplitButton2.Image")));
            this.toolStripSplitButton2.ImageTransparentColor = System.Drawing.Color.Magenta;
            this.toolStripSplitButton2.Name = "toolStripSplitButton2";
            this.toolStripSplitButton2.Size = new System.Drawing.Size(32, 20);
            this.toolStripSplitButton2.Text = "toolStripSplitButton2";
            // 
            // Form1
            // 
            this.AutoScaleBaseSize = new System.Drawing.Size(6, 14);
            this.ClientSize = new System.Drawing.Size(512, 385);
            this.Controls.Add(this.toolStrip1);
            this.Controls.Add(this.menuStrip1);
            this.Controls.Add(this.linkLabel2);
            this.Controls.Add(this.button6);
            this.Controls.Add(this.toolBar1);
            this.Controls.Add(this.statusBar1);
            this.Controls.Add(this.tabControl1);
            this.MainMenuStrip = this.menuStrip1;
            this.Menu = this.mainMenu1;
            this.Name = "Form1";
            this.Text = "NEEMedia USkin.Net SDI Sample ";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.tabControl1.ResumeLayout(false);
            this.NormalControl.ResumeLayout(false);
            this.groupBox3.ResumeLayout(false);
            this.groupBox3.PerformLayout();
            this.groupBox2.ResumeLayout(false);
            this.groupBox1.ResumeLayout(false);
            this.tabPage3.ResumeLayout(false);
            this.groupBox6.ResumeLayout(false);
            this.groupBox5.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.groupBox4.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.tabPage4.ResumeLayout(false);
            this.tabPage4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanel1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.statusBarPanel2)).EndInit();
            this.toolStrip1.ResumeLayout(false);
            this.toolStrip1.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

		}
		#endregion

		/// <summary>
		/// Ӧ�ó��������ڵ㡣
		/// </summary>
		[STAThread]
		static void Main() 
		{
            USkinSDK.USkinInit("", "", "..\\Skins\\DiyGreen.msstyles");
			Application.Run(new Form1());
		}

		private void Form1_Load(object sender, System.EventArgs e)
		{
			
            //USkinSDK.USkinLoadSkin("..\\Skins\\DiyGreen.msstyles");
		}

		private void button1_Click(object sender, System.EventArgs e)
		{
			printPreviewDialog1.ShowDialog(this);
		}

		private void button5_Click(object sender, System.EventArgs e)
		{
			folderBrowserDialog1.ShowDialog(this);
		}

		private void button2_Click(object sender, System.EventArgs e)
		{
			openFileDialog1.ShowDialog(this);
		}

		private void button3_Click(object sender, System.EventArgs e)
		{
			colorDialog1.ShowDialog(this);
		}

		private void button4_Click(object sender, System.EventArgs e)
		{
			
		}

		private void menuItem9_Click(object sender, System.EventArgs e)
		{
			//About Skin
			//skin.USkinAboutSkin();
		}

		private void menuItem8_Click(object sender, System.EventArgs e)
		{
			//Color Theme
			int Seed=System.DateTime.Now.Millisecond;
			System.Random rnd=new Random(Seed);
			//skin.USkinApplyColorTheme((float)rnd.NextDouble(),(float)rnd.NextDouble());
		}

		private void menuItem5_Click(object sender, System.EventArgs e)
		{
			//Close
			Application.Exit();
		}
		
		[ DllImport( "shell32.dll", CharSet=CharSet.Auto )]
		private static extern IntPtr ShellExecute(IntPtr hwnd,
										  string lpOperation,
										  string lpFile,
										  string lpParameters,
										  string lpDirectory,
										  int nShowCmd
										  );
		private void menuItem11_Click(object sender, System.EventArgs e)
		{
			//Open Web
			ShellExecute(this.Handle,"open","http://www.neemedia.com",null,null,0);	
		}

		private void linkLabel2_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			ShellExecute(this.Handle,"open","http://www.neemedia.com",null,null,0);	
		}

		private void linkLabel1_LinkClicked(object sender, System.Windows.Forms.LinkLabelLinkClickedEventArgs e)
		{
			ShellExecute(this.Handle,"open","http://www.neemedia.com",null,null,0);	
		}

		private void button6_Click(object sender, System.EventArgs e)
		{
			//Choose Skin
            openFileDialog1.Filter = "NEEMedia Skin Files(*.msstyles)|*.msstyles";
			
			if(openFileDialog1.ShowDialog(this)==DialogResult.OK)
			{
				//Skin it
				USkinSDK.USkinLoadSkin(openFileDialog1.FileName);
			}
		}

		private void toolBar1_ButtonClick(object sender, System.Windows.Forms.ToolBarButtonClickEventArgs e)
		{
			if(e.Button == btnHelp)
			{		
				USkinAboutBox about=new USkinAboutBox ();
				about.ShowDialog(this);
				return;
			}
				
			MessageBox.Show(this,"you clicked button " + e.Button.Text);
		}
	}
}
