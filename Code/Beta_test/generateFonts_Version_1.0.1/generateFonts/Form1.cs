using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

// load font file needs to be modded to support RA8875 and Basic (and Adafruit)

namespace generateFonts
{
    public partial class Form1 : Form
    {
        string font = "";
        float fontSize = 0;
        bool extendedChars = false;
        FontStyle fontStyle = FontStyle.Regular;
        preview previewForm = null;
        string m_fileName = "";
        List<List<string>> glyphs = new List<List<string>>();
        List<string> glyphData = new List<string>();

        public Form1()
        {
            InitializeComponent();          

            ((ToolStripDropDownMenu)menuToolStripMenuItem.DropDown).ShowImageMargin = false;
            ((ToolStripDropDownMenu)menuToolStripMenuItem.DropDown).ShowCheckMargin = false;
    

            fontSizeComboBox.SelectedIndex = 0;
            fontStyleComboBox.SelectedIndex = 0;
            
            fontSize = Convert.ToInt32(fontSizeComboBox.SelectedItem);
            fontStyle = FontStyle.Regular;

            fontComboBox.DrawMode = DrawMode.OwnerDrawFixed;
            fontComboBox.DrawItem += ComboBoxFonts_DrawItem;

            List<string> fonts = new List<string>();
            foreach (FontFamily font in FontFamily.Families)
            {
                fonts.Add(font.Name);
            }

            fontComboBox.DataSource = fonts;
            fontComboBox.AutoCompleteMode = AutoCompleteMode.SuggestAppend;
            fontComboBox.SelectedIndex = 0;
            font = fontComboBox.SelectedItem.ToString();
        }

        private void ComboBoxFonts_DrawItem(object sender, DrawItemEventArgs e)
        {
            var comboBox = (ComboBox)sender;
            var fontFamily = new FontFamily(comboBox.Items[e.Index].ToString());
            var font = new Font(fontFamily, comboBox.Font.SizeInPoints);
            e.DrawBackground();
            e.Graphics.DrawString(font.Name, font, Brushes.Black, e.Bounds.X, e.Bounds.Y);
        }

        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            font = fontComboBox.SelectedItem.ToString();
            Console.WriteLine("font set to:" + font);
        }

        private void comboBox2_SelectedIndexChanged(object sender, EventArgs e)
        {
            fontSize = Convert.ToInt32(fontSizeComboBox.SelectedItem);
            Console.WriteLine("fontSize set to:" + fontSize);
        }

