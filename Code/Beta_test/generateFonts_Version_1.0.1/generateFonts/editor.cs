using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace generateFonts
{
    public partial class editor : Form
    {
        List<string> m_glyphsBuffer = null;
        List<string> m_glyphs = null;
        string m_glyphData = "";
        int charWidth = 0;
        int charHeight = 0;
        int m_returnArrayPos = 0;
        preview m_preview = null;
        bool allowClose = false;
        int yOffset = 0;
        bool pictureCropped = false;
        int m_totalCount = 0;
        int totalPixels = 0;

        Graphics g;
        Bitmap bm;
        SolidBrush blackBrush = new SolidBrush(Color.Black);
        SolidBrush grayBrush = new SolidBrush(Color.Gray);
        SolidBrush whiteBrush = new SolidBrush(Color.White);
        SolidBrush greenBrush = new SolidBrush(Color.Green);

        public editor(preview a_preview)
        {
            m_preview = a_preview;
            SetAutoSizeMode(AutoSizeMode.GrowAndShrink);
            InitializeComponent();
        }

        private void editor_Load(object sender, EventArgs e)
        {
            ((ToolStripDropDownMenu)resizeToolStripMenuItem.DropDown).ShowImageMargin = false;
            ((ToolStripDropDownMenu)resizeToolStripMenuItem.DropDown).ShowCheckMargin = false;
            ((ToolStripDropDownMenu)optionsToolStripMenuItem.DropDown).ShowImageMargin = false;
            ((ToolStripDropDownMenu)optionsToolStripMenuItem.DropDown).ShowCheckMargin = false;
        }

        protected override void OnFormClosing(FormClosingEventArgs e)
        {
            base.OnFormClosing(e);

            if (e.CloseReason == CloseReason.WindowsShutDown || allowClose)
            {
                allowClose = false;
                blackBrush.Dispose();
                grayBrush.Dispose();
                whiteBrush.Dispose();
                greenBrush.Dispose();
                return;
            }
       
            e.Cancel = true;

            switch (MessageBox.Show(this, "Are you sure you want to finish editing this glyph without saving?", "Closing Editor", MessageBoxButtons.YesNo))
            {
                case DialogResult.Yes:
                    Hide();
                    break;
                default:
                    break;
            }
        }

        internal void overrideClose()
        {
            allowClose = true;
        }

        internal void setData(List<string> a_glyphs, string a_glyphData)
        {
            m_glyphs = a_glyphs;
            m_glyphsBuffer = new List<string>(m_glyphs);
            m_glyphData = a_glyphData;
            pictureCropped = false;
            totalPixels = 0;
            loadChar();
        }

        private void loadChar()
        {
            string[] glyphData = m_glyphData.Split(',');
            string charHex = glyphData[0];
            m_returnArrayPos = Convert.ToByte(charHex, 16) - 32;
            int charByteCount = Convert.ToInt32(glyphData[1]);
            charWidth = Convert.ToInt32(glyphData[2]);
            charHeight = Convert.ToInt32(glyphData[3]);

            Height = (charHeight * 12) + 70;

            if (((charWidth * 12) + 20) > optionsToolStripMenuItem.Width + resizeToolStripMenuItem.Width)
                Width = (charWidth * 12) + 20;

            pictureBox1.Size = new Size(charWidth * 12, charHeight * 12);
            pictureBox1.BackColor = Color.Green;
           // pictureBox1.Image = new Bitmap(charWidth * 12, charHeight * 12);

            bm = new Bitmap(charWidth * 12, charHeight * 12);
            g = Graphics.FromImage(bm);
            g.Clear(Color.Green);

            //Refresh();

            int widthCount = 0, heightCount = 0;
            m_totalCount = 0;

           // Console.WriteLine("editor Form width and height = " + Width + " & " + Height);
           // Console.WriteLine("editor picBox width and height = " + pictureBox1.Width + " & " + pictureBox1.Height);
           // Console.WriteLine("expected pixel width = " + charWidth);
           // Console.WriteLine("expected pixel height = " + charHeight);
           // Console.WriteLine("expected total count = " + (charWidth * charHeight));

            foreach (string hex in m_glyphs)
            {
                string hexVal = hex.Replace("\r", "").Replace("\n", "").Trim();
                byte data = Convert.ToByte(hexVal, 16);

                for (int b = 7; b >= 0; b--)
                {
                    if (m_totalCount < charWidth * charHeight)
                    {
                        if (GetBit(data, b) == true)
                        {
                            totalPixels++;
                            drawSquare(widthCount, heightCount, blackBrush);
                        }
                        else
                        {
                            totalPixels++;
                            drawSquare(widthCount, heightCount, whiteBrush);
                        }

                        if (widthCount < charWidth - 1) { 
                            widthCount++;
                        }
                        else
                        {
                            widthCount = 0;
                            if (heightCount < charHeight)
                                heightCount++;
                        }
                        m_totalCount++;
                    }
                }
            }
            pictureBox1.Image = bm;
            pictureBox1.Update();
            Refresh();
        }

        private void drawSquare(int x, int y, Brush color)
        {
            x *= 12;
            y *= 12;
            try
            {
                g.FillRectangle(color, x + 1, y + 1, 10, 10);
                //pictureBox1.Refresh();
            }
            catch (Exception ex)
            {
                Console.WriteLine("drawSquare error: " + ex);
            }
        }

        public static bool GetBit(byte b, int bitNumber)
        {
            return (b & (1 << bitNumber)) != 0;
        }

        private void pictureBox1_Click(object sender, EventArgs e)
        {
            MouseEventArgs me = (MouseEventArgs)e;
            int xPos = me.X;
            int yPos = me.Y;

            int cellX = xPos > 12 ? xPos / 12 : 0;
            if (cellX > charWidth)
            {
                Console.WriteLine("Click x out of picbox bounds");
                return;
            }

            int cellY = yPos > 12 ? yPos / 12 : 0;
            if (cellY > charHeight)
            {
                Console.WriteLine("Click y out of picbox bounds");
                return;
            }

            int bitPos = 7 - ((cellX + (cellY * charWidth)) % 8);
            int arrayPos = (cellX + (cellY * charWidth)) / 8;

            if (arrayPos >= m_glyphsBuffer.Count)
            {
                Console.WriteLine("Click out of array bounds");
                return;
            }

            if (me.Button == MouseButtons.Right) // Delete 
            {
                pictureCropped = true;
                drawSquare(cellX, cellY, grayBrush);
            }
            else if (me.Button == MouseButtons.Left) // Change cell color (white / black)
            {
                Color colour = (pictureBox1.Image as Bitmap).GetPixel(xPos, yPos);

                if (colour.R == 0 && colour.G == 0 && colour.B == 0)
                {
                    drawSquare(cellX, cellY, whiteBrush);
                }
                else if (colour.R == 255 && colour.G == 255 && colour.B == 255)
                {
                    drawSquare(cellX, cellY, blackBrush);
                }
                else if ((colour.R == 255 && colour.G == 0 && colour.B == 0) || (colour.R == 128 && colour.G == 128 && colour.B == 128))
                {
                    drawSquare(cellX, cellY, blackBrush);
                }
                else
                {
                    return; // clicked green grid line
                }

                byte hexVal = Convert.ToByte(m_glyphsBuffer[arrayPos], 16);
                hexVal ^= (byte)(1 << bitPos);
                string hexStr = "0x" + hexVal.ToString("X2");
                m_glyphsBuffer[arrayPos] = hexStr;
            }
            pictureBox1.Image = bm;
            pictureBox1.Update();
            Refresh();
        }

        public void autoCropToolStripMenuItem_Click(object sender, EventArgs e)
        {

            int firstX = charWidth, firstY = -1, secondX = 0, secondY = 0;
            int widthCount = 0, heightCount = 0;         
            pictureCropped = true;        

            getCharBounds(firstX, firstY, secondX, secondY, heightCount, widthCount,
                out int topMargin, out int bottomMargin, out int leftMargin, out int rightMargin);

            if (topMargin < 0 || bottomMargin < 0 || leftMargin < 0 || rightMargin < 0)
                return;

            widthCount = 0;
            heightCount = 0;
            m_totalCount = 0;

            while (m_totalCount < (charWidth * charHeight))
            {
                if (!(widthCount >= leftMargin && widthCount < rightMargin && heightCount >= topMargin && heightCount <= bottomMargin))
                {
                    drawSquare(widthCount, heightCount, grayBrush);
                }

                if (widthCount < charWidth - 1)
                    widthCount++;
                else
                {
                    widthCount = 0;
                    heightCount++;
                }
                m_totalCount++;
            }

            pictureBox1.Image = bm;
            pictureBox1.Update();
        }

        public void autoTrim(int topMargin, int bottomMargin, int leftMargin, int rightMargin)
        {
            int widthCount = 0, heightCount = 0;

            pictureCropped = true;

            widthCount = 0;
            heightCount = 0;
            m_totalCount = 0;
        
            while (m_totalCount < (charWidth * charHeight))
            {
                // trim top
                if (heightCount < topMargin)
                    drawSquare(widthCount, heightCount, grayBrush);
                // trim bottom
                if (heightCount >= charHeight - bottomMargin)
                    drawSquare(widthCount, heightCount, grayBrush);
                // trim left
                if (widthCount < leftMargin)
                    drawSquare(widthCount, heightCount, grayBrush);
                // trim right
                if (widthCount >= charWidth - rightMargin)
                    drawSquare(widthCount, heightCount, grayBrush);

                if (widthCount < charWidth - 1)
                    widthCount++;
                else
                {
                    widthCount = 0;
                    heightCount++;
                }
                m_totalCount++;
            }      

            pictureBox1.Image = bm;
            pictureBox1.Update();
        }

        public void getCharBounds(int a_firstX, int a_firstY, int a_secondX, int a_secondY, int a_heightCount, int a_widthCount,
            out int topMargin, out int bottomMargin, out int leftMargin, out int rightMargin)
        {
            m_totalCount = 0;
            foreach (string hex in m_glyphsBuffer)
            {
                string hexVal = hex.Replace("\r", "").Replace("\n", "").Trim();
                byte data = Convert.ToByte(hexVal, 16);

                for (int b = 7; b >= 0; b--)
                {
                    if (m_totalCount < totalPixels) //(charWidth * charHeight))
                    {
               
                        if (GetBit(data, b) == true) // found black pixel
                        {
                            if (a_firstY == -1)
                                a_firstY = a_heightCount; // lock in place on first find

                            if (a_heightCount > a_secondY)
                                a_secondY = a_heightCount;

                            if (a_widthCount < a_firstX)
                                a_firstX = a_widthCount;

                            if (a_widthCount >= a_secondX)
                                a_secondX = a_widthCount+1;
                        }


                        if (a_widthCount < charWidth - 1)
                            a_widthCount++;
                        else
                        {
                            a_widthCount = 0;
                            a_heightCount++;
                        }
                        m_totalCount++;
                    }
                }
            }
            topMargin = a_firstY;
            bottomMargin = a_secondY;
            leftMargin = a_firstX;
            rightMargin = a_secondX;
        }

        public void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Hide();
            if (pictureCropped)
                ConvertPicBoxToList();
            else
                m_preview.UpdateGlyph(m_glyphsBuffer, m_returnArrayPos, charWidth, charHeight, yOffset, false);
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Hide();
        }

        private void ConvertPicBoxToList()
        {
            List<Color> colorBuffer = new List<Color>();

            int firstX = -1, firstY = -1, secondX = -1, secondY = -1;       

            for (int h = 0; h < charHeight; h++)
            {
                for (int w = 0; w < charWidth; w++)
                {
                    Color colour = (pictureBox1.Image as Bitmap).GetPixel((w * 12) + 5, (h * 12) + 5);
                    colorBuffer.Add(colour);

                    if ((colour.R == 0 && colour.G == 0 && colour.B == 0) || (colour.R == 255 && colour.G == 255 && colour.B == 255))
                    {
                        if (firstY == -1)
                            firstY = h;

                        if (h > secondY)
                            secondY = h;

                        if (firstX == -1)
                            firstX = w;

                        if (w > secondX)
                            secondX = w;
                    }
                }
            }
            if (firstX == -1 || secondX == -1 || firstY == -1 || secondY == -1)
            {
                MessageBox.Show("Error cropping char.\nAborting!");
                return;
            }
            // ---------------------------------------------- color buffer of after save and then auto crop and save, issue is below ?
            Console.WriteLine("firstX = " + firstX);
            Console.WriteLine("secondX = " + secondX);
            Console.WriteLine("firstY = " + firstY);
            Console.WriteLine("secondY = " + secondY);
            int newWidth = (secondX - firstX) + 1;
            int newHeight = (secondY - firstY) + 1;
            Console.WriteLine("Old width = " + charWidth);
            Console.WriteLine("Old height = " + charHeight);
            Console.WriteLine("New width = " + newWidth);
            Console.WriteLine("New height = " + newHeight);

           // int insertCount = 0;
            byte hexVal = 0;
            byte bitPos = 7;
            List<string> buffer = new List<string>();

            foreach (Color colour in colorBuffer)
            {
                if (colour.R == 255 && colour.G == 255 && colour.B == 255)
                {
                    if (bitPos > 0)
                        bitPos--;
                    else
                    {
                        bitPos = 7;
                        buffer.Add(hexVal.ToString("X2"));
                        hexVal = 0;
                    }
                }
                else if (colour.R == 0 && colour.G == 0 && colour.B == 0)
                {
                    hexVal |= (byte)(1 << bitPos);

                    if (bitPos > 0)
                        bitPos--;
                    else
                    {
                        bitPos = 7;
                        buffer.Add(hexVal.ToString("X2"));
                        hexVal = 0;
                    }
                }                   
            }
            if (bitPos != 7)
            {
                buffer.Add(hexVal.ToString("X2"));
            }

            Console.WriteLine("New buffer.Count = " + buffer.Count);
            Console.WriteLine("Buffer.Count size is " + (((buffer.Count * 8) >= newWidth * newHeight) ? "correct" : "incorrect by " + ((newWidth * newHeight) - (buffer.Count * 8)) + " bits"));

            yOffset = firstY + 1;
            m_preview.UpdateGlyph(buffer, m_returnArrayPos, newWidth, newHeight, yOffset, true);
        }

        private void addColumnLeftToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ConvertImgToCells(true, false, false, false);
        }

        private void addRowTopToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ConvertImgToCells(false, false, true, false);
        }

        private void addColumnRightToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ConvertImgToCells(false, true, false, false);
        }

        private void addRowBottomToolStripMenuItem_Click(object sender, EventArgs e)
        {
            ConvertImgToCells(false, false, false, true);
        }

        private void ConvertImgToCells(bool addColumnLeft, bool addColumnRight, bool addRowTop, bool addRowBottom)
        {
            Console.WriteLine("Old charWidth: " + charWidth);
            Console.WriteLine("Old charHeight: " + charHeight);
            Console.WriteLine("Old pictureBox1.Width: " + pictureBox1.Width);
            Console.WriteLine("Old pictureBox1.Height: " + pictureBox1.Height);
            Console.WriteLine("Old pictureBox1.Image.Width: " + pictureBox1.Image.Width);
            Console.WriteLine("Old pictureBox1.Image.Height: " + pictureBox1.Image.Height);

            int expectedArraySize = (charWidth * charHeight);
            if (addColumnLeft || addColumnRight)
                expectedArraySize += charHeight;
            else if (addRowTop || addRowBottom)
                expectedArraySize += charWidth;
            Console.WriteLine("Expected array size: " + expectedArraySize);

            List <Color> colorBuffer = new List<Color>();

            if (addRowTop)
            {
                for (int w = 0; w < charWidth; w++)
                {
                    colorBuffer.Add(Color.White);
                }
            }

            for (int h = 0; h < charHeight; h++)
            {
                if (addColumnLeft)
                {
                    colorBuffer.Add(Color.White);
                }

                for (int w = 0; w < charWidth; w++)
                {
                    Color colour = (pictureBox1.Image as Bitmap).GetPixel((w * 12) + 5, (h * 12) + 5);

                    colorBuffer.Add(colour);
                }

                if (addColumnRight)
                {
                    colorBuffer.Add(Color.White);
                }
            }

            if (addRowBottom)
            {
                for (int w = 0; w < charWidth; w++)
                {
                    colorBuffer.Add(Color.White);
                }
            }

            Console.WriteLine("Actual array size: " + colorBuffer.Count);
            // ---------------------------------------------------

            pictureBox1.Image = null;

            int imgWidth = pictureBox1.Width;
            int imgHeight = pictureBox1.Height;
            int widthCount = 0, heightCount = 0, arrayPos = 0;
            byte hexVal = 0;
            byte bitPos = 7;
            List<string> buffer = new List<string>();

            if (addRowTop || addRowBottom)
            {
                charHeight += 1;
                imgHeight += 12;
                Height += 12;
                if (addRowTop)
                    yOffset += 1;
            }                
            else if (addColumnLeft || addColumnRight)
            {
                charWidth += 1;
                imgWidth += 12;
                Width += 12;
            }
     
            pictureBox1.Size = new Size(imgWidth, imgHeight);
            pictureBox1.BackColor = Color.Green;
            pictureBox1.Image = new Bitmap(imgWidth, imgHeight);

            bm = new Bitmap(imgWidth, imgHeight);
            g = Graphics.FromImage(bm);
            g.Clear(Color.Green);

            for (int h = 0; h < charHeight; h++)
            {
                for (int w = 0; w < charWidth; w++)
                {
                    Color colorBuffVal = colorBuffer[arrayPos];
                    arrayPos++;
                    if (colorBuffVal.R == 0 && colorBuffVal.G == 0 && colorBuffVal.B == 0)
                    {
                        hexVal |= (byte)(1 << bitPos);

                        if (bitPos > 0)
                            bitPos--;
                        else
                        {
                            bitPos = 7;
                            buffer.Add(hexVal.ToString("X2"));
                            hexVal = 0;
                        }
                    }
                    else
                    {
                        if (bitPos > 0)
                            bitPos--;
                        else
                        {
                            bitPos = 7;
                            buffer.Add(hexVal.ToString("X2"));
                            hexVal = 0;
                        }
                    }

                    if (colorBuffVal.R == 0 && colorBuffVal.G == 0 && colorBuffVal.B == 0)
                    {
                        drawSquare(w, h, blackBrush);
                    }
                    else if (colorBuffVal.R == 255 && colorBuffVal.G == 255 && colorBuffVal.B == 255)
                    {
                        drawSquare(w, h, whiteBrush);
                    }
                    else
                    {
                        drawSquare(w, h, grayBrush);
                    }

                    if (widthCount < charWidth - 1)
                        widthCount++;
                    else
                    {
                        widthCount = 0;
                        heightCount++;
                    }
                }
            }

            if (bitPos > 0 && hexVal != 0)
            {
                buffer.Add(hexVal.ToString("X2"));
            }

            pictureBox1.Image = bm;
            pictureBox1.Update();
            Refresh();

            m_glyphsBuffer = new List<string>(buffer);

            Console.WriteLine("New charWidth: " + charWidth);
            Console.WriteLine("New charHeight: " + charHeight);
            Console.WriteLine("New pictureBox1.Width: " + pictureBox1.Width);
            Console.WriteLine("New pictureBox1.Height: " + pictureBox1.Height);
            Console.WriteLine("New pictureBox1.Image.Width: " + pictureBox1.Image.Width);
            Console.WriteLine("New pictureBox1.Image.Height: " + pictureBox1.Image.Height);
        }

        //private void pictureBox1_Paint(object sender, PaintEventArgs e)
        //{
        //    Graphics g = e.Graphics;
        //    g.DrawRectangle(Pens.Black, 5, 5, 100, 60);
        //}
    }
}
