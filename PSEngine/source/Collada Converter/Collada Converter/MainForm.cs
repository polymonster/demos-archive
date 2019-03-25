using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Collada_Converter
{
    public enum Responsibility
    {
        ALL,
        ANIMATION
    }

    public struct ParseData
    {
        public String filename;
        public Responsibility resposibility;
        
    }

    public partial class MainForm : Form
    {
        converter cv;
        List<ParseData> parse_data = new List<ParseData>();
        String destination_folder;

        int current_selected = -1;

        public MainForm()
        {
            InitializeComponent();

            cv = new converter();
        }

        private void convertButton_Click(object sender, EventArgs e)
        {
            if (destination_folder != null && parse_data.Count > 0)
            {
                statusStrip1.Items[0].Text = "Parsing....";

                cv.colladaToPsm(parse_data, destination_folder);
            }
            else
            {
                statusStrip1.Items[0].Text = "Select a source file and destination first";
            }
        }

        private void addSourceButton_Click(object sender, EventArgs e)
        {
            OpenFileDialog ofd = new OpenFileDialog();

            ofd.Filter = "Collada Scene File| *.dae";
            ofd.Title = "Add a file to export components";

            ofd.ShowDialog();

            if (ofd.FileName != "")
            {
                ParseData pd = new ParseData();
                pd.filename = ofd.FileName;

                if (parse_data.Count == 0) pd.resposibility = Responsibility.ALL;
                else pd.resposibility = Responsibility.ANIMATION;

                parse_data.Add(pd);

                updateListView();
            }
        }

        private void changeDestButton_Click(object sender, EventArgs e)
        {
            SaveFileDialog sfd = new SaveFileDialog();

            sfd.Filter = "Polymonster Model| *.psm";
            sfd.Title = "Select output directory";

            sfd.ShowDialog();

            if (sfd.FileName != "")
            {
                destination_folder = sfd.FileName;
            }
        }

        public void updateListView()
        {
            inputList.SuspendLayout();

            inputList.Clear();

            inputList.Columns.Add("Filenames");
            inputList.Columns[0].Width = 1024;

            foreach(ParseData pd in parse_data)
            {
                inputList.Items.Add(pd.filename);
            }

            inputList.Invalidate();
            inputList.ResumeLayout();
        }

        private void inputList_SelectedIndexChanged(object sender, EventArgs e)
        {
            current_selected = inputList.SelectedIndices[0];

            if(current_selected >= 0 && current_selected < parse_data.Count)
            {
                if(parse_data[current_selected].resposibility == Responsibility.ALL)
                {
                    responseCombo.SelectedIndex = 0;
                }
                else
                {
                    responseCombo.SelectedIndex = 1;
                }
            }
        }

        private void responseCombo_SelectedIndexChanged(object sender, EventArgs e)
        {
            ParseData pd;

            if (current_selected != -1)
            {
                switch (responseCombo.SelectedIndex)
                {
                    case 0:

                        pd = parse_data[current_selected];
                        pd.resposibility = Responsibility.ALL;

                        parse_data[current_selected] = pd;

                        break;

                    case 1:

                        pd = parse_data[current_selected];
                        pd.resposibility = Responsibility.ANIMATION;

                        parse_data[current_selected] = pd;

                        break;
                }
            }
        }
    }
}