        private void comboBox3_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (fontStyleComboBox.SelectedIndex == 0)
                fontStyle = FontStyle.Regular;
            else if (fontStyleComboBox.SelectedIndex == 1)
                fontStyle = FontStyle.Bold;
            else if(fontStyleComboBox.SelectedIndex == 2)
                fontStyle = FontStyle.Italic;
            Console.WriteLine("fontStyle set to:" + fontStyle.ToString());
        }

        private async void button1_Click(object sender, EventArgs e)
        {
            await Task.Run(() => StartProcess());
     
            //DialogResult dialogResult = MessageBox.Show("Would you like to view the created font and edit any characters if necessary?", "Show results", MessageBoxButtons.YesNo);
            //if (dialogResult == DialogResult.Yes)
            //{
                previewForm.setData(glyphs, glyphData, m_fileName);
                previewForm.Show();
            //}
        }

        private void StartProcess()
        {
            glyphs.Clear();
            glyphData.Clear();

            if (previewForm != null)
            {
                previewForm.overrideClose();
                Action close = () => previewForm.Close();
                if (InvokeRequired)
                    Invoke(close);
                else
                    close();
                previewForm = null;
            }
            
            previewForm = new preview();


            createFontFile();
        }

        private void createFontFile()
        {
            if (!Directory.Exists("FontFiles"))
                Directory.CreateDirectory("FontFiles");

            //Create file
            m_fileName = "FontFiles/" + font.Replace(" ", "_") + "_" + fontSize + "px_" + fontStyle + ".h";

            if (!File.Exists(m_fileName))
                File.Delete(m_fileName);

            StreamWriter writer = new StreamWriter(m_fileName);

            // Write the file header
            writer.WriteLine("#pragma once\n");
            writer.WriteLine("// https://github.com/kawasakizx10rr/generateFonts_Version_1.0.1");
            writer.WriteLine("// Created using GenerateFonts, version 1.0.1");
            writer.WriteLine("// Font creation date: " + DateTime.Now.ToLocalTime());
            writer.WriteLine("// Output mode: native");
            writer.WriteLine("// Auto Cropped: false");
            writer.WriteLine("// Font style: " + font.Replace(" ", "_") + "_" + fontSize + "px_" + fontStyle + "\n");

            FontFamily fontFamily = new FontFamily(font);
            Font myFont = new Font(font, fontSize, fontStyle, GraphicsUnit.Pixel);

            int lineSpacing = fontFamily.GetLineSpacing(fontStyle);
            int lineSpacingPixel = Convert.ToInt32(fontSize * lineSpacing / fontFamily.GetEmHeight(fontStyle));
            

            List<string> inserts = new List<string>();
            int ArrayPos = 0;
            string c_hex = "";

            int maxChar = 127;
            if (extendedChars)
                maxChar = 255;

            for (byte i = 32; i < maxChar; i++) // 192 to 255
            {
                // skip the empty chars as the font sets do not seem to flow the ascci table
                if (extendedChars && i == 127)
                    i = 192;

                string c = Convert.ToChar(i).ToString();
                List<string> bytes = new List<string>();
                char[] bits = new char[8] { '0', '0', '0', '0', '0', '0', '0', '0' };
                byte bitPos = 0;

                if (i >= 192)
                    c_hex = "0x" + (i-64).ToString("X2");
                else
                    c_hex = "0x" + i.ToString("X2");
                
                Image bmp = new Bitmap(200, 200);
                Graphics g = Graphics.FromImage(bmp);
                SizeF size = g.MeasureString(c, myFont);               

                if (i == 32) // space char
                    size.Height = 1;

                bmp = new Bitmap(bmp, new Size(Convert.ToInt32(size.Width), Convert.ToInt32(size.Height)));
                g = Graphics.FromImage(bmp);
                g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.SingleBitPerPixelGridFit;
                g.DrawString(c, myFont, new SolidBrush(Color.Black), 0, 0);

                this.Invoke((MethodInvoker)delegate ()
                {
                    for (int h = 0; h < bmp.Height; h++)
                    {
                        for (int w = 0; w < bmp.Width; w++)
                        {
                            Color colour = (bmp as Bitmap).GetPixel(w, h);

                            if (colour.A == 0)
                                bits[bitPos] = '0';
                            else
                                bits[bitPos] = '1';


                            if (bitPos < 7)
                                bitPos++;
                            else
                            {
                                string bitStr = new string(bits);
                                string hex = "0x" + Convert.ToByte(bitStr, 2).ToString("X2");
                                bytes.Add(hex);
                                bitPos = 0;
                                for (byte n = 0; n < 8; n++)
                                {
                                    bits[n] = '0';
                                }
                            }
                        }
                    }

                    if (bitPos != 0)
                    {
                        bitPos = 0;
                        string bitStr = new string(bits);
                        string hex = "0x" + Convert.ToByte(bitStr, 2).ToString("X2");
                        bytes.Add(hex);
                    }

                    int insertCnt = 0;

                    writer.WriteLine("// Char: '" + c + "'");
                    writer.WriteLine("// Width: " + bmp.Width + "px");
                    writer.WriteLine("// Height: " + bmp.Height + "px");
                    writer.WriteLine("const byte " + font.Replace(" ", "_") + "_" + fontSize + "px_" + fontStyle + "_char_" + c_hex + "[" + bytes.Count + "] PROGMEM = {");
                    foreach (string str in bytes)
                    {
                        insertCnt++;
                        if (insertCnt == 1)
                        {
                            writer.Write("  " + str + ", ");
                        }
                        else if (insertCnt == 8)
                        {
                            writer.WriteLine(str + ", ");
                            insertCnt = 0;
                        }
                        else
                        {
                            writer.Write(str + ", ");
                        }
                    }
                    if (insertCnt == 0)
                        writer.WriteLine("};");
                    else
                        writer.WriteLine("\n};");

                    writer.WriteLine("const fontDataStruct " + font.Replace(" ", "_") + "_" + fontSize + "px_" + fontStyle + "_" + c_hex + " = { " + font.Replace(" ", "_") + "_" + fontSize + "px_"
                        + fontStyle + "_char_" + c_hex + ", " + bytes.Count + ", " + bmp.Width + ", " + bmp.Height + ", 0 };\n");

                    inserts.Add(font.Replace(" ", "_") + "_" + fontSize + "px_" + fontStyle + "_" + c_hex);

                    glyphData.Add(c_hex + "," + bytes.Count + "," + bmp.Width + "," + bmp.Height + ",0");
                    glyphs.Add(bytes.ToList());
                    ArrayPos++;

                    bytes.Clear();
                });
            }



            writer.WriteLine("const fontArrayStruct " + font.Replace(" ", "_") + "_" + fontSize + "px_" + fontStyle + "_array" + "[" + inserts.Count + "] = {");
            foreach (string str in inserts)
            {
                writer.WriteLine("  &" + str + ",");
            }
            writer.WriteLine("};\n");
            writer.WriteLine("font_returnStruct " + font.Replace(" ", "_") + "_" + fontSize + "px_" + fontStyle + " = {" + font.Replace(" ", "_") + "_" + fontSize + "px_" + fontStyle + "_array" + ", " + inserts.Count + ", " + 1 + ", " + lineSpacingPixel + ", " + 0x20 + "};");

            writer.Close();
        }
        

        private void newToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Form1 form1 = new Form1();
            form1.Show();
        }

        private void loadFileToolStripMenuItem_Click(object sender, EventArgs e)
        {
            glyphs.Clear();
            glyphData.Clear();

            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                string initalPath = Directory.GetCurrentDirectory() + "\\FontFiles\\";
                Console.WriteLine("Attempting to set load file path to:" + initalPath);
                if (!Directory.Exists(initalPath))
                    Console.WriteLine("Could not locate file path: " + initalPath);
                openFileDialog.InitialDirectory = initalPath;
                openFileDialog.Filter = "header file (*.h)|*.h|All files (*.*)|*.*";
                openFileDialog.FilterIndex = 2;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    m_fileName = openFileDialog.FileName;

                    loadFontFile(m_fileName);

                    if (previewForm != null)
                    {
                        previewForm.overrideClose();
                        Action close = () => previewForm.Close();
                        if (InvokeRequired)
                            Invoke(close);
                        else
                            close();
                        previewForm = null;
                    }

                    previewForm = new preview();
                    previewForm.setData(glyphs, glyphData, m_fileName);
                    previewForm.Show();
                }
            }
        }

        private void loadFontFile(string a_fileName)
        {
            string c_hex = "";
            int byteCount = 0;
            int charWidth = 0;
            int charHeight = 0;
            bool foundArray = false;
            List<string> bytes = new List<string>();

            using (StreamReader reader = new StreamReader(m_fileName))
            {
                string line;
                while ((line = reader.ReadLine()) != null)
                {
                    Console.WriteLine(line);
                    if (line.Contains("Char: '"))
                    {
                        if (line.Contains("'l'"))
                        {
                            Console.WriteLine("?");
                        }
                        if (line.Contains("'''"))
                        {
                            c_hex = "0x27";
                        }
                        else
                        {
                            string[] data = line.Split('\'');
                            byte b = Convert.ToByte(Convert.ToChar(data[1]));
                            c_hex = "0x" + b.ToString("X2");
                        }
                    }
                    else if (line.Contains("Width: "))
                    {
                        string[] data = line.Split(':');
                        charWidth = Convert.ToInt32(data[1].Replace(" ", "").Replace("px", ""));
                    }
                    else if (line.Contains("Height: "))
                    {
                        string[] data = line.Split(':');
                        charHeight = Convert.ToInt32(data[1].Replace(" ", "").Replace("px", ""));
                    }
                    else if (line.Contains("const byte "))
                    {
                        foundArray = true;
                    }
                    else if (line.Contains("};") && foundArray)
                    {
                        foundArray = false;
                    }
                    else if (line.Contains("const fontDataStruct ") && c_hex != "")
                    {
                        string[] data = line.Replace(" ", "").Split(',');
                        byteCount = Convert.ToInt32(data[1]);
                        glyphData.Add(c_hex + ", " + byteCount + ", " + charWidth + ", " + charHeight);
                        glyphs.Add(bytes.ToList());
                        bytes.Clear();
                        c_hex = "";
                    }
                    else if (line.Contains("const fontDataStruct ") && c_hex == "")
                    {
                        Console.WriteLine("c_hex is null");
                    }
                    if (foundArray && !line.Contains("const byte "))
                    {
                        string[] data = line.Replace(" ", "").Split(',');
                        foreach (string str in data)
                        {
                            if (str != "")
                                bytes.Add(str);
                        }
                    }
                }
            }
        }
        

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            switch (MessageBox.Show(this, "Are you sure you want to exit?", "Exit Font Generator", MessageBoxButtons.YesNo))
            {
                case DialogResult.Yes:
                    Close();
                    break;
                default:
                    break;
            }
        }

        private void exctendedCharsCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            extendedChars = exctendedCharsCheckBox.Checked;
        }
    }
}
