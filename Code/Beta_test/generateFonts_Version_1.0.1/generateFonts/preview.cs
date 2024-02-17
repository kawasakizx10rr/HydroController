using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace generateFonts
{
    public partial class preview : Form
    {
        List<List<string>> m_glyphs = new List<List<string>>();
        List<string> m_glyphData = new List<string>();
        string m_fileName = "";
        editor editBox = null;
        bool allowClose = false;

        public preview()
        {
            InitializeComponent();
            editBox = new editor(this);
        }


        private void preview_Load(object sender, EventArgs e)
        {
            ((ToolStripDropDownMenu)optionsToolStripMenuItem.DropDown).ShowImageMargin = false;
            ((ToolStripDropDownMenu)optionsToolStripMenuItem.DropDown).ShowCheckMargin = false;
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            base.OnFormClosing(e);

            if (e.CloseReason == CloseReason.WindowsShutDown || allowClose)
            {
                if (editBox != null)
                {
                    editBox.overrideClose();
                    Action close = () => editBox.Close();
                    if (InvokeRequired)
                        Invoke(close);
                    else
                        close();
                    editBox = null;
                }

                allowClose = false;
                return;
            }

            e.Cancel = true;

            switch (MessageBox.Show(this, "Are you sure you want to finish editing the font", "Finish Editing Font", MessageBoxButtons.YesNo))
            {
                case DialogResult.Yes:
                    Hide();
                    editBox.Hide();
                    break;
                default:
                    break;
            }
        }

        internal void setData(List<List<string>> a_glyphs, List<string> a_glyphData, string a_fileName)
        {
            m_glyphs = a_glyphs;
            m_glyphData = a_glyphData;
            m_fileName = a_fileName;
            if (a_glyphs.Count > 0 && a_glyphData.Count > 0) // preload 
                editBox.setData(a_glyphs[1], a_glyphData[1]);
            loadFont();
        }

        private void loadFont()
        {
            int xPos = 10;
            int yPos = 10;
            foreach (List<string> subList in m_glyphs)
            {
                string[] glyphData = m_glyphData[m_glyphs.IndexOf(subList)].Split(',');

                string charHex = glyphData[0];
                int charByteCount = Convert.ToInt32(glyphData[1]);
                int charWidth = Convert.ToInt32(glyphData[2]);
                int charHeight = Convert.ToInt32(glyphData[3]);

                //
                Panel panel1 = new Panel();
                Label label1 = new Label();
                // Initialize the Label and TextBox controls.
                label1.Name = "Label" + charHex;
                label1.Size = new Size(33, 14);
                label1.AutoSize = true;
                label1.Text = charHex; 
                int panelWidth = label1.Width > (charWidth + 1) ? label1.Width : (charWidth + 1);
                label1.Location = new Point((panelWidth - label1.Width) / 2, 0);
                label1.Cursor = Cursors.Hand;
                label1.Click += delegate (object sender, EventArgs e) { button_Click(sender, e, label1.Name.Replace("Label", "")); };
                // Initialize the Panel control.
                panel1.Name = "Panel" + charHex;
                panel1.Location = new Point(xPos, yPos);
                panel1.Size = new Size(panelWidth, charHeight + 20);
                panel1.BorderStyle = BorderStyle.Fixed3D;
                panel1.Cursor = Cursors.Hand;
                panel1.Click += delegate (object sender, EventArgs e) { button_Click(sender, e, panel1.Name.Replace("Panel", "")); };
                // 
                PictureBox pic = new PictureBox();
                pic.Location = new Point((panelWidth - (charWidth + 1)) / 2, 14);
                pic.Name = "Pic" + charHex;
                pic.Size = new Size(charWidth + 1, charHeight + 1);
                pic.BackColor = Color.White;
                pic.Image = new Bitmap(charWidth + 1, charHeight + 1);
                pic.Cursor = Cursors.Hand;
                pic.Click += delegate (object sender, EventArgs e) { button_Click(sender, e, pic.Name.Replace("Pic", "")); };
                xPos += charWidth + 10;

                int widthCount = 0, heightCount = 0;

                foreach (string hex in subList)
                {
                    byte data = Convert.ToByte(hex.Replace("\t",""), 16);

                    for (int b = 7; b >= 0; b--)
                    {
                        if (GetBit(data, b) == true)
                        {
                            ((Bitmap)pic.Image).SetPixel(widthCount, heightCount, Color.Black);
                        }

                        if (widthCount < charWidth - 1)
                            widthCount++;
                        else
                        {
                            widthCount = 0;
                            if (heightCount < charHeight)
                                heightCount++;
                        }
                    }                       
                }

                flowLayoutPanel1.Controls.Add(panel1);
                flowLayoutPanel1.Refresh();
                panel1.Controls.Add(label1);
                panel1.Controls.Add(pic);
            }
        }

        public static bool GetBit(byte b, int bitNumber)
        {
            return (b & (1 << bitNumber)) != 0;
        }

        void button_Click(object sender, EventArgs e, string name)
        {
            int n = Convert.ToByte(name, 16) - 32;
            string glyphData = m_glyphData[n];
            List<string> glyphs = m_glyphs[n];
            editBox.Show();
            editBox.setData(glyphs, glyphData);         
            Console.WriteLine("Picturebox " + name + " clicked");
        }

        internal void UpdateGlyph(List<string> a_glyphs, int a_arrayPos, int a_width, int a_height, int a_yOffset, bool autoCropped)
        {
            m_glyphs[a_arrayPos] = a_glyphs.ToList();

            string id = "0x" + (32 + a_arrayPos).ToString("X2");
            int widthCount = 0, heightCount = 0;
            PictureBox pic = flowLayoutPanel1.Controls.Find("Pic" + id, true).FirstOrDefault() as PictureBox;
            Label label = flowLayoutPanel1.Controls.Find("Label" + id, true).FirstOrDefault() as Label;
            Panel panel = flowLayoutPanel1.Controls.Find("Panel" + id, true).FirstOrDefault() as Panel;

            int panelWidth = label.Width > a_width ? label.Width : a_width;
            label.Location = new Point((panelWidth - label.Width) / 2, 0);
            panel.Size = new Size(panelWidth, a_height + 20);
            pic.Size = new Size(a_width, a_height);
            pic.Location = new Point((panelWidth - a_width) / 2, 14);
            pic.Image = new Bitmap(a_width, a_height);

            string fontInfo = id + "," + a_glyphs.Count + "," + a_width + "," + a_height + "," + a_yOffset;
            m_glyphData[a_arrayPos] = fontInfo;           
                
            int totalCount = 0;

            foreach (string hex in a_glyphs)
            {
                byte data = Convert.ToByte(hex, 16);

                for (int b = 7; b >= 0; b--)
                {
                    if (totalCount < a_width * a_height)
                    {
                        if (GetBit(data, b) == true)
                        {
                            ((Bitmap)pic.Image).SetPixel(widthCount, heightCount, Color.Black);
                        }
                        else
                        {
                            ((Bitmap)pic.Image).SetPixel(widthCount, heightCount, Color.White);
                        }
                        if (widthCount < a_width - 1)
                            widthCount++;
                        else
                        {
                            widthCount = 0;
                            heightCount++;
                        }
                        totalCount++;
                    }
                }

            }
            pic.Refresh();
            flowLayoutPanel1.Refresh();          

            // Update glyph array in file
            List<string> lines = new List<string>();
            List<string> fileData = new List<string>();

            using (StreamReader reader = new StreamReader(m_fileName))
            {              
                string line;
                while ((line = reader.ReadLine()) != null)
                {
                    lines.Add(line);
                }
            }

            bool continueInserting = false;
            bool insertComplete = false;
            bool foundMatch = false;
            int insertCnt = 0;
            string hexVal = "  ";
           // string endLine = "";

            for (int i = 0; i < lines.Count(); i++)
            {
                if (lines[i].Contains("Auto Cropped:"))
                {
                    lines[i] = "// Auto Cropped: " + (autoCropped ? "true" : "false");
                }
                else if (lines[i].Contains("Font creation date:"))
                {
                    lines[i] = "// Font creation date: " + DateTime.Now.ToLongDateString() + " " + DateTime.Now.ToLongTimeString();
                }
                if (!foundMatch && lines[i].Contains("_char_" + id + "["))
                {
                    fileData[i - 1] = "// Height: " + a_height + "px";
                    fileData[i - 2] = "// Width: " + a_width + "px";
                    string[] title = lines[i].Split('[');
                    fileData.Add(title[0] + "[" + a_glyphs.Count() + "] PROGMEM = {");
                    foundMatch = true;
                }
                else if (!foundMatch || continueInserting)
                {
                    fileData.Add(lines[i]);
                }
                else if (insertComplete && lines[i].Contains("};") && !continueInserting)
                {
                    string[] dataStruct = lines[i + 1].Split(',');
                    fileData.Add("};");
                    fileData.Add(dataStruct[0] + ", " + a_glyphs.Count + ", " + a_width + ", " + a_height + ", " + a_yOffset + " };");
                    i++;
                    continueInserting = true;
                }
                else if (foundMatch && !insertComplete)
                {
                    foreach (string str in a_glyphs)
                    {
                        insertCnt++;
                        if (!str.Contains("0x"))
                            hexVal += "0x" + str + ", ";
                        else
                            hexVal += str + ", ";                          
                    }
                    fileData.Add(hexVal);
                    insertComplete = true;
                }
            }

            using (StreamWriter writer = new StreamWriter(m_fileName))
            {
                foreach(string str in fileData)
                {
                    writer.WriteLine(str);
                }
            }

            if (m_glyphs.Count >= a_arrayPos && m_glyphData.Count >= a_arrayPos) // preload 
                editBox.setData(m_glyphs[a_arrayPos], m_glyphData[a_arrayPos]);
        }

        internal void overrideClose()
        {
            allowClose = true;
        }

        private void autoCropAllToolStripMenuItem_Click(object sender, EventArgs e)
        {
            for (int n = 1; n < m_glyphData.Count(); n++)
            {
                string glyphData = m_glyphData[n];
                List<string> glyphs = m_glyphs[n];
                editBox.setData(glyphs, glyphData);
                editBox.autoCropToolStripMenuItem_Click(sender, e);
                editBox.saveToolStripMenuItem_Click(sender, e);
            }
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            switch (MessageBox.Show(this, "Are you sure you want to finish editing the font", "Finish Editing Font", MessageBoxButtons.YesNo))
            {
                case DialogResult.Yes:
                    Hide();
                    break;
                default:
                    break;
            }
        }
    }
}
